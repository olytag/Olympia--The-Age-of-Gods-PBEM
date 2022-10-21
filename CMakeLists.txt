cmake_minimum_required(VERSION 3.5)
project(olytag C)
include_directories(src)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLINUX -DNEW_TRADE")

if(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOLYTAG_CC_WINDOWS")
endif()

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

add_executable(mapgen
        mapgen/mapgen.c mapgen/mapgen.h
        mapgen/z.c mapgen/z.h
        mapgen/rnd.c
        mapgen/lists.c mapgen/lists.h)
target_include_directories(mapgen PRIVATE mapgen/)

add_executable(prng_test
        src/prng.c src/prng.h
        src/prng_test.c)
target_include_directories(prng_test PRIVATE src/)

add_executable(oly
        src/main.c

        src/add.c
        src/adv.c
        src/alchem.c
        src/art.c
        src/artifacts.c src/artifacts.h
        src/basic.c
        src/beast.c
        src/build.c src/build.h
        src/buy.c
        src/c1.c
        src/c2.c
        src/check.c
        src/cloud.c
        src/code.c src/code.h
        src/combat.c src/combat.h
        src/day.c
        src/dir.c src/dir.h
        src/display.c src/display.h
        src/eat.c src/eat.h
        src/effect.c src/effect.h
        src/etc.h
        src/faery.c
        src/garr.c src/garr.h
        src/gate.c
        src/glob.c
        src/gm.c
        src/hades.c
        src/hero.c
        src/immed.c
        src/input.c
        src/io.c
        src/kill.c src/kill.h
        src/loc.c src/loc.h
        src/loop.h
        src/lore.c
        src/make.c
        src/map.c
        src/mining.c
        src/move.c
        src/necro.c
        src/npc.c
        src/order.c src/order.h
        src/perm.c
        src/produce.c
        src/quest.c src/quest.h
        src/ranger.c
        src/relig.c src/relig.h
        src/report.c
        src/savage.c
        src/scry.c
        src/seed.c
        src/shipcraft.c
        src/sout.c src/sout.h
        src/stack.c src/stack.h
        src/stealth.c
        src/storm.c
        src/summary.c
        src/swear.c src/swear.h
        src/times.c
        src/trading.c
        src/u.c src/u.h
        src/use.c src/use.h
        src/win.c
        src/z.c src/z.h
        src/ilist.c src/ilist.h

        src/helpers.c
        src/os_windows.c src/os_linux.c src/os_mac.c
        src/types.h src/forward.h
        src/prng.c src/prng.h
        src/random_test.c
        )
target_include_directories(oly PRIVATE src/)