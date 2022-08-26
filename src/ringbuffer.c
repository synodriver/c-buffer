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
    self->head = self->data;
    self->tail = self->data;
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

int ringbuffer_append(ringbuffer_t *self, uint8_t *str, size_t len)
{
    if ((ringbuffer_get_size(self) + len) > (self->cap-1)) // 已有长度加上他比cap还大 塞不进去了 cap不可能扩大的 -1是为了不重合
    {
        return -1;
    }
    if ((self->tail + len) > (self->data + self->cap)) // 要回转了
    {
        size_t delta = (self->tail + len) - (self->data + self->cap); // 这一部分要写到前面了
        memcpy(self->tail, str, len - delta); // 剩下的写到后面
        memcpy(self->data, str + len - delta, delta); // 写到前面
        self->tail = self->data + delta;
    }
    else  // 不用回转
    {
        memcpy(self->tail, str, len);
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
    if ((self->head + len) > (self->data + self->cap)) // 要回转了
    {
        self->head = self->head + len - self->cap;
    }
    else
    {
        self->head += len;
    }
    return 0;
}