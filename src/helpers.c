// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <direct.h>
#endif

void bcopy(const void *src, void *dest, size_t n) {
    memcpy(dest, src, n);
}

void bzero(void *s, size_t n) {
    memset(s, 0, n);
}



int makedir(const char *path, int mode) {
    int rc;
#ifdef __linux__
    rc = mkdir(path, mode);
#else
    rc = _mkdir(path);
#endif
    return rc;
}