#ifndef RING_BUF_H
#define RING_BUF_H

#include "c_arr.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct ring_buffer {
  ARR(const char, 4096, data);
  size_t head, tail;
}ring_buffer;

void rb_init(ring_buffer *rb) {
  rb->head = 0;
  rb->tail = 0;
  memset((char*)rb->data, 0, sizeof(rb->data));
}

uint8_t rb_put(ring_buffer *__restrict rb, const char *__restrict data, const size_t size) {
  if(rb->head + size <= sizeof(rb->data)) {
    memcpy((char *)rb->data + rb->head, data, size);
    rb->head += size;
  } else {
    size_t new_head = size - (sizeof(rb->data) - rb->head);
    if(new_head >= rb->tail) {
      fprintf(stderr, "Unable to write to Ring Buffer: Data will overwrite previous data\n");
      return -1;
    }
    memcpy((char *)rb->data + rb->head, data, sizeof(rb->data) - rb->head);
    memcpy((char *)rb->data, data + (sizeof(rb->data) - rb->head), new_head);
    rb->head = new_head;
  }
  return 0;
}

uint8_t rb_put_ow(ring_buffer *__restrict rb, const char *__restrict data, const size_t size) {
  if(rb->head + size <= sizeof(rb->data)) {
    memcpy((char *)rb->data + rb->head, data, size);
  } else {
    memcpy((char *)rb->data + rb->head, data, sizeof(rb->data) - rb->head);
    size_t new_head = size - (sizeof(rb->data) - rb->head);
    memcpy((char *)rb->data, data + (sizeof(rb->data) - rb->head), new_head);
    rb->head = new_head;
  }
  return 0;
}

void rb_get(ring_buffer *__restrict rb, char *__restrict buf, const size_t size, const size_t buf_size) {

  if(size > buf_size) fprintf(stderr, "Unable to read to buffer: Buffer smaller than requested data size\n");

  if(rb->head > rb->tail && rb->tail + size > rb->head) {
    memcpy(buf, (char *)rb->data + rb->tail, size - (rb->tail - rb->head));
    memset((char*)rb->data + rb->tail, 0, rb->head - rb->tail);
    rb->tail += (rb->head - rb->tail);
  } else if(rb->tail + size <= sizeof(rb->data)) {
    memcpy(buf, (char *)rb->data + rb->tail, size);
    memset((char*)rb->data + rb->tail, 0, size);
    rb->tail += size;
  } else {
    memcpy(buf, (char *)rb->data + rb->tail, sizeof(rb->data) - rb->tail);
    memset((char*)rb->data + rb->tail, 0, sizeof(rb->data) - rb->tail);
    memcpy(buf, (char *)rb->data, size - (sizeof(rb->data) - rb->tail));
    memset((char*)rb->data + rb->tail, 0, sizeof(rb->data) - rb->tail);
    rb->tail = sizeof(rb->data) - rb->tail;
  }
}

#endif
