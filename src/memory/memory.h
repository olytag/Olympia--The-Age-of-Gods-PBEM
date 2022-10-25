// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_MEMORY_H
#define OLYTAG_MEMORY_H

void *my_malloc(unsigned size);
void *my_realloc(void *ptr, unsigned size);
void my_free(void *ptr);

extern int my_malloc_size;
extern int my_realloc_size;

#endif //OLYTAG_MEMORY_H
