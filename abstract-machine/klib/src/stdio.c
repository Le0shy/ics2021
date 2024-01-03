#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define MAX_PRINT 1024
// #define ALL_SPECIFIER(f) f(d) f(x) f(s) f(c) f(p)
// #define def_SPECIFIER(name) CONCAT(spec_, name),
// enum {
//   MAP(ALL_SPECIFIER, def_SPECIFIER) TOTAL_SPEC
// };

// #define def_SPEC_TABLE(name) [CONCAT(spec_, name)] = ((#name)[0]),
// static const char spec_table[TOTAL_SPEC] = {
//   MAP(ALL_SPECIFIER, def_SPEC_TABLE)
// };

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...){
  panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

static int itoa (char* res, int num) {
  char buf[10];
  int size = 0;
  int digits;

  if(num == 0){
    *res = '0';
    return 1;
  }
  for(; num != 0; num /= 10){
    buf[size++] = '0' + (char)(num % 10);
  }
  digits = size;
  for(int i = 0; i < digits; i +=1){
    size -= 1;
    res[i] = buf[size];
  }

  return digits;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  //panic("Not implemented");
  if(n > 1024) {
    panic("not implemented: invalid input for vsnprintf");
  }
  size_t iter = 0;
  char buf[MAX_PRINT];
  size_t buf_size = 0;

  int integer;
  char * string;
  while(fmt[iter] != 0) {
    if(fmt[iter] != '%'){
      buf[buf_size++] = fmt[iter];
    }
    else {
      switch(fmt[iter]) {
        case 'd':
          integer = va_arg(ap, int);
          if (integer < 0){
            buf[buf_size++] = '-';
            integer = -integer;
          }
          buf_size += itoa(&buf[buf_size], integer);
          break;
        case 's':
          string = va_arg(ap, char*);
          strcpy(&buf[buf_size], string);
          buf_size += strlen(string);
          break;
        default: panic("not implemented: other specifiers or invalid input ");
      }
    }
    iter += 1;
  }
  return buf_size;

}

#endif
