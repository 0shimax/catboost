#pragma once

#include "public.h"

#include <util/generic/maybe.h>
#include <util/generic/ptr.h>

class IInputStream;

namespace NYson {
    ////////////////////////////////////////////////////////////////////////////////

    class TYsonParser {
    public:
        TYsonParser(
            IYsonConsumer* consumer,
            IInputStream* stream,
            EYsonType type = ::NYson::EYsonType::Node,
            bool enableLinePositionInfo = false,
            TMaybe<ui64> memoryLimit = Nothing());

        ~TYsonParser();

        void Parse();

    private:
        class TImpl;
        THolder<TImpl> Impl;
    };

    ////////////////////////////////////////////////////////////////////////////////

    class TStatelessYsonParser {
    public:
        TStatelessYsonParser(
            IYsonConsumer* consumer,
            bool enableLinePositionInfo = false,
            TMaybe<ui64> memoryLimit = Nothing());

        ~TStatelessYsonParser();

        void Parse(const TStringBuf& data, EYsonType type = ::NYson::EYsonType::Node);

    private:
        class TImpl;
        THolder<TImpl> Impl;
    };

    ////////////////////////////////////////////////////////////////////////////////

    class TYsonListParser {
    public:
        TYsonListParser(
            IYsonConsumer* consumer,
            IInputStream* stream,
            bool enableLinePositionInfo = false,
            TMaybe<ui64> memoryLimit = Nothing());

        ~TYsonListParser();

        bool Parse(); // Returns false, if there is no more list items

    private:
        class TImpl;
        THolder<TImpl> Impl;
    };

    ////////////////////////////////////////////////////////////////////////////////

    void ParseYsonStringBuffer(
        const TStringBuf& buffer,
        IYsonConsumer* consumer,
        EYsonType type = ::NYson::EYsonType::Node,
        bool enableLinePositionInfo = false,
        TMaybe<ui64> memoryLimit = Nothing());

    ////////////////////////////////////////////////////////////////////////////////

} // namespace NYson
