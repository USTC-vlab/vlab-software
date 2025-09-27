#define _GNU_SOURCE
#include <string.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/prctl.h>

#define TARGET ".xsession-errors"

static int pipe_w = -1;
static pid_t drainer_pid = -1;
static int hook_disabled = 0;

// TODO: Drop this to a separate helper executable?
static int ensure_drainer(void) {
    if (pipe_w != -1 && drainer_pid > 0) {
        if (kill(drainer_pid, 0) == 0 || errno == EPERM) {
            return 0;
        }
        close(pipe_w); pipe_w = -1;
        drainer_pid = -1;
    }

    int fds[2];
    if (pipe(fds) != 0) {
        return -1;
    }


    pid_t pid = fork();
    if (pid < 0) {
        close(fds[0]);
        close(fds[1]);
        return -1;
    }

    if (pid == 0) {
        hook_disabled = 1;

        prctl(PR_SET_NAME, "xsess-err-fwd");

        int rfd = fds[0];
        int wfd = fds[1];
        close(wfd);

        signal(SIGPIPE, SIG_IGN);

        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        const char *logpath = "/dev/log";
        strncpy(addr.sun_path, logpath, sizeof(addr.sun_path) - 1);

        const size_t CHUNK = 1024;
        char buf[8192];

        for (;;) {
            ssize_t n = read(rfd, buf, sizeof(buf));
            if (n == 0) {
                break;
            }
            if (n < 0) {
                if (errno == EINTR) continue;
                break;
            }
            size_t off = 0;
            while (off < (size_t)n) {
                size_t to_send = (size_t)n - off;
                if (to_send > CHUNK) to_send = CHUNK;

                int s = socket(AF_UNIX, SOCK_DGRAM | SOCK_CLOEXEC, 0);
                if (s >= 0) {
                    ssize_t sent = sendto(s, buf + off, to_send, 0,
                                          (struct sockaddr *)&addr, sizeof(addr));
                    (void)sent;
                    close(s);
                }
                off += to_send;
            }
        }

        close(rfd);
        _exit(0);
    }
    close(fds[0]);
    pipe_w = fds[1];
    drainer_pid = pid;
    return 0;
}

static int make_writer_fd_from_pipe(void) {
    if (ensure_drainer() != 0) {
        errno = EIO;
        return -1;
    }
    int fd = fcntl(pipe_w, F_DUPFD, 0);
    if (fd < 0) return -1;

    return fd;
}

int open(const char *pathname, int flags, ...) {
    static int (*real_open)(const char *, int, ...) = NULL;
    if (!real_open) {
        real_open = dlsym(RTLD_NEXT, "open");
    }
    mode_t mode = 0;
    int has_mode = 0;
    if (flags & O_CREAT) {
        va_list ap; va_start(ap, flags);
        mode = va_arg(ap, mode_t);
        va_end(ap);
        has_mode = 1;
    }

    if (pathname && strcmp(pathname, TARGET) == 0) {
        return make_writer_fd_from_pipe();
    }
    if (has_mode) {
        return real_open(pathname, flags, mode);
    } else {
        return real_open(pathname, flags);
    }
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
