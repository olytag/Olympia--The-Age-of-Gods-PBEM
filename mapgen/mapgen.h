// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_MAPGEN_H
#define OLYTAG_MAPGEN_H

#include "z.h"

//void add_road(struct tile *from,long to_loc,long hidden,char *name);
//struct tile *adjacent_tile_water(long row, long col);
//struct tile *adjacent_tile_sup(long row, long col, long dir);
//struct tile *adjacent_tile_terr(long row, long col);
long alloc_inside(void);
void bridge_caddy_corners(void);
int bridge_corner_sup(long row, long col);
long bridge_map_hole_sup(long row, long col);
void bridge_map_holes(void);
void bridge_mountain_ports(void);
void bridge_mountain_sup(long row, long col);
void clear_alloc_flag(void);
void clear_province_marks(void);
void clear_subloc_marks(void);
void count_cities(void);
void count_continents(void);
void count_subloc_coverage(void);
void count_sublocs(void);
void count_tiles(void);
long create_a_building(long sl, long hidden, long kind);
void create_a_graveyard(long row, long col);
long create_a_subloc(long row, long col, long hidden, long kind);
long create_a_city(long row, long col, char *name, long major);
void dir_assert(void);
void dump_continents(void);
void dump_gates(void);
void dump_roads(void);
char *fetch_inside_name(void);
long flood_land_clumps(long row, long col, char *name);
long flood_land_inside(long row, long col, long ins);
long flood_water_inside(long row, long col, long ins);
void fix_terrain_land(void);
void gate_continental_tour(void);
void gate_land_ring(long rings);
void gate_link_islands(long rings);
void gate_province_islands(long times);
void gate_stone_circles(void);
int is_port_city(long row, long col);
int island_allowed(long row, long col);
//void link_roads(struct tile *from, struct tile *to, long hidden, char *name);
void make_appropriate_subloc(long row, long col, int unused);
void make_gates(void);
void make_graveyards(void);
void make_islands(void);
void make_roads(void);
void mark_bad_locs(void);
void map_init(void);
char *name_guild(long skill);
//void new_gate(struct tile *from, struct tile *to, long key);
long not_place_random_subloc(long kind, long hidden);
void not_random_province(long *row, long *col);
void open_fps(void);
long place_random_subloc(long kind, long hidden, long terr);
void place_sublocations(void);
void print_continent(long i);
void print_inside_locs(long n);
void print_inside_sublocs(long flag, long row, long col);
void print_map(void);
void print_sublocs(void);
void print_subloc_gates(long n);
//int prov_dest(struct tile *t, long dir);
long random_island(void);
void random_province(long *row, long *col, long terr);
void random_province_gates(long gate_times);
void randomize_dir_vector(void);
long rc_to_region(long row, long col);
void read_map(void);
long rnd_alloc_num(long low, long high);
void set_province_clumps(void);
void set_regions(void);
void show_gate_coverage(void);
void unnamed_province_clumps(void);

#endif //OLYTAG_MAPGEN_H
