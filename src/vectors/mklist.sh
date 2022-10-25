#!/bin/bash
#
# olytag - Olympia: The Age of Gods
#
# Copyright (c) 2022 by the OlyTag authors.
# Please see the LICENSE file in the root directory of this repository for further information.
#

function mklist() {
sed \
  -e "s/_EB_/_ORDER_LIST_/" \
  -e "s/struct entity_build/struct order_list/g" \
  -e "s/eb_ptr/order_list_ptr/g" \
  -e "s/eb_list/order_list_list/g"
}

mklist < eb_list.c > order_list_list.c

mklist < eb_list.h > order_list_list.h

exit 0
