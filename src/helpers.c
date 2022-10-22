// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#include <string.h>


void bcopy(const void *src, void *dest, size_t n) {
    memcpy(dest, src, n);
}


void bzero(void *s, size_t n) {
    memset(s, 0, n);
}
