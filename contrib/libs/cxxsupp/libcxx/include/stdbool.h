// -*- C++ -*-
//===--------------------------- stdbool.h --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#ifndef _LIBCPP_STDBOOL_H
#define _LIBCPP_STDBOOL_H


/*
    stdbool.h synopsis

Macros:

    __bool_true_false_are_defined

*/

#include <__config>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

#ifdef _LIBCPP_COMPILER_MSVC
#include _LIBCPP_MSVC_INCLUDE(stdbool.h)
#else
#include_next <stdbool.h>
#endif

#ifdef __cplusplus
#undef bool
#undef true
#undef false
#undef __bool_true_false_are_defined
#define __bool_true_false_are_defined 1
#endif

#endif // _LIBCPP_STDBOOL_H
