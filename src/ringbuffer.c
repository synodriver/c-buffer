#include "ringbuffer.h"

ringbuffer_t *ringbuffer_new(size_t cap)
{
    ringbuffer_t *self = (ringbuffer_t *) BUFFER_MALLOC(sizeof(ringbuffer_t));
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
    self->cap = cap;
    self->head = 0;
    self->tail = 0;
    return self;
}

void ringbuffer_del(ringbuffer_t **self)
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

int ringbuffer_copy_into(ringbuffer_t *self, size_t len, uint8_t *dst)
{
    if (len > ringbuffer_get_size(self)) // 比自己还要长 没门
    {
        return -1;
    }
    if (self->tail >= self->head) // 没有反转
    {
        memcpy(dst, self->data + self->head, len);
    }
    else
    {
        if ((self->cap - self->head) >= len)
        {
            memcpy(dst, self->data + self->head, len);
        }
        else  // 后面部分不够 还要从前面继续copy
        {
            size_t delta = self->cap - self->head; // 后面部分有多少
            memcpy(dst, self->data + self->head, delta);
            memcpy(dst + delta, self->data, len - delta);
        }

    }
    return 0;
}


int ringbuffer_append(ringbuffer_t *self, uint8_t *str, size_t len)
{
    if ((ringbuffer_get_size(self) + len) > (self->cap - 1)) // 已有长度加上他比cap还大 塞不进去了 cap不可能扩大的 -1是为了不重合
    {
        return -1;
    }
    if ((self->tail + len) >= self->cap) // 要回转了
    {
        size_t delta = self->cap - self->tail; // 这一部分写到后面了
        memcpy(self->data + self->tail, str, delta);
        size_t tail = len-delta;
        if (tail > 0) // 剩下的写到前面
        {
            memcpy(self->data, str + delta, tail);
        }
        self->tail = tail;
    }
    else  // 不用回转
    {
        memcpy(self->data+self->tail, str, len);
        self->tail += len;
    }
    return 0;
}

int ringbuffer_pop(ringbuffer_t *self, size_t len)
{
    if (ringbuffer_get_size(self) < len)  // 已有长度小于要pop的 没门
    {
        return -1;
    }
    if ((self->head + len) >= self->cap) // 要回转了
    {
        self->head = self->head + len - self->cap;
    }
    else
    {
        self->head += len;
    }
    return 0;
}