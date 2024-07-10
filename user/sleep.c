/*
    Created by Mouse Zhang on 2024/7/10.

    SPDX-FileCopyrightText: 2024 Mouse Zhang <mousezhang7@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("please input time.\n");
        exit(-1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}
