// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_RANDOM_RANDOM_H
#define OLYTAG_RANDOM_RANDOM_H


// rnd returns a pseudo-random value in the range of low...high
int rnd(int low, int high);

void load_seed(char *seedFileName);

void save_seed(char *seedFileName);

#endif //OLYTAG_RANDOM_RANDOM_H
