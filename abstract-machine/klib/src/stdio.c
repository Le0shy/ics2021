#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


enum SPECIFIER_TABLE{
  d
};

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  //panic("Not implemented");
  size_t len = strlen(fmt);
  size_t iter = 0;
  char buf[1024];
  size_t buf_size = 0;
  while(iter < n) {
    if(fmt[iter] != '%'){
      buf[buf_size] = fmt[iter];
    }
    else {
      iter += 1;
      switch(fmt[iter]) {
        case 'd':;
        default:;
      }
    }
  }
}

#endif
