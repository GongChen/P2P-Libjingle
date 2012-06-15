/*
 * libjingle
 * Copyright 2004--2010, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "talk/base/latebindingsymboltable.h"

#ifdef LINUX
#include <dlfcn.h>
#endif

#include "talk/base/logging.h"

namespace talk_base {

inline static const char *GetDllError() {
#ifdef LINUX
  const char *err = dlerror();
  if (err) {
    return err;
  } else {
    return "No error";
  }
#else
#error Not implemented
#endif
}

DllHandle InternalLoadDll(const char dll_name[]) {
#ifdef LINUX
  DllHandle handle = dlopen(dll_name, RTLD_NOW);
#else
#error Not implemented
#endif
  if (handle == kInvalidDllHandle) {
    LOG(LS_WARNING) << "Can't load " << dll_name << ": " << GetDllError();
  }
  return handle;
}

void InternalUnloadDll(DllHandle handle) {
#ifdef LINUX
  if (dlclose(handle) != 0) {
    LOG(LS_ERROR) << GetDllError();
  }
#else
#error Not implemented
#endif
}

static bool LoadSymbol(DllHandle handle,
                       const char *symbol_name,
                       void **symbol) {
#ifdef LINUX
  *symbol = dlsym(handle, symbol_name);
  const char *err = dlerror();
  if (err) {
    LOG(LS_ERROR) << "Error loading symbol " << symbol_name << ": " << err;
    return false;
  } else if (!*symbol) {
    LOG(LS_ERROR) << "Symbol " << symbol_name << " is NULL";
    return false;
  }
  return true;
#else
#error Not implemented
#endif
}

// This routine MUST assign SOME value for every symbol, even if that value is
// NULL, or else some symbols may be left with uninitialized data that the
// caller may later interpret as a valid address.
bool InternalLoadSymbols(DllHandle handle,
                         int num_symbols,
                         const char *const symbol_names[],
                         void *symbols[]) {
#ifdef LINUX
  // Clear any old errors.
  dlerror();
#endif
  for (int i = 0; i < num_symbols; ++i) {
    if (!LoadSymbol(handle, symbol_names[i], &symbols[i])) {
      return false;
    }
  }
  return true;
}

}  // namespace talk_base
