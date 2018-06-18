@ This file was created from a .asm file
@  using the ads2gas_apple.pl script.

	.set WIDE_REFERENCE, 0
	.set ARCHITECTURE, 5
	.set DO1STROUNDING, 0
 @
 @  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 @
 @  Use of this source code is governed by a BSD-style license
 @  that can be found in the LICENSE file in the root of the source
 @  tree. An additional intellectual property rights grant can be found
 @  in the file PATENTS.  All contributing project authors may
 @  be found in the AUTHORS file in the root of the source tree.
 @


    .globl _vpx_push_neon
	.globl vpx_push_neon
    .globl _vpx_pop_neon
	.globl vpx_pop_neon

   @ ARM
   @ 
   @ PRESERVE8

.text
.p2align 2

_vpx_push_neon:
	vpx_push_neon: @
    vstm            r0!, {d8-d15}
    bx              lr

    @

_vpx_pop_neon:
	vpx_pop_neon: @
    vldm            r0!, {d8-d15}
    bx              lr

    @


