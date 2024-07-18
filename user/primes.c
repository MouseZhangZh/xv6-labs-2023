/*
    Created by Mouse Zhang on 2024/7/11.

    SPDX-FileCopyrightText: 2024 Mouse Zhang <mousezhang7@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kernel/types.h"
#include "user/user.h"

__attribute((noreturn))
void primes(int fd[2]) {
    int p;
    if (read(fd[0], &p, sizeof(p))) {
        printf("prime %d\n", p);
    } else {
        exit(0);
    }

    int new_fd[2];
    pipe(new_fd);

    const int pid = fork();
    if (pid == 0) {
        close(new_fd[1]);
        primes(new_fd);
    } else {
        close(new_fd[0]);
        int n;
        while (read(fd[0], &n, sizeof(n))) {
            if (n % p) {
                write(new_fd[1], &n, sizeof(n));
            }
        }
        close(new_fd[1]);
    }
    wait(0);
    exit(0);
}

int main(int argc, char *argv[]) {
    int pipe_fd[2];
    pipe(pipe_fd);

    const int pid = fork();
    if (pid == 0) {
        close(pipe_fd[1]);
        primes(pipe_fd);
    } else if (pid > 0) {
        close(pipe_fd[0]);
        printf("prime 2\n");
        for (int i = 2; i < 36; ++i) {
            if (i % 2 != 0) {
                // printf("write %d\n", i);
                write(pipe_fd[1], &i, sizeof(i));
            }
        }
        close(pipe_fd[1]);
    } else {
        printf("fork error!\n");
    }

    wait(0);
    exit(0);
}
