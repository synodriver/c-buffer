#ifndef C_BUFFER_RINGBUFFER_H
#define C_BUFFER_RINGBUFFER_H

#ifndef BUFFER_MALLOC
#define BUFFER_MALLOC malloc
#define BUFFER_REALLOC realloc
#define BUFFER_FREE free
#endif /* BUFFER_MALLOC */

#ifdef BUFFER_INCLUDE
#include BUFFER_INCLUDE
#endif


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ringbuffer_s
{
    uint8_t *data; // 底层数据指针
    size_t cap;   // 底层buf大小
    uint8_t *head; // 头
    uint8_t *tail; // 尾部  // 长度就是尾部减去头部
} ringbuffer_t;

/*
 * direction  ----------------->
 *  data    head                 tail
 * |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *
 * data    tail                  head
 * |  |  |  |  |  |  |  |  |  |  |  |  |  |
 * */
ringbuffer_t* ringbuffer_new(size_t cap);

void ringbuffer_del(ringbuffer_t **self);

inline size_t ringbuffer_get_size(ringbuffer_t *self)
{
    if (self->tail >= self->head)
    {
        return self->tail - self->head;
    }
    else
    {
        return self->cap - (self->head - self->tail);
    }
}

int ringbuffer_append(ringbuffer_t *self, uint8_t* str, size_t len);

int ringbuffer_pop(ringbuffer_t *self, size_t len);


#endif //C_BUFFER_RINGBUFFER_H
