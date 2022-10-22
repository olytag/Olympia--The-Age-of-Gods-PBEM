// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_GENERIC_H
#define OLYTAG_GENERIC_H

// file_lock tries to lock a file.
// it prints a message to stderr and terminates if it can't.
void file_lock(char *name, int fd);

int get_process_id(void);
int makedir(const char *path, int mode);

#endif //OLYTAG_GENERIC_H
