// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_SWEAR_H
#define OLYTAG_SWEAR_H

extern void set_lord(int who, int new_lord, int k, int lev);

extern void swear_char(int who, int target);

extern int unit_deserts(int who, int to_who, int loy_check, int k, int lev);

extern void set_loyal(int who, int k, int lev);

extern int is_unit(int pl, int v);

extern int lord(int n);

extern int player(int n);

#endif //OLYTAG_SWEAR_H
