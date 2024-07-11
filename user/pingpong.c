/*
    Created by Mouse Zhang on 2024/7/10.

    SPDX-FileCopyrightText: 2024 Mouse Zhang <mousezhang7@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p[2];
    char buf[64] = { 0 };

    pipe(p);

    const int pid = fork();
    if (pid > 0) {
        const char *args = "x";
        write(p[1], args, sizeof(args));
        read(p[0], buf, sizeof(buf));
        printf("%d: received pong\n", getpid());
    } else if (pid == 0) {
        const int n = read(p[0], buf, sizeof(buf));
        printf("%d: received ping\n", getpid());
        write(p[1], buf, n);
    } else {
        printf("fork error!\n");
        exit(-1);
    }
    close(p[0]);
    close(p[1]);
    exit(0);
}
