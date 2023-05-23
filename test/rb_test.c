#include <assert.h>

#include "../include/ring_buf.h"
#include "../include/string.h"

int main() {
  
  ring_buffer rb;

  String str;
  
  string_set(&str, "THIS IS A TEST STRING FOR THE RB TEST");

  rb_init(&rb);
  assert(rb_put(&rb, str.data, str.size) == -1);
  assert(rb_put_ow(&rb, str.data, str.size) == 0);

  char *recv = malloc(str.size + 3);
  rb_get(&rb, recv, 15, str.size);
  printf("%s : head = %lu, tail = %lu\n", recv, rb.head, rb.tail);
  rb_get(&rb, recv, 10, str.size);
  printf("%s : head = %lu, tail = %lu\n", recv, rb.head, rb.tail);
  rb_get(&rb, recv, 10, str.size);
  printf("%s : head = %lu, tail = %lu\n", recv, rb.head, rb.tail);

  
}
