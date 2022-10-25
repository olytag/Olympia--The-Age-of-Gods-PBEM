// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include "windows.h"

#ifdef OLYTAG_CC_WINDOWS

#include <stdlib.h>
#include <stdio.h>
#include <direct.h>


// file_lock tries to lock a file.
// it prints a message to stderr and terminates if it can't.
void file_lock(char *name, int fd) {
    fprintf(stderr, "todo: implement flock on windows!\n");
    exit(-1);
};

// todo: implement
int get_process_id(void) {
    return 54321;
}

int makedir(const char *path, int mode) {
    return _mkdir(path);
}


#endif //OLYTAG_CC_WINDOWS