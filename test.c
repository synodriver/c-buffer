#include <stdio.h>
#include <string.h>

#include "assert.h"
#include "./inc/buffer.h"


int main(int argc, char** argv) {
    buffer_t buf;
    buffer_new_from_string(&buf, "12345");
    assert(memcmp(buffer_as_string(&buf), "12345", buffer_get_size(&buf)) == 0);
    assert(buffer_get_size(&buf)==5);
    fwrite(buffer_as_string(&buf), 1, buffer_get_size(&buf), stdout);
    putchar('\n');
    buffer_append_right(&buf, "12345", 5);
    fwrite(buffer_as_string(&buf), 1, buffer_get_size(&buf), stdout);
    putchar('\n');
    assert(buffer_get_size(&buf)==10);
    assert(memcmp(buffer_as_string(&buf), "1234512345", buffer_get_size(&buf)) == 0);
    buffer_pop_left(&buf, 1);
    assert(buffer_get_size(&buf)==9);
    assert(memcmp(buffer_as_string(&buf), "234512345", buffer_get_size(&buf)) == 0);
    fwrite(buffer_as_string(&buf), 1, buffer_get_size(&buf), stdout);
    putchar('\n');

    buffer_del(&buf);

    return 0;
}
