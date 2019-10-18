// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This is a shim that injects Chrome-specific defitions into sqlite3.c
// BUILD.gn uses this instead of building the sqlite3 amalgamation directly.

// We prefix chrome_ to SQLite's exported symbols, so that we don't clash with
// other SQLite libraries loaded by the system libraries. This only matters when
// using the component build, where our SQLite's symbols are visible to the
// dynamic library loader.
#include "third_party/sqlite/amalgamation/rename_exports.h"

// Linux-specific configuration fixups.
#if defined(__linux__)

// features.h, included below, indirectly includes sys/mman.h. The latter header
// only defines mremap if _GNU_SOURCE is defined. Depending on the order of the
// files in the amalgamation, removing the define below may result in a build
// error on Linux.
#if defined(__GNUC__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <features.h>

// SQLite wants to track malloc sizes. On OSX it uses malloc_size(), on Windows
// _msize(), elsewhere it handles it manually by enlarging the malloc and
// injecting a field. Enable malloc_usable_size() for Linux.
//
// malloc_usable_size() is not exported by the Android NDK. It is not
// implemented by uclibc.
#if !defined(__UCLIBC__) && !defined(__ANDROID__)
#define HAVE_MALLOC_H 1
#define HAVE_MALLOC_USABLE_SIZE 1
#endif

#endif  // defined(__linux__)

// For unfortunately complex reasons, Chrome has release builds where
// DCHECK_IS_ON() (so we want SQLITE_DEBUG to be on) but NDEBUG is also defined.
// This causes declarations for mutex-checking functions used by SQLITE_DEBUG
// code (sqlite3_mutex_held, sqlite3_mutex_notheld) to be omitted, resulting in
// warnings.
//
// The easiest solution for now is to undefine NDEBUG when SQLITE_DEBUG is
// defined. The #undef only takes effect for the SQLite implementation (included
// below), and does not impact any dependency.
#if defined(SQLITE_DEBUG) && defined(NDEBUG)
#undef NDEBUG
#endif  // defined(SQLITE_DEBUG) && defined(NDEBUG)

#include "third_party/sqlite/amalgamation/sqlite3.c"