// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_ROAD_H
#define OLYTAG_ROAD_H

struct road {
    long ent_num;
    char *name;
    long to_loc;
    long hidden;
};

typedef struct road road_t;

#endif //OLYTAG_ROAD_H
