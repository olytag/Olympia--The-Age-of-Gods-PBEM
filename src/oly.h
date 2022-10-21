// olytag - Olympia: The Age of Gods
//
// Copyright (c) 2022 by the OlyTag authors.
// Please see the LICENSE file in the root directory of this repository for further information.

#ifndef OLYTAG_OLY_H
#define OLYTAG_OLY_H

#include "oly3.h"

typedef unsigned char uchar;
#ifndef OLY_FORWARD_schar
#define OLY_FORWARD_schar
typedef signed char schar;
#endif

#define        DEFAULT_PASSWORD    "defpwd123"

#define        MAX_BOXES    102400
#define        MONTH_DAYS    30
#define        NUM_MONTHS    8

#define        T_deleted    0    /* forget on save */
#define        T_player    1
#define        T_char        2
#define        T_loc        3
#define        T_item        4
#define        T_skill        5
#define        T_gate        6
#define        T_road        7
#define        T_deadchar    8
#define        T_ship        9
#define        T_post        10
#define        T_storm        11
#define        T_unform    12    /* unformed noble */
#define        T_lore        13
#define        T_nation    14
#define        T_MAX        15    /* one past highest T_xxx define */

#define        sub_ocean        1
#define        sub_forest        2
#define        sub_plain        3
#define        sub_mountain        4
#define        sub_desert        5
#define        sub_swamp        6
#define        sub_under        7    /* underground */
#define        sub_faery_hill        8    /* gateway to Faery */
#define        sub_island        9    /* island subloc */
#define        sub_stone_cir        10    /* ring of stones */
#define        sub_mallorn_grove    11
#define        sub_bog            12
#define        sub_cave        13
#define        sub_city        14
#define        sub_lair        15    /* dragon lair */
#define        sub_graveyard        16
#define        sub_ruins        17
#define        sub_battlefield        18
#define        sub_ench_forest        19    /* enchanted forest */
#define        sub_rocky_hill        20
#define        sub_tree_circle        21
#define        sub_pits        22
#define        sub_pasture        23
#define        sub_oasis        24
#define        sub_yew_grove        25
#define        sub_sand_pit        26
#define        sub_sacred_grove    27
#define        sub_poppy_field        28
#define        sub_temple        29
#define        sub_galley        30
#define        sub_roundship        31
#define        sub_castle        32
#define        sub_galley_notdone    33
#define        sub_roundship_notdone    34
#define        sub_ghost_ship        35
#define        sub_temple_notdone    36
#define        sub_inn            37
#define        sub_inn_notdone        38
#define        sub_castle_notdone    39
#define        sub_mine        40
#define        sub_mine_notdone    41
#define        sub_scroll        42    /* item is a scroll */
#define        sub_magic        43    /* this skill is magical */
#define        sub_palantir        44
#define        sub_auraculum        45
#define        sub_tower        46
#define        sub_tower_notdone    47
#define        sub_pl_system        48    /* system player */
#define        sub_pl_regular        49    /* regular player */
#define        sub_region        50    /* region wrapper loc */
#define        sub_pl_savage        51    /* Savage King */
#define        sub_pl_npc        52
#define        sub_mine_collapsed    53
#define        sub_ni            54    /* ni=noble_item */
#define        sub_demon_lord        55    /* undead lord */
#define        sub_dead_body        56    /* dead noble's body */
#define        sub_fog            57
#define        sub_wind        58
#define        sub_rain        59
#define        sub_hades_pit        60
#define        sub_artifact        61
#define        sub_pl_silent        62
#define        sub_npc_token        63    /* npc group control art */
#define        sub_garrison        64    /* npc group control art */
#define        sub_cloud        65    /* cloud terrain type */
#define        sub_raft        66    /* raft made out of flotsam */
#define        sub_raft_notdone    67
#define        sub_suffuse_ring    68
#define         sub_religion            69
#define        sub_holy_symbol        70      /* Holy symbol of some sort */
#define    sub_mist        71
#define        sub_book        72      /* Book */
#define        sub_guild        73      /* Requires skill to enter */
#define        sub_trade_good        74
#define        sub_city_notdone    75
#define        sub_ship        76
#define        sub_ship_notdone    77
#define        sub_mine_shaft        78
#define        sub_mine_shaft_notdone    79
#define        sub_orc_stronghold    80
#define        sub_orc_stronghold_notdone    81
#define    sub_special_staff    82
#define    sub_lost_soul        83
#define        sub_undead        84
#define    sub_pen_crown        85
#define         sub_animal_part         86
#define         sub_magic_artifact      87

#define        SUB_MAX            88    /* one past highest sub_ */

#define        item_gold        1

#define        item_peasant        10
#define        item_worker        11
#define        item_soldier        12
#define        item_archer        13
#define        item_knight        14
#define        item_elite_guard    15
#define        item_pikeman        16
#define        item_blessed_soldier    17
#define        item_ghost_warrior    18
#define        item_sailor        19
#define        item_swordsman        20
#define        item_crossbowman    21
#define        item_elite_arch        22
#define        item_angry_peasant    23
#define        item_pirate        24
#define        item_elf        25
#define        item_spirit        26
#define        item_postulant        27
#define        item_fanatic        28
#define        item_ninja        29
#define        item_angel        30

#define        item_corpse        31
#define        item_savage        32
#define        item_skeleton        33
#define        item_barbarian        34
#define        item_wagon        35
#define        item_skirmisher        36
#define        item_hvy_foot        37
#define        item_hvy_xbowman    38
#define        item_elvish_arrow    39
#define        item_hvy_xbow        40
#define        item_horse_archer    41
#define         item_pctg_token         42
#define    item_cavalier        43
#define        item_new_wagon        44
#define        item_hvy_wagon        45
#define        item_war_wagon        46

#define        item_wild_horse        51
#define        item_riding_horse    52
#define        item_warmount        53
#define        item_pegasus        54
#define        item_nazgul        55

#define        item_flotsam        59
#define        item_battering_ram    60
#define        item_catapult        61
#define        item_siege_tower    62
#define        item_ratspider_venom    63
#define        item_lana_bark        64
#define        item_avinia_leaf    65
#define        item_spiny_root        66
#define        item_farrenstone    67
#define        item_yew        68
#define        item_elfstone        69
#define        item_mallorn_wood    70
#define        item_pretus_bones    71
#define        item_longbow        72
#define        item_plate        73
#define        item_longsword        74
#define        item_pike        75
#define        item_ox            76
#define        item_lumber        77
#define        item_stone        78
#define        item_iron        79
#define        item_leather        80
#define        item_ratspider        81
#define        item_mithril        82
#define        item_gate_crystal    83
#define        item_blank_scroll    84
#define        item_crossbow        85
#define        item_rug        86
#define        item_fish        87
#define        item_pepper        88
#define        item_pipeweed        89
#define        item_ale        90
#define        item_wool        91
#define        item_jewel        92
#define        item_opium        93
#define        item_basket        94    /* woven basket */
#define        item_pot        95    /* clay pot */
#define        item_tax_cookie        96
#define        item_fish_oil        97
#define        item_drum        98
#define        item_hide        99
#define        item_mob_cookie        101
#define        item_lead        102
#define        item_fine_cloak        103
#define        item_chocolate        104
#define        item_ivory        105
#define        item_cardamom        106
#define        item_honey        107
#define        item_ink        108
#define        item_licorice        109
#define        item_soap        110
#define        item_old_book        111
#define        item_jade_idol        112
#define        item_purple_cloth    113
#define        item_rose_perfume    114
#define        item_silk        115
#define        item_incense        116
#define        item_ochre        117
#define        item_jeweled_egg    118
#define        item_obsidian        119
#define        item_orange        251
#define        item_cinnabar        252
#define        item_myrhh        253
#define        item_saffron        254
#define        item_dried_fish        255
#define        item_tallow        256
#define        item_candles        257
#define        item_wax        258
#define        item_sugar        259
#define        item_salt        260
#define        item_glue        261
#define        item_linen        262
#define        item_beans        263
#define        item_walnuts        264
#define        item_flax        265
#define        item_flutes        266
#define        item_cassava        267
#define        item_plum_wine        268
#define        item_vinegar        269
#define        item_tea        270
#define        item_centaur        271
#define        item_minotaur        272
#define        item_undead_cookie    273
#define        item_fog_cookie        274
#define        item_wind_cookie    275
#define        item_rain_cookie    276
#define        item_mage_menial    277    /* mage menial labor cookie */
#define        item_spider        278    /* giant spider */
#define        item_rat        279    /* horde of rats */
#define        item_lion        280
#define        item_bird        281    /* giant bird */
#define        item_lizard        282
#define        item_bandit        283
#define        item_chimera        284
#define        item_harpie        285
#define        item_dragon        286
#define        item_orc        287
#define        item_gorgon        288
#define        item_wolf        289
#define        item_orb        290
#define        item_cyclops        291
#define        item_giant        292
#define        item_faery        293
#define        item_petty_thief    294
#define        item_seagrass        295
#define        item_firewort        296
#define        item_beastnip        297
#define        item_elf_poppy        298
#define        item_ironwood        299
#define        item_kings_fern        300
#define        item_moon_palms        301
#define        item_otter        302
#define        item_mole        303
#define        item_bull        304
#define        item_eagle        305
#define        item_monkey        306
#define        item_hare        307
#define        item_wardog        308
#define        item_sand_rat        309
#define        item_balrog        310
#define        item_dirt_golem        311
#define        item_flesh_golem    312
#define        item_iron_golem        313
#define        item_lesser_demon    314
#define        item_greater_demon    315
#define        item_green_rose    316
#define        item_elf_ear        317
#define        item_savage_ear        318
#define        item_nazgul_tail    319
#define        item_centaur_hide    320
#define        item_minotaur_hide    321
#define        item_spider_eye        322
#define        item_rat_tail        323
#define        item_lion_mane        324
#define        item_bird_feather    325
#define        item_lizard_tail    326
#define        item_bandit_ear        327
#define        item_chimera_eye    328
#define        item_harpie_feather    329
#define        item_dragon_scale    330
#define        item_orc_scalp        331
#define        item_gorgon_liver    332
#define        item_wolf_hide        333
#define        item_cyclops_eye    334
#define        item_giant_tongue    335
#define        item_balrog_horn    336

#define        sk_shipcraft        1000
#define        sk_combat        1100
#define        sk_stealth        1200
#define        sk_beast        123
#define        sk_persuasion        1300
#define        sk_construction        1400
#define        sk_alchemy        1500
#define        sk_forestry        1600
#define        sk_mining        1700
#define        sk_trading        1800
#define    sk_ranger        1900
#define        sk_religion        150
#define        sk_eres            2000
#define        sk_anteus        2100
#define        sk_dol            2200
#define        sk_timeid        2300
#define        sk_ham            2400
#define        sk_kireus        2500
#define        sk_halon        2600
#define        sk_domingo        2700
#define        sk_basic        2800
#define        sk_weather        161
#define        sk_scry            2900
#define        sk_gate            3000
#define        sk_artifact        3100
#define        sk_necromancy        3200
#define        sk_adv_sorcery        3300
#define        sk_heroism           3400
#define        sk_basic_religion      3500

#define        lore_skeleton_npc_token        9001
#define        lore_orc_npc_token        9002
#define        lore_undead_npc_token        9003
#define        lore_savage_npc_token        9004
#define        lore_barbarian_npc_token    9005
#define        lore_orb            9006
#define        lore_faery_stone        9007
#define        lore_barbarian_kill        9008
#define        lore_savage_kill        9009
#define        lore_undead_kill        9010
#define        lore_orc_kill            9011
#define        lore_skeleton_kill        9012
#define        lore_pen_crown            9013

#define        sk_meditate        2801
#define        sk_forge_aura        3130    /* forge auraculum */
#define        sk_mage_menial        2802    /* menial labor for mages */
#define        sk_appear_common    2803
#define        sk_view_aura        2804
#define        sk_quick_cast        2830    /* speed next cast */
#define    sk_fortify_castle    1491
#define        sk_detect_artifacts    3101
#define        sk_reveal_artifacts    3102
#define        sk_mutate_artifact    3131
#define        sk_conceal_artifacts    3132
#define        sk_teleport        3030
#define        sk_obscure_artifact    3133
#define    sk_strengthen_castle    1492
#define        sk_detect_gates        3001
#define        sk_jump_gate        3002
#define        sk_seal_gate        3031
#define        sk_unseal_gate        3032
#define        sk_notify_unseal    3033
#define        sk_rem_seal        3034    /* forcefully unseal gate */
#define        sk_reveal_key        3035
#define        sk_notify_jump        3036
#define        sk_reveal_mage        2831    /* reveal abilities of mage */
#define        sk_fierce_wind        2030
#define        sk_transcend_death    3238
#define        sk_tap_health        2832
#define    sk_moat_castle        1493
#define    sk_widen_entrance    1494
#define    sk_deepen_mine        1703
#define    sk_wooden_shoring    1790
#define    sk_iron_shoring        1791
#define        sk_forge_weapon        3134
#define        sk_forge_armor        3135
#define        sk_forge_bow        3136
#define        sk_bind_storm        9134
#define    sk_lightning_bolt    2840
#define    sk_foresee_defense    2940
#define    sk_drain_mana        3039
#define    sk_raise_soldiers    3236
#define    sk_fireball        3332
#define    sk_conceal_nation    1291
#define        sk_scry_region        2901
#define        sk_shroud_region    2930
#define        sk_dispel_region    2931    /* dispel region shroud */
#define        sk_remove_obscurity    3137
#define        sk_spot_hidden        1901
#define        sk_protect_noble    1990
#define        sk_write_basic        2833
#define        sk_assassinate        1292
#define        sk_find_food        1903
#define        sk_write_scry        2932
#define        sk_write_gate        3037
#define        sk_write_art        3138
#define        sk_write_necro        3230
#define        sk_prot_blast_1        2042
#define        sk_prot_blast_2        2139
#define        sk_prot_blast_3        2237
#define        sk_prot_blast_4        2338
#define        sk_prot_blast_5        2437
#define        sk_bar_loc        2933    /* create location barrier */
#define        sk_unbar_loc        2934
#define        sk_prot_blast_6        2536
#define        sk_prot_blast_7        2637
#define        sk_destroy_art        3103
#define        sk_rev_jump        3038
#define        sk_prot_blast_8        2735
#define        sk_locate_char        2935
#define        sk_deep_identify    3104
#define        sk_shroud_abil        2834    /* ability shroud */
#define        sk_detect_abil        2835    /* detect ability scry */
#define        sk_detect_scry        2936    /* detect region scry */
#define        sk_proj_cast        2937    /* project next cast */
#define        sk_dispel_abil        2836    /* dispel ability shroud */
#define        sk_adv_med        2837    /* advanced meditation */
#define        sk_hinder_med        2838    /* hinder meditation */
#define        sk_forge_palantir    3139
#define        sk_save_proj        2938    /* save projected cast */
#define        sk_save_quick        2839    /* save speeded cast */
#define        sk_summon_ghost        3201    /* summon ghost warriors */
#define        sk_raise_corpses    3202    /* summon undead corpses */
#define        sk_undead_lord        3231    /* summon undead unit */
#define        sk_renew_undead        3232
#define        sk_banish_undead    3233
#define        sk_eat_dead        3234
#define        sk_aura_blast        3203
#define        sk_absorb_blast        3235
#define        sk_summon_rain        2036
#define        sk_summon_wind        2037
#define        sk_summon_fog        2038
#define        sk_direct_storm        2039
#define        sk_dissipate        2031
#define        sk_renew_storm        2032
#define        sk_lightning        2033
#define        sk_seize_storm        2034
#define        sk_death_fog        2035
#define        sk_banish_corpses    2939
#define        sk_trance        3330
#define        sk_teleport_item    3331

/* 2000 Eres */
#define        sk_resurrect        2001
#define        sk_pray            2002
#define        sk_last_rites        2003
#define        sk_gather_holy_plant        2004
#define        sk_bless_follower    2005
#define        sk_proselytise    2006
#define        sk_create_holy_symbol    2007
#define        sk_heal            2008
#define         sk_summon_water_elemental 2040
#define        sk_write_weather    2041
#define        sk_dedicate_temple    2009

/* 2100 Anteus */
#define        sk_find_mtn_trail    2130
#define        sk_obscure_mtn_trail    2131
#define        sk_improve_mining    2132
#define        sk_conceal_mine        2133
#define        sk_protect_mine        2134
#define        sk_bless_fort        2135
#define        sk_weaken_fort        2136
#define        sk_boulder_trap        2137
#define        sk_write_anteus        2138

/* 2200 */
#define        sk_detect_beasts        2234
#define        sk_snake_trap        2235
#define        sk_write_dol        2236

/* 2300 */
#define        sk_find_forest_trail    2330
#define        sk_obscure_forest_trail 2331
#define        sk_improve_forestry    2332
#define        sk_reveal_forest    2333
#define        sk_improve_fort        2334
#define        sk_create_deadfall    2335
#define        sk_recruit_elves    2336
#define        sk_write_timeid        2337

/* 2400 */
#define        sk_reveal_vision    2434
#define        sk_enchant_guard    2430
#define        sk_urchin_spy        2431
#define        sk_draw_crowds        2432
#define        sk_arrange_mugging    2433
#define        sk_write_ham        2435
#define        sk_pr_shroud_loc    2436

/* 2500 */
#define    sk_improve_quarry    2530
#define    sk_improve_smithing    2531
#define    sk_edge_of_kireus    2532
#define    sk_create_mithril    2533
#define    sk_quicksand_trap    2534
#define        sk_write_kireus        2535

/* 2600 */
#define        sk_calm_ap        2630
#define        sk_improve_charisma    2631
#define        sk_mesmerize_crowd    2632
#define        sk_improve_taxes    2633
#define        sk_guard_loyalty    2634
#define        sk_instill_fanaticism    2635
#define        sk_write_halon        2636

/* 2700 */
#define        sk_find_hidden        2730
#define        sk_conceal_loc        2731
#define        sk_mists_of_conceal    2732
#define        sk_create_ninja        2733
#define        sk_write_domingo    2734

#define        sk_survive_fatal    3483
#define        sk_pilot_ship        1001
#define        sk_shipbuilding        1002
#define        sk_bird_spy        2231
#define        sk_fight_to_death    1102
#define        sk_capture_beasts    2232
#define        sk_use_beasts        1195    /* use beasts in battle */
#define        sk_breed_beasts        2233
#define        sk_petty_thief        1201
#define        sk_deep_sea        1094
#define        sk_bribe_noble        1301
#define        sk_catch_horse        1930
#define        sk_spy_inv        1202    /* determine char inventory */
#define        sk_spy_skills        1203    /* determine char skill */
#define        sk_spy_lord        1204    /* determine char's lord */
#define        sk_find_rich        1230
#define        sk_torture        1231
#define        sk_train_wild        1931
#define        sk_train_warmount    1932
#define        sk_persuade_oath    1330
#define        sk_raise_mob        1302
#define        sk_rally_mob        1303
#define        sk_incite_mob        1331
#define        sk_make_ram        1601    /* make battering ram */
#define        sk_make_catapult    1194
#define        sk_make_siege        1401
#define        sk_extract_venom    1530    /* from ratspider */
#define        sk_brew_slave        1531    /* potion of slavery */
#define        sk_brew_heal        1501
#define        sk_brew_death        1502
#define        sk_brew_weightlessness    1590    /* potion of weightlessness */
#define        sk_add_ram        1095    /* add ram to galley */
#define        sk_cloak_trade        1232
#define        sk_mine_iron        1701
#define        sk_mine_gold        1702
#define        sk_mine_mithril        1730
#define        sk_quarry_stone        1402
#define        sk_mine_crystal        1731
#define        sk_harvest_lumber    1602
#define        sk_harvest_yew        1603
#define        sk_defense        1104
#define        sk_record_skill        1532
#define        sk_sneak_build        1233
#define        sk_archery        1902
#define        sk_swordplay        1105
#define        sk_weaponsmith        1106
#define        sk_fishing        1004
#define        sk_collect_foliage    1604
#define        sk_collect_elem        1503
#define        sk_summon_savage    1332
#define        sk_keep_savage        1333
#define        sk_harvest_mallorn    1630
#define        sk_harvest_opium    1605
#define        sk_improve_opium    1631
#define        sk_lead_to_gold        1533
#define        sk_hide_lord        1205
#define        sk_train_angry        1304
#define        sk_hide_self        1290
#define         sk_control_battle       1107
#define         sk_attack_tactics       1131
#define         sk_defense_tactics      1132
#define        sk_combat_discipline    1108
#define        sk_train_armor        1192

#define    sk_smuggle_goods    1830
#define    sk_smuggle_men        1831
#define    sk_avoid_taxes        1832
#define    sk_build_wagons        1801
#define    sk_increase_demand    1802
#define    sk_decrease_demand    1803
#define    sk_increase_supply    1804
#define    sk_decrease_supply    1805
#define    sk_hide_money        1890
#define    sk_hide_item        1891
#define        sk_grow_pop        1390
#define        sk_build_city        1490

#define        sk_train_knight        1190
#define        sk_train_paladin    1191

#define        sk_add_sails        1005
#define        sk_add_forts        1091
#define        sk_add_ports        1006
#define        sk_add_keels        1090

#define        sk_remove_sails        1096
#define        sk_remove_forts        1093
#define        sk_remove_ports        1097
#define        sk_remove_keels        1092
#define        sk_remove_ram        1098
#define        sk_brew_fiery        1591

#define        sk_dirt_golem        2841
#define        sk_flesh_golem        3237
#define        sk_iron_golem        3333

/* 3400 Heroism */
#define    sk_swordplay2        3401
#define    sk_defense2        3402
#define        sk_survive_fatal2    3483
#define        sk_avoid_wounds        3480
#define        sk_avoid_illness    3481
#define        sk_improved_recovery    3482
#define        sk_personal_fttd    3403
#define        sk_forced_march        3484
#define        sk_extra_attacks    3485
#define        sk_extra_missile_attacks 3486
#define        sk_acute_senses        3487
#define        sk_improved_explore    3488
#define        sk_uncanny_accuracy    3489
#define        sk_blinding_speed    3490

/* 3500 Basic Religion */
#define    sk_heal_b        3501
#define    sk_last_rites_b        3502
#define    sk_resurrect_b        3530
#define    sk_create_holy_b    3503
#define    sk_dedicate_temple_b    3504
#define    sk_pray_b        3505
#define    sk_bless_b        3506
#define    sk_gather_holy_plant_b    3507
#define    sk_write_religion_b    3508
#define    sk_proselytise_b    3509
#define    sk_banish_undead_b    3510
#define    sk_prot_blast_b        3531
#define    sk_hinder_med_b        3532
#define    sk_scry_b        3533

#define        PROG_bandit        1    /* wilderness spice */
#define        PROG_subloc_monster    2
#define        PROG_npc_token        3
#define        PROG_balrog        4
#define        PROG_dumb_monster    5
#define        PROG_smart_monster    6
#define        PROG_orc        7
#define        PROG_elf        8
#define        PROG_daemon        9

#define        use_death_potion    1
#define        use_heal_potion        2
#define        use_slave_potion    3
#define        use_palantir        4
#define        use_proj_cast        5    /* stored projected cast */
#define        use_quick_cast        6    /* stored cast speedup */
#define        use_drum        7    /* beat savage's drum */
#define        use_faery_stone        8    /* Faery gate opener */
#define        use_orb            9    /* crystal orb */
#define        use_barbarian_kill    10
#define        use_savage_kill        11
#define        use_corpse_kill        12
#define        use_orc_kill        13
#define        use_skeleton_kill    14
#define        use_ancient_aura    15    /* bta's auraculum */
#define        use_weightlessness_potion    16
#define        use_fiery_potion    17      /* Fiery Death */
#define        use_nothing        18      /* Something that does nothing */
#define    use_special_staff    19

#define        DIR_N        1
#define        DIR_E        2
#define        DIR_S        3
#define        DIR_W        4
#define        DIR_UP        5
#define        DIR_DOWN    6
#define        DIR_IN        7
#define        DIR_OUT        8
#define        MAX_DIR        9    /* one past highest direction */

#define        LOC_region    1    /* top most continent/island group */
#define        LOC_province    2    /* main location area */
#define        LOC_subloc    3    /* inner sublocation */
#define        LOC_build    4    /* building, structure, etc. */

#define        LOY_UNCHANGED    (-1)
#define        LOY_unsworn    0
#define        LOY_contract    1
#define        LOY_oath    2
#define        LOY_fear    3
#define        LOY_npc        4
#define        LOY_summon    5

#define        exp_novice    1    /* apprentice */
#define        exp_journeyman    2
#define        exp_teacher    3
#define        exp_master    4
#define        exp_grand    5    /* grand master */

#define    S_body        1       /* kill to a dead body */
#define        S_soul        2       /* kill to a lost soul */
#define        S_nothing    3       /* kill completely */

#ifndef OLY_FORWARD_sparse
#define OLY_FORWARD_sparse
typedef ilist sparse;
#endif

typedef struct {
    short day;            /* day of month */
    short turn;            /* turn number */
    int days_since_epoch;        /* days since game begin */
} olytime;

#define    oly_month(a)    (((a).turn-1) % NUM_MONTHS)
#define oly_year(a)    (((a).turn-1) / NUM_MONTHS)


struct loc_info {
    int where;
    ilist here_list;
};

struct box {
    schar kind;
    schar skind;
    char *name;

    struct loc_info x_loc_info;
    struct entity_player *x_player;
    struct entity_char *x_char;
    struct entity_loc *x_loc;
    struct entity_subloc *x_subloc;
    struct entity_item *x_item;
    struct entity_skill *x_skill;
    struct entity_nation *x_nation;
    struct entity_gate *x_gate;
    struct entity_misc *x_misc;
    struct att_ent *x_disp;

    struct command *cmd;
    struct item_ent **items;    /* ilist of items held */
    struct trade **trades;    /* pending buys/sells */
    struct effect **effects;      /* ilist of effects */

    int temp;            /* scratch space */
    int output_order;        /* for report ordering -- not saved */

    int x_next_kind;        /* link to next entity of same type */
    int x_next_sub;            /* link to next of same subkind */
};

struct entity_player {
    char *full_name;
    char *email;
    char *vis_email;        /* address to put in player list */
    char *password;
    int first_turn;            /* which turn was their first? */
    int last_order_turn;        /* last turn orders were submitted */
    struct order_list **orders;    /* ilist of orders for units in */
    /* this faction */
    sparse known;            /* visited, lore seen, encountered */

    ilist units;            /* what units are in our faction? */
    struct admit **admits;        /* admit permissions list */
    ilist unformed;            /* nobles as yet unformed */

    int split_lines;        /* split mail at this many lines */
    int split_bytes;        /* split mail at this many bytes */

    short nation;                   /* Player's nation */
    short magic;                /* MUs or Priests? */
    short noble_points;        /* how many NP's the player has */
    short jump_start;               /* Jump start points */

    schar format;            /* turn report formatting control */
    char *rules_path;             /* external path for HTML */
    char *db_path;                /* external path for HTML */
    schar notab;            /* player can't tolerate tabs */
    schar first_tower;        /* has player built first tower yet? */
    schar sent_orders;        /* sent in orders this turn? */
    schar dont_remind;        /* don't send a reminder */
    schar compuserve;        /* get Times from CIS */
    schar nationlist;             /* Receive the nation mailing list? */
    schar broken_mailer;        /* quote begin lines */

/* not saved: */

    schar times_paid;             /* Times paid this month? */
    schar swear_this_turn;        /* have we used SWEAR this turn? */
    short cmd_count;        /* count of cmds started this turn */
    short np_gained;        /* np's added this turn -- not saved */
    short np_spent;            /* np's lost this turn -- not saved */
    ilist deliver_lore;        /* show these to player -- not saved */
    sparse weather_seen;        /* locs we've viewed the weather */
    sparse output;            /* units with output -- not saved */
    sparse locs;            /* locs we touched -- not saved */
};

struct order_list {
    int unit;            /* unit orders are for */
    char **l;            /* ilist of orders for unit */
};

struct accept_ent {
    int item;            /* 0 = any item */
    int from_who;            /* 0 = anyone, else char or player */
    int qty;            /* 0 = any qty */
};

#define        ATT_NONE    0    /* no attitude -- default */
#define        NEUTRAL        1    /* explicitly neutral */
#define        HOSTILE        2    /* attack on sight */
#define        DEFEND        3    /* defend if attacked */
#define    MONSTER_ATT    -1       /* phony id for "all monsters" */

struct att_ent {
    ilist neutral;
    ilist hostile;
    ilist defend;
};

/*
 *  Traps
 *  Fri Oct 18 12:40:26 1996 -- Scott Turner
 *
 *  Encodes some common traps...
 *
 */
struct trap_struct {
    int type;               /* Type of trap. */
    int religion;           /* Religion that ignores this trap. */
    int num_attacks;        /* Number of attacks */
    int attack_chance;      /* Chance of attack killing someone */
    char *name;             /* Name of trap. */
    char *ignored;          /* Message if you can ignore this trap. */
    char *flying;           /* Message if you fly over the trap. */
    char *attack;           /* Message if it attacks you. */
};

extern struct trap_struct traps[];


/*
 *  Effect structure.  See effect.c
 *  Mon Aug  5 12:44:30 1996 -- Scott Turner
 *
 */
struct effect {
    int type;         /* Type of effect, usually == to a sk_ number */
    int subtype;      /* A subtype, surprise! */
    int days;         /* Remaining days of the effect. */
    int data;         /* Generic data for effect. */
};

/*
 *  Effect identifiers.
 *
 */
#define ef_defense        1001    /* Add to defense of stack. */
#define ef_religion_trap    1002    /* A religion trap. */
#define ef_fast_move        1004    /* Fast move into the next province */
#define ef_slow_move        1005    /* Slow moves into this province */
#define ef_improve_mine        1006    /* Improve a mine's production */
#define ef_protect_mine        1007    /* Protect a mine against calamities */
#define ef_bless_fort        1008    /* Bless a fort */
#define ef_improve_production    1010    /* Generic production improvement +50%*/
#define ef_improve_make        1011    /* Generic make improvement +100% */
#define ef_improve_fort        1012    /* Increase a fortification */
#define ef_edge_of_kireus    1013    /* Edged weapons +25 attack */
#define ef_urchin_spy        1014    /* Report unit's doings for 7 days */
#define ef_charisma        1015    /* Double leadership */
#define ef_improve_taxes    1016    /* Double taxes */
#define ef_guard_loyalty    1017    /* Unit immune to loyalty checks */
#define ef_hide_item        1018    /* Hide an item. */
#define ef_hide_money        1019    /* Hide money. */
#define ef_smuggle_goods    1020    /* Smuggling. */
#define ef_smuggle_men        1021    /* Ditto. */
#define ef_grow            1022    /* Encourage pop to grow. */
#define ef_weightlessness    1023    /* Negative weight */
#define ef_scry_offense        1024    /* + to offense in battle */
#define ef_conceal_nation    1025    /* conceal your nation */
#define ef_kill_dirt_golem      1026    /* Timer on golems */
#define ef_kill_flesh_golem     1027    /* Timer on golems */
#define ef_food_found        1028    /* Food found by a ranger */
#define ef_conceal_artifacts    1029    /* Conceal someone's artifacts */
#define ef_obscure_artifact    1030    /* Obscure artifact's identity */
#define ef_forced_march        1031    /* Next move at riding speed */
#define ef_faery_warning    1032    /* Give people a month grace. */
#define ef_tap_wound        1033    /* The wound from Tap Health. */
#define ef_magic_barrier    1034    /* New implementation of magic barrier */
#define ef_inhibit_barrier    1035    /* After magic barrier falls. */
#define ef_cs            1036    /* Combat spell control. */

/*
 *  Artifacts
 *  Thu Oct  1 18:32:31 1998 -- Scott Turner
 *
 *  Generalized magic items -- not unlike effects.
 *
 */
#define ART_NONE    0    /* No effect */
#define ART_COMBAT      1    /* + to combat */
#define ART_CTL_MEN    2    /* + to controlled men */
#define ART_CTL_BEASTS    3    /* + to controlled beasts */
#define ART_SAFETY    4    /* safety from one monster type */
#define ART_IMPRV_ATT    5    /* improve attack of particular unit */
#define ART_IMPRV_DEF    6    /* improve defense of particular unit */
#define ART_SAFE_SEA    7    /* safety at sea */
#define ART_TERRAIN    8    /* improved defense in particular terrain */
#define ART_FAST_TERR    9    /* fast movement into particular terrain */
#define ART_SPEED_USE    10    /* speed use of skill */
#define ART_PROT_HADES    11    /* protection in Hades */
#define ART_PROT_FAERY    12    /* protection in Faery */
#define ART_WORKERS    13    /* improve productivity of workers */
#define ART_INCOME    14    /* improved income from X */
#define ART_LEARNING    15    /* improved learning */
#define ART_TEACHING    16    /* improved teaching */
#define ART_TRAINING    17    /* improved training (of X?) */
#define ART_DESTROY    18    /* destroy monster X (uses) */
#define ART_SKILL    19    /* grants a skill */
#define ART_FLYING    20    /* permit user to fly */
#define ART_PROT_SKILL    21    /* protection from a skill (aura blast,scry) */
#define ART_SHIELD_PROV    22    /* protect entire province from scrying */
#define ART_RIDING    23    /* permit user riding pace */
#define ART_POWER    24    /* increase piety/aura (1 use) */
#define ART_SUMMON_AID    25    /* summon help (1 use) */
#define ART_MAINTENANCE    26    /* reduce maintenance costs */
#define ART_BARGAIN    27    /* better market prices */
#define ART_WEIGHTLESS    28    /* weightlessness */
#define ART_HEALING    29    /* faster healing */
#define ART_SICKNESS    30    /* protection from sickness */
#define ART_RESTORE    31    /* restore life */
#define ART_TELEPORT    32    /* teleportation (uses) */
#define ART_ORB        33    /* orb */
#define ART_CROWN    34    /* crown */
#define ART_AURACULUM    35    /* auraculum */
#define ART_CARRY    36    /* increase land carry capacity */
#define ART_PEN        37    /* the Pen Crown */

#define ART_LAST    38

/*
 *  Masks to pull out the appropriate bits of combat artifacts.
 *
 */
#define CA_N_MELEE     (1<<0)
#define CA_N_MISSILE   (1<<1)
#define CA_N_SPECIAL   (1<<2)
#define CA_M_MELEE     (1<<3)
#define CA_M_MISSILE   (1<<4)
#define CA_M_SPECIAL   (1<<5)
#define CA_N_MELEE_D   (1<<6)
#define CA_N_MISSILE_D (1<<7)
#define CA_N_SPECIAL_D (1<<8)
#define CA_M_MELEE_D   (1<<9)
#define CA_M_MISSILE_D (1<<10)
#define CA_M_SPECIAL_D (1<<11)

/*
 *  Artifact structure
 *  Fri Oct  2 18:35:40 1998 -- Scott Turner
 *
 */
struct entity_artifact {
    int type;
    int param1, param2;
    int uses;
};

/*
 *  Religion structure
 *  Thu Aug  8 12:23:52 1996 -- Scott Turner
 *
 *  Captures a nobles religious standing, such as it is.
 *
 */
struct char_religion {
    int priest;        /* Who this noble is dedicated to, if anyone. */
    ilist followers;   /* Who is dedicated to us, if anyone. */
    int piety;         /* Our current piety. */
};

struct entity_char {
    int unit_item;            /* unit is made of this kind of item */

    schar health;
    schar sick;            /* 1=character is getting worse */

    schar guard;            /* character is guarding the loc */
    schar loy_kind;            /* LOY_xxx */
    int loy_rate;            /* level with kind of loyalty */

    olytime death_time;        /* when was character killed */

    struct skill_ent **skills;    /* ilist of skills known by char */

#if 0
    struct effect **effects;        /* ilist of effects on char */
#endif

    int moving;            /* daystamp of beginning of movement */
    int unit_lord;            /* who is our owner? */

    ilist contact;            /* who have we contacted, also, who */
    /* has found us */

    struct char_magic *x_char_magic;

    schar prisoner;            /* is this character a prisoner? */
    schar behind;            /* are we behind in combat? */
    schar time_flying;        /* time airborne over ocean */
    schar break_point;        /* break point when fighting */
    schar personal_break_point;    /* personal break point when fighting */
    schar rank;            /* noble peerage status */
    schar npc_prog;            /* npc program */

    short guild;                    /* This is the guild we belong to. */

    short attack;            /* fighter attack rating */
    short defense;            /* fighter defense rating */
    short missile;            /* capable of missile attacks? */

    struct char_religion religion;        /* Our religion info... */

    int pay;                /* How much will you pay to enter? */

/*
 *  The following are not saved by io.c:
 */

    schar melt_me;            /* in process of melting away */
    schar fresh_hire;        /* don't erode loyalty */
    schar new_lord;            /* got a new lord this turn */
    short studied;            /* num days we studied */
    struct accept_ent **accept;    /* what we can be given */
};

struct char_magic {
    int max_aura;            /* maximum aura level for magician */
    int cur_aura;            /* current aura level for magician */
    int auraculum;            /* char created an auraculum */

    sparse visions;            /* visions revealed */
#if 0
    int pledge;			/* lands are pledged to another */
#endif
    int token;            /* we are controlled by this art */

    int project_cast;        /* project next cast */
    short quick_cast;        /* speed next cast */
    short ability_shroud;

    schar hide_mage;        /* hide magician status */
    schar hinder_meditation;
    schar magician;            /* is a magician */
    schar aura_reflect;        /* reflect aura blast */
    schar hide_self;        /* character is hidden */
    schar swear_on_release;        /* swear to one who frees us */
    schar knows_weather;        /* knows weather magic */

    schar mage_worked;        /* worked this month -- not saved */
    schar ferry_flag;        /* ferry has tooted its horn -- ns */
    ilist pledged_to_us;        /* temp -- not saved */
};


#define        TOUGH_NUM    2520

#define        SKILL_dont    0    /* don't know the skill */
#define        SKILL_learning    1    /* in the process of learning it */
#define        SKILL_know    2    /* know it */

struct skill_ent {
    int skill;
    int days_studied;        /* days studied * TOUGH_NUM */
    int experience;        /* experience level with skill */
    char know;            /* SKILL_xxx */

/*
 *  Not saved:
 */

    char exp_this_month;        /* flag for add_skill_experience() */
};

/*
 *  Thu Mar 20 12:05:50 1997 -- Scott Turner
 *
 *  Location control: whether or not we're open, fees.
 *
 */
#ifndef OLY_FORWARD_loc_control_ent
#define OLY_FORWARD_loc_control_ent
struct loc_control_ent {
    int closed;
    int nobles, men, weight;
};
#endif

struct item_ent {
    int item;
    int qty;
};

struct entity_loc {
    ilist prov_dest;        /* cached exits for flood fills */
    int near_grave;            /* nearest graveyard */
    short shroud;            /* magical scry shroud */
    short barrier;            /* magical barrier */
    short tax_rate;            /* Tax rate for this loc. */
    int recruited;            /* How many recruited this month */
    schar hidden;            /* is location hidden? */
    schar dist_from_sea;        /* provinces to sea province */
    schar dist_from_swamp;
    schar dist_from_gate;
    schar sea_lane;            /* fast ocean travel here */
    /* also "tracks" for npc ferries */
#if 0
    struct effect **effects;        /* ilist of effects on location */
#endif
    struct entity_mine *mine_info;  /* If there's a mine. */
    /*
     *  Loction control -- need two so that we can only change
     *  fees at the end of the month.
     *
     *  Control2 doesn't need to be saved.
     *
     */
    struct loc_control_ent control, control2;
};

/*
 *  Fri Jan 10 15:50:45 1997 -- Scott Turner
 *
 *  Build descriptor.  Describes what kind of build is going on in a location.
 *
 */
#define BT_BUILD 1
#define BT_FORTIFY 2
#define BT_STRENGTHEN 3
#define BT_MOAT 4

struct entity_build {
    uchar type;            /* What work is going on? */
    schar build_materials;    /* fifths of materials we've used */
    int effort_required;        /* not finished if nonzero */
    int effort_given;
};

/*
 *  Thu Jan  2 12:48:39 1997 -- Scott Turner
 *
 *  Ship descriptor.
 *
 */
struct entity_ship {
    int hulls;  /* Various ship parts */
    int forts;
    int sails;
    int ports;
    int keels;
    schar galley_ram;        /* galley is fitted with a ram */
};

/*
 *  Thu Jan 23 20:35:24 1997 -- Scott Turner
 *
 *  Mine descriptors
 *
 */
#define MINE_MAX 20
#define NO_SHORING 0
#define WOODEN_SHORING 1
#define IRON_SHORING 2

struct mine_contents {
    struct item_ent **items;    /* ilist of items held */
    /* int iron, gold, mithril, gate_crystals; */
};

struct entity_mine {
    struct mine_contents mc[MINE_MAX];
    int shoring[MINE_MAX];
};

struct entity_subloc {
    ilist teaches;            /* skills location offers */
    int opium_econ;            /* addiction level of city */
    int defense;            /* defense rating of structure */

    schar loot;            /* loot & pillage level */
    uchar hp;                       /* "hit points" */
    uchar damage;            /* 0=none, hp=fully destroyed */
    uchar moat;                     /* Has a moat? */
#if 0
    short shaft_depth;		/* depth of mine shaft */
#endif

    struct entity_build **builds;   /* Ongoing builds here. */

    int moving;            /* daystamp of beginning of movement */
    struct entity_ship *x_ship;       /* Maybe a ship? */
#if 0
    int capacity;			/* capacity of ship */
    schar galley_ram;		/* galley is fitted with a ram */
#endif

    ilist near_cities;        /* cities rumored to be nearby */
    schar safe;            /* safe haven */
    schar major;            /* major city */
    schar prominence;        /* prominence of city */

#if 0
    schar link_when;		/* month link is open, -1 = never */
    schar link_open;		/* link is open now */
#endif
    ilist link_to;            /* where we are linked to */
    ilist link_from;        /* where we are linked from */
#if 0
    ilist bound_storms;		/* storms bound to this ship */
#endif
    int guild;                      /* what skill, if a sub_guild */
#if 0
    struct effect **effects;        /* ilist of effects on sub-location */
#endif
    short tax_market;        /* Market tax rate. */
    short tax_market2;              /* Temporary until end of month */
    /* Location control -- either here or loc */
    struct loc_control_ent control, control2;
    int entrance_size;              /* Size of entrance to subloc */
};

struct entity_item {
    short weight;
    short land_cap;
    short ride_cap;
    short fly_cap;
    short attack;        /* fighter attack rating */
    short defense;        /* fighter defense rating */
    short missile;        /* capable of missile attacks? */
    short maintenance;      /* Maintenance cost */
    short npc_split;        /* Size to "split" at... */
    short animal_part;      /* Produces this when killed. */

    schar is_man_item;    /* unit is a character like thing */
    schar animal;        /* unit is or contains a horse or an ox */
    schar prominent;    /* big things that everyone sees */
    schar capturable;    /* ni-char contents are capturable */
    schar ungiveable;       /* Can't be transferred between nobles. */
    schar wild;        /* Appears in the wild as a random encounter. */
    /* Value is actually the NPC_prog. */

    char *plural_name;
    int base_price;        /* base price of item for market seeding */
    int trade_good;         /* Is this thing a trade good? & how much*/
    int who_has;        /* who has this unique item */

    struct item_magic *x_item_magic;
    struct entity_artifact *x_item_artifact; /* Eventually replace item_magic */
};

struct item_magic {
    int creator;
    int lore;            /* deliver this lore for the item */

    int religion;                   /* Might be a religious artifact */

    schar curse_loyalty;        /* curse noncreator loyalty */
    schar cloak_region;
    schar cloak_creator;
    schar use_key;            /* special use action */

    ilist may_use;            /* list of usable skills via this */
    ilist may_study;        /* list of skills studying from this */

    int project_cast;        /* stored projected cast */
    int token_ni;            /* ni for controlled npc units */
    short quick_cast;        /* stored quick cast */

    schar attack_bonus;
    schar defense_bonus;
    schar missile_bonus;
    schar aura_bonus;
    schar aura;            /* auraculum aura */

    schar token_num;        /* how many token controlled units */
    schar orb_use_count;        /* how many uses left in the orb */

/*
 *  Not saved:
 */

    schar one_turn_use;        /* flag for one use per turn */
};

/*
 *  Religion sub-structure.
 *
 */
struct entity_religion_skill {
    char *name;        /* Of the god.  */
    int strength;      /* Related strength skill */
    int weakness;      /* Related weakness skill */
    int plant;         /* The holy plant. */
    int animal;        /* The holy plant. */
    int terrain;       /* Holy terrain. */
    int high_priest;   /* The high priest */
    int bishops[2];    /* The two bishops */
};

/*
 *  Skill flags.
 *
 */
#define MAX_FLAGS 4
#define IS_POLLED (1<<0)
#define REQ_HOLY_SYMBOL (1<<1)
#define REQ_HOLY_PLANT (1<<2)
#define COMBAT_SKILL (1<<3)

/*
 *  The specific religion skills require quite a bit of addition to the
 *  basic entity_skill structure.  This stuff is captured by making a new
 *  skill subkind ("religion") and putting in a pointer to a
 *  "religion" structure.
 *
 */
struct entity_skill {
    int time_to_learn;    /* days of study req'd to learn skill */
    int time_to_use;    /* days to use this skill */
    int flags;            /* Flags such as IS_POLLED, REQ_HOLY_SYMBOL, etc. */
    int required_skill;    /* skill required to learn this skill */
    int np_req;        /* noble points required to learn */
    ilist offered;        /* skills learnable after this one */
    ilist research;        /* skills researable with this one */
    ilist guild;                  /* skills offered if you're a guild member. */

    struct req_ent **req;    /* ilist of items required for use or cast */
    int produced;        /* simple production skill result */

    int no_exp;        /* this skill not rated for experience */
    int practice_cost;    /* cost to practice this skill. */
    int practice_time;    /* time to practice this skill. */
    int practice_prog;    /* A day longer to practice each N experience levels. */
    struct entity_religion_skill *religion_skill; /* Possible religion pointer. */
    int piety;            /* Piety required to cast religious skill */
    /* Or aura to cast magic skill; not used for all. */
/* not saved */

    int use_count;        /* times skill used during turn */
    int last_use_who;    /* who last used the skill (this turn) */
};

#define    REQ_NO    0        /* don't consume item */
#define    REQ_YES    1        /* consume item */
#define    REQ_OR    2        /* or with next */

struct req_ent {
    int item;        /* item required to use */
    int qty;        /* quantity required */
    schar consume;        /* REQ_xx */
};

struct entity_gate {
    int to_loc;        /* destination of gate */
    int notify_jumps;    /* whom to notify */
    int notify_unseal;    /* whom to notify */
    short seal_key;        /* numeric gate password */
    schar road_hidden;    /* this is a hidden road or passage */
};

struct entity_misc {
    char *display;        /* entity display banner */
    int npc_created;    /* turn peasant mob created */
    int npc_home;        /* where npc was created */
    int npc_cookie;        /* allocation cookie item for us */
    int summoned_by;    /* who summoned us? */
    char *save_name;    /* orig name of noble for dead bodies */
    int old_lord;        /* who did this dead body used to belong to */
    sparse npc_memory;
    int only_vulnerable;    /* only defeatable with this rare artifact */
    int garr_castle;    /* castle which owns this garrison */
    short border_open;      /* Is the garrison's border open? */
    int bind_storm;        /* storm bound to this ship */

    short storm_str;    /* storm strength */
    schar npc_dir;        /* last direction npc moved */
    schar mine_delay;    /* time until collapsed mine vanishes */
    char cmd_allow;        /* unit under restricted control */

    schar opium_double;    /* improved opium production -- not saved */
    char **post_txt;    /* text of posted sign -- not saved */
    int storm_move;        /* next loc storm will move to -- not saved */
    ilist garr_watch;    /* units garrison watches for -- not saved */
    ilist garr_host;    /* units garrison will attack -- not saved */
    int garr_tax;        /* garrison taxes collected -- not saved */
    int garr_forward;    /* garrison taxes forwarded -- not saved */
};


/*
 *  In-process command structure
 */

#define    DONE    0
#define    LOAD    1
#define    RUN    2
#define    ERROR    3

struct wait_arg {
    int tag;
    int a1, a2;
    char *flag;
};

#ifndef OLY_FORWARD_struct_command
#define OLY_FORWARD_struct_command
struct command {
    int who;        /* entity this is under (redundant) */
    int wait;        /* time until completion */
    int cmd;        /* index into cmd_tbl */

    int use_skill;        /* skill we are using, if any */
    int use_ent;        /* index into use_tbl[] for skill usage */
    int use_exp;        /* experience level at using this skill */
    int days_executing;    /* how long has this command been running */

    int a, b, c, d, e, f, g, h, i, j;/* command arguments */

    char *line;        /* original command line */
    char *parsed_line;    /* cut-up line, pointed to by parse */
    char **parse;        /* ilist of parsed arguments */

    schar state;        /* LOAD, RUN, ERROR, DONE */
    schar status;        /* success or failure */
    schar poll;        /* call finish routine each day? */
    schar pri;        /* command priority or precedence */
    schar conditional;    /* 0=none 1=last succeeded 2=last failed */
    schar inhibit_finish;    /* don't call d_xxx */

    schar fuzzy;        /* command matched fuzzy -- not saved */
    schar second_wait;    /* delay resulting from auto attacks -- saved */
    struct wait_arg **wait_parse;    /* not saved */
    schar debug;        /* debugging check -- not saved */
};
#endif

#define    numargs(c)    (ilist_len(c->parse) - 1)

/*
 *  How long a command has been running
 */

#define    command_days(c)        (c->days_executing)

/*
 *  Wed Dec 27 07:36:27 2000 -- Scott Turner
 *
 *  Types of command arguments.
 *
 */
#define CMD_undef 0
#define CMD_unit  1
#define CMD_item  2
#define CMD_skill 3
#define CMD_days  4
#define CMD_qty   5
#define CMD_gold  6
#define CMD_use   7
#define CMD_practice   8

struct cmd_tbl_ent {
    char *allow;        /* who may execute the command */
    char *name;        /* name of command */

    int (*start)(struct command *);        /* initiator */
    int (*finish)(struct command *);    /* conclusion */
    int (*interrupt)(struct command *);    /* interrupted order */

    int time;        /* how long command takes */
    int poll;        /* call finish each day, not just at end */
    int pri;        /* command priority or precedence */
    /*
     *  Tue Dec 26 18:37:31 2000 -- Scott Turner
     *
     *  Mods to add some command checking during the eat phase:
     *
     *    -- Num_args_required
     *    -- Max_args
     *    -- Arg_types[]
     *    -- cmd_comment()
     *    -- cmd_check()
     *
     */
    int num_args_required;
    int max_args;
    int arg_types[5];

    char *(*cmd_comment)(struct command *);

    void (*cmd_check)(struct command *);
};


#define    BUY        1
#define    SELL        2
#define    PRODUCE        3
#define    CONSUME        4

struct trade {
    int kind;        /* BUY or SELL */
    int item;
    int qty;
    int cost;
    int cloak;        /* don't reveal identity of trader */
    int have_left;
    int month_prod;        /* month city produces item */
    int who;        /* redundant -- not saved */
    int sort;        /* temp key for sorting -- not saved */
    int old_qty;          /* qty at beginning of month, for trade goods */
    int counter;          /* Counter to age and lose untraded goods. */
};


struct admit {
    int targ;        /* char or loc admit is declared for */
    int sense;        /* 0=default no, 1=all but.. */
    ilist l;

    int flag;        /* first time set this turn -- not saved */
};

/*
 *  Nations Structure
 *  Tue Apr  8 11:57:04 1997 -- Scott Turner
 *
 */
struct entity_nation {
    char *name;         /* Name of the nation, e.g., Mandor Empire */
    char *citizen;      /* Name of a citizen, eg., Mandorean */
    int nobles;         /* Total nobles */
    int nps;            /* Total NPS */
    int gold;          /* Total gold */
    int players;        /* Num players */
    short win;          /* Win? */
    ilist proscribed_skills;  /* Skills you can't have... */
    int player_limit;   /* Limit to # of players */
    int capital;        /* Capital city. */
    short jump_start;   /* Jump start points to start */
    short neutral;      /* Can't capture/lose NPs. */
};

#if 0
#define MAX_NATIONS 10
extern int num_nations;
extern struct entity_nation nations[MAX_NATIONS];
#endif

#define    if_malloc(p)        ((p) ? (p) : ((p) = my_malloc(sizeof(*(p)))))

/*
 *  malloc-on-demand substructure references
 */

#define    p_loc_info(n)        (&bx[n]->x_loc_info)
#define    p_char(n)        if_malloc(bx[n]->x_char)
#define    p_loc(n)        if_malloc(bx[n]->x_loc)
#define    p_subloc(n)        if_malloc(bx[n]->x_subloc)
#define    p_item(n)        if_malloc(bx[n]->x_item)
#define    p_player(n)        if_malloc(bx[n]->x_player)
#define    p_skill(n)        if_malloc(bx[n]->x_skill)
#define p_nation(n)        if_malloc(bx[n]->x_nation)
#define    p_gate(n)        if_malloc(bx[n]->x_gate)
#define    p_misc(n)        if_malloc(bx[n]->x_misc)
#define    p_disp(n)        if_malloc(bx[n]->x_disp)
#define    p_command(n)        if_malloc(bx[n]->cmd)
#define    p_ship(n)            if_malloc(p_subloc(n)->x_ship)

/*
 *  "raw" pointers to substructures, may be NULL
 */

#define    rp_loc_info(n)        (&bx[n]->x_loc_info)
#define    rp_char(n)        (bx[n]->x_char)
#define    rp_loc(n)        (bx[n]->x_loc)
#define    rp_subloc(n)        (bx[n]->x_subloc)
#define    rp_ship(n)        (rp_subloc(n) ? rp_subloc(n)->x_ship : NULL)
#define    rp_item(n)        (bx[n]->x_item)
#define    rp_player(n)        (bx[n]->x_player)
#define    rp_skill(n)        (bx[n]->x_skill)
#define rp_relig_skill(n)       (bx[n]->x_skill ? bx[n]->x_skill->religion_skill : NULL)
#define    rp_gate(n)        (bx[n]->x_gate)
#define    rp_misc(n)        (bx[n]->x_misc)
#define    rp_disp(n)        (bx[n]->x_disp)
#define    rp_command(n)        (bx[n]->cmd)
#define rp_nation(n)        (bx[n]->x_nation)

#define    rp_magic(n)        (rp_char(n) ? rp_char(n)->x_char_magic : NULL)
#define    p_magic(n)        if_malloc(p_char(n)->x_char_magic)

#define    rp_item_magic(n)    (rp_item(n) ? rp_item(n)->x_item_magic : NULL)
#define    p_item_magic(n)        if_malloc(p_item(n)->x_item_magic)

#define    rp_item_artifact(n)    (rp_item(n) ? rp_item(n)->x_item_artifact : NULL)
#define    p_item_artifact(n)    if_malloc(p_item(n)->x_item_artifact)
#define is_artifact(n)        (rp_item(n) ?  rp_item(n)->x_item_artifact : NULL)

extern struct box **bx;
extern int box_head[];            /* head of x_next_kind chain */
extern int sub_head[];            /* head of x_next_sub chain */


#define    kind(n)        (((n) > 0 && (n) < MAX_BOXES && bx[n]) ? \
                        bx[n]->kind : T_deleted)

#define    subkind(n)    (bx[n] ? bx[n]->skind : 0)
#define    valid_box(n)    (kind(n) != T_deleted)

#define    kind_first(n)    (box_head[(n)])
#define    kind_next(n)    (bx[(n)]->x_next_kind)

#define    sub_first(n)    (sub_head[(n)])
#define    sub_next(n)    (bx[(n)]->x_next_sub)

#define    is_loc_or_ship(n)    (kind(n) == T_loc || kind(n) == T_ship)
#define    is_ship(n)    (subkind(n) == sub_galley || subkind(n) == sub_roundship || subkind(n) == sub_raft | subkind(n) == sub_ship)
#define    is_ship_notdone(n)    (subkind(n) == sub_galley_notdone || subkind(n) == sub_roundship_notdone || subkind(n) == sub_raft_notdone || subkind(n) == sub_ship_notdone)
#define    is_ship_either(n)    (is_ship(n) || is_ship_notdone(n))
#define effects(n) (bx[n]->effects)

/*
 *	_moving indicates that the unit has initiated movement
 *	_gone indicates that the unit has actually left the locations,
 *	and should not be interacted with anymore.
 *
 *	The distinction allows zero time commands to interact with
 *	the entity on the day movement is begun.
 */

#define    ship_moving(n)    (rp_subloc(n) ? rp_subloc(n)->moving : 0)
#define    ship_gone(n)    (ship_moving(n) ? sysclock.days_since_epoch - \
                    ship_moving(n) + evening : 0)

#define    char_moving(n)    (rp_char(n) ? rp_char(n)->moving : 0)
#define    char_gone(n)    (char_moving(n) ? sysclock.days_since_epoch - \
                    char_moving(n) + evening : 0)

#define player_split_lines(n)    (rp_player(n) ? rp_player(n)->split_lines : 0)
#define player_split_bytes(n)    (rp_player(n) ? rp_player(n)->split_bytes : 0)
#define    player_email(n)        (rp_player(n) ? rp_player(n)->email : NULL)
#define    times_paid(n)        (rp_player(n) ? rp_player(n)->times_paid : 0)
#define    player_format(n)    (rp_player(n) ? rp_player(n)->format : 0)
#define    player_notab(n)        (rp_player(n) ? rp_player(n)->notab : 0)
#define    player_compuserve(n)    (rp_player(n) ? rp_player(n)->compuserve : 0)
#define    player_broken_mailer(n)    (rp_player(n) ? rp_player(n)->broken_mailer : 0)
#define banner(n)        (rp_misc(n) ? rp_misc(n)->display : NULL)
#define storm_bind(n)        (rp_misc(n) ? rp_misc(n)->bind_storm : 0)
#define npc_program(n)        (rp_char(n) ? rp_char(n)->npc_prog : 0)
#define char_guard(n)        (rp_char(n) ? rp_char(n)->guard : 0)
#define char_health(n)        (rp_char(n) ? rp_char(n)->health : 0)
#define char_sick(n)        (rp_char(n) ? rp_char(n)->sick : 0)
#define loyal_kind(n)        (rp_char(n) ? rp_char(n)->loy_kind : 0)
#define loyal_rate(n)        (rp_char(n) ? rp_char(n)->loy_rate : 0)
#define noble_item(n)        (rp_char(n) ? rp_char(n)->unit_item : 0)
#define char_new_lord(n)    (rp_char(n) ? rp_char(n)->new_lord : 0)
#define char_melt_me(n)        (rp_char(n) ? rp_char(n)->melt_me : 0)
#define char_behind(n)        (rp_char(n) ? rp_char(n)->behind : 0)
#define char_hidden(n)        (rp_magic(n) ? rp_magic(n)->hide_self : 0)
#define char_hide_mage(n)    (rp_magic(n) ? rp_magic(n)->hide_mage : 0)
#define char_cur_aura(n)    (rp_magic(n) ? rp_magic(n)->cur_aura : 0)
#define char_max_aura(n)    (rp_magic(n) ? rp_magic(n)->max_aura : 0)
#define char_piety(n)    ((is_priest(n) && rp_char(n)) ? rp_char(n)->religion.piety : 0)
#define reflect_blast(n)    (rp_magic(n) ? rp_magic(n)->aura_reflect : 0)
#if 0
#define char_pledge(n)		(rp_magic(n) ? rp_magic(n)->pledge : 0)
#endif
#define char_auraculum(n)    (rp_magic(n) ? rp_magic(n)->auraculum : 0)
#define char_abil_shroud(n)    (rp_magic(n) ? rp_magic(n)->ability_shroud : 0)
#define board_fee(n)        (rp_magic(n) ? rp_magic(n)->fee : 0)
#define ferry_horn(n)        (rp_magic(n) ? rp_magic(n)->ferry_flag : 0)
#define loc_prominence(n)    (rp_subloc(n) ? rp_subloc(n)->prominence : 0)
#define loc_opium(n)        (rp_subloc(n) ? rp_subloc(n)->opium_econ : 0)
#define loc_hp(n)        (rp_subloc(n) ? rp_subloc(n)->hp : 0)
#define loc_moat(n)        (rp_subloc(n) ? rp_subloc(n)->moat : 0)
/* #define loc_barrier(n)		(rp_loc(n) ? rp_loc(n)->barrier : 0) */
#define loc_barrier(n)        (get_effect(n, ef_magic_barrier, 0, 0))
#define no_barrier(n)        (get_effect(n, ef_inhibit_barrier, 0, 0))
#define loc_shroud(n)        (rp_loc(n) ? rp_loc(n)->shroud : 0)
#define loc_sea_lane(n)        (rp_loc(n) ? rp_loc(n)->sea_lane : 0)
#define sea_dist(n)        (rp_loc(n) ? rp_loc(n)->dist_from_sea : 0)
#define body_old_lord(n)    (rp_misc(n) ? rp_misc(n)->old_lord : 0)
#define swamp_dist(n)        (rp_loc(n) ? rp_loc(n)->dist_from_swamp : 0)
#define gate_dist(n)        (rp_loc(n) ? rp_loc(n)->dist_from_gate : 0)
#define    road_dest(n)        (rp_gate(n) ? rp_gate(n)->to_loc : 0)
#define    road_hidden(n)        (rp_gate(n) ? rp_gate(n)->road_hidden : 0)
#define item_animal_part(n)     (rp_item(n) ? rp_item(n)->animal_part : 0)
#define    item_animal(n)        (rp_item(n) ? rp_item(n)->animal : 0)
#define    item_prominent(n)    (rp_item(n) ? rp_item(n)->prominent : 0)
#define    item_unique(n)        (rp_item(n) ? rp_item(n)->who_has : 0)
#define    req_skill(n)        (rp_skill(n) ? rp_skill(n)->required_skill : 0)
#define    practice_cost(n)    (rp_skill(n) ? rp_skill(n)->practice_cost : 0)
#define    practice_time(n)    (rp_skill(n) ? rp_skill(n)->practice_time : 0)
#define    practice_progressive(n)    (rp_skill(n) ? rp_skill(n)->practice_prog : 0)
#define char_persuaded(n)    (rp_char(n) ? rp_char(n)->persuaded : 0)
#define char_rank(n)        (rp_char(n) ? rp_char(n)->rank : 0)
#define    skill_time_to_use(n)    (rp_skill(n) ? rp_skill(n)->time_to_use : 0)
#define    skill_flags(n)        (rp_skill(n) ? rp_skill(n)->flags : 0)
#define    combat_skill(n)        (rp_skill(n) ? (rp_skill(n)->flags & COMBAT_SKILL): 0)
#define    skill_produce(n)    (rp_skill(n) ? rp_skill(n)->produced : 0)
#define    skill_no_exp(n)        (rp_skill(n) ? rp_skill(n)->no_exp : 0)
#define    skill_np_req(n)        (rp_skill(n) ? rp_skill(n)->np_req : 0)
#define    ship_has_ram(n)        (rp_ship(n) ? rp_ship(n)->galley_ram : 0)
#if 0
#define	loc_link_open(n)	(rp_subloc(n) ? rp_subloc(n)->link_open : 0)
#endif
#define    loc_damage(n)        (rp_subloc(n) ? rp_subloc(n)->damage : 0)
#define loc_defense(n)        (rp_subloc(n) ? rp_subloc(n)->defense : 0)
#define    loc_pillage(n)        (rp_subloc(n) ? rp_subloc(n)->loot : 0)
#define    recent_pillage(n)    (rp_subloc(n) ? rp_subloc(n)->recent_loot : 0)
#define    safe_haven(n)        (rp_subloc(n) ? rp_subloc(n)->safe: 0)
#define    major_city(n)        (rp_subloc(n) ? rp_subloc(n)->major : 0)
#define    learn_time(n)        (rp_skill(n) ? rp_skill(n)->time_to_learn : 0)
#define skill_exp(who, sk)    (rp_skill_ent(who,sk) ? rp_skill_ent(who,sk)->experience : 0)
#define    player_np(n)        (rp_player(n) ? rp_player(n)->noble_points : 0)
#define    player_js(n)        (rp_player(n) ? rp_player(n)->jump_start : 0)
#define    gate_seal(n)        (rp_gate(n) ? rp_gate(n)->seal_key : 0)
#define    gate_dest(n)        (rp_gate(n) ? rp_gate(n)->to_loc : 0)
#define    char_proj_cast(n)    (rp_magic(n) ? rp_magic(n)->project_cast : 0)
#define    char_quick_cast(n)    (rp_magic(n) ? rp_magic(n)->quick_cast : 0)
#define    is_magician(n)        (rp_magic(n) ? rp_magic(n)->magician : 0)
#define    weather_mage(n)        (rp_magic(n) ? rp_magic(n)->knows_weather : 0)
#define    garrison_castle(n)    (rp_misc(n) ? rp_misc(n)->garr_castle : 0)
#define    border_open(n)        (rp_misc(n) ? rp_misc(n)->border_open : 0)
#define    npc_last_dir(n)        (rp_misc(n) ? rp_misc(n)->npc_dir : 0)
#define    restricted_control(n)    (rp_misc(n) ? rp_misc(n)->cmd_allow : 0)
#define    item_capturable(n)    (rp_item(n) ? rp_item(n)->capturable : 0)
#define    item_wild(n)        (rp_item(n) ? rp_item(n)->wild : 0)
#define    item_prog(n)        (rp_item(n) ? rp_item(n)->wild : 0)
#define    storm_strength(n)    (rp_misc(n) ? rp_misc(n)->storm_str : 0)
#define    npc_summoner(n)        (rp_misc(n) ? rp_misc(n)->summoned_by : 0)
#define    char_break(n)        (rp_char(n) ? rp_char(n)->break_point : 0)
#define    personal_break(n)    (rp_char(n) ? rp_char(n)->personal_break_point : 0)
#define    only_defeatable(n)    (rp_misc(n) ? rp_misc(n)->only_vulnerable : 0)

#if 0
#define	mine_depth(n)	  (rp_subloc(n) ? rp_subloc(n)->shaft_depth / 3 : 0)
#endif

#define entrance_size(n)  (rp_subloc(n) ? rp_subloc(n)->entrance_size : 0)
#define    release_swear(n)  (rp_magic(n) ? rp_magic(n)->swear_on_release : 0)
#define    our_token(n)      (rp_magic(n) ? rp_magic(n)->token : 0)

#define    item_token_num(n)  (rp_item_magic(n) ? rp_item_magic(n)->token_num : 0)
#define    item_token_ni(n)   (rp_item_magic(n) ? rp_item_magic(n)->token_ni : 0)

#define    item_lore(n)         (rp_item_magic(n) ? rp_item_magic(n)->lore : 0)
#define    item_use_key(n)      (rp_item_magic(n) ? rp_item_magic(n)->use_key : 0)
#define    item_creator(n)      (rp_item_magic(n) ? rp_item_magic(n)->creator : 0)
#define    item_aura(n)         (rp_item_magic(n) ? rp_item_magic(n)->aura : 0)
#define    item_creat_cloak(n)  (rp_item_magic(n) ? \
                    rp_item_magic(n)->cloak_creator : 0)
#define    item_reg_cloak(n)    (rp_item_magic(n) ? \
                    rp_item_magic(n)->cloak_region : 0)
#if 0
#define	item_creat_loc(n)    (rp_item_magic(n) ? \
                    rp_item_magic(n)->region_created : 0)
#endif

#define    item_curse_non(n)    (rp_item_magic(n) ? \
                    rp_item_magic(n)->curse_loyalty : 0)

#define    item_attack(n)        (rp_item(n) ? rp_item(n)->attack : 0)
#define    item_defense(n)        (rp_item(n) ? rp_item(n)->defense : 0)
#define    item_missile(n)        (rp_item(n) ? rp_item(n)->missile : 0)
#define    item_split(n)        (rp_item(n) ? (rp_item(n)->npc_split ? rp_item(n)->npc_split : 50) : 50)

#define    char_attack(n)        (rp_char(n) ? rp_char(n)->attack : 0)
#define    char_defense(n)        (rp_char(n) ? rp_char(n)->defense : 0)
#define    char_missile(n)        (rp_char(n) ? rp_char(n)->missile : 0)

#define    item_attack_bonus(n)    \
        (rp_item_magic(n) ? rp_item_magic(n)->attack_bonus : 0)
#define    item_defense_bonus(n)    \
        (rp_item_magic(n) ? rp_item_magic(n)->defense_bonus : 0)
#define    item_missile_bonus(n)    \
        (rp_item_magic(n) ? rp_item_magic(n)->missile_bonus : 0)
#define    item_aura_bonus(n)    \
        (rp_item_magic(n) ? rp_item_magic(n)->aura_bonus : 0)

#define    is_fighter(n)        (item_attack(n) || item_defense(n) || item_missile(n) || n == item_ghost_warrior)

#define man_item(n)    (rp_item(n) ? rp_item(n)->is_man_item : 0)

/*
 *  Religion stuff
 *  Tue Aug 13 09:15:29 1996 -- Scott Turner
 *
 */
#define    is_priest(n)        (skill_school(has_subskill((n), sub_religion)))
#define    is_wizard(n)        (has_subskill((n), sub_magic))
#define is_follower(n)          (rp_char(n) ? rp_char(n)->religion.priest : 0)
#define is_temple(n)  ((subkind(n) == sub_temple && rp_subloc(n)->guild) ? rp_subloc(n)->guild : 0)
#define holy_plant(n)           (is_priest(n) ? rp_relig_skill(is_priest(n))->plant : 0)
#define god_name(n)           (rp_relig_skill(n)->name)
#define skill_piety(n)          (rp_skill(n) ? rp_skill(n)->piety : 0)
#define skill_aura(n)          (rp_skill(n) ? rp_skill(n)->piety : 0)

/*
 *  Guild stuff
 *  Fri Nov 15 11:59:15 1996 -- Scott Turner
 *
 */
#define is_guild(n)  ((subkind(n) == sub_guild && rp_subloc(n)->guild) ? rp_subloc(n)->guild : 0)
#define guild_member(n) (rp_char(n) ? rp_char(n)->guild : 0)

/*
 *  Return exactly where a unit is
 *  May point to another character, a structure, or a region
 */

#define    loc(n)        (rp_loc_info(n)->where)


#include "loop.h"

/*
 *  Prototypes, defines and externs
 */

#include "code.h"
#include "dir.h"
#include "display.h"
#include "etc.h"
#include "garr.h"
#include "loc.h"
#include "order.h"
#include "sout.h"
#include "stack.h"
#include "swear.h"
#include "u.h"
#include "use.h"
#include "effect.h"
#include "relig.h"
#include "combat.h"
#include "quest.h"
#include "build.h"
#include "kill.h"
#include "artifacts.h"
#include "eat.h"

extern char *libdir;

#define    LINES    "---------------------------------------------"

/*
 *  Saved in libdir/system:
 */

extern olytime sysclock;        /* current time in Olympia */
extern int game_number;                 /* we hope to have many :-) */
extern int indep_player;        /* independent unit player */
extern int gm_player;            /* The Fates */
extern int deserted_player;        /* Deserted nobles */
extern int eat_pl;            /* Order scanner */
extern int skill_player;        /* Player for skill list report */
extern int post_has_been_run;
extern int seed_has_been_run;
extern int dist_sea_compute;
extern int near_city_init;
extern int cookie_init;
extern int faery_region;
extern int faery_player;
extern int hades_region;
extern int hades_pit;            /* Pit of Hades */
extern int hades_player;
extern int cloud_region;
extern int npc_pl;
extern int garr_pl;            /* garrison player */
extern int combat_pl;            /* combat log */
extern int garrison_magic;
extern int show_to_garrison;
extern int xsize, ysize;

/*
 *  This structure holds game "options" for various different flavors
 *  of TAG.
 *
 */
struct options_struct {
    schar turn_limit;                /* Limit players to a certain # of turns. */
    schar auto_drop;                 /* Drop non-responsive players. */
    schar free;                      /* Don't charge for this game. */
    char turn_charge[24];            /* How much to charge per turn. */
    schar mp_antipathy;              /* Do mages & priests hate each other? */
    schar survive_np;           /* Does SFW return NPs when forgotten? */
    schar death_nps;           /* What NPs get returned at death? */
    schar guild_teaching;           /* Do guilds teach guild skills? */
    char *accounting_dir;             /* Directory to "join" from. */
    char *accounting_prog;            /* Path of the accounting program. */
    char *html_path;                 /* Path to html directories */
    char *html_passwords;            /* Path to html passwords */
    schar times_pay;           /* What the Times pays for an article. */
    char cpp[80];                    /* Path of cpp */
    schar full_markets;           /* City markets buy wood, etc. */
    schar output_tags;           /* include <tag> in output */
    schar open_ended;           /* No end to game. */
    int num_books;                   /* Number of teaching books in city */
    int market_age;                  /* Months untouched in market before removal. */
    int min_piety;           /* Any priest can have this much piety. */
    int piety_limit;           /* Normal priest limited to
				             piety_limit * num_followers */
    int head_priest_piety_limit;     /* Head priest limited to
				     head_priest_piety_limit * num_followers */
    int top_piety;           /* Monthly +piety for head priest */
    int middle_piety;           /* Monthly +piety for junior priests */
    int bottom_piety;           /* Monthly +piety for everyone else */
    int claim_give;           /* Allow putting gold in claim? */
    int check_balance;               /* No orders w/o positive balance. */
    int free_np_limit;               /* Play for free with this many NPs. */
};
extern struct options_struct options;

#define    in_faery(n)        (region(n) == faery_region)
#define    in_hades(n)        (region(n) == hades_region)
#define    in_clouds(n)        (region(n) == cloud_region)

extern int immed_see_all;    /* override hidden-ness, for debugging */

#define    see_all(n)        immed_see_all


#ifndef NEW_TRADE
extern ilist trades_to_check;
#endif
extern char *kind_s[];
extern char *subkind_s[];
extern char *dir_s[];
extern char *loc_depth_s[];
extern char *short_dir_s[];
extern char *full_dir_s[];
extern char *month_names[];

extern char *entab(int);

extern int exit_opposite[];
extern int immediate;
extern int indent;
extern int show_day;
extern struct cmd_tbl_ent cmd_tbl[];
extern int evening;            /* are we in the evening phase? */
extern char *from_host;
extern char *reply_host;
extern ilist new_players;        /* new players added this turn */


#define        wait(n)        (rp_command(n) ? rp_command(n)->wait : 0)
#define        is_prisoner(n)    (rp_char(n) ? rp_char(n)->prisoner : FALSE)
#define        magic_skill(n)    (subkind(skill_school(n)) == sub_magic)
#define        religion_skill(n) (subkind(skill_school(n)) == sub_religion)

#define        add_s(n)    ((n) == 1 ? "" : "s")
#define        add_ds(n)    n, ((n) == 1 ? "" : "s")

#define        alive(n)    (kind(n) == T_char)

#define        is_npc(n)    (subkind(n) || loyal_kind(n) == LOY_npc || loyal_kind(n) == LOY_summon)
#define        is_real_npc(n)    (player(n) < 1000)
/* subkind(n) == sub_ni || loyal_kind(n) == LOY_npc) */

/*
 *  Alone except for angels & ninjas
 *
 */
#define    char_alone(n)    (count_stack_any_real(n, TRUE, TRUE) == 1)
/*
 *  Alone except for ninjas.
 *
 *  Thu Dec 14 11:50:17 2000 -- Scott Turner
 *
 *  Permit angels.
 *
 */
#define    char_alone_stealth(n)    (count_stack_any_real(n, TRUE, TRUE) == 1)
/*
 *  Alone counting everything.
 *
 */
#define    char_really_alone(n)    (count_stack_any_real(n, FALSE, FALSE) == 1)
/*
 *  Hidden & alone (w/ angels & ninjas)
 *
 */
#define    char_really_hidden(n)    (char_hidden(n) && \
                 char_alone_stealth(n) && \
                 !is_prisoner(n))

#define will_pay(n)    (rp_char(n) ? rp_char(n)->pay : 0)
#define refugee(n) (!nation(n) && subkind(n) != sub_ni)


#define        CHAR_FIELD    5    /* field length for box_code_less */


#define    MAX_POST 60    /* max line length for posts and messages */

/* Used in combat.c and move.c */

#define        A_WON    1
#define        B_WON    2
#define        TIE    3
#define        NO_COMBAT 4

/*  Defines a faction as either MUs or Priests.  */
#define        MU_FACTION 1
#define        PRIEST_FACTION 2

/* Define format flags. */
#define NONE 0
#define HTML (1 << 0)
#define TEXT (1 << 1)
#define TAGS (1 << 2)
#define RAW (1 << 3)
#define ALT (1 << 4)


#endif //OLYTAG_OLY_H
