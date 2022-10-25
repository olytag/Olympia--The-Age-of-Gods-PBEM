// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include "mac.h"

#ifdef OLYTAG_CC_MAC

#include <stdlib.h>
#include <stdio.h>

// file_lock tries to lock a file.
// it prints a message to stderr and terminates if it can't.
void file_lock(char *name, int fd) {
    fprintf(stderr, "todo: implement flock on mac!\n");
    exit(-1);
};

int get_process_id(void) {
    return getpid();
}

int makedir(const char *path, int mode) {
    return = mkdir(path, mode);
}

#endif //OLYTAG_CC_MAC
