# CMake
Please note that this file details building the program using `cmake`.
For instructions on using the traditional `make` build, please see the README_install file.

## Building

Building with CMake requires version 3.5 or newer.
We highly recommend installing `ninja-build` along with CMake.

If you're on a Mac or Linux machine, it should be something like:

    ~$ cmake --version
      cmake version 3.5.1

    ~$ git clone https://github.com/olytag/Olympia--The-Age-of-Gods-PBEM.git olytag

    ~$ cd olytag

    olytag$ git remote -v
        origin  git@github.com:olytag/Olympia--The-Age-of-Gods-PBEM.git (fetch)
        origin  git@github.com:olytag/Olympia--The-Age-of-Gods-PBEM.git (push)

    olytag$ cd build

    build$ cmake ..
        -- The C compiler identification is GNU 9.4.0
        -- Detecting C compiler ABI info
        -- Detecting C compiler ABI info - done
        -- Check for working C compiler: /usr/bin/cc - skipped
        -- Detecting C compile features
        -- Detecting C compile features - done
        -- Configuring done
        -- Generating done
        -- Build files have been written to: ~/olymtag/build

    build$ cd ..

    olytag$ cmake --build build

All the resulting binaries are in `build/`.

Compiling is never easy.
Please feel free to reach out to the GitHub site if you have any questions.

## Resources

There's a helpful page on CMake at
[HSF Training](https://hsf-training.github.io/hsf-training-cmake-webpage/02-building/index.html).
