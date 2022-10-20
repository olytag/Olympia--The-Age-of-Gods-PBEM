cmake_minimum_required(VERSION 3.5)
project(olytag C)
include_directories(src)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLINUX -DNEW_TRADE")

add_executable(checkcode
        src/checkcode.c
        )
target_include_directories(checkcode PRIVATE src/)

add_executable(ilist_test
        mapgen/ilist_test.c
        mapgen/rnd.c
        mapgen/lists.c mapgen/lists.h
        )
target_include_directories(ilist_test PRIVATE mapgen/)

#add_executable(mapgen
#        mapgen/mapgen.c mapgen/mapgen.h
#        mapgen/z.c mapgen/z.h
#        mapgen/rnd.c
#        mapgen/lists.c mapgen/lists.h)
#target_include_directories(mapgen PRIVATE mapgen/)

add_executable(oly
        src/main.c

        src/z.c
        src/swear.c
        src/code.c
        src/helpers.c

        src/z.h
        src/code.h
        src/loop.h

        src/forward.h
        src/types.h
#        src/add.c
#        src/adv.c
#        src/alchem.c
#        src/art.c
#        src/basic.c
#        src/beast.c
#        src/build.c
#        src/buy.c
#        src/check.c
#        src/c1.c
#        src/c2.c
#        src/cloud.c
#        src/code.c
#        src/combat.c
#        src/day.c
#        src/dir.c
#        src/display.c
#        src/eat.c
#        src/faery.c
#        src/garr.c
#        src/gate.c
#        src/glob.c
#        src/gm.c
#        src/hades.c
#        src/immed.c
#        src/input.c
#        src/io.c
#        src/loc.c
#        src/lore.c
#        src/make.c
#        src/move.c
#        src/necro.c
#        src/npc.c
#        src/order.c
#        src/perm.c
#        src/produce.c
#        src/quest.c
#        src/relig.c
#        src/report.c
#        src/savage.c
#        src/scry.c
#        src/seed.c
#        src/sout.c
#        src/stack.c
#        src/stealth.c
#        src/storm.c
#        src/summary.c
#        src/swear.c
#        src/u.c
#        src/use.c
#        src/effect.c
#        src/trading.c
#        src/shipcraft.c
#        src/mining.c
#        src/win.c
#        src/kill.c
#        src/ranger.c
#        src/times.c
#        src/artifacts.c
#        src/map.c
#        src/hero.c
#        src/loop.h
#        src/code.h
#        src/dir.h
#        src/display.h
#        src/etc.h
#        src/garr.h
#        src/loc.h
#        src/order.h
#        src/sout.h
#        src/stack.h
#        src/swear.h
#        src/u.h
#        src/use.h
#        src/effect.h
#        src/relig.h
#        src/combat.h
#        src/quest.h
#        src/build.h
#        src/kill.h
#        src/artifacts.h
#        src/eat.h
        )
target_include_directories(oly PRIVATE src/)