#ifndef C_BUFFER_BUFFER_H
#define C_BUFFER_BUFFER_H

#ifndef BUFFER_MALLOC
#define BUFFER_MALLOC malloc
#define BUFFER_REALLOC realloc
#define BUFFER_FREE free
#endif /* BUFFER_MALLOC */

#ifdef BUFFER_INCLUDE
#include BUFFER_INCLUDE
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define cast(type, obj) (type)(obj)

typedef struct buffer_s
{
    uint8_t *data;
    size_t len;
    size_t cap;
} buffer_t;

// 仅仅分配内存,不初始化
buffer_t *buffer_new(size_t cap);

#define buffer_new_from_string(str) buffer_new_from_string_and_size((uint8_t*)(str), strlen(str))

// 分配内存,执行深拷贝 cap就是len
buffer_t *buffer_new_from_string_and_size(uint8_t *str, size_t len);

inline size_t buffer_get_size(buffer_t *self)
{
    return self->len;
}

inline size_t buffer_get_cap(buffer_t *self)
{
    return self->cap;
}

inline uint8_t *buffer_as_string(buffer_t *self)
{
    return self->data;
}

// append ， 执行深拷贝 成功返回0 失败返回-1
int buffer_append_right(buffer_t *self, uint8_t *str, size_t len);

// 从buffer里面清除多少数据 等效 del buffer[:len] 成功返回0 失败返回-1
int buffer_pop_left(buffer_t *self, size_t len);

// 确保（self.cap-self.len）>size, 视情况可能会realloc, 可能会改变cap，len不会改变
int buffer_make_room_for(buffer_t *self, size_t size);

// 析构函数
void buffer_del(buffer_t **self);

#endif //C_BUFFER_BUFFER_H
