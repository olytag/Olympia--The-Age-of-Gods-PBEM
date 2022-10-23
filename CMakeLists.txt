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

        src/oly/helpers.c
        src/oly/types.h src/oly/forward.h

        src/memory/memory.c src/memory/memory.h
        src/os/linux.c src/os/linux.h
        src/os/mac.c src/os/mac.h
        src/os/windows.c src/os/windows.h
        src/random/sfc32.c src/random/sfc32.h
        src/random/random.c src/random/random.h
        src/vectors/ilist.c src/vectors/ilist.h src/os/generic.h)
target_include_directories(oly PRIVATE src/oly)


add_executable(mapgen
        src/mapgen/mapgen.c src/mapgen/mapgen.h
        src/mapgen/z.c src/mapgen/z.h
        src/mapgen/road.h
        src/mapgen/tile.h
        src/memory/memory.c src/memory/memory.h
        src/random/random.c src/random/random.h
        src/vectors/ilist.c src/vectors/ilist.h
        src/vectors/rlist.c src/vectors/rlist.h
        src/vectors/tlist.c src/vectors/tlist.h
        src/random/sfc32.c src/random/sfc32.h
        src/vectors/tlist.c src/vectors/tlist.h)
target_include_directories(mapgen PRIVATE src/mapgen)


add_executable(checkcode
        src/checkcode/main.c)
target_include_directories(checkcode PRIVATE src/checkcode)


add_executable(ilist_test
        src/vectors/ilist_test.c
        src/random/random.c src/random/random.h
        src/random/sfc32.c src/random/sfc32.h
        src/vectors/ilist.c src/vectors/ilist.h)
target_include_directories(ilist_test PRIVATE src/vectors)


add_executable(random_test
        src/random/random_test.c
        src/random/random.c src/random/random.h
        src/random/sfc32.c src/random/sfc32.h)
target_include_directories(random_test PRIVATE src/random)


add_executable(rlist_test
        src/vectors/rlist_test.c
        src/random/random.c src/random/random.h
        src/random/sfc32.c src/random/sfc32.h
        src/vectors/rlist.c src/vectors/rlist.h)
target_include_directories(rlist_test PRIVATE src/vectors)


add_executable(sfc32_test
        src/random/sfc32_test.c
        src/random/sfc32.c src/random/sfc32.h)
target_include_directories(sfc32_test PRIVATE src/random)


add_executable(tlist_test
        src/vectors/tlist_test.c
        src/random/random.c src/random/random.h
        src/random/sfc32.c src/random/sfc32.h
        src/vectors/tlist.c src/vectors/tlist.h)
target_include_directories(tlist_test PRIVATE src/vectors)


