# Olympia: The Age of Gods PBEM

Olympia is a play by e-mail game created by
[Rich Skrenta](https://github.com/skrenta).
Two versions (G1 and G2) were run as a commercial game by Shadow Island Games.
When the game shut down in the mid 2000's, Skrenta released the
[code base](http://www.pbm.com/oly/).

The Age of Gods, by
[Scott Turner](http://www.pbm.com/oly/tag/srt.html),
updated and enhanced the game.

    In 1998 I launched The Age of Gods.
    The Age of Gods is a fantasy strategy game based upon
    Rich Skrenta's excellent Olympia -- thus bringing me back
    full circle. The game features hundreds of new skills,
    more complex military and economic systems, and many other
    improvements over Olympia.
        -- Scott Turner

It was also hosted by Shadow Island Games at
[Olympia: The Age of Gods](http://www.pbm.com/oly/tag/).

The original source for Olympia is
[here](http://www.pbm.com/oly/olympia.tgz);
the code for Olympia: The Age of Gods is
[here](http://www.pbm.com/oly/olympia-tag.tgz).

## Reporting errors
Please report errors using the
[issue tracker on Github](https://github.com/olytag/Olympia--The-Age-of-Gods-PBEM/issues).

## License
The code in this repository is released under the [BSD 3-clause license](LICENSE).

## Updates (October, 2022)
We are working on a cleanup of the code with the goal of getting the source to run on Linux, Mac, and Windows.

The effort entails formatting the source and adding support for CMake.
It does not include bug fixes or enhancements.
The only change planned for the code is updating `ilist` to work on both 32- and 64-bit systems.
(The base code requires the -m32 flag because `ilist` assumes that pointers and integers are the same size.)
