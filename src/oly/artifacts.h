// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_ARTIFACTS_H
#define OLYTAG_ARTIFACTS_H

int best_artifact(int who, int type, int param2, int uses);
int create_random_artifact(int monster);
int has_artifact(int who, int type, int p1, int p2, int charges);
int create_specific_artifact(int monster, int type);

#endif //OLYTAG_ARTIFACTS_H
