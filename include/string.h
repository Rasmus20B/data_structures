#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <string.h>
#include <stdio.h>

typedef struct String {
  char *data;
  size_t size;
}String;

void string_set(String *str, const char* data) {
  str->size = strlen(data);
  str->data = (char*)data;
}
#endif
