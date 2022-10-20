// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifdef OLYTAG_CC_LINUX

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>


// file_lock tries to lock a file.
// it prints a message to stderr and terminates if it can't.
void file_lock(char *name, int fd) {
    if (flock(fd, LOCK_EX | LOCK_NB)) {
        fprintf(stderr, "TAG already running in this directory!\n");
        exit(-1);
    };
};

int get_process_id(void) {
    return getpid();
}

int makedir(const char *path, int mode) {
    return = mkdir(path, mode);
}



#endif //OLYTAG_CC_LINUX