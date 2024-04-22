# git-lfs-fetch-cpp
Partial reimpl of https://github.com/liberapay/git-lfs-fetch.py in C++


# Huh? Why?
I need git Large File System (LFS) fetching in a C++ project that prefers not to shell out to the real git-lfs CLI, or shipping the python runtime.
Brings me no joy to conclude bjarnelang is the best choice here.
