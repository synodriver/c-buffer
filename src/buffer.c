//
// Created by jhc on 2022/8/26.
//
#include "buffer.h"

buffer_t *buffer_new(size_t cap)
{
    buffer_t *self = (buffer_t *) BUFFER_MALLOC(sizeof(buffer_t));
    if (self == NULL)
    {
        return NULL;
    }
    self->data = (uint8_t *) BUFFER_MALLOC(cap);
    if (self->data == NULL)
    {
        BUFFER_FREE(self);
        return NULL;
    }
    self->len = 0;
    self->cap = cap;
    return self;
}

buffer_t *buffer_new_from_string_and_size(uint8_t *str, size_t len)
{
    buffer_t *self = (buffer_t *) BUFFER_MALLOC(sizeof(buffer_t));
    if (self == NULL)
    {
        return NULL;
    }
    self->data = (uint8_t *) BUFFER_MALLOC(len);
    if (self->data == NULL)
    {
        BUFFER_FREE(self);
        return NULL;
    }
    memcpy(self->data, str, len);
    self->len = len;
    self->cap = len;
    return self;
}

int buffer_append_right(buffer_t *self, uint8_t *str, size_t len)
{
    if ((self->len + len) > self->cap)// cap不够了
    {
        size_t dst = 2 * (self->len + len); // 2倍扩张
        self->data = (uint8_t *) BUFFER_REALLOC(self->data, dst);
        if (self->data == NULL)
        {
            return -1;
        }
        self->cap = dst;
    }
    memcpy(self->data + self->len, str, len);
    self->len += len;
    return 0;
}

int buffer_pop_left(buffer_t *self, size_t len)
{
    if(len>self->len) // 比自己还长 应该报错
    {
        return -1;
    }
    self->len -= len;
    memmove(self->data, self->data + len, self->len);
    if (self->cap > (2 * self->len)) // pop了很多数据的话可以适当减少cap
    {
        size_t dst = self->len + 10; // 缩小
        self->data = (uint8_t *) BUFFER_REALLOC(self->data, dst);
        if (self->data == NULL)
        {
            return -1;
        }
        self->cap = dst;
    }
    return 0;
}

int buffer_make_room_for(buffer_t *self, size_t size)
{
    if ((self->cap - self->len)<size) // 空闲空间不够大
    {
        size_t dst = self->cap + size;
        self->data = (uint8_t *) BUFFER_REALLOC(self->data, dst);
        if (self->data == NULL)
        {
            return -1;
        }
        self->cap = dst;
    }
    return 0;
}

void buffer_del(buffer_t **self)
{
    if (*self != NULL)
    {
        if ((*self)->data != NULL)
        {
            BUFFER_FREE((*self)->data);
            (*self)->data = NULL;
        }
        BUFFER_FREE(*self);
        *self = NULL;
    }
}