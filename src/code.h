// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_CODE_H
#define OLYTAG_CODE_H

extern char *int_to_code(int l);

extern int code_to_int(char *s);

extern int scode(char *s);

extern char *name(int n);

extern void set_name(int n, char *s);

extern void set_banner(int n, char *s);

extern char *display_name(int n);

extern char *display_kind(int n);

extern char *box_code(int n);

extern char *box_code_less(int n);

extern char *box_name(int n);

extern char *just_name(int n);

extern char *just_name_qty(int item, int qty);

extern char *plural_item_name(int item, int qty);

extern char *plural_item_box(int item, int qty);

extern char *box_name_qty(int item, int qty);

extern char *box_name_kind(int n);

#endif //OLYTAG_CODE_H