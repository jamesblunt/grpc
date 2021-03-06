/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __GRPC_SUPPORT_SLICE_BUFFER_H__
#define __GRPC_SUPPORT_SLICE_BUFFER_H__

#include <grpc/support/slice.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Represents an expandable array of slices, to be interpreted as a single item
   TODO(ctiller): inline some small number of elements into the struct, to
                  avoid per-call allocations */
typedef struct {
  /* slices in the array */
  gpr_slice *slices;
  /* the number of slices in the array */
  size_t count;
  /* the number of slices allocated in the array */
  size_t capacity;
  /* the combined length of all slices in the array */
  size_t length;
} gpr_slice_buffer;

/* initialize a slice buffer */
void gpr_slice_buffer_init(gpr_slice_buffer *sb);
/* destroy a slice buffer - unrefs any held elements */
void gpr_slice_buffer_destroy(gpr_slice_buffer *sb);
/* Add an element to a slice buffer - takes ownership of the slice.
   This function is allowed to concatenate the passed in slice to the end of
   some other slice if desired by the slice buffer. */
void gpr_slice_buffer_add(gpr_slice_buffer *sb, gpr_slice slice);
/* add an element to a slice buffer - takes ownership of the slice and returns
   the index of the slice.
   Guarantees that the slice will not be concatenated at the end of another
   slice (i.e. the data for this slice will begin at the first byte of the
   slice at the returned index in sb->slices)
   The implementation MAY decide to concatenate data at the end of a small
   slice added in this fashion. */
size_t gpr_slice_buffer_add_indexed(gpr_slice_buffer *sb, gpr_slice slice);
void gpr_slice_buffer_addn(gpr_slice_buffer *sb, gpr_slice *slices, size_t n);
/* add a very small (less than 8 bytes) amount of data to the end of a slice
   buffer: returns a pointer into which to add the data */
gpr_uint8 *gpr_slice_buffer_tiny_add(gpr_slice_buffer *sb, unsigned len);
/* clear a slice buffer, unref all elements */
void gpr_slice_buffer_reset_and_unref(gpr_slice_buffer *sb);

#ifdef __cplusplus
}
#endif

#endif /* __GRPC_SUPPORT_SLICE_BUFFER_H__ */
