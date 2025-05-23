/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│ vi: set et ft=c ts=2 sts=2 sw=2 fenc=utf-8                               :vi │
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/intrin/directmap.h"
#include "libc/nt/enum/pageflags.h"
#include "libc/sysv/consts/prot.h"

privileged int __prot2nt(int prot, int iscow) {
  switch (prot & (PROT_READ | PROT_WRITE | PROT_EXEC)) {
    case PROT_READ:
      return kNtPageReadonly;
    case PROT_EXEC:
    case PROT_EXEC | PROT_READ:
      return kNtPageExecuteRead;
    case PROT_WRITE:
    case PROT_READ | PROT_WRITE:
      if (iscow) {
        return kNtPageWritecopy;
      } else {
        return kNtPageReadwrite;
      }
    case PROT_WRITE | PROT_EXEC:
    case PROT_READ | PROT_WRITE | PROT_EXEC:
      if (iscow) {
        return kNtPageExecuteWritecopy;
      } else {
        return kNtPageExecuteReadwrite;
      }
    default:
      if (prot & PROT_GUARD)
        return kNtPageReadwrite | kNtPageGuard;
      return kNtPageNoaccess;
  }
}
