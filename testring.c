#include "assert.h"
#include "ringbuffer.h"

int main(int argc, char **argv)
{
    ringbuffer_t *buf = ringbuffer_new(20); // 20cap的buf 最多可以装19字节
    if (buf == NULL)
    {
        return -1;
    }
    uint8_t result[20];
    for (int i = 0; i < 10000; i++)
    {
        assert(ringbuffer_get_size(buf) == 0); // 初始长度肯定是0
        assert(ringbuffer_append(buf, "1234", 4) == 0); //肯定塞得进去
        assert(ringbuffer_get_size(buf) == 4); // 现在肯定是4
        assert(ringbuffer_append(buf, "123412341234", 12) == 0); //肯定塞得进去
        assert(ringbuffer_get_size(buf) == 16); // 现在肯定是16 哪怕是回转了
        assert(ringbuffer_copy_into(buf, 4, result) == 0);
        assert(memcmp(result, "1234", 4) == 0);
        assert(ringbuffer_pop(buf, 1) == 0);
        assert(ringbuffer_get_size(buf) == 15); // 现在肯定是15
        assert(ringbuffer_copy_into(buf, 4, result) == 0);
        assert(ringbuffer_copy_into(buf, 19, result) == -1); // 应该失败 没这么大
        assert(memcmp(result, "2341", 4) == 0);

        assert(ringbuffer_copy_into(buf, 15, result) == 0);
        assert(memcmp(result, "234123412341234", 15) == 0);
        assert(ringbuffer_pop(buf, 15) == 0);
        assert(ringbuffer_get_size(buf) == 0); // 现在0
        assert(ringbuffer_pop(buf, 1) == -1);  // 应该失败 已经空了

        assert(ringbuffer_append(buf, result, 20) == -1);// 应该失败 只能塞19

    }
    ringbuffer_del(&buf);
    assert(buf == NULL);
    printf("Tests are passed!!\n");

}