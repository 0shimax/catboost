// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: rennie@google.com (Jeffrey Rennie)

#ifndef GOOGLE_PROTOBUF_COMPILER_CPP_OPTIONS_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP_OPTIONS_H__

#include <string>
#include <google/protobuf/stubs/port.h>

namespace google {
namespace protobuf {
namespace compiler {
class AccessInfoMap;

namespace cpp {

enum class EnforceOptimizeMode {
  kNoEnforcement,  // Use the runtime specified by the file specific options.
  kSpeed,          // Full runtime with a generated code implementation.
  kCodeSize,       // Full runtime with a reflective implementation.
  kLiteRuntime,
};

// Generator options (see generator.cc for a description of each):
struct Options {
  TProtoStringType dllexport_decl;
  bool safe_boundary_check = false;
  bool proto_h = false;
  bool transitive_pb_h = true;
  bool annotate_headers = false;
  EnforceOptimizeMode enforce_mode = EnforceOptimizeMode::kNoEnforcement;
  bool table_driven_parsing = false;
  bool table_driven_serialization = false;
  bool lite_implicit_weak_fields = false;
  bool bootstrap = false;
  bool opensource_runtime = false;
  bool annotate_accessor = false;
  bool unused_field_stripping = false;
  TProtoStringType runtime_include_base;
  int num_cc_files = 0;
  TProtoStringType annotation_pragma_name;
  TProtoStringType annotation_guard_name;
  const AccessInfoMap* access_info_map = nullptr;
  enum {
    kTCTableNever,
    kTCTableGuarded,
    kTCTableAlways
  } tctable_mode = kTCTableNever;
  bool inject_field_listener_events = false;
  bool eagerly_verified_lazy = false;
  bool force_eagerly_verified_lazy = false;
};

}  // namespace cpp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_CPP_OPTIONS_H__
