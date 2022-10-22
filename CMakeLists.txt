cmake_minimum_required(VERSION 3.5)
project(olytag C)
include_directories(src)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNEW_TRADE")

if (LINUX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOLYTAG_CC_LINUX -DLINUX -m32")
endif ()

if (WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOLYTAG_CC_WINDOWS")
endif ()

add_executable(checkcode
        src/checkcode/main.c)
target_include_directories(checkcode PRIVATE src/checkcode)

add_executable(mapgen
        src/mapgen/mapgen.c src/mapgen/mapgen.h
        src/mapgen/z.c src/mapgen/z.h
        src/mapgen/ilist.c src/mapgen/ilist.h
        src/mapgen/ilist_test.c src/mapgen/ilist_test.h
        src/mapgen/rlist.c src/mapgen/rlist.h
        src/mapgen/prng.c src/mapgen/prng.h
        src/mapgen/prng_test.c src/mapgen/prng_test.h
        src/mapgen/random.c src/mapgen/random.h
        src/mapgen/random_test.c src/mapgen/random_test.h)
target_include_directories(mapgen PRIVATE src/mapgen)

add_executable(oly
        src/oly/main.c

        src/oly/add.c
        src/oly/adv.c
        src/oly/alchem.c
        src/oly/art.c
        src/oly/artifacts.c src/oly/artifacts.h
        src/oly/basic.c
        src/oly/beast.c
        src/oly/build.c src/oly/build.h
        src/oly/buy.c
        src/oly/c1.c
        src/oly/c2.c
        src/oly/check.c
        src/oly/cloud.c
        src/oly/code.c src/oly/code.h
        src/oly/combat.c src/oly/combat.h
        src/oly/day.c
        src/oly/dir.c src/oly/dir.h
        src/oly/display.c src/oly/display.h
        src/oly/eat.c src/oly/eat.h
        src/oly/effect.c src/oly/effect.h
        src/oly/etc.h
        src/oly/faery.c
        src/oly/garr.c src/oly/garr.h
        src/oly/gate.c
        src/oly/glob.c
        src/oly/gm.c
        src/oly/hades.c
        src/oly/hero.c
        src/oly/immed.c
        src/oly/input.c
        src/oly/io.c
        src/oly/kill.c src/oly/kill.h
        src/oly/loc.c src/oly/loc.h
        src/oly/loop.h
        src/oly/lore.c
        src/oly/make.c
        src/oly/map.c
        src/oly/mining.c
        src/oly/move.c
        src/oly/necro.c
        src/oly/npc.c
        src/oly/order.c src/oly/order.h
        src/oly/perm.c
        src/oly/produce.c
        src/oly/quest.c src/oly/quest.h
        src/oly/ranger.c
        src/oly/relig.c src/oly/relig.h
        src/oly/report.c
        src/oly/savage.c
        src/oly/scry.c
        src/oly/seed.c
        src/oly/shipcraft.c
        src/oly/sout.c src/oly/sout.h
        src/oly/stack.c src/oly/stack.h
        src/oly/stealth.c
        src/oly/storm.c
        src/oly/summary.c
        src/oly/swear.c src/oly/swear.h
        src/oly/times.c
        src/oly/trading.c
        src/oly/u.c src/oly/u.h
        src/oly/use.c src/oly/use.h
        src/oly/win.c
        src/oly/z.c src/oly/z.h
        src/oly/ilist.c src/oly/ilist.h

        src/oly/helpers.c
        src/oly/os_windows.c src/oly/os_linux.c src/oly/os_mac.c
        src/oly/types.h src/oly/forward.h
        src/oly/prng.c src/oly/prng.h
        src/oly/random_test.c)
target_include_directories(oly PRIVATE src/oly)

add_executable(prng_test
        src/prng_test/main.c

        src/oly/prng.c src/oly/prng.h)
target_include_directories(prng_test PRIVATE src/prng_test src/oly)
