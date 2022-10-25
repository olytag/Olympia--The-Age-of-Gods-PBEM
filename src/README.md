# Programming Notes

## Operating Systems
Tests for the target OS should be in the `CMakeLists` file.

We use the `os_xxxx.c` files to hide the OS dependencies.
Functions that are borked between Mac, Linux, and Windows are redefined in those files.
The target flags from `CMake` control what gets included in the final build.

This style is from Peter Miller's `cook` program.