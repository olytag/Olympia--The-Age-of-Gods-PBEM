
#include	<stdio.h>
#include	"z.h"
#include	"oly.h"


struct use_tbl_ent {
	char *allow;		/* who may execute the command */
	int skill;

	int (*start)(struct command *);		/* initiator */
	int (*finish)(struct command *);	/* conclusion */
	int (*interrupt)(struct command *);	/* interrupted order */

	int time;		/* how long command takes */
	int poll;		/* call finish each day, not just at end */
};

int v_attack_tactics();
int v_defense_tactics();
int v_prac_control();
int v_prac_protect();
int v_use_beasts();

int v_sail(), d_sail(), i_sail();
int v_brew(), d_brew_heal(), d_brew_death(), d_brew_slave(), d_brew_fiery();
int d_brew_weightlessness();
int v_add_ram(), d_add_ram();
int v_remove_ram(), d_remove_ram();

int v_wood(), v_yew(), v_catch();
int v_spy_inv(), d_spy_inv(), v_spy_lord(), d_spy_lord();
int v_spy_skills(), d_spy_skills();
int v_adv_med(), d_adv_med(), v_hinder_med(), d_hinder_med();

int v_defense(), d_defense();
int v_archery(), d_archery();
int v_swordplay(), d_swordplay();

int v_detect_gates(), d_detect_gates(), v_jump_gate(), v_teleport();
int v_reverse_jump(), v_reveal_key(), d_reveal_key();
int v_seal_gate(), d_seal_gate(), v_unseal_gate(), d_unseal_gate();
int v_notify_unseal(), d_notify_unseal(), v_rem_seal(), d_rem_seal();
int v_notify_jump(), d_notify_jump(), v_meditate(), d_meditate();
int v_heal(), d_heal(), v_reveal_mage(), d_reveal_mage(), v_appear_common();
int v_view_aura(), d_view_aura(), v_shroud_abil(), d_shroud_abil();
int v_detect_abil(), d_detect_abil(), v_detect_scry(), d_detect_scry();
int v_scry_region(), d_scry_region(), v_shroud_region(), d_shroud_region();
int v_dispel_region(), d_dispel_region(), v_dispel_abil(), d_dispel_abil();
int v_proj_cast(), d_proj_cast(), v_locate_char(), d_locate_char();
int v_bar_loc(), d_bar_loc(), v_unbar_loc(), d_unbar_loc();
int v_forge_palantir(), d_forge_palantir(), v_destroy_art(), d_destroy_art();
int v_show_art_creat(), d_show_art_creat(), v_show_art_reg(), d_show_art_reg();
int v_save_proj(), d_save_proj(), v_save_quick(), d_save_quick();
int v_quick_cast(), d_quick_cast(), v_rem_art_cloak(), d_rem_art_cloak();
int v_write_spell(), d_write_spell(), v_curse_noncreat(), d_curse_noncreat();
int v_cloak_creat(), d_cloak_creat(), v_cloak_reg(), d_cloak_reg();
int v_forge_aura(), d_forge_aura(), v_bribe(), d_bribe();
int v_shipbuild(), v_summon_savage(), v_keep_savage(), d_keep_savage();
int v_improve_opium(), d_improve_opium(), v_lead_to_gold(), d_lead_to_gold();
int v_raise(), d_raise(), v_rally(), d_rally(), v_incite(), d_incite();
int v_bird_spy(), d_bird_spy(), v_eat_dead(), d_eat_dead();
int v_raise_corpses(), v_undead_lord(), d_undead_lord();
int v_banish_undead(), d_banish_undead(), v_keep_undead(), d_keep_undead();
int v_aura_blast(), d_aura_blast(), v_aura_reflect(), v_banish_corpses(), d_banish_corpses();
int v_summon_rain(), d_summon_rain(), v_summon_wind(), d_summon_wind();
int v_summon_fog(), d_summon_fog(), v_dissipate(), d_dissipate();
int v_direct_storm(), v_renew_storm(), d_renew_storm();
int v_lightning(), d_lightning();
int v_seize_storm(), d_seize_storm(), v_death_fog(), d_death_fog();
int v_hide(), d_hide(), v_sneak(), d_sneak(), v_fierce_wind(), d_fierce_wind();
int v_conceal_nation(), d_conceal_nation();
int v_mage_menial(), d_mage_menial(),
  v_reveal_vision(), d_reveal_vision();
int v_create_dirt_golem(), d_create_dirt_golem();
int v_create_flesh_golem(), d_create_flesh_golem();
int v_create_iron_golem(), d_create_iron_golem();
int v_resurrect(), d_resurrect(), v_prep_ritual(), d_prep_ritual();
int v_last_rites(), d_last_rites(), v_remove_bless(), d_remove_bless();
int v_find_rich(), d_find_rich(), v_torture(), d_torture();
int v_fight_to_death(), v_breed(), d_breed(), v_fish();
int v_petty_thief(), d_petty_thief(), i_petty_thief(),
  v_persuade_oath(), d_persuade_oath();
int v_forge_art_x(), d_forge_art_x(), v_trance(), d_trance();
int v_teleport_item(), d_teleport_item(), v_tap_health(), d_tap_health();
#if 0
int v_bind_storm(), d_bind_storm();
#endif 0
int v_use_train_riding(), v_use_train_war();
int v_assassinate(), d_assassinate();
int v_find_food(), d_find_food();

int v_generic_trap(),   d_generic_trap();

int v_bless_follower(), v_proselytise(), v_create_holy_symbol();
int d_create_holy_symbol();
int d_detect_beasts(), v_detect_beasts();
int d_capture_beasts(), v_capture_beasts();
int v_gather_holy_plant(), d_gather_holy_plant();

int v_find_mountain_trail(), v_obscure_mountain_trail();
int d_obscure_mountain_trail();
int v_improve_mining(), d_improve_mining();
int v_conceal_mine(), d_conceal_mine();
int v_protect_mine(), d_protect_mine();
int v_bless_fort(), d_bless_fort();
int v_weaken_fort(), d_weaken_fort();

int v_find_forest_trail(), d_obscure_forest_trail(), v_obscure_forest_trail();
int d_improve_logging(), v_improve_logging(), d_find_hidden_features();
int v_find_hidden_features(), v_improve_fort(), d_improve_fort();
int v_recruit_elves(), d_recruit_elves();

int d_improve_quarrying(), v_improve_quarrying();
int d_improve_smithing(), v_improve_smithing();
int d_edge_of_kireus(), v_edge_of_kireus();
int d_create_mithril(), v_create_mithril();

int v_enchant_guard(), d_enchant_guard();
int v_urchin_spy(), d_urchin_spy();
int v_draw_crowds(), d_draw_crowds();
int v_arrange_mugging(), d_arrange_mugging();

int d_calm_peasants();
int v_improve_charisma(), d_improve_charisma();
int d_mesmerize_crowd();
int d_improve_taxes();
int d_guard_loyalty();
int d_instill_fanaticism();

int d_find_all_hidden_features();
int d_conceal_location(), v_conceal_location();
int d_create_ninja(), v_create_ninja();
int d_create_mist();

int d_dedicate_temple(), v_dedicate_temple();

/*  Trading */
int d_dedicate_tower(), v_dedicate_tower();
int v_smuggle_goods(struct command *c),
d_smuggle_goods(struct command *c),
v_smuggle_men(struct command *c),
d_smuggle_men(struct command *c),
v_build_wagons(struct command *c),
d_build_wagons(struct command *c),
v_increase_demand(struct command *c),
d_increase_demand(struct command *c),
v_decrease_demand(struct command *c),
d_decrease_demand(struct command *c),
v_increase_supply(struct command *c),
d_increase_supply(struct command *c),
v_decrease_supply(struct command *c),
d_decrease_supply(struct command *c),
v_hide_money(struct command *c),
d_hide_money(struct command *c),
v_hide_item(struct command *c),
d_hide_item(struct command *c);

int v_grow_pop(struct command *c),
d_grow_pop(struct command *c);

/* Shipcraft */
int d_add_sails(struct command *c);
int v_add_sails(struct command *c);
int d_remove_sails(struct command *c);
int v_remove_sails(struct command *c);
int d_add_forts(struct command *c);
int v_add_forts(struct command *c);
int d_remove_forts(struct command *c);
int v_remove_forts(struct command *c);
int d_add_keels(struct command *c);
int v_add_keels(struct command *c);
int d_remove_keels(struct command *c);
int v_remove_keels(struct command *c);
int d_add_ports(struct command *c);
int v_add_ports(struct command *c);
int d_remove_ports(struct command *c);
int v_remove_ports(struct command *c);

/* Construction */
int d_fortify_castle(struct command *c);
int v_fortify_castle(struct command *c);
int d_strengthen_castle(struct command *c);
int v_strengthen_castle(struct command *c);
int d_moat_castle(struct command *c);
int v_moat_castle(struct command *c);
int d_widen_entrance(struct command *c);
int v_widen_entrance(struct command *c);

/* Mining */
int v_mine_iron(), d_mine_iron(), v_mine_gold(), d_mine_gold();
int v_mine_mithril(), d_mine_mithril(), v_quarry();
int v_mine_gate_crystal(), d_mine_gate_crystal();
int d_add_wooden_shoring(), v_add_wooden_shoring();
int d_add_iron_shoring(), v_add_iron_shoring();

/* Artifact construction */
int v_mutate_art();
int d_mutate_art();
int v_conceal_arts();
int d_conceal_arts();
int v_detect_arts();
int d_detect_arts();
int v_obscure_art();
int d_obscure_art();
int v_unobscure_art();
int d_unobscure_art();
int v_reveal_arts();
int d_reveal_arts();
int v_deep_identify();

/* Heroism */
int v_personal_fight_to_death();
int v_forced_march();

/* Basic Religion */
int v_hinder_med_b(), d_hinder_med_b();
int v_vision_reg(), d_vision_reg();


int v_use_cs();
int v_implicit();

struct use_tbl_ent use_tbl[] = {
{NULL, 0,                0,                0,                0,        0, 0},

/*
allow  skill             start             finish            intr    time poll
 */

{"c", sk_meditate,       v_meditate,       d_meditate,       NULL,     7, 1},
{"c", sk_detect_gates,   v_detect_gates,   d_detect_gates,   NULL,     7, 0},
{"c", sk_jump_gate,      v_jump_gate,      NULL,             NULL,     1, 0},
{"c", sk_teleport,       v_teleport,       NULL,             NULL,     1, 0},
{"c", sk_seal_gate,      v_seal_gate,      d_seal_gate,      NULL,     7, 0},
{"c", sk_unseal_gate,    v_unseal_gate,    d_unseal_gate,    NULL,     7, 0},
{"c", sk_notify_unseal,  v_notify_unseal,  d_notify_unseal,  NULL,     7, 0},
{"c", sk_rem_seal,       v_rem_seal,       d_rem_seal,       NULL,     7, 0},
{"c", sk_reveal_key,     v_reveal_key,     d_reveal_key,     NULL,     7, 0},
{"c", sk_notify_jump,    v_notify_jump,    d_notify_jump,    NULL,     7, 0},
{"c", sk_rev_jump,       v_reverse_jump,   NULL,             NULL,     1, 0},
{"c", sk_reveal_mage,    v_reveal_mage,    d_reveal_mage,    NULL,     7, 0},
{"c", sk_view_aura,      v_view_aura,      d_view_aura,      NULL,     7, 0},
{"c", sk_shroud_abil,    v_shroud_abil,    d_shroud_abil,    NULL,     3, 0},
{"c", sk_detect_abil,    v_detect_abil,    d_detect_abil,    NULL,     7, 0},
{"c", sk_scry_region,    v_scry_region,    d_scry_region,    NULL,     7, 0},
{"c", sk_shroud_region,  v_shroud_region,  d_shroud_region,  NULL,     3, 0},
{"c", sk_pr_shroud_loc,  v_shroud_region,  d_shroud_region,  NULL,     3, 0},
{"c", sk_detect_scry,    v_detect_scry,    d_detect_scry,    NULL,     7, 0},
{"c", sk_dispel_region,  v_dispel_region,  d_dispel_region,  NULL,     3, 0},
{"c", sk_dispel_abil,    v_dispel_abil,    d_dispel_abil,    NULL,     3, 0},
{"c", sk_adv_med,        v_adv_med,        d_adv_med,        NULL,     7, 1},
{"c", sk_hinder_med,     v_hinder_med,     d_hinder_med,     NULL,    10, 0},
{"c", sk_proj_cast,      v_proj_cast,      d_proj_cast,      NULL,     7, 0},
{"c", sk_locate_char,    v_locate_char,    d_locate_char,    NULL,    10, 0},
{"c", sk_bar_loc,        v_bar_loc,        d_bar_loc,        NULL,    10, 0},
{"c", sk_unbar_loc,      v_unbar_loc,      d_unbar_loc,      NULL,     7, 0},
{"c", sk_forge_palantir, v_forge_palantir, d_forge_palantir, NULL,    10, 0},
{"c", sk_destroy_art,    v_destroy_art,    d_destroy_art,    NULL,     7, 0},
{"c", sk_save_proj,      v_save_proj,      d_save_proj,      NULL,     7, 0},
{"c", sk_save_quick,     v_save_quick,     d_save_quick,     NULL,     7, 0},
{"c", sk_quick_cast,     v_quick_cast,     d_quick_cast,     NULL,     4, 0},
{"c", sk_write_basic,    v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_write_weather,  v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_write_scry,     v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_write_gate,     v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_write_art,      v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_write_necro,    v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_dirt_golem, v_create_dirt_golem, d_create_dirt_golem, NULL,   7, 0},
{"c", sk_flesh_golem, v_create_flesh_golem, d_create_flesh_golem, NULL,7, 0},
{"c", sk_iron_golem, v_create_iron_golem, d_create_iron_golem, NULL,   7, 0},
{"c", sk_forge_aura,     v_forge_aura,     d_forge_aura,     NULL,    14, 0},
{"c", sk_mutate_artifact, v_mutate_art, d_mutate_art, NULL, 30, 0},
{"c", sk_conceal_artifacts, v_conceal_arts, d_conceal_arts,  NULL, 7, 0},
{"c", sk_detect_artifacts, v_detect_arts, d_detect_arts,  NULL, 7, 0},
{"c", sk_obscure_artifact, v_obscure_art, d_obscure_art,  NULL, 7, 0},
{"c", sk_remove_obscurity, v_unobscure_art, d_unobscure_art,  NULL, 7, 0},
{"c", sk_reveal_artifacts, v_reveal_arts, d_reveal_arts,  NULL, 7, 0},
{"c", sk_deep_identify, v_deep_identify, NULL,  NULL, 7, 0},
{"c", sk_shipbuilding,   v_shipbuild,      NULL,             NULL,     0, 0},
{"c", sk_pilot_ship,     v_sail,           d_sail,           i_sail,  -1, 0},
{"c", sk_train_wild,     v_use_train_riding, NULL,           NULL,     7, 0},
{"c", sk_train_warmount, v_use_train_war,  NULL,             NULL,    14, 0},
{"c", sk_make_ram,       NULL,             NULL,             NULL,    14, 0},
{"c", sk_make_catapult,  NULL,             NULL,             NULL,    14, 0},
{"c", sk_make_siege,     NULL,             NULL,             NULL,    14, 0},
{"c", sk_brew_slave,     v_brew,           d_brew_slave,     NULL,     7, 0},
{"c", sk_brew_heal,      v_brew,           d_brew_heal,      NULL,     7, 0},
{"c", sk_brew_death,     v_brew,           d_brew_death,     NULL,    10, 0},
{"c", sk_brew_weightlessness,
                         v_brew,           d_brew_weightlessness,  NULL,10, 0},
{"c", sk_brew_fiery,     v_brew,           d_brew_fiery,     NULL,    14, 0},
{"c", sk_mine_iron,      v_mine_iron,      d_mine_iron,      NULL,     7, 0},
{"c", sk_mine_gold,      v_mine_gold,      d_mine_gold,      NULL,     7, 0},
{"c", sk_mine_mithril,   v_mine_mithril,   d_mine_mithril,   NULL,     7, 0},
{"c", sk_mine_crystal,   v_mine_gate_crystal,   d_mine_gate_crystal,   NULL,     7, 0},
{"c", sk_quarry_stone,   v_quarry,         NULL,             NULL,    -1, 1},
{"c", sk_catch_horse,    v_catch,          NULL,             NULL,    -1, 1},
{"c", sk_extract_venom,  NULL,             NULL,             NULL,     7, 0},
{"c", sk_harvest_lumber, v_wood,           NULL,             NULL,    -1, 1},
{"c", sk_harvest_yew,    v_yew,            NULL,             NULL,    -1, 1},
{"c", sk_add_ram,        v_add_ram,        d_add_ram,        NULL,    10, 0},
{"c", sk_remove_ram,     v_remove_ram,     d_remove_ram,     NULL,    10, 0},
{"c", sk_assassinate,    v_assassinate,    d_assassinate,    NULL,     7, 0},
{"c", sk_find_food,      v_find_food,      d_find_food,      NULL,     7, 0},
{"c", sk_spy_inv,        v_spy_inv,        d_spy_inv,        NULL,     7, 0},
{"c", sk_spy_skills,     v_spy_skills,     d_spy_skills,     NULL,     7, 0},
{"c", sk_spy_lord,       v_spy_lord,       d_spy_lord,       NULL,     7, 0},
{"c", sk_record_skill,   v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_bribe_noble,    v_bribe,          d_bribe,          NULL,     7, 0},
{"c", sk_summon_savage,  v_summon_savage,  NULL,             NULL,     1, 0},
{"c", sk_keep_savage,    v_keep_savage,    d_keep_savage,    NULL,     7, 0},
{"c", sk_improve_opium,  v_improve_opium,  d_improve_opium,  NULL,     7, 0},
{"c", sk_raise_mob,      v_raise,          d_raise,          NULL,     7, 0},
{"c", sk_rally_mob,      v_rally,          d_rally,          NULL,     7, 0},
{"c", sk_incite_mob,     v_incite,         d_incite,         NULL,     7, 0},
{"c", sk_bird_spy,       v_bird_spy,       d_bird_spy,       NULL,     3, 0},
{"c", sk_lead_to_gold,   v_lead_to_gold,   d_lead_to_gold,   NULL,     7, 0},
{"c", sk_raise_corpses,  v_raise_corpses,  NULL,             NULL,    -1, 1},
{"c", sk_undead_lord,    v_undead_lord,    d_undead_lord,    NULL,     7, 0},
{"c", sk_banish_undead,  v_banish_undead,  d_banish_undead,  NULL,     7, 0},
{"c", sk_renew_undead,   v_keep_undead,    d_keep_undead,    NULL,     7, 0},
{"c", sk_eat_dead,       v_eat_dead,       d_eat_dead,       NULL,    14, 0},
{"c", sk_aura_blast,     v_aura_blast,     d_aura_blast,     NULL,     1, 0},
{"c", sk_absorb_blast,   v_aura_reflect,   NULL,             NULL,     0, 0},
{"c", sk_summon_rain,    v_summon_rain,    d_summon_rain,    NULL,     7, 0},
{"c", sk_summon_wind,    v_summon_wind,    d_summon_wind,    NULL,     7, 0},
{"c", sk_summon_fog,     v_summon_fog,     d_summon_fog,     NULL,     7, 0},
{"c", sk_direct_storm,   v_direct_storm,   NULL,             NULL,     1, 0},
{"c", sk_renew_storm,    v_renew_storm,    d_renew_storm,    NULL,     3, 0},
{"c", sk_dissipate,      v_dissipate,      d_dissipate,      NULL,     7, 0},
{"c", sk_lightning,      v_lightning,      d_lightning,      NULL,     7, 0},
{"c", sk_fierce_wind,    v_fierce_wind,    d_fierce_wind,    NULL,     7, 0},
{"c", sk_seize_storm,    v_seize_storm,    d_seize_storm,    NULL,     7, 0},
{"c", sk_death_fog,      v_death_fog,      d_death_fog,      NULL,     7, 0},
{"c", sk_banish_corpses, v_banish_corpses, d_banish_corpses, NULL,     7, 0},
{"c", sk_hide_self,      v_hide,           d_hide,           NULL,     3, 0},
{"c", sk_conceal_nation, v_conceal_nation, d_conceal_nation, NULL,     7, 0},
{"c", sk_sneak_build,    v_sneak,          d_sneak,          NULL,     3, 0},
{"c", sk_mage_menial,    v_mage_menial,    d_mage_menial,    NULL,     7, 1},
{"c", sk_petty_thief,    v_petty_thief,    d_petty_thief,    NULL,     7, 1},
{"c", sk_appear_common,  v_appear_common,  NULL,             NULL,     1, 0},
{"c", sk_defense,        v_defense,        d_defense,        NULL,     7, 0},
{"c", sk_defense2,        v_defense,        d_defense,        NULL,     7, 0},
{"c", sk_archery,        v_archery,        d_archery,        NULL,     7, 0},
{"c", sk_swordplay,      v_swordplay,      d_swordplay,      NULL,     7, 0},
{"c", sk_swordplay2,      v_swordplay,      d_swordplay,      NULL,     7, 0},
{"c", sk_find_rich,      v_find_rich,      d_find_rich,      NULL,     7, 0},
{"c", sk_harvest_opium,  v_implicit,       NULL,             NULL,     0, 0},
{"c", sk_train_angry,    v_implicit,       NULL,             NULL,     0, 0},
{"c", sk_weaponsmith,    v_implicit,       NULL,             NULL,     0, 0},
{"c", sk_hide_lord,      v_implicit,       NULL,             NULL,     0, 0},
{"c", sk_transcend_death,v_implicit,       NULL,             NULL,     0, 0},
{"c", sk_collect_foliage,v_implicit,       NULL,             NULL,     0, 0},
{"c", sk_fishing,        v_fish,           NULL,             NULL,     0, 0},
{"c", sk_summon_ghost,   v_implicit,       NULL,             NULL,     0, 0},
{"c", sk_capture_beasts, v_capture_beasts, d_capture_beasts, NULL,     7, 0},
{"c", sk_use_beasts,     v_use_beasts,     NULL, 	     NULL,     0, 0},
{"c", sk_collect_elem,   v_implicit,       NULL,             NULL,     0, 0},
{"c", sk_torture,        v_torture,        d_torture,        NULL,     7, 0},
{"c", sk_fight_to_death, v_fight_to_death, NULL,             NULL,     0, 0},
{"c", sk_breed_beasts,   v_breed,          d_breed,          NULL,     7, 0},
{"c", sk_persuade_oath,  v_persuade_oath,  d_persuade_oath,  NULL,     7, 0},
{"c", sk_forge_weapon,   v_forge_art_x,    d_forge_art_x,    NULL,     7, 0},
{"c", sk_forge_armor,    v_forge_art_x,    d_forge_art_x,    NULL,     7, 0},
{"c", sk_forge_bow,      v_forge_art_x,    d_forge_art_x,    NULL,     7, 0},
{"c", sk_trance,         v_trance,         d_trance,         NULL,    28, 0},
{"c", sk_teleport_item,  v_teleport_item,  d_teleport_item,  NULL,     3, 0},
{"c", sk_tap_health,     v_tap_health,     d_tap_health,     NULL,     7, 0},
#if 0
{"c", sk_bind_storm,     v_bind_storm,     d_bind_storm,     NULL,     7, 0},
#endif 0
{"c", sk_control_battle, v_prac_control,   NULL,	     NULL,     3, 0},
{"c", sk_protect_noble,  v_prac_protect,   NULL,	     NULL,     3, 0},
{"c", sk_attack_tactics, v_attack_tactics, NULL,	     NULL,     7, 0},
{"c", sk_defense_tactics,v_defense_tactics,NULL,	     NULL,     0, 0},

{"c", sk_resurrect,      v_resurrect,      d_resurrect,      NULL,    30, 0},
{"c", sk_pray,           v_prep_ritual,    d_prep_ritual,    NULL,     7, 0},
{"c", sk_last_rites,     v_last_rites,     d_last_rites,     NULL,    10, 0},
{"c", sk_gather_holy_plant,   v_gather_holy_plant,   d_gather_holy_plant,   NULL,    10, 0},
{"c", sk_bless_follower, v_bless_follower, NULL,	     NULL,     1, 0},
{"c", sk_proselytise,    v_proselytise,    NULL,	     NULL,     1, 0},
{"c", sk_create_holy_symbol,    v_create_holy_symbol,
      d_create_holy_symbol,	NULL,     14, 0},
{"c", sk_heal,           v_heal,           d_heal,           NULL,     7, 0},
{"c", sk_summon_water_elemental,
                         v_generic_trap,   d_generic_trap,   NULL,     7, 0},
{"c", sk_find_mtn_trail,  v_find_mountain_trail,  NULL,	      NULL,     1, 0},
{"c", sk_obscure_mtn_trail, v_obscure_mountain_trail,
                            d_obscure_mountain_trail,	      NULL,     7, 0},
{"c", sk_improve_mining,  v_improve_mining,  d_improve_mining,NULL,     7, 0},
{"c", sk_conceal_mine,    v_conceal_mine,    d_conceal_mine,  NULL,    30, 0},
{"c", sk_protect_mine,    v_protect_mine,    d_protect_mine,  NULL,     7, 0},
{"c", sk_bless_fort,      v_bless_fort,      d_bless_fort,    NULL,     7, 0},
{"c", sk_weaken_fort,     v_weaken_fort,     d_weaken_fort,    NULL,    3, 0},
{"c", sk_boulder_trap,    v_generic_trap,    d_generic_trap,  NULL,     7, 0},
{"c", sk_write_anteus,     v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_detect_beasts,   v_detect_beasts,  d_detect_beasts,  NULL,     3, 0},
{"c", sk_capture_beasts,  v_capture_beasts, d_capture_beasts, NULL,    7, 0},
{"c", sk_snake_trap,  	  v_generic_trap,   d_generic_trap,   NULL,    7, 0},
{"c", sk_write_dol,     v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_find_forest_trail, v_find_forest_trail, NULL,	      NULL,     1, 0},
{"c", sk_obscure_forest_trail, v_obscure_forest_trail,
                            d_obscure_forest_trail,	      NULL,     7, 0},
{"c", sk_improve_forestry,v_improve_logging,d_improve_logging,NULL,     7, 0},
{"c", sk_reveal_forest,   v_find_hidden_features,
   				    d_find_hidden_features,   NULL,     7, 0},
{"c", sk_improve_fort,    v_improve_fort,   d_improve_fort,   NULL,     7, 0},
{"c", sk_create_deadfall, v_generic_trap,   d_generic_trap,   NULL,     7, 0},
{"c", sk_recruit_elves,   v_recruit_elves,  d_recruit_elves,  NULL,     7, 0},
{"c", sk_write_timeid,     v_write_spell,    d_write_spell,    NULL,     7, 0},

{"c", sk_reveal_vision,  v_reveal_vision,  d_reveal_vision,  NULL,    10, 0},
{"c", sk_enchant_guard,   v_enchant_guard,  d_enchant_guard,  NULL,     7, 0},
{"c", sk_urchin_spy,   v_urchin_spy,  d_urchin_spy,  NULL,     7, 0},
{"c", sk_draw_crowds,   v_draw_crowds,  d_draw_crowds,  NULL,     7, 0},
{"c", sk_arrange_mugging,   v_arrange_mugging,  d_arrange_mugging,  NULL,     7, 0},
{"c", sk_write_ham,     v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_improve_quarry,v_improve_quarrying,d_improve_quarrying,NULL,     7, 0},
{"c", sk_improve_smithing,v_improve_smithing,d_improve_smithing,NULL,     7, 0},
{"c", sk_edge_of_kireus,  v_edge_of_kireus, d_edge_of_kireus,  NULL,     7, 0},
{"c", sk_create_mithril,  v_create_mithril, d_create_mithril,  NULL,     7, 0},
{"c", sk_quicksand_trap,  v_generic_trap,   d_generic_trap,   NULL,     7, 0},
{"c", sk_write_kireus,     v_write_spell,    d_write_spell,    NULL,     7, 0},
{"c", sk_calm_ap,         NULL,            d_calm_peasants,   NULL,     7, 0},
{"c", sk_improve_charisma, v_improve_charisma, d_improve_charisma,   NULL,     7, 0},
{"c", sk_mesmerize_crowd, NULL, 	d_mesmerize_crowd,   NULL,     7, 0},
{"c", sk_improve_taxes,   NULL, 	d_improve_taxes,   NULL,     7, 0},
{"c", sk_guard_loyalty,   NULL, 	d_guard_loyalty,   NULL,     7, 0},
{"c", sk_instill_fanaticism,   NULL, 	d_instill_fanaticism,   NULL,     7, 0},
{"c", sk_write_halon,     v_write_spell,    d_write_spell,    NULL,     7, 0},

{"c", sk_find_hidden,     NULL,       d_find_all_hidden_features,  NULL,     7, 0},
{"c", sk_conceal_loc,     v_conceal_location, d_conceal_location,  NULL,     7, 0},
{"c", sk_create_ninja,    v_create_ninja,   d_create_ninja,   NULL,     7, 0},
{"c", sk_mists_of_conceal,NULL,             d_create_mist,    NULL,     7, 0},
{"c", sk_write_domingo,     v_write_spell,    d_write_spell,    NULL,     7, 0},

{"c", sk_dedicate_temple, v_dedicate_temple, d_dedicate_temple, NULL,     7, 0},

{"c", sk_smuggle_goods, v_smuggle_goods, d_smuggle_goods, NULL,     7, 0},
{"c", sk_smuggle_men, v_smuggle_men, d_smuggle_men, NULL,     7, 0},
{"c", sk_build_wagons, NULL, NULL, NULL,     7, 0},
{"c", sk_increase_demand, v_increase_demand, d_increase_demand, NULL,     7, 0},
{"c", sk_decrease_demand, v_decrease_demand, d_decrease_demand, NULL,     7, 0},
{"c", sk_increase_supply, v_increase_supply, d_increase_supply, NULL,     7, 0},
{"c", sk_decrease_supply, v_decrease_supply, d_decrease_supply, NULL,     7, 0},
{"c", sk_hide_money, v_hide_money, d_hide_money, NULL,     7, 0},
{"c", sk_hide_item, v_hide_item, d_hide_item, NULL,     7, 0},
{"c", sk_grow_pop, v_grow_pop, d_grow_pop, NULL,     7, 0},

{"c", sk_add_sails, v_add_sails, d_add_sails, NULL,     3, 0},
{"c", sk_remove_sails, v_remove_sails, d_remove_sails, NULL,     3, 0},
{"c", sk_add_forts, v_add_forts, d_add_forts, NULL,     7, 0},
{"c", sk_remove_forts, v_remove_forts, d_remove_forts, NULL,     7, 0},
{"c", sk_add_ports, v_add_ports, d_add_ports, NULL,     4, 0},
{"c", sk_remove_ports, v_remove_ports, d_remove_ports, NULL,     4, 0},
{"c", sk_add_keels, v_add_keels, d_add_keels, NULL,     5, 0},
{"c", sk_remove_keels, v_remove_keels, d_remove_keels, NULL,     5, 0},

{"c", sk_fortify_castle, v_fortify_castle, d_fortify_castle, NULL,     5, 0},
{"c", sk_strengthen_castle, v_strengthen_castle, d_strengthen_castle, NULL,     5, 0},
{"c", sk_moat_castle, v_moat_castle, d_moat_castle, NULL,     5, 1},
{"c", sk_widen_entrance, v_widen_entrance, d_widen_entrance, NULL,     5, 0},
  
{"c", sk_wooden_shoring, v_add_wooden_shoring, d_add_wooden_shoring, NULL,     5, 0},
{"c", sk_iron_shoring, v_add_iron_shoring, d_add_iron_shoring, NULL,     5, 0},

  /* Combat Spells */

{"c", sk_lightning_bolt, v_use_cs,   NULL,             NULL,     0, 0},
{"c", sk_foresee_defense, v_use_cs,  NULL,             NULL,     0, 0},
{"c", sk_drain_mana, v_use_cs,       NULL,             NULL,     0, 0},
{"c", sk_raise_soldiers, v_use_cs,   NULL,             NULL,     0, 0},
{"c", sk_fireball, v_use_cs,         NULL,             NULL,     0, 0},

  /* Heroism */
{"c", sk_personal_fttd,	v_personal_fight_to_death, NULL,  NULL,     0, 0},
{"c", sk_forced_march,	v_forced_march, NULL,  NULL,     0, 0},

  /* Basic religion */
{"c", sk_resurrect_b,      v_resurrect,      d_resurrect,      NULL,    30, 0},
{"c", sk_pray_b,           v_prep_ritual,    d_prep_ritual,    NULL,     7, 0},
{"c", sk_last_rites_b,     v_last_rites,     d_last_rites,     NULL,    10, 0},
{"c", sk_gather_holy_plant_b,   v_gather_holy_plant,   d_gather_holy_plant,   NULL,    10, 0},
{"c", sk_bless_b, v_bless_follower, NULL,	     NULL,     3, 0},
{"c", sk_proselytise_b,    v_proselytise,    NULL,	     NULL,    3, 0},
{"c", sk_create_holy_b,    v_create_holy_symbol,
      d_create_holy_symbol,	NULL,     14, 0},
{"c", sk_heal_b,           v_heal,           d_heal,           NULL,     7, 0},
{"c", sk_dedicate_temple_b, v_dedicate_temple, d_dedicate_temple, NULL, 7, 0},
{"c", sk_write_religion_b,  v_write_spell,    d_write_spell,   NULL,     7, 0},
{"c", sk_hinder_med_b, v_hinder_med_b,     d_hinder_med_b,     NULL,    10, 0},
{"c", sk_scry_b,       v_vision_reg,    d_vision_reg,    NULL,     7, 0},
{"c", sk_banish_undead_b,  v_banish_undead,  d_banish_undead,  NULL,     7, 0},
  
{NULL, 0,                0,                0,                0,        0, 0}

};


int
v_implicit(struct command *c)
{

	wout(c->who, "Use of this skill is automatic when appropriate.");
	wout(c->who, "No direct USE function exists.");
	return FALSE;
}

int
v_use_cs(struct command *c)
{
  int sk = c->use_skill;
  int i, arg, num;
  char *rounds = "";
  int found = 0;
    
  if (c->a == 0) {
    wout(c->who, "Cleared all uses of %s.", box_name(sk));
    delete_all_effects(c->who, ef_cs, sk);
    return TRUE;
  };

  num = numargs(c);
  for(i=1;i<=num;i++) {
    arg = parse_arg(c->who, c->parse[i]);
    if (arg == 1) {
      wout(c->who, "You cannot use combat spells during the first round of combat.");
    } else if (arg > 10) {
      wout(c->who, "You can only specify combat spell use up to round ten.");
    } else {
      add_effect(c->who, ef_cs, sk, -1, arg);
      rounds = sout("%s %d", rounds, arg);
    };
  };

  if (strcmp("",rounds) != 0) wout(c->who, "Use %s in rounds:%s.",box_name(sk), rounds);

  return TRUE;
}

int
v_shipbuild(struct command *c)
{

	wout(c->who, "Use the BUILD order to build ships.");
	return FALSE;
}


int
find_use_entry(int skill)
{
	int i;

	for (i = 1; use_tbl[i].skill; i++)
		if (use_tbl[i].skill == skill)
			return i;

	return -1;
}


/*
 *  If we know the skill, return the skill number
 *
 *  If an artifact grants us the ability to use the skill,
 *  return the item number of the artifact.
 *
 *  If a one-shot scroll lets us use the skill, return the
 *  item number of the scroll.
 */

static int
may_use_skill(int who, int sk)
{
  struct item_ent *e;
  struct item_magic *p;
  int ret = 0;
  int scroll = 0;

  if (has_skill(who, sk) > 0)
    return sk;

  /*
   *  Items other than scrolls should take precedence, to preserve
   *  the one-shot scrolls.
   */
  loop_inv(who, e) {
      p = rp_item_magic(e->item);
      if (p && ilist_lookup(p->may_use, sk) >= 0) {
	if (subkind(e->item) == sub_scroll ||
	    subkind(e->item) == sub_book)
	  scroll = e->item;
	else
	  ret = e->item;
      }
  } next_inv;

  if (ret)
    return ret;

  if (scroll)
    return scroll;

  return 0;
}


static void
magically_speed_casting(struct command *c, int sk)
{
	struct char_magic *p;
	int n;		/* amount speeded by */

	if (magic_skill(sk) &&
	    char_quick_cast(c->who) &&
	    sk != sk_save_quick &&
	    sk != sk_trance)
	{
		p = p_magic(c->who);

		if (c->wait == 0)
		{
			n = 0;			/* don't do anything */
		}
		else if (p->quick_cast < c->wait)
		{
			n = p->quick_cast;

			c->wait -= p->quick_cast;
			p->quick_cast = 0;
		}
		else
		{
			n = c->wait - 1;
			p->quick_cast = 0;
			c->wait = 1;
		}

		wout(c->who, "(speeded cast by %d day%s)", n, add_s(n));
	}
}


/*
 *  To use a spell through a scroll, the character should USE the
 *  spell number, and not USE the scroll.  If they use the scroll,
 *  guess which spell they meant to use from within the scroll,
 *  and use it.
 */

static int
correct_use_item(struct command *c)
{
	int item = c->a;
	struct item_magic *p;

	if (item_use_key(item))
		return item;

	p = rp_item_magic(item);

	if (p == NULL || ilist_len(p->may_use) < 1)
		return item;

	c->a = p->may_use[0];
	return c->a;
}


static int
meets_requirements(int who, int skill)
{
	int i;
	struct entity_skill *p;
	struct req_ent **l;

	p = rp_skill(skill);
	if (p == NULL)
		return TRUE;

	l = p->req;

	i = 0;
	while (i < ilist_len(l))
	{
		while (has_item(who, l[i]->item) < l[i]->qty &&
		       l[i]->consume == REQ_OR)
		{
			i++;

			if (i >= ilist_len(l))
			{
				fprintf(stderr, "skill = %d\n", skill);
				assert(FALSE);
			}
/*
 *  If this assertion fails then a req list ended
 *  with a REQ_OR instead of a REQ_YES or a REQ_NO
 */
		}

		if (has_item(who, l[i]->item) < l[i]->qty)
		{
			wout(who, "%s does not have %s.", just_name(who),
					box_name_qty(l[i]->item, l[i]->qty));
			return FALSE;
		}

		while (l[i]->consume == REQ_OR && i < ilist_len(l))
			i++;
		i++;
	}

	return TRUE;
}


static void
consume_requirements(int who, int skill)
{
	int i;
	struct entity_skill *p;
	struct req_ent **l;
	int ret;
	int item, qty;

	p = rp_skill(skill);
	if (p == NULL)
		return;

	l = p->req;

	i = 0;
	while (i < ilist_len(l))
	{
		while (has_item(who, l[i]->item) < l[i]->qty &&
		       l[i]->consume == REQ_OR)
		{
			i++;

			if (i >= ilist_len(l))
			{
				fprintf(stderr, "req list ends with REQ_OR, "
						"skill = %d\n", skill);
				assert(FALSE);
			}
/*
 *  If this assertion fails then a req list ended
 *  with a REQ_OR instead of a REQ_YES or a REQ_NO
 */
		}

		item = l[i]->item;
		qty = l[i]->qty;

		while (l[i]->consume == REQ_OR && i < ilist_len(l))
			i++;

		if (l[i]->consume == REQ_YES)
			ret = consume_item(who, item, qty);

		i++;
	}
}


static void
consume_scroll(int who, int basis, int amount)
{
  if (kind(basis) == T_item && p_item_magic(basis)) {
    /*
     *  Use up some days.
     *
     */
    p_item_magic(basis)->orb_use_count -= amount;
    
    /*
     *  Perhaps its used up by now.
     *
     */
    if (p_item_magic(basis)->orb_use_count < 1) {
      wout(who, "%s crumbles into dust.", box_name(basis));
      if (item_unique(basis))
	destroy_unique_item(who, basis);
      else
	consume_item(who, basis, 1);
    };
  };
};

void
experience_use_speedup(struct command *c)
{
	int exp;

	exp = max(c->use_exp - 1, 0);

	/*
	 *  Shorten use of some skills based on experience
	 */
	if (exp && c->wait >= 7)
	{
		if (c->wait >= 14)
			c->wait -= exp;
		else if (c->wait >= 10)
			c->wait -= exp / 2;
		else if (exp >= 2)
			c->wait--;
	}
}

/*
 *  Wed Oct 30 13:22:42 1996 -- Scott Turner
 *
 *  Speed up strength skills by 1 day; slow weakness skills
 *  by 2 days.
 *
 */
void
religion_use_speedup(struct command *c)
{
  if (is_follower(c->who) &&
      rp_relig_skill(is_priest(is_follower(c->who)))->strength ==
	    skill_school(c->use_skill) &&
      c->wait >= 5) {
    c->wait--;
  };
  
  if (is_follower(c->who) &&
      rp_relig_skill(is_priest(is_follower(c->who)))->weakness ==
	    skill_school(c->use_skill)) {
    c->wait += 2;
  };
}

/*
 *  Tue Oct  6 18:18:27 1998 -- Scott Turner
 *
 *  Some artifacts speed skill use.
 *
 */
void
artifact_use_speedup(struct command *c)
{
  int art;
  if (art = best_artifact(c->who, ART_SPEED_USE, c->use_skill, 0)) {
    c->wait -= rp_item_artifact(art)->param1;
    wout(c->who,"Using this skill is magically sped by %s day%s.",
	 nice_num(rp_item_artifact(art)->param1),
	 rp_item_artifact(art)->param1 > 1 ? "s" : "");
    if (c->wait < 1) c->wait = 1;
  };
}

int
v_use(struct command *c)
{
	int sk = c->a;
	int ent;
	int n;
	int basis;		/* what our skill ability is based upon */
	int parent;

	c->use_skill = sk;

	if (!valid_box(sk))
	{
		wout(c->who, "%s is not a valid skill to use.", c->parse[1]);
		c->use_skill = 0;
		return FALSE;
	}

	if (kind(sk) == T_item)
		sk = correct_use_item(c);

	if (kind(sk) == T_item)
		return v_use_item(c);

	if (kind(sk) != T_skill)
	{
		wout(c->who, "%s is not a valid skill to use.", c->parse[1]);
		return FALSE;
	}

	parent = skill_school(sk);

	if (parent == sk)
	{
		wout(c->who, "Skill schools have no direct use.  "
			"Only subskills within a school may be used.");
		return FALSE;
	}

	basis = may_use_skill(c->who, sk);

	if (!basis)
	{
		wout(c->who, "%s does not know %s.",
					just_name(c->who), box_code(sk));
		return FALSE;
	}

/*
 *  If this was (has_skill(c->who, parent) < 1) then a category skill 
 *  couldn't be used from an item.
 */
	if (!may_use_skill(c->who, parent))
	{
 		wout(c->who, "Knowledge of %s is first required before "
		     "%s may be used.",
		     box_name(parent),
		     box_code(sk));
		return FALSE;
	}

	ent = find_use_entry(sk);

	if (ent <= 0)
	{
	  wout(c->who, "There's no way to use %s.",box_code(sk));
	  return FALSE;
	}

	/*
	 *  Mon May  5 12:31:15 1997 -- Scott Turner
	 *  Special checks for magic skills.
	 *
	 */
	if (magic_skill(sk)){
	  if (in_safe_now(c->who)) {
	    wout(c->who, "Magic may not be used in safe havens.");
	    return FALSE;
	  };
	  /*
	   *  Aura
	   *
	   */
	  if (skill_aura(sk) && !check_aura(c->who, skill_piety(sk))) {
	    wout(c->who, "You don't have the aura required to use that spell.");
	    return FALSE;
	  };
	};
	/*
	 *  Special checks for religion skills
	 *  Mon Oct 21 15:32:53 1996 -- Scott Turner
	 *
	 *  + Can't use religion in safe havens.
	 *
	 *  + Check piety, holy symbol, holy plant, etc.
	 *
	 */
	if (religion_skill(sk)) {
	  /*
	   *  Safe Havens.
	   *
	   */
	  if (in_safe_now(c->who)) {
		wout(c->who, "The gods cannot answer prayers in safe havens.");
		return FALSE;
	  };
	  /*
	   *  Right kind of priest.
	   *
	   */
	  if ((skill_school(sk) != sk_basic_religion) &&
	      is_priest(c->who) != skill_school(sk)) {
	    wout(c->who, "You must be a priest of %s to use that prayer.",
		 god_name(skill_school(sk)));
	    return FALSE;
	  };

	  /*
	   *  Piety
	   *
	   */
	  if (!has_piety(c->who, skill_piety(sk))) {
	    wout(c->who, "You don't have the piety required to use that prayer.");
	    return FALSE;
	  };

	  /*
	   *  Holy symbol
	   *
	   */
	  if ((skill_flags(sk) & REQ_HOLY_SYMBOL) && !has_holy_symbol(c->who)) {
	    wout(c->who, "You must have a holy symbol to use that prayer.");
	    return FALSE;
	  };

	  /*
	   *  Holy plant
	   *
	   */
	  if ((skill_flags(sk) & REQ_HOLY_PLANT) && !has_holy_plant(c->who)) {
	    wout(c->who, "You must have a holy plant to use that prayer.");
	    return FALSE;
	  };
	}
	
	if (!meets_requirements(c->who, sk))
		return FALSE;

	cmd_shift(c);
	c->use_ent = ent;
	c->use_skill = sk;
	c->use_exp = has_skill(c->who, sk);
	/* c->poll = use_tbl[ent].poll; */
	c->poll = skill_flags(sk) & IS_POLLED;
	/*	c->wait = use_tbl[ent].time; */
	c->wait = skill_time_to_use(sk);
	c->h = basis;

	experience_use_speedup(c);
	religion_use_speedup(c);
	artifact_use_speedup(c);

	if (use_tbl[ent].start != NULL)
	{
		int ret;

		ret = (*use_tbl[ent].start)(c);

		if (ret)
			magically_speed_casting(c, sk);

		return ret;
	}

/*
 *  Perhaps an assertion here that we are indeed a production
 *  skill use, to catch skills without implementations
 */

	if (n = skill_produce(sk))
	{
		wout(c->who, "Work to produce one %s.", just_name(n));
	}
	else if (use_tbl[ent].finish == NULL)
	{
		fprintf(stderr, "sk == %s\n", box_name(sk));
		assert(FALSE);
	}

	return TRUE;
}


/*
 *  Increment the experience counts for a skill and its parent
 */

void
add_skill_experience(int who, int sk)
{
	struct skill_ent *p;

	p = rp_skill_ent(who, sk);

/*
 *  Don't increase the experience if we don't actually know the
 *  skill.  For instance, use through a scroll or book should
 *  not add experience to the character, unless the character
 *  knows the skill himself.
 */

	if (p == NULL)
		return;

	if (p->exp_this_month == FALSE)
	{
		p->experience++;
		p->exp_this_month = TRUE;
	}
}


int
d_use(struct command *c)
{
	int sk = c->use_skill;
	int ent = c->use_ent;
	int n;
	int basis = c->h;
	int ret = TRUE;

	if (kind(sk) == T_item)
		return d_use_item(c);

/*
 *  c->use_ent is not saved
 *  if it is zero here, look it up again
 *  This is so that it will be re-looked-up across turn boundaries
 */

	if (ent <= 0)
		ent = find_use_entry(sk);

	if (ent <= 0)
	{
		fprintf(stderr, "d_use: no use table entry for %s\n",
					c->parse[1]);
		out(c->who, "Internal error.");
		return FALSE;
	}

/*
 *  Don't call poll routine for ordinary delays
 */

	if (c->wait > 0 && c->poll == 0)
		return TRUE;

	if (c->poll == 0 && !meets_requirements(c->who, sk))
		return FALSE;

/*
 *  Maintain count of how many times each skill is used during
 *  a turn, for informational purposes only.
 *
 *  Wed Dec 23 19:10:39 1998 -- Scott Turner
 *
 *  This counts each day of a polled use as 1!  not good.
 */

	if (sk != sk_breed_beasts &&
	    (!c->poll || c->wait == 0))	
		p_skill(sk)->use_count++;
	p_skill(sk)->last_use_who = c->who;

	/*
	 *  Special checks for religion skills
	 *  Mon Oct 21 15:32:53 1996 -- Scott Turner
	 *
	 *  + Use up piety and plants.
	 *
	 */
	if (religion_skill(sk)) {
	  /*
	   *  Safe Havens.
	   *
	   */
	  if (in_safe_now(c->who)) {
		wout(c->who, "The gods cannot answer prayers in safe havens.");
		return FALSE;
	  };
	  /*
	   *  Right kind of priest.
	   *
	   */
	  if (skill_school(sk) != sk_basic_religion &&
	      is_priest(c->who) != skill_school(sk)) {
	    wout(c->who, "You must be a priest of %s to use that prayer.",
		 god_name(skill_school(sk)));
	    return FALSE;
	  };

	  /*
	   *  Holy symbol
	   *
	   */
	  if ((skill_flags(sk) & REQ_HOLY_SYMBOL) && !has_holy_symbol(c->who)) {
	    wout(c->who, "You must have a holy symbol to use that prayer.");
	    return FALSE;
	  };

	  /*
	   *  Piety
	   *
	   */
	  if (!has_piety(c->who, skill_piety(sk))) {
	    wout(c->who, "You don't have the piety required to use that prayer.");
	    return FALSE;
	  };

	  /*
	   *  Holy plant
	   *
	   */
	  if ((skill_flags(sk) & REQ_HOLY_PLANT) && !has_holy_plant(c->who)) {
	    wout(c->who, "You must have a holy plant to use that prayer.");
	    return FALSE;
	  };
	}

	/*
	 *  Thu Oct 24 15:18:44 1996 -- Scott Turner
	 *
	 *  Calculate return if there's a finish function.  Otherwise
	 *  it automatically worked.
	 *
	 */
	if (use_tbl[ent].finish != NULL) {
	  ret = (*use_tbl[ent].finish)(c);
	};

	/*
	 *  Were we successful?
	 *
	 */
	if (ret) {
	  /*
	   *  Special checks for religion skills
	   *  Mon Oct 21 15:32:53 1996 -- Scott Turner
	   *
	   *  + Use up piety and plants.
	   *
	   */
	  if (religion_skill(sk)) {
	    /*
	     *  Holy plant
	     *
	     */
	    if ((skill_flags(sk) & REQ_HOLY_PLANT)) {
	      move_item(c->who, 0, holy_plant(c->who), 1);
	      wout(c->who,"Used one %s.",box_name(holy_plant(c->who)));
	    };
	    /*
	     *  Piety
	     *
	     */
	    if (!use_piety(c->who, skill_piety(sk))) {
	      wout(c->who, "That was exceedingly strange.  Mention this to the DM, please.");
	    } else if (skill_piety(sk)) {
	      wout(c->who, "Used %s piety.", nice_num(skill_piety(sk)));
	    };
	  };

	  /*
	   *  Experience.
	   *
	   *  Mon Nov 25 11:39:13 1996 -- Scott Turner
	   *
	   *  Modified to only augment for "make" skills.
	   *
	   */
	  if (c->wait == 0 && c->use_exp && ret)
	    add_skill_experience(c->who, sk);

	  /*
	   *  Consumables
	   *
	   */
	  consume_scroll(c->who, basis, c->wait);
	  consume_requirements(c->who, sk);
	  
	  /*
	   *  Production
	   *
	   */
	  if (n = skill_produce(sk)) {
	    gen_item(c->who, n, 1);
	    wout(c->who, "Produced one %s.", box_name(n));
	  }
	};

	return ret;
}


int
i_use(struct command *c)
{
	int sk = c->use_skill;
	int ent = c->use_ent;

	if (ent < 0)
	{
		out(c->who, "Internal error.");
		fprintf(stderr, "i_use: c->use_ent is %d\n", c->use_ent);
		return FALSE;
	}

	if (use_tbl[ent].interrupt != NULL)
		return (*use_tbl[ent].interrupt)(c);
}

/*
 *  Tue Dec  7 17:47:13 1999 -- Scott Turner
 *
 *  Use a special staff to find more staves.
 *
 */
int
v_use_special_staff(struct command *c)
{
  int i, where = province(c->who), reg = region(c->who), dist;
  int flag = FALSE;
  
  if ((char_cur_aura(c->who) < 1 && !has_piety(c->who,1)) ||
      in_faery(where) || in_hades(where) || in_clouds(where)) {
    wout(c->who, "Nothing special happens.");
    return FALSE;
  };

  loop_subkind(sub_special_staff, i) {
    /*
     *  Don't report on items held by the same noble or not
     *  in this region.
     *
     */
    if (item_unique(i) == c->who) continue;
    if (region(item_unique(i)) != reg) continue;
    dist = los_province_distance(c->who, item_unique(i));
    wout(c->who, "You sense another part of the staff %s province%s away.",
	 nice_num(dist), dist == 1 ? "" : "s");
    flag = TRUE;
  } next_subkind;

  if (!flag) {
    wout(c->who, "Nothing special happens.");
    return FALSE;
  };

  return TRUE;
};

int
v_use_item(struct command *c)
{
	int item = c->a;
	int n;
	int ret;

	c->poll = FALSE;
	c->wait = 0;

	if (has_item(c->who, item) < 1)
	{
		wout(c->who, "%s has no %s.", just_name(c->who),
						box_code(item));
		return FALSE;
	}

	n = item_use_key(item);

	if (n == 0 && subkind(item) == sub_special_staff) {
	  n = use_special_staff;
	};

	if (n == 0 && is_artifact(item)) {
	  n = 100+is_artifact(item)->type;
	};

	if (n == 0)
	{
		wout(c->who, "Nothing special happens.");
		return FALSE;
	}

#if 0
	cmd_shift(c);
#endif

/*
 *  If they use a magical object, and we're in a safe
 *  haven, don't allow it.
 */

	switch (n)
	{
	case use_palantir:
	case use_proj_cast:
	case use_quick_cast:
	case use_orb:
	case use_barbarian_kill:
	case use_savage_kill:
	case use_corpse_kill:
	case use_orc_kill:
	case use_skeleton_kill:
	case use_ancient_aura:

		if (in_safe_now(c->who))
		{
			wout(c->who, "Magic may not be used in safe havens.");
			c->wait = 0;
			c->inhibit_finish = TRUE;
			return FALSE;
		}
	}

	switch (n)
	{
	case use_special_staff:
	  ret = v_use_special_staff(c);
	  break;

	case use_heal_potion:
		ret = v_use_heal(c);
		break;

	case use_slave_potion:
		ret = v_use_slave(c);
		break;

	case use_death_potion:
		ret = v_use_death(c);
		break;

	case use_fiery_potion:
		ret = v_use_fiery(c);
		break;

#if 0
	case use_palantir:
		ret = v_use_palantir(c);
		break;
#endif 0

	case use_proj_cast:
		ret = v_use_proj_cast(c);
		break;

	case use_quick_cast:
		ret = v_use_quick_cast(c);
		break;

	case use_drum:
		ret = v_use_drum(c);
		break;

#if 0
	case use_faery_stone:
		ret = v_use_faery_stone(c);
		break;
#endif 0

	case use_orb:
		ret = v_use_orb(c);
		break;
#if 0
	case use_barbarian_kill:
		ret = v_suffuse_ring(c, item_barbarian);
		break;

	case use_savage_kill:
		ret = v_suffuse_ring(c, item_savage);
		break;

	case use_corpse_kill:
		ret = v_suffuse_ring(c, item_corpse);
		break;

	case use_orc_kill:
		ret = v_suffuse_ring(c, item_orc);
		break;

	case use_skeleton_kill:
		ret = v_suffuse_ring(c, item_skeleton);
		break;
#endif 0

	case use_weightlessness_potion:
		ret = v_use_weightlessness(c);
		break;

#if 0
	case use_ancient_aura:
		ret = v_use_ancient_aura(c);
		break;
#endif 0

	case use_nothing:
	  wout(c->who,"Nothing happens.");
	  break;

	case ART_PROT_FAERY+100:
	  ret = v_use_faery_artifact(c);
	  break;

	case ART_DESTROY+100:
	  ret = v_art_destroy(c);
	  break;

	case ART_POWER+100:
	  ret = v_power_jewel(c);
	  break;

	case ART_SUMMON_AID+100:
	  ret = v_summon_aid(c);
	  break;

	case ART_TELEPORT+100:
	  ret = v_art_teleport(c);
	  break;

	case ART_ORB+100:
	case ART_PEN+100:
	  ret = v_art_orb(c);
	  break;

	case ART_CROWN+100:
	  ret = v_art_crown(c);
	  break;

	default:
	  wout(c->who, "Nothing happens.");
	  break;
	}

	if (ret != TRUE || c->wait == 0)
	{
		c->wait = 0;
		c->inhibit_finish = TRUE;
	}

	return ret;
}


int
d_use_item(struct command *c)
{
	int item = c->a;
	int n;

	if (has_item(c->who, item) < 1)
	{
		wout(c->who, "%s no longer has %s.",
				just_name(c->who), box_code(item));
		return FALSE;
	}

	n = item_use_key(item);

	if (n == 0)
	{
		wout(c->who, "Nothing special happens.");
		return FALSE;
	}

	switch (n)
	{
#if 0
	case use_palantir:		return d_use_palantir(c);
	case use_ancient_aura:		return d_use_ancient_aura(c);
#endif 0

	default:
		fprintf(stderr, "bad use key: %d\n", n);
		assert(FALSE);
	}
 
	return TRUE;
}


static int
exp_level(int exp)
{

	switch (exp)
	{
	case 0:
	case 1:		return exp_novice;
	case 2:
	case 3:		return exp_journeyman;
	case 4:
	case 5:		return exp_teacher;
	case 6:
	case 7:
	case 8:		return exp_master;

	default:	return exp_grand;
	}
}


char *
exp_s(int level)
{

	switch (level)
	{
	case exp_novice:	return "apprentice";
	case exp_journeyman:	return "journeyman";
	case exp_teacher:	return "adept";
	case exp_master:	return "master";
	case exp_grand:		return "grand master";

	default:
		assert(FALSE);
	}
}


struct skill_ent *
rp_skill_ent(int who, int skill)
{
	struct entity_char *p;
	int i;

	p = rp_char(who);

	if (p == NULL)
		return NULL;

	for (i = 0; i < ilist_len(p->skills); i++)
		if (p->skills[i]->skill == skill)
			return p->skills[i];

	return NULL;
}


struct skill_ent *
p_skill_ent(int who, int skill)
{
	struct entity_char *p;
	struct skill_ent *new;
	int i;

	p = p_char(who);

	for (i = 0; i < ilist_len(p->skills); i++)
		if (p->skills[i]->skill == skill)
			return p->skills[i];

	new = my_malloc(sizeof(*new));
	new->skill = skill;

	ilist_append((ilist *) &p->skills, (int) new);

	return new;
}

/*
 *  Mon Dec 21 06:20:44 1998 -- Scott Turner
 *
 *  I believe that forgetting a category skill should cause you to also
 *  forget all the subskills of that category.
 *
 */
int
forget_skill(int who, int skill)
{
	struct entity_char *p;
	struct skill_ent *t;
	struct char_magic *ch;
	int i;

	p = rp_char(who);
	if (p == NULL)
		return FALSE;

	t = rp_skill_ent(who, skill);

	if (t == NULL)
		return FALSE;

	ilist_rem_value((ilist *) &p->skills, (int) t);
	wout(who, "Forgot %s.", box_code(skill));

	/*
	 *  Recursively forget if a category skill.
	 *
	 */
 	if (skill_school(skill) == skill) {
	  loop_skill(i) {
	    if (i != skill &&
		skill_school(i) == skill && p_skill_ent(who, i))
	      forget_skill(who, i);
	  } next_skill;
 	}

	if (magic_skill(skill))
	{
		ch = p_magic(who);
		ch->max_aura--;
		if (ch->max_aura < 0)
			ch->max_aura = 0;
	}

	return TRUE;
}


int
v_forget(struct command *c)
{
	int skill = c->a;
	int i;

	if (kind(skill) != T_skill)
	{
		wout(c->who, "%s is not a skill.", box_code(skill));
		return FALSE;
	}

	if (!forget_skill(c->who, skill)) {
		wout(c->who, "Don't know any %s.", box_code(skill));
		return FALSE;
	};

	return TRUE;
}

/*
 *  Fri Aug  9 12:13:04 1996 -- Scott Turner
 *
 *  This is needed because we sometimes want to know if a char has
 *  any religion skill.
 *
 *  Fri Oct  9 18:28:33 1998 -- Scott Turner
 *
 *  We need to check all artifacts?  Blech.
 *
 *  Tue May 23 06:46:04 2000 -- Scott Turner
 *
 *  Modified to check all skills, not just the "known" ones.  That
 *  will pick up priests & magicians half-way through their training.
 *
 *  Fri Jun  2 11:57:26 2000 -- Scott Turner
 *
 *  Modified it back... causes problems elsewhere.
 *
 */
int
has_subskill(int who, int subskill)
{
  struct skill_ent *e;
  struct item_ent *i;

  loop_char_skill_known(who, e) {
    if (subkind(e->skill) == subskill) {
      exit_char_skill_known;
      return e->skill;
    };
  } next_char_skill_known;

  /*
   *  Run through all his ART_SKILL artifacts :-(
   *
   */
  loop_inv(who, i) {
    struct entity_artifact *a = is_artifact(i->item);
    if (a) {
      if (a->type == ART_SKILL &&
	  subkind(a->param1) == subskill) return a->param1;
    };
  } next_inv;

  return 0;
}

/*
 *  Fri Oct  9 18:25:33 1998 -- Scott Turner
 *
 *  You might also have this skill via an artifact!
 *
 */
int
has_skill(int who, int skill)
{
	struct skill_ent *p;

	p = rp_skill_ent(who, skill);

	if (p == NULL || p->know != SKILL_know) {
	  int a = 
	    has_artifact(who, ART_SKILL, skill, 0, 0);
	  if (a)
	    return exp_teacher;
	  return 0;
	};

	return exp_level(p->experience);
}


/*
 *  Use learn_skill() to grant a character a skill
 */

void
set_skill(int who, int skill, int know)
{
	struct skill_ent *p;

	p = p_skill_ent(who, skill);

	p->know = know;
}


int
skill_school(int sk)
{
	int n;
	int count = 0;

	while (1)
	{
		assert(count++ < 1000);

		if (!valid_box(sk) || !rp_skill(sk)) return FALSE;
		n = req_skill(sk);
		if (n == 0)
			return sk;
		sk = n;
	}
}


/*
 *  Order skills for display
 *
 *  Subskills follow their parent
 *  Skills we don't know are pushed to the end
 */

static int
rep_skill_comp(a, b)
struct skill_ent **a;
struct skill_ent **b;
{
	int pa;		/* parent skill of a */
	int pb;		/* parent skill of b */

#if 1
	if ((*a)->know != SKILL_know && ((*b)->know == SKILL_know))
		return -1;

	if ((*b)->know != SKILL_know && ((*a)->know == SKILL_know))
		return 1;
#else
	if ((*a)->know == 0)
		return (*a)->level - (*b)->level;

	if ((*b)->level == 0)
		return (*b)->level - (*a)->level;
#endif

	pa = skill_school((*a)->skill);
	pb = skill_school((*b)->skill);

	if (pa != pb)
		return pa - pb;

	return (*a)->skill - (*b)->skill;
}


static int
flat_skill_comp(a, b)
struct skill_ent **a;
struct skill_ent **b;
{

	return (*a)->skill - (*b)->skill;
}


#if 0
void
list_skill_sup(int who, struct skill_ent *e)
{
	char *exp;

	if (skill_no_exp(e->skill) || skill_school(e->skill) == e->skill)
		wiout(who, CHAR_FIELD+2, "%*s  %s",
			CHAR_FIELD,
			box_code_less(e->skill),
			cap(just_name(e->skill)));
	else
		wiout(who, CHAR_FIELD+2, "%*s  %s, %s %s",
		      CHAR_FIELD,
		      box_code_less(e->skill),
		      cap(just_name(e->skill)),
		      exp_s(exp_level(e->experience)),
		      practice_s(e));
}

#else
void
list_skill_sup(int who, struct skill_ent *e, char *prefix)
{

	if (!prefix[0])
	  tagout(who, "<tag type=skill id=%d skill=%d exp=%d>",
	       who, e->skill, e->experience);
	if (skill_no_exp(e->skill) || skill_school(e->skill) == e->skill)
		wout(who, "%s%s", prefix, box_name(e->skill));
	else
		wout(who, "%s%s, (Level %d)", prefix, box_name(e->skill),
		     e->experience);

	/*
	 *  Fri Apr 27 10:55:26 2001 -- Scott Turner
	 *
	 *  Add a line for combat skills that says what round you will
	 *  use them.
	 *
	 */
	if (kind(who) == T_char &&
	    combat_skill(e->skill) &&
	    get_effect(who, ef_cs, e->skill, 0)) {
	  char *rounds = "";
	  struct effect **ef = NULL;
	  int i;
	  /*
	   *  Walk through all the effects and gather up the rounds.
	   *
	   */
	  ef = effects(who);
	  for(i=0;i < ilist_len(ef);i++)
	    if (ef[i]->type == ef_cs &&
		ef[i]->subtype == e->skill) {
	      rounds = sout("%s %d", rounds, ef[i]->data);
	    };
	  wout(who, "  Use in rounds: %s", rounds);
	};

	if (!prefix[0]) 
	  tagout(who, "</tag type=skill>");

}
#endif


void
list_skills(int who, int num, char *prefix)
{
	int i;
	struct skill_ent **l;
	int flag = TRUE;

	assert(valid_box(num));

	if (!prefix[0]) out(who, "");
	out(who, "%sSkills known:", prefix);
	indent += 3;

	if (rp_char(num) == NULL)
		goto list_skills_end;

	if (ilist_len(rp_char(num)->skills) < 1)
		goto list_skills_end;

	l = (struct skill_ent **) ilist_copy((ilist) rp_char(num)->skills);
	qsort(l, ilist_len(l), sizeof(int), rep_skill_comp);

	for (i = 0; i < ilist_len(l); i++)
	{
		if (l[i]->know != SKILL_know)
			continue;

		flag = FALSE;
#if 0
		if (i > 0 &&
		    skill_school(l[i]->skill) != skill_school(l[i-1]->skill))
			out(who, "");
#endif

		if (req_skill(l[i]->skill))
		{
			indent += 6;
			list_skill_sup(who, l[i], prefix);
			indent -= 6;
		}
		else
		{
#if 0
			if (i > 0 && req_skill(l[i-1]->skill))
				out(who, "");
#endif

			list_skill_sup(who, l[i], prefix);
		}
	}

	ilist_reclaim((ilist *) &l);

list_skills_end:

	if (flag)
		out(who, "%snone", prefix);

	indent -= 3;
}



/*
 *  Archery, 0/7
 *  Archery, 1/7
 *  Archery, 0/7, 1 NP req'd
 */

static char *
fractional_skill_qualifier(struct skill_ent *p)
{
	extern char *np_req_s();
	int frac;
	char *explanation = "";

	assert(p->know != SKILL_know);

	if (p->know == SKILL_dont)
	{
		return sout("0/%d%s",
				learn_time(p->skill),
				np_req_s(p->skill));
	}

	assert(p->know == SKILL_learning);

	if (p->days_studied / TOUGH_NUM >= learn_time(p->skill)) {
	  explanation = sout("(Religion weakness skill)");
	};

	frac = p->days_studied % TOUGH_NUM;

	if (frac == 0)
		return sout("%d/%d %s", p->days_studied / TOUGH_NUM,
				learn_time(p->skill),
			    explanation);

	return sout("%d.%d/%d %s", p->days_studied / TOUGH_NUM,
				frac * 100 / TOUGH_NUM,
				learn_time(p->skill),
		    explanation);
}


void
list_partial_skills(int who, int num, char *prefix)
{
	int i;
	struct skill_ent **l;
	int flag = TRUE;

	assert(valid_box(num));

	if (rp_char(num) == NULL)
		return;

	if (ilist_len(rp_char(num)->skills) < 1)
		return;

	l = (struct skill_ent **) ilist_copy((ilist) rp_char(num)->skills);
	qsort(l, ilist_len(l), sizeof(int), flat_skill_comp);

	for (i = 0; i < ilist_len(l); i++)
	{
		if (l[i]->know == SKILL_know)
			continue;

		if (flag)
		{
			if (!prefix[0]) out(who, "");
			out(who, "%sPartially known skills:",prefix);
			indent += 3;
			flag = FALSE;
		}

		wiout(who, 6, "%s%s, %s",
		      prefix,
		      box_name(l[i]->skill),
		      fractional_skill_qualifier(l[i]));
	}

	if (!flag)
	{
		indent -= 3;
	}

	ilist_reclaim((ilist *) &l);
}


int
skill_cost(int sk)
{

	return 100;
}

/*
 *  Thu Dec 24 08:12:51 1998 -- Scott Turner
 *
 *  Can "teach" in a tower or a guild, if you're teaching a skill
 *  from that guild.
 *
 */
static
int teachable_place(int where, int sk)
{
    return (subkind(where) == sub_tower ||
	    is_guild(where) == skill_school(sk));
};

/*
 *  Sat Nov  2 09:00:07 1996 -- Scott Turner
 *
 *  Returns one of the following flags or the id of the scroll/book
 *  being used.
 *
 *  We want to try to find TAUGHT_SPECIFIC, then scroll/book, then
 *  TAUGHT_GENERIC.
 *
 *  Thu Apr 16 09:04:12 1998 -- Scott Turner
 *
 *  Added a separate return for the item being used.
 *
 */
int
being_taught(int who, int sk, int *item, int *teach_bonus)
{
  int school = skill_school(sk);
  int where = subloc(who);
  struct entity_subloc *p = rp_subloc(where);
  int teacher, loc, i;
  struct command *c, *c2;
  int taught_specific = 0, taught_generic = 0;
  int v_teach(), v_study();
  int ret_specific = 0, ret_generic = 0;
  int taught_studypoints = 0;
  int a;

  *item = 0;
  /*
   *  Possibly being taught in this location.
   *
   */
  if (p && ilist_lookup(p->teaches, sk) >= 0)
    taught_specific = 1;
  
  if (p && ilist_lookup(p->teaches, school) >= 0)
    taught_generic = 1;

  /*
   *  Are you in a teaching tower?
   *
   */
  if (teachable_place(where, sk)) {
    /*
     *  Mon Dec 21 07:23:15 1998 -- Scott Turner
     *
     *  Teacher is the first noble in the tower using the
     *  "teach" command.
     *
     *  Thu Mar 23 12:51:41 2000 -- Scott Turner
     *
     *  Hmm, must check to make sure he's actually able to teach
     *  this command!
     *
     */
    loop_stack(where, i) {
      c = rp_command(i);
      if (c && cmd_tbl[c->cmd].start == v_teach &&
	  c->state == RUN) {
	teacher = i;
	break;
      };
    } next_stack;

    /*  Is he teaching? */
    if (teacher && c && cmd_tbl[c->cmd].start == v_teach) {
      /*  Are we close to him? */
      /*
       *  Mon Dec 21 06:55:29 1998 -- Scott Turner
       *
       *  Modified to say that you only have to be one of the first
       *  five students that could benefit from his teaching.
       *
       */
      int loc = 0;
      loop_stack(where, i) {
	if (i == who) break;  /* you. */
	/*
	 *  Get this person's command.
	 *
	 */
	c2 = rp_command(i);
	/*
	 *  Is this person studying and benefiting
	 *  from the teacher?
	 *
	 */
	if (c2 && cmd_tbl[c2->cmd].start == v_study &&
	    (c2->a == c->a || skill_school(c2->a) == c->a))
	  loc++;
      } next_stack;

      if (loc <= 6) {
	/*  What is he teaching? */
	if (c->a == sk) taught_specific = 1;
	if (c->a == school) taught_generic = 1;
	/*
	 *  Maybe he has an artifact bonus.
	 *
	 */
	if (a = best_artifact(c->who, ART_TEACHING, 0, 0)) {
	  *teach_bonus += rp_item_artifact(a)->param1;
	};
      }
    };
  };

  /*  
   *  Maybe you have a book?
   *
   */

  {
    struct item_ent *e;
    struct item_magic *p;

    loop_inv(who, e) {
      p = rp_item_magic(e->item);
      if (p && ilist_lookup(p->may_study, sk) >= 0 &&
	  p->orb_use_count > 0 &&
	  subkind(e->item) == sub_book) {
	ret_specific = e->item;
	break;
      } else if (p && ilist_lookup(p->may_study, school) >= 0 &&
	  p->orb_use_count > 0 &&
	  subkind(e->item) == sub_book) {
	ret_generic = e->item;
      };
    } next_inv;
  } 

  /*
   *  Wed Apr 15 11:58:33 1998 -- Scott Turner
   *
   *  Another version of study points, that provides instruction
   *  albeit only in the "common" skills.
   *
   */
  if ((sk == sk_combat ||
       sk == sk_shipcraft ||
       sk == sk_construction ||
       sk == sk_forestry ||
       sk == sk_ranger ||
       sk == sk_mining ||
       sk == sk_trading) && player_js(player(who))) {
    taught_studypoints = 1;
  };

  if (taught_specific) return TAUGHT_SPECIFIC;
  if (ret_specific) {
    *item = ret_specific;
    return TAUGHT_SPECIFIC;
  };
  if (taught_generic) return TAUGHT_GENERIC; 
  if (ret_generic) {
    *item = ret_generic;
    return TAUGHT_GENERIC;
  };
  if (taught_studypoints) return TAUGHT_STUDYPOINTS;
  return NOT_TAUGHT; 
}

#if 0
/*
 *  If it's taught by the location
 *  If it's offered by a skill that we know
 *  If it's offered by an item that we have
 */

static int
may_study(int who, int sk)
{
	int where = subloc(who);

/*
 *  Does the location offer the skill?
 */

	{
		struct entity_subloc *p;

		p = rp_subloc(where);

		if (p && ilist_lookup(p->teaches, sk) >= 0)
			return where;
	}

/*
 *  Is the skill offered by a skill that we already know?
 */

	{
		struct entity_skill *q;
		struct skill_ent *e;
		int ret = 0;

		loop_char_skill_known(who, e)
		{
			q = rp_skill(e->skill);

			if (q && ilist_lookup(q->offered, sk) >= 0)
			{
				ret = e->skill;
				break;
			}
		}
		next_char_skill_known;

		if (ret)
			return ret;
	}

/*
 *  Is instruction offered by a scroll or a book? 
 *
 *  Items other than scrolls should take precedence, to preserve
 *  the one-shot scrolls.
 */

	{
		struct item_ent *e;
		struct item_magic *p;
		int ret = 0;
		int scroll = 0;

		loop_inv(who, e)
		{
			p = rp_item_magic(e->item);
			if (p && ilist_lookup(p->may_study, sk) >= 0)
			{
				if (subkind(e->item) == sub_scroll)
					scroll = e->item;
				else
					ret = e->item;
			}
		}
		next_inv;

		if (ret)
			return ret;

		if (scroll)
			return scroll;
	}

	return 0;
}
#endif 0

static int
begin_study(struct command *c, int sk)
{
  int cost;
  struct skill_ent *p;
  int np_req, num_base_skills = 0;
  struct skill_ent *e;
  
  cost = skill_cost(sk);
  np_req = skill_np_req(sk);

  /*
   *  Fri Nov  1 13:02:38 1996 -- Scott Turner
   *
   *  Add on cost if this is the 4th+ skill category this guy
   *  is learning.
   *
   */
  loop_char_skill(c->who, e) {
    if (e->skill == sk_basic_religion ||
	e->skill == sk_adv_sorcery) continue;
    if (!rp_skill(e->skill)->required_skill) num_base_skills++;
  } next_char_skill;

  if (num_base_skills >= 3 && !rp_skill(sk)->required_skill) {
    np_req++;
  };

  if (!is_npc(c->who) && np_req > 0) {
    wout(c->who, "It will require %s noble point%s to complete "
	 "the study of %s.",
	 cap(nice_num(np_req)),
	 add_s(np_req),
	 box_code(sk));
  };

  if (!is_npc(c->who) && cost > 0) {
    if (!charge(c->who, cost)) {
      wout(c->who, "Cannot afford %s to begin study.",
	   gold_s(cost));
      return FALSE;
    }
    wout(c->who, "Paid %s to begin study.", gold_s(cost));
  }

  p = p_skill_ent(c->who, sk);
  p->know = SKILL_learning;

  return TRUE;
}

static int
end_study(struct command *c, int sk)
{
  int num_base_skills = 0;
  struct skill_ent *e;
  int np_req = skill_np_req(sk);

  /*
   *  Fri Nov  1 13:02:38 1996 -- Scott Turner
   *
   *  Add on cost if this is the 4th+ skill category this guy
   *  is learning.
   *
   */
  loop_char_skill(c->who, e) {
    if (e->know != SKILL_know) continue;
    if (e->skill == sk_basic_religion ||
	e->skill == sk_adv_sorcery) continue;
    if (!rp_skill(e->skill)->required_skill) num_base_skills++;
  } next_char_skill;

  if (num_base_skills >= 3 && !rp_skill(sk)->required_skill) {
    np_req++;
  };

  if (!is_npc(c->who) && np_req > 0) {
    wout(c->who, "Deducted %s noble point%s to complete study.",
	 nice_num(np_req), add_s(np_req));
    deduct_np(player(c->who), np_req);
  } else if (np_req > 0) {
    wout(c->who, "%s noble point%s %s required to complete "
	 "the study of %s.",
	 cap(nice_num(np_req)),
	 add_s(np_req),
	 np_req == 1 ? "is" : "are",
	 box_code(sk));
    return FALSE;
  };
  
  return TRUE;
};

#if 0
/*
 *  To study through a scroll, the character should STUDY the
 *  skill number, not study the scroll.  If they study the scroll,
 *  guess which skill they meant to learn from within the scroll.
 */
static int
correct_study_item(struct command *c)
{
  int item = c->a;
  struct item_magic *p;

  p = rp_item_magic(item);

  if (p == NULL || ilist_len(p->may_study) < 1)
    return item;

  c->c = item;
  c->a = p->may_study[0];
  return c->a;
}
#endif 0

/*
 *  Thu Apr  9 08:48:40 1998 -- Scott Turner
 *
 *  Code to check to see if we can study something.
 *
 */
static int
check_study(struct command *c, int requires_instruction)
{
  struct skill_ent *p;
  struct entity_skill *q;
  int sk = c->a;
  int parent, category = 0, teachable = 0, unteachable = 0, bt = 0;
  int guild = 0;
  int basis = 0;
  int item = 0;
  int bonus = 0;

#if 0
  /*
   *  Thu Jan  7 18:30:50 1999 -- Scott Turner
   *
   *  I believe this is no longer needed.
   *
   */
  
  /*
   *  In case they're studying from a book...
   *
   */
  c->c = 0;
  if (kind(sk) == T_item) sk = correct_study_item(c);
#endif 0

  if (kind(sk) != T_skill) {
    wout(c->who, "%s is not a valid skill.", c->parse[1]);
    return FALSE;
  }

  parent = skill_school(sk);

  if (nation(c->who) &&
      (ilist_lookup(rp_nation(nation(c->who))->proscribed_skills, sk) != -1 ||
       ilist_lookup(rp_nation(nation(c->who))->proscribed_skills, parent) != -1))
    {
      wout(c->who,"%ss may not learn that skill.",
	   rp_nation(nation(c->who))->citizen);
      return FALSE;
    };

  if (parent != sk && has_skill(c->who, parent) < 1) {
    wout(c->who, "%s must be learned before %s can be known.",
	 cap(box_name(parent)),
	 box_code(sk));
    return FALSE;
  }

  p = rp_skill_ent(c->who, sk);

  if (p && p->know == SKILL_know) {
    wout(c->who, "Already know %s.", box_name(sk));
    return FALSE;
  }

  /*
   *  Tue Feb 22 09:00:39 2000 -- Scott Turner
   *
   *  You cannot study Basic Religion or Advanced Sorcery
   *
   */
  if (sk == sk_basic_religion || sk == sk_adv_sorcery) {
    wout(c->who, "You cannot study that skill.");
    return FALSE;
  };

  /*
   *  Fri Aug  9 12:17:47 1996 -- Scott Turner
   *
   *  Can't learn a (top-level) religion skill if you already
   *  know one.
   *
   */
  if (is_priest(c->who) &&
      religion_skill(sk) &&
      skill_school(sk) != sk_basic_religion && 
      skill_school(sk) != is_priest(c->who)) {
    wout(c->who, "A priest may not learn a second religion.");
    return FALSE;
  };

  /*
   *  Fri Nov 29 09:24:28 1996 -- Scott Turner
   *
   *  Priests must be guild members of their strength skill.
   *
   */
  if (religion_skill(sk) &&
      skill_school(sk) != sk_basic_religion &&
      guild_member(c->who) &&
      guild_member(c->who) != rp_relig_skill(skill_school(sk))->strength) {
    wout(c->who,"Because you're a member of the %s Guild, the %s will not accept you.",
	 box_name(guild_member(c->who)),
	 box_name(sk));
    return FALSE;
  };

  /*
   *  Fri Aug  9 12:19:36 1996 -- Scott Turner
   *
   *  Priests can't learn magic.
   */
  if (magic_skill(sk) && is_priest(c->who)) {
    wout(c->who, "Priests may not study magic.");
    return FALSE;
  };

  /*
   *  Tue Apr  7 11:35:34 1998 -- Scott Turner
   *
   *  Followers can't learn magic.
   */
  if (magic_skill(sk) && is_follower(c->who)) {
    wout(c->who, "Your dedication to %s prevents you from learning magic.",
	 box_name(is_priest(is_follower(c->who))));
    return FALSE;
  };

  /*
   *  Fri Aug  9 12:19:36 1996 -- Scott Turner
   *
   *  Priests can't learn their weakness skill.
   * 
   */
  if (is_priest(c->who) &&
      rp_skill(is_priest(c->who))->religion_skill->weakness == sk) {
    wout(c->who, "Priests may not study their weakness skill.");
    return FALSE;
  };

  /*
   *  Fri Nov 29 09:27:29 1996 -- Scott Turner
   *
   *  Uh, magicians can't learn religion, either.
   *
   */
  if (religion_skill(sk) &&
      is_wizard(c->who)) {
    wout(c->who, "Magicians may not study religion.");
    return FALSE;
  };

  /*
   *  Thu Apr  9 08:55:53 1998 -- Scott Turner
   *
   *  If we don't require instruction, then we're done.
   *
   */
  if (is_npc(c->who) || !requires_instruction) return TRUE;

  /*
   *  What kind of skill are they learning?
   *
   *  Fri Nov 29 09:53:08 1996 -- Scott Turner
   *
   *  Added guild skills.
   */
  category = (parent == sk);
  if (!category) {
    teachable = ilist_lookup(rp_skill(parent)->offered,sk) != -1;
    guild = ilist_lookup(rp_skill(parent)->guild,sk) != -1;
  };
  if (!category && !teachable && !guild) unteachable = 1;

  /*
   *  What's the teaching situation?
   *
   */
  bt = being_taught(c->who, sk, &item, &bonus);

  /*
   *  Correct for missing flag.
   *
   */
  if (category && bt == TAUGHT_STUDYPOINTS && !c->b) {
    wout(c->who, "You have no source of instruction.");
    wout(c->who, "To use study points, add a flag to your "
	 "study command, e.g.,");
    wout(c->who, "    study %d 1", c->a);
    return FALSE;
  };

  /*
   *  Category skills must be taught.
   *
   */
  if (category && !bt) {
    wout(c->who,"Category skills may only be learned with a teacher.");
    return FALSE;
  };

  /*
   *  You must be a guild member to learn a guild skill.
   *
   */
  if (guild && guild_member(c->who) != skill_school(sk)) {
    wout(c->who, "You must be a guild member to learn that skill.");
    return FALSE;
  };

  /*
   *  Guild skills must be learned in a guild.
   *
   */
  if (guild && is_guild(subloc(c->who)) != skill_school(sk)) {
    wout(c->who,"Guild skills may only be studied in a guild.");
    return FALSE;
  };
  
  /*
   *  Unteachable skills do not benefit from a teacher.
   *
   */
  if (unteachable && bt) {
    wout(c->who,"Instruction does not benefit in learning %s.",
	 box_name(sk));
  };
  return TRUE;
};

/*
 *	if we know it already, then error
 *	if we have already started studying, then continue
 *	if we may not study it, then error
 *
 *	if we don't have enough money, then error
 *
 *	start studying skill:
 *		init skill entry
 *		deduct money
 *
 *   Fri Aug  9 12:11:36 1996 -- Scott Turner
 *
 *   Added restrictions due to religion.
 *
 *   Sat Nov  2 08:59:33 1996 -- Scott Turner
 *
 *   New teaching changes
 *
 *   Tue Jul  1 16:24:00 1997 -- Scott Turner
 *
 *   Can't learn a skill on your nation's "proscribed_skill" list.
 *
 *  Wed Apr 22 11:09:09 1998 -- Scott Turner
 *
 *  NPCs don't need instruction or money, but learn slowly
 *
 *  Wed Oct 28 07:18:37 1998 -- Scott Turner
 *
 *  Need to save the "solitary" status somewhere in "c" so that we can
 *  check that it has held the entire time of the study, not just at the
 *  end.
 *
 */
int
v_study(struct command *c)
{
  struct skill_ent *p;
  int sk = c->a;
  int where;

#if 0
  /*
   *  In case they're studying from a book...
   *
   */
  c->c = 0;
  if (kind(sk) == T_item) sk = correct_study_item(c);
#endif 0

  if (numargs(c) < 1) {
    wout(c->who, "Must specify a skill to study.");
    return FALSE;
  }

  if (!check_study(c,TRUE)) return FALSE;

  p = rp_skill_ent(c->who, sk);

  /*
   *  Give him the skill and charge him.
   *
   */
  if (!p) {
    if (!begin_study(c, sk)) return FALSE;
  };

  /*
   *  If you're alone in a tower, then get a +1 day bonus.
   *  We keep the status for this in c->c.
   */
  c->c = 1;
  where = subloc(c->who);
  if (subkind(where) != sub_tower ||
      !alone_here(c->who)) {
    c->c = 0;
  };

  wout(c->who, "Study %s for %s day%s.",
       just_name(sk),
       nice_num(c->wait), add_s(c->wait));
  
  return TRUE;
}

/*
 *  Use learn_skill() to grant a character a skill
 *
 *  Fri Aug  9 11:47:15 1996 -- Scott Turner
 *
 *  Modified for religions.
 *
 */

int
learn_skill(int who, int sk)
{
  struct skill_ent *p;
  struct char_magic *ch;

  if (nation(who) &&
      (ilist_lookup(rp_nation(nation(who))->proscribed_skills, sk) != -1 ||
       ilist_lookup(rp_nation(nation(who))->proscribed_skills, skill_school(sk)) != -1)) {
    return FALSE;
  };
  
  p = p_skill_ent(who, sk);

  /*
   *  Archery grants a missile attack.
   *
   */
  if (sk == sk_archery) {
    struct entity_char *pc;

    pc = p_char(who);

    if (pc->missile < 50)
      pc->missile += 50;
  }

  /*
   *  Personal FTTD needs to set the personal break point
   *
   */
  if (sk == sk_personal_fttd) {
    p_char(who)->personal_break_point = 100;
  }

  ch = p_magic(who);

  if (magic_skill(sk))
    {
      ch->max_aura++;
      add_aura(who,1);

      wout(who, "Maximum aura now %d.", ch->max_aura);

      p_magic(who)->magician = TRUE;

      if (sk == sk_weather)
	p_magic(who)->knows_weather = 1;

      /*
       *  Undedicate him, if he's dedicated to someone.
       *
       */
      if (rp_char(who)->religion.priest) {
	wout(rp_char(who)->religion.priest,
	     "An angel informs you that your follower %s has become a heretic.",
	     box_name(who));
	ilist_rem_value(&(rp_char(rp_char(who)->religion.priest)->religion.followers), who);
	rp_char(who)->religion.priest = 0;
      };
    }

  /*
   *  It's his first religion skill, so let's do all the
   *  special tasks for religion skills.
   *
   */
  if (religion_skill(sk) && !is_priest(who)) {
    struct skill_ent *e;
    /*
     *  Undedicate him, if he's dedicated to someone.
     *
     */
    if (rp_char(who)->religion.priest) {
      wout(rp_char(who)->religion.priest,
	   "An angel informs you that your follower %s has become a priest of %s.",
	   box_name(who),
	   god_name(sk));
      ilist_rem_value(&(rp_char(rp_char(who)->religion.priest)->religion.followers), who);
      rp_char(who)->religion.priest = 0;
    };
	  
    /*
     *  Eliminate any magic skills.
     *
     */
    loop_char_skill_known(who, e) {
      if (magic_skill(e->skill)) {
	forget_skill(who, e->skill);
      };
    } next_char_skill_known;
    p_magic(who)->max_aura = 0;
    p_magic(who)->cur_aura = 0;

    /*
     *  Eliminate the "weakness" skill.
     *
     */
    if (rp_skill(sk)->religion_skill->weakness &&
	rp_skill(rp_skill(sk)->religion_skill->weakness)) {
      forget_skill(who,rp_skill(sk)->religion_skill->weakness);
    };
    /*
     *  Add the "strength skill" -- careful of a loop here!
     *
     */
    if (rp_skill(sk)->religion_skill->strength &&
	rp_skill(rp_skill(sk)->religion_skill->strength))
      learn_skill(who,rp_skill(sk)->religion_skill->strength);

    /*
     *  Tue Dec 29 12:43:33 1998 -- Scott Turner
     *
     *  He also learns "Basic Religion"
     *
     *  Thu Feb 10 19:27:13 2000 -- Scott Turner
     *
     *   Make sure he knows the religion skill, so we don't
     *   get into an infinite loop.
     *
     */
    p->know = SKILL_know;
    p->days_studied = 0;
    learn_skill(who, sk_basic_religion);

    /*
     *  Mon Apr  2 08:43:40 2001 -- Scott Turner
     *
     *  Give him enough piety to dedicate a temple.
     *
     */
    add_piety(who, skill_piety(sk_dedicate_temple_b), 0);
    wout(who,"%s grants you %s piety for your new devotion as priest.",
	 god_name(sk), nice_num(skill_piety(sk_dedicate_temple_b)));
  };

  wout(who, "Learned %s.", box_name(sk));
  p->know = SKILL_know;
  p->days_studied = 0;

  return TRUE;
}

/*
 *  Wed Apr 15 12:10:44 1998 -- Scott Turner
 *
 *  Use up a study point.
 *
 */
void
use_studypoint(int who)
{
  int pl = player(who);
  if (pl) {
    rp_player(pl)->jump_start--;
    if (rp_player(pl)->jump_start < 0)
      rp_player(pl)->jump_start = 0;
  };
}

/*
 *  Mon Dec 13 18:24:28 1999 -- Scott Turner
 *
 *  To be alone here, you must be the only unit in the location, and not
 *  have any men in your inventory.
 *
 */
int
alone_here(int who)
{
  ilist ll = NULL;
  int alone = 0;
  
  all_char_here(subloc(who), &ll);
  alone = ilist_len(ll)-1;
  ilist_reclaim(&ll);
  return (!alone ? char_alone(who) : 0);
};

/*
 *  Note:  d_study is polled daily
 */

int
d_study(struct command *c)
{
  struct skill_ent *p;
  int sk = c->a;
  struct entity_char *ch;
  int parent = 0, category = 0, teachable = 0, unteachable = 0;
  int diminish, bt, where;
  int religion_bonus = 0, building_bonus = 0, artifact_bonus = 0;
  struct item_magic *ip;
  int a;
  int item;
  int npc_penalty = is_npc(c->who) ? 2 : 1;
  int bonuses = 0;
  
  if (kind(sk) != T_skill) {
    log_output(LOG_CODE, "d_study: skill %d is gone, who=%d\n", sk, c->who);
    out(c->who, "Internal error: skill %s is gone", box_code(sk));
    return FALSE;
  }

  ch = p_char(c->who);
  ch->studied++;
  
  if (ch->studied <= 7) diminish = 14;
  else if (ch->studied <= 14) diminish = 8;
  else if (ch->studied <= 21) diminish = 4;
  else diminish = 2;

  /*
   *  What kind of skill are they learning?
   *
   */
  parent = skill_school(sk);
  category = (parent == sk);
  if (!category)
    teachable = ilist_lookup(rp_skill(parent)->offered,sk) != -1;
  if (!category && !teachable) unteachable = 1;

  /*
   *  Sun Nov  3 19:08:28 1996 -- Scott Turner
   *
   *  Adjust diminish for the state of being taught.
   *
   */
  bt = being_taught(c->who, sk, &item, &artifact_bonus);
  if (!is_npc(c->who) && category && !bt) {
    wout(c->who,"Category skills may only be learned with a teacher.");
    return FALSE;
  };

  if ((teachable || category) && bt == TAUGHT_STUDYPOINTS && !c->b) {
    if (category) {
      wout(c->who, "You have no source of instruction.");
      wout(c->who, "To use study points, add a flag to your "
	   "study command, e.g.,");
      wout(c->who, "    study %d 1", c->a);
      return FALSE;
    };
    bt = 0;
  };

  /*
   *  Unteachable skills do not benefit from a teacher.
   *
   */
  if (teachable && bt) {
    if (bt == TAUGHT_GENERIC) {
      diminish *= 2;
    } else if (bt == TAUGHT_SPECIFIC) {
      diminish *= 4;
    } else if (bt == TAUGHT_STUDYPOINTS) {
      use_studypoint(c->who);
    };
  };

  if (category && bt == TAUGHT_STUDYPOINTS) {
    use_studypoint(c->who);
  };

  if ((teachable || category) && bt && item &&
      (ip = rp_item_magic(item))) {
    consume_scroll(c->who, item, 1);
  };

  p = p_skill_ent(c->who, sk);

  p->days_studied += (TOUGH_NUM * diminish) / (14 * npc_penalty);

  /*
   *  Wed Oct 30 13:00:44 1996 -- Scott Turner
   *
   *  If you're a follower, then you get a bonus of 1 day in
   *  learning skills in your strength skill category, and -2 days
   *  in your weakness category.
   *
   */
  if (is_follower(c->who) &&
      rp_relig_skill(is_priest(is_follower(c->who)))->strength ==
      skill_school(sk)) {
    religion_bonus = TOUGH_NUM;
  };

  if (is_follower(c->who) &&
      rp_relig_skill(is_priest(is_follower(c->who)))->weakness ==
      skill_school(sk)) {
    religion_bonus = -2 * TOUGH_NUM;
  };

  /*
   *  If you're alone in a tower, then get a +1 day bonus.
   *
   */
  where = subloc(c->who);
  if (subkind(where) != sub_tower ||
      !alone_here(c->who)) {
    c->c = 0;
  };
  if (c->c) {
    building_bonus = TOUGH_NUM;
  };

  /*
   *  Bonuses from artifacts.
   *
   */
  if (a = best_artifact(c->who, ART_LEARNING, 0, 0)) {
    artifact_bonus += rp_item_artifact(a)->param1;
  };

  if (p->days_studied + religion_bonus + building_bonus + artifact_bonus
      >= learn_time(sk) * TOUGH_NUM)
    {
      /*
       *  Report bonuses.
       *
       */
      if (building_bonus) {
	wout(c->who,"Your study benefits from the solitude of your tower.");
      };
      if (religion_bonus > 0) {
	wout(c->who,"Your study is inspired by holy energy.");
      };
      if (religion_bonus < 0) {
	wout(c->who,"This topic offends your values and is difficult to learn.");
      };

      /*
       *  Now try to end our study
       *
       */
      if (!end_study(c, sk)) {
	c->wait = 0;
	return FALSE;
      }
      /*
       *  Otherwise learn the skill and be done.
       *
       */
      learn_skill(c->who, sk);
      c->wait = 0;
      return TRUE;
    }

  /* message here if done? */
  return TRUE;
}

#if 0
int
v_acquire(struct command *c)
{
  struct skill_ent *p;
  int sk = c->a;

  if (numargs(c) < 1) {
    wout(c->who, "Must specify a skill to acquire.");
    return FALSE;
  }

  if (!check_study(c,FALSE)) return FALSE;

  p = rp_skill_ent(c->who, sk);

  /*
   *  Give him the skill and charge him.
   *
   */
  if (!begin_study(c, sk)) return FALSE;
  learn_skill(c->who, sk);
  return TRUE;
}
#endif

/*
 *  Thu Jul  3 15:16:22 1997 -- Scott Turner
 *
 *  Practice a skill.
 *
 */
int
v_practice(struct command *c)
{
  struct skill_ent *p;
  struct entity_skill *q;
  int sk = c->a, days = c->b;

  if (numargs(c) < 1) {
    wout(c->who, "Must specify a skill to practice.");
    return FALSE;
  }

  if (kind(sk) != T_skill) {
    wout(c->who, "%s is not a valid skill.", c->parse[1]);
    return FALSE;
  }

  /*
   *  They must know the skill.
   *
   */
  if (!has_skill(c->who,sk)) {
    wout(c->who, "Oddly enough, you must know a skill before practicing it.");
    return FALSE;
  }
  /*
   *  It must be "practiceable"
   *
   */
  if (!practice_time(sk)) {
    wout(c->who, "Cannot practice '%s'.", c->parse[1]);
    return FALSE;
  };

  /*
   *  Need the practice cost.
   *
   */
  if (practice_cost(sk) > 0 && !charge(c->who, practice_cost(sk))) {
    wout(c->who, "Can't afford %s to practice %s.",
	 gold_s(practice_cost(sk)), box_name(sk));
    return FALSE;
  }

  /*
   *  Days to practice?
   *
   */
  c->wait = practice_time(sk);

  /*
   *  Wed May 27 13:09:00 1998 -- Scott Turner
   *
   *  It might be progressively harder.
   *
   */
  if (practice_progressive(sk)) {
    c->wait += (p_skill_ent(c->who, sk)->experience)/practice_progressive(sk);
  };

  wout(c->who, "Practice %s for %s day%s.",
       just_name(sk),
       nice_num(c->wait), add_s(c->wait));
  
  return TRUE;
}

/*
 *  Note:  d_practice is polled daily
 *
 *  Wed May 13 19:00:16 1998 -- Scott Turner
 *
 *  Not any more!
 *
 */
int
d_practice(struct command *c)
{
  struct skill_ent *p;
  int sk = c->a;
  
  if (kind(sk) != T_skill) {
    log_output(LOG_CODE, "d_study: skill %d is gone, who=%d\n", sk, c->who);
    out(c->who, "Internal error: skill %s is gone", box_code(sk));
    return FALSE;
  }

  if (numargs(c) < 1) {
    wout(c->who, "Must specify a skill to study.");
    return FALSE;
  }

  if (kind(sk) != T_skill) {
    wout(c->who, "%s is not a valid skill.", c->parse[1]);
    return FALSE;
  }

  /*
   *  They must know the skill.
   *
   */
  if (!has_skill(c->who,sk)) {
    wout(c->who, "Oddly enough, you must know a skill before practicing it.");
    return FALSE;
  }
  /*
   *  It must be "practiceable"
   *
   */
  if (!practice_time(sk)) {
    wout(c->who, "Cannot practice '%s'.", c->parse[1]);
    return FALSE;
  };

  p = p_skill_ent(c->who, sk);
  p->experience++;
  wout(c->who, "You are now level %s in experience for %s.",
	 nice_num(p->experience), box_name(sk));
  return TRUE;
}

#if 0
static int
research_notknown(int who, int sk)
{
	static ilist l = NULL;
	int i;
	struct entity_skill *p;
	int new;

	p = rp_skill(sk);

	if (p == NULL)
		return 0;

	ilist_clear(&l);

	for (i = 0; i < ilist_len(p->research); i++)
	{
		new = p->research[i];

		if (rp_skill_ent(who, new) == NULL &&
		    rp_skill_ent(who, req_skill(new)) != NULL)
		{
			ilist_append(&l, new);
		}
	}

	if (ilist_len(l) <= 0)
		return 0;

	i = rnd(0, ilist_len(l)-1);

	return l[i];
}


int
v_research(struct command *c)
{
  int sk = c->a;
  int where = subloc(c->who);
  int parent, category = 0, teachable = 0, unteachable = 0;

  /*
   *  In case they're researching from a scroll or book...
   *
   */
  c->b = 0;
  if (kind(sk) == T_item) sk = correct_study_item(c);

  if (numargs(c) < 1) {
    wout(c->who, "Must specify skill to research.");
    return FALSE;
  }

  if (kind(sk) != T_skill) {
    wout(c->who, "%s is not a valid skill.", c->parse[1]);
    return FALSE;
  }

  if (has_skill(c->who, sk)) {
    wout(c->who, "%s already knows %s",
	 box_name(c->who), box_name(sk));
    return FALSE;
  }

  /*
   *  What kind of skill are they learning?
   *
   */
  parent = skill_school(sk);
  category = (parent == sk);
  if (!category)
    teachable = (ilist_lookup(rp_skill(parent)->offered,sk) != -1);
  if (!category && !teachable) unteachable = 1;

  if (teachable) {
    wout(c->who, "%s does not require research.  "
	 "You may study that skill at any time.", box_name(sk));
    return FALSE;
  };

  /*
   *  If it's a unteachable skill, they have to already know
   *  the parent skill.
   *
   */
  if (unteachable && !has_skill(c->who, parent)) {
    wout(c->who, "Before you can research %s, you must learn %s.",
	 box_name(sk), box_name(parent));
    return FALSE;
  };

  if (c->b) {
    (void) p_skill_ent(c->who, sk);
    wout(c->who,"You research %s immediately from %s.",
	 box_name(sk), box_name(c->b));
    consume_scroll(c->who, c->b, 7);
    return FALSE;  /* Stops the research from continuing. */
  };

  if (being_taught(c->who, sk)) {
    (void) p_skill_ent(c->who, sk);
    wout(c->who,"Your teacher helps you research %s immediately.",
	 box_name(sk));
    return FALSE;  /* Stops the research from continuing. */
  };

  if (!can_pay(c->who, 25)) {
    wout(c->who, "Can't afford 25 gold to research.");
    return FALSE;
  }
  
  if (sk == sk_religion) {
    if (subkind(where) != sub_temple) {
      wout(c->who, "%s may only be researched "
	   "in a temple.", box_name(sk_religion));
      return FALSE;
    }

    if (building_owner(where) != c->who) {
      wout(c->who, "Must be the first character inside "
	   "the temple to research.");
      return FALSE;
    }
  } else {
    if (subkind(where) != sub_tower) {
      wout(c->who, "Research must be performed in a tower.");
      return FALSE;
    }

    if (building_owner(where) != c->who) {
      wout(c->who, "Must be the first character inside "
	   "the tower to research.");
      return FALSE;
    }
  }
  
  wout(c->who, "Research %s.", box_name(sk));
  return TRUE;
}


int
d_research(struct command *c)
{
  int sk = c->a;
  int new_skill;
  int chance = 20;

  if (kind(sk) != T_skill) {
    wout(c->who, "Internal error.");
    fprintf(stderr, "d_research: skill %d is gone, who=%d\n",
	    sk, c->who);
    return FALSE;
  }

  if (!charge(c->who, 25)) {
    wout(c->who, "Can't afford 25 gold to research.");
    return FALSE;
  }

  /*
   *  10% bonus to research in your strength skill, if any.
   *
   */
  if (is_follower(c->who) &&
      rp_relig_skill(is_priest(is_follower(c->who)))->strength == sk) {
    chance += 10;
  };

  /*
   *  -10% bonus to research in your weakness skill, if any.
   *
   */
  if (is_follower(c->who) &&
      rp_relig_skill(is_priest(is_follower(c->who)))->weakness == sk) {
    chance -= 10;
  };

  if (rnd(1,100) > chance) {
    wout(c->who, "Research is unsuccessful.");
    return FALSE;
  };

  /*
   *  Cause the new skill to be partially known
   */

  (void) p_skill_ent(c->who, sk);

    wout(c->who, "Research successful for %s",
	 box_name(sk));
    
    wout(c->who, "To begin learning this skill, order 'study %s'.",
	 box_code_less(sk));
    
    return TRUE;

#endif 

void check_skill_times()
{
  int sk, entry;

  loop_skill(sk) {
    if (sk >= 9000) {
      entry = find_use_entry(sk);
      if (entry && skill_time_to_use(sk) != use_tbl[entry].time) {
	fprintf(stderr, "Mismatched time to use for %s should be %d.\n",
		box_code(sk), use_tbl[entry].time);
      };
      if (entry && (skill_flags(sk) & IS_POLLED) != use_tbl[entry].poll) {
	fprintf(stderr, "Mismatched poll for %s should be %d.\n",
		box_code(sk), use_tbl[entry].poll);
      };
    };
  } next_skill;
  
};

/*
 *  V_TEACH
 *  Fri Nov  1 16:02:14 1996 -- Scott Turner
 *
 *  Teach -- should be top dog in a tower.  Command takes the skill
 *  to teach as an arg.
 *
 */
int
v_teach(struct command *c)
{
  struct skill_ent *p;
  int sk = c->a;
  int days = c->b;
  int where = subloc(c->who), i;
  struct command *c2;
  
  if (numargs(c) < 1) {
    wout(c->who, "Must specify a skill to teach.");
    return FALSE;
  }

  if (kind(sk) != T_skill) {
    wout(c->who, "%s is not a valid skill.", c->parse[1]);
    return FALSE;
  }

  p = rp_skill_ent(c->who, sk);

  if (!p || p->know != SKILL_know) {
    wout(c->who, "You cannot teach a skill you don't know.");
    return FALSE;
  };

  /*
   *  In the top of a tower?
   *
   *  Fri Nov 29 09:56:12 1996 -- Scott Turner
   *
   *  Can teach skills in a guild.
   *
   */
  if (!teachable_place(where, sk)) {
    wout(c->who, "Teaching must be performed in a tower or (appropriate) guild.");
    return FALSE;
  }

  /*
   *  Thu Dec 24 08:10:09 1998 -- Scott Turner
   *
   *  You no longer have to be the tower owner, but if someone else
   *  is already teaching we should mention it to you!
   *
   */
  loop_stack(where, i) {
    if (i == c->who) break;
    c2 = rp_command(i);
    if (c2 && cmd_tbl[c2->cmd].start == v_teach) {
      wout(c->who, "Someone else is already teaching here.");
      wout(c->who, "Your teaching won't be effective until they stop.");
      break;
    };
  } next_stack;
  
  if (days) {
    c->wait = days;
  };

  wout(c->who, "Teaching %s for %d days.", box_name(sk), c->wait);

  /*
   *  Wed Apr 14 19:33:28 1999 -- Scott Turner
   *
   *  Lower the priority of teaching so it finishes *after* any
   *  "study".
   *
   */
  c->pri += 2;
  return TRUE;

};

