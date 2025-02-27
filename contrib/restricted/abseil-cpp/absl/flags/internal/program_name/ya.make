# Generated by devtools/yamaker.

LIBRARY()



LICENSE(Apache-2.0)

PEERDIR(
    contrib/restricted/abseil-cpp/absl/base
    contrib/restricted/abseil-cpp/absl/base/internal/low_level_alloc
    contrib/restricted/abseil-cpp/absl/base/internal/raw_logging
    contrib/restricted/abseil-cpp/absl/base/internal/spinlock_wait
    contrib/restricted/abseil-cpp/absl/base/internal/throw_delegate
    contrib/restricted/abseil-cpp/absl/base/log_severity
    contrib/restricted/abseil-cpp/absl/debugging
    contrib/restricted/abseil-cpp/absl/debugging/stacktrace
    contrib/restricted/abseil-cpp/absl/debugging/symbolize
    contrib/restricted/abseil-cpp/absl/demangle
    contrib/restricted/abseil-cpp/absl/numeric
    contrib/restricted/abseil-cpp/absl/strings
    contrib/restricted/abseil-cpp/absl/strings/internal
    contrib/restricted/abseil-cpp/absl/synchronization
    contrib/restricted/abseil-cpp/absl/synchronization/internal
    contrib/restricted/abseil-cpp/absl/time
    contrib/restricted/abseil-cpp/absl/time/civil_time
    contrib/restricted/abseil-cpp/absl/time/time_zone
)

ADDINCL(
    GLOBAL contrib/restricted/abseil-cpp
)

NO_COMPILER_WARNINGS()

NO_UTIL()

CFLAGS(
    -DNOMINMAX
)

SRCDIR(contrib/restricted/abseil-cpp/absl/flags/internal)

SRCS(
    program_name.cc
)

END()
