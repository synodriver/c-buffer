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
    size_t cap;   // 底层buf大小  实际上只能近cap-1
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

// 取得长度
inline size_t ringbuffer_get_size(ringbuffer_t *self)
{
    if (self->tail >= self->head) // 指针重叠的时候 大小视为0
    {
        return self->tail - self->head;
    }
    else
    {
        return self->cap - (self->head - self->tail);
    }
}

// 取得底层指针
inline uint8_t * ringbuffer_get_data(ringbuffer_t *self)
{
    return self->data;
}

// 把这个buf按照head和tail的规范 拷贝进dst dst的长度必须大于ringbuffer_get_size(self)
void ringbuffer_copy_into(ringbuffer_t *self, uint8_t* dst);

//成功返回0 失败-1
int ringbuffer_append(ringbuffer_t *self, uint8_t* str, size_t len);

//成功返回0 失败-1
int ringbuffer_pop(ringbuffer_t *self, size_t len);


#endif //C_BUFFER_RINGBUFFER_H
