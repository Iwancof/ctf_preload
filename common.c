#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"

typedef void (*log_func_t)(const char *, ...);

void log_impl(const char *message, ...) {
  va_list args;
  va_start(args, message);

  fprintf(stderr, "[libctf_preload] ");
  vfprintf(stderr, message, args);

  va_end(args);
}

void log_noop(const char *message, ...) { (void)message; }

void libctf_log(const char *message, ...) __attribute__((ifunc("resolv_log")));

static log_func_t resolv_log(void) {
  set_envp();

  if (LIBCTF_ENABLE_LOGGING) {
    return log_impl;
  } else {
    return log_noop;
  }

  unset_envp();
}

extern char **_dl_argv;
char **get_trick_envp() {
  // we don't have legal way to get envp in `ld.so`, so we need to use this trick
  // ref: https://stackoverflow.com/questions/20353246/reading-the-environment-when-executing-elf-ifunc-dispatch-functions
  
  int argc = *(int *)(_dl_argv - 1);
  return (char**)(_dl_argv + argc + 1);
}

char **old_environ = NULL;

void set_envp() {
  extern char **environ;

  old_environ = environ;
  environ = get_trick_envp();

}

void unset_envp() {
  extern char **environ;

  environ = old_environ;
}

__attribute__((constructor)) void libctf_init(void) {
  libctf_log("loaded\n");

  if (LIBCTF_DISABLE_SECCOMP) {
    libctf_log("seccomp disabled\n");
  }
}
