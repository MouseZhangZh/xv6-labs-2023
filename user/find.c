/*
    Created by Mouse Zhang on 2024/7/11.

    SPDX-FileCopyrightText: 2024 Mouse Zhang <mousezhang7@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void tranverse_dir(char* path, char* name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_DEVICE:
        case T_FILE:
            // The buf in fmtname is static, so we need to copy it to a new buffer.
            // If we don't do this, strcmp(fmtname(path), name) will always return 0.
            memset(name+strlen(name), ' ', DIRSIZ-strlen(name));
            if (strcmp(name, "") != 0 && strcmp(fmtname(path), name) == 0) {
                printf("%s\n", path);
            }
            break;

        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (stat(buf, &st) < 0) {
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                    tranverse_dir(buf, name);
                }
            }
            break;
    }
    close(fd);
}

void find(char* path, char* name) {
    if (strcmp(path, "") == 0) {
        path = ".";
    }
    tranverse_dir(path, name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        find(".", "");
        exit(0);
    }
    if (argc == 2) {
        find(argv[1], "");
        exit(0);
    }
    for (int i = 2; i < argc; ++i) {
        find(argv[1], argv[i]);
    }
    exit(0);
}