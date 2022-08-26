//
// Created by jhc on 2022/8/26.
// Modified by fumiama 2022/8/26
//

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/buffer.h"

static inline size_t min_cap(size_t target) {
    for(int i = 0; i < sizeof(size_t)*8; i++) {
        if(target) {
            target >>= 1;
        } else {
            if(i > 24) i = 24;
            return 1<<i;
        }
    }
    return 16;
}

buffer_t *buffer_init(buffer_t *buf, size_t cap) {
    if(buf == NULL) { return NULL; }
    cap = min_cap(cap);
    buf->data = buf->ptr = (uint8_t*) malloc(cap);
    if (buf->data == NULL) { return NULL; }
    buf->len = 0;
    buf->cap = cap;
    return buf;
}

buffer_t *buffer_new_from_string_and_size(buffer_t *buf, const uint8_t* restrict str, size_t len) {
    if(buf == NULL) { return NULL; }
    size_t cap = min_cap(len);
    buf->data = buf->ptr = (uint8_t*) malloc(cap);
    if (buf->data == NULL) { return NULL; }
    memcpy(buf->data, str, len);
    buf->len = len;
    buf->cap = cap;
    return buf;
}

int buffer_append_right(buffer_t *self, const uint8_t* restrict str, size_t len) {
    if (buffer_make_room_for(self, len)) { return -1; }
    memcpy(self->data + self->len, str, len);
    self->len += len;
    return 0;
}

int buffer_pop_left(buffer_t *self, size_t len) {
    if(len>self->len) { return -1; }    // 比自己还长 应该报错
    self->data += len;
    self->len -= len;
    return 0;
}

int buffer_make_room_for(buffer_t *self, size_t size) {
    if ((self->len + size) > self->cap) {    // cap不够了
        size_t delta = self->data - self->ptr;
        size_t dst = min_cap(self->len + size + delta); // 指数扩张
        self->ptr = (uint8_t*) realloc(self->ptr, dst);
        if (self->ptr == NULL) { return -1; }
        self->data = self->ptr + delta;
        self->cap = dst;
    }
    return 0;
}

void buffer_del(buffer_t *self) {
    if (self == NULL || self->ptr == NULL) return;
    free(self->ptr);
}
