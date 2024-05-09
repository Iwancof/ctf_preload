#include <stdbool.h>

extern bool log_enabled;
extern bool disable_seccomp;

extern void libctf_log(const char* message, ...);

// If target binary has FULL RELRO, ifunc resolvers are called before _init functions which includes `environ` setup in glibc.
// so, we need to use envp inside of `ld` instead of `environ`.
void set_envp();

void unset_envp();

#define LIBCTF_ENABLE_LOGGING (getenv("LC_LOG") != NULL)
#define LIBCTF_DISABLE_SECCOMP (getenv("LC_DISABLE_SECCOMP") != NULL)
