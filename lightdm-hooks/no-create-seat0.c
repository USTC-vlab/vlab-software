#define _GNU_SOURCE
#include <stdarg.h>
#include <string.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// Hook strcmp
int strcmp(const char *s1, const char *s2) {
    static int (*real_strcmp)(const char *, const char *) = NULL;
    if (!real_strcmp) {
        real_strcmp = dlsym(RTLD_NEXT, "strcmp");
    }

    // 特殊处理 seat0
    if (s2 && real_strcmp(s2, "seat0") == 0) {
        return 1;
    }

    return real_strcmp(s1, s2);
}

int execve(const char *pathname, char *const argv[], char *const envp[]) {
    static int (*real_execve)(const char *, char *const[], char *const[]) = NULL;
    if (!real_execve) {
        real_execve = dlsym(RTLD_NEXT, "execve");
    }

    // Unset LD_PRELOAD to avoid affecting child processes
    unsetenv("LD_PRELOAD");

    // Call the original execve
    return real_execve(pathname, argv, envp);
}

int execvp(const char *file, char *const argv[]) {
    static int (*real_execvp)(const char *, char *const[]) = NULL;
    if (!real_execvp) {
        real_execvp = dlsym(RTLD_NEXT, "execvp");
    }

    // Same trick: unset LD_PRELOAD before executing new program
    unsetenv("LD_PRELOAD");

    return real_execvp(file, argv);
}
