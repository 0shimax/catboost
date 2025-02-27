#include "saveload.h"

#include <util/stream/input.h>
#include <util/stream/output.h>
#include <util/system/yassert.h>

#include <type_traits>

using namespace NChromiumTrace;

namespace {
    void SaveStr(IOutputStream* out, TStringBuf str) {
        ::SaveSize(out, str.size());
        ::SavePodArray(out, str.data(), str.size());
    }

    void LoadStr(IInputStream* in, TStringBuf& str, TMemoryPool& pool) {
        size_t size = ::LoadSize(in);
        char* data = ::AllocateFromPool(pool, size);
        ::LoadPodArray(in, data, size);

        str = TStringBuf(data, size);
    }

    using TConstAnyEventPtr = std::variant<
        const TDurationBeginEvent*,
        const TDurationEndEvent*,
        const TDurationCompleteEvent*,
        const TInstantEvent*,
        const TAsyncEvent*,
        const TCounterEvent*,
        const TMetadataEvent*>;

    struct TSavePtrVisitor {
        IOutputStream* Out;

        template <typename T>
        void operator()(const T* event) const {
            Y_ASSERT(event);
            ::Save(Out, *event);
        }
    };

    struct TSaveVisitor {
        IOutputStream* Out;

        template <typename T>
        void operator()(T value) const {
            ::Save(Out, value);
        }
    };

    template <>
    inline void TSaveVisitor::operator()(TStringBuf value) const {
        ::SaveStr(Out, value);
    }

    struct TEventWithArgsPtr {
        TConstAnyEventPtr Event;
        const TEventArgs* Args;

        void Save(IOutputStream* out) const {
            static const TEventArgs emptyArgs;

            ::Save(out, static_cast<i8>(Event.index()));
            std::visit(TSavePtrVisitor{out}, Event);
            if (Args) {
                ::Save(out, *Args);
            } else {
                ::Save(out, emptyArgs);
            }
        }
    };

    struct TSaveAnyEventVisitor {
        IOutputStream* Out;
        const TEventArgs* Args;

        template <typename T>
        void operator()(const T& value) const {
            ::Save(Out, TEventWithArgsPtr{&value, Args});
        }
    };
}

#define CHECK_EVENT_TAG_I8(type) static_assert( \
    TVariantIndexV<type, TAnyEvent> <= 127 && TVariantIndexV<type, TAnyEvent> != std::variant_npos && \
    TVariantIndexV<const type*, TConstAnyEventPtr> <= 127 && \
    TVariantIndexV<const type*, TConstAnyEventPtr> != std::variant_npos, \
    "tag of " #type " is too big")

#define CHECK_ARG_TAG_I8(type) static_assert( \
    TVariantIndexV<type, TEventArgs::TArg::TValue> <= 127 && \
    TVariantIndexV<type, TEventArgs::TArg::TValue> != std::variant_npos, \
    "tag of " #type " is too big")

CHECK_EVENT_TAG_I8(TDurationBeginEvent);
CHECK_EVENT_TAG_I8(TDurationEndEvent);
CHECK_EVENT_TAG_I8(TDurationCompleteEvent);
CHECK_EVENT_TAG_I8(TInstantEvent);
CHECK_EVENT_TAG_I8(TAsyncEvent);
CHECK_EVENT_TAG_I8(TCounterEvent);
CHECK_EVENT_TAG_I8(TMetadataEvent);

CHECK_ARG_TAG_I8(i64);
CHECK_ARG_TAG_I8(double);
CHECK_ARG_TAG_I8(TStringBuf);

TSaveLoadTraceConsumer::TSaveLoadTraceConsumer(IOutputStream* stream)
    : Stream(stream)
{
}

void TSaveLoadTraceConsumer::AddEvent(const TDurationBeginEvent& event, const TEventArgs* args) {
    ::Save(Stream, TEventWithArgsPtr{&event, args});
}

void TSaveLoadTraceConsumer::AddEvent(const TDurationEndEvent& event, const TEventArgs* args) {
    ::Save(Stream, TEventWithArgsPtr{&event, args});
}

void TSaveLoadTraceConsumer::AddEvent(const TDurationCompleteEvent& event, const TEventArgs* args) {
    ::Save(Stream, TEventWithArgsPtr{&event, args});
}

void TSaveLoadTraceConsumer::AddEvent(const TCounterEvent& event, const TEventArgs* args) {
    ::Save(Stream, TEventWithArgsPtr{&event, args});
}

void TSaveLoadTraceConsumer::AddEvent(const TMetadataEvent& event, const TEventArgs* args) {
    ::Save(Stream, TEventWithArgsPtr{&event, args});
}

void TSerializer<TEventArgs::TArg>::Save(IOutputStream* out, const TEventArgs::TArg& v) {
    // TODO: saveload for std::variant (?)

    ::SaveStr(out, v.Name);

    i8 tag = v.Value.index();
    ::Save(out, tag);
    std::visit(TSaveVisitor{out}, v.Value);
}

void TSerializer<TEventArgs::TArg>::Load(IInputStream* in, TEventArgs::TArg& v, TMemoryPool& pool) {
    using TValue = TEventArgs::TArg::TValue;

    ::LoadStr(in, v.Name, pool);

    i8 tag = 0;
    ::Load(in, tag);
    switch (tag) {
        case TVariantIndexV<TStringBuf, TValue>:
            v.Value = TStringBuf();
            ::LoadStr(in, std::get<TStringBuf>(v.Value), pool);
            break;

        case TVariantIndexV<i64, TValue>:
            v.Value = i64();
            ::Load(in, std::get<i64>(v.Value));
            break;

        case TVariantIndexV<double, TValue>:
            v.Value = double();
            ::Load(in, std::get<double>(v.Value));
            break;

        default:
            ythrow TSerializeException() << "Invalid variant tag: " << tag;
    }
}

void TSerializer<TEventArgs>::Save(IOutputStream* out, const TEventArgs& v) {
    // TODO: saveload for TStackVec

    ::SaveSize(out, v.Items.size());
    ::SaveRange(out, v.Items.begin(), v.Items.end());
}

void TSerializer<TEventArgs>::Load(IInputStream* in, TEventArgs& v, TMemoryPool& pool) {
    ::LoadSizeAndResize(in, v.Items);
    ::LoadRange(in, v.Items.begin(), v.Items.end(), pool);
}

void TSerializer<TDurationBeginEvent>::Save(IOutputStream* out, const TDurationBeginEvent& v) {
    ::SaveMany(out, v.Origin, v.Time, v.Flow);
    ::SaveStr(out, v.Name);
    ::SaveStr(out, v.Categories);
}

void TSerializer<TDurationBeginEvent>::Load(IInputStream* in, TDurationBeginEvent& v, TMemoryPool& pool) {
    ::LoadMany(in, v.Origin, v.Time, v.Flow);
    ::LoadStr(in, v.Name, pool);
    ::LoadStr(in, v.Categories, pool);
}

void TSerializer<TDurationCompleteEvent>::Save(IOutputStream* out, const TDurationCompleteEvent& v) {
    ::SaveMany(out, v.Origin, v.BeginTime, v.EndTime, v.Flow);
    ::SaveStr(out, v.Name);
    ::SaveStr(out, v.Categories);
}

void TSerializer<TDurationCompleteEvent>::Load(IInputStream* in, TDurationCompleteEvent& v, TMemoryPool& pool) {
    ::LoadMany(in, v.Origin, v.BeginTime, v.EndTime, v.Flow);
    ::LoadStr(in, v.Name, pool);
    ::LoadStr(in, v.Categories, pool);
}

void TSerializer<TInstantEvent>::Save(IOutputStream* out, const TInstantEvent& v) {
    ::SaveMany(out, v.Origin, v.Time, v.Scope);
    ::SaveStr(out, v.Name);
    ::SaveStr(out, v.Categories);
}

void TSerializer<TInstantEvent>::Load(IInputStream* in, TInstantEvent& v, TMemoryPool& pool) {
    ::LoadMany(in, v.Origin, v.Time, v.Scope);
    ::LoadStr(in, v.Name, pool);
    ::LoadStr(in, v.Categories, pool);
}

void TSerializer<TAsyncEvent>::Save(IOutputStream* out, const TAsyncEvent& v) {
    ::SaveMany(out, v.SubType, v.Origin, v.Time, v.Id);
    ::SaveStr(out, v.Name);
    ::SaveStr(out, v.Categories);
}

void TSerializer<TAsyncEvent>::Load(IInputStream* in, TAsyncEvent& v, TMemoryPool& pool) {
    ::LoadMany(in, v.SubType, v.Origin, v.Time, v.Id);
    ::LoadStr(in, v.Name, pool);
    ::LoadStr(in, v.Categories, pool);
}

void TSerializer<TCounterEvent>::Save(IOutputStream* out, const TCounterEvent& v) {
    ::SaveMany(out, v.Origin, v.Time);
    ::SaveStr(out, v.Name);
    ::SaveStr(out, v.Categories);
}

void TSerializer<TCounterEvent>::Load(IInputStream* in, TCounterEvent& v, TMemoryPool& pool) {
    ::LoadMany(in, v.Origin, v.Time);
    ::LoadStr(in, v.Name, pool);
    ::LoadStr(in, v.Categories, pool);
}

void TSerializer<TMetadataEvent>::Save(IOutputStream* out, const TMetadataEvent& v) {
    ::Save(out, v.Origin);
    ::SaveStr(out, v.Name);
}

void TSerializer<TMetadataEvent>::Load(IInputStream* in, TMetadataEvent& v, TMemoryPool& pool) {
    ::Load(in, v.Origin);
    ::LoadStr(in, v.Name, pool);
}

void TSerializer<TEventWithArgs>::Save(IOutputStream* out, const TEventWithArgs& v) {
    std::visit(TSaveAnyEventVisitor{out, &v.Args}, v.Event);
}

void TSerializer<TEventWithArgs>::Load(IInputStream* in, TEventWithArgs& v, TMemoryPool& pool) {
    i8 tag = 0;
    ::Load(in, tag);
    switch (tag) {
#define CASE(type)                                 \
    case TVariantIndexV<type, TAnyEvent>:          \
        v.Event = type();                          \
        ::Load(in, std::get<type>(v.Event), pool); \
        break;

        CASE(TDurationBeginEvent)
        CASE(TDurationEndEvent)
        CASE(TDurationCompleteEvent)
        CASE(TInstantEvent)
        CASE(TAsyncEvent)
        CASE(TCounterEvent)
        CASE(TMetadataEvent)

#undef CASE

        default:
            ythrow TSerializeException() << "Invalid variant tag: " << tag;
    }
    ::Load(in, v.Args, pool);
}
