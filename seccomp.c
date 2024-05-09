#include <dlfcn.h>
#include <seccomp.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#include <unistd.h>

typedef int (*seccomp_load_t)(scmp_filter_ctx ctx);

int disabled_seccomp_load(scmp_filter_ctx ctx) {
  libctf_log("seccomp_load(%p) -> disabled\n", ctx);
  (void)ctx;

  return 0;
}

int seccomp_load(scmp_filter_ctx ctx)
    __attribute__((ifunc("resolv_seccomp_load")));

static seccomp_load_t resolv_seccomp_load(void) {
  seccomp_load_t f;

  set_envp();

  if (LIBCTF_DISABLE_SECCOMP) {
    f = disabled_seccomp_load;
  } else {
    f = dlsym(RTLD_NEXT, "seccomp_load");
  }

  unset_envp();

  return f;
}

