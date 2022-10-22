# vectors
This directory consolidates the `ilist` code.

    a collection of reallocation lists
    length is stored in list[0], maximum in list[1].
    the user-visible list entries are shifted to &list[2],
    so that iterations can start from index 0.

