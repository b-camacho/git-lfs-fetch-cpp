# git-lfs-fetch-cpp
Partial reimpl of https://github.com/liberapay/git-lfs-fetch.py in C++


# Huh? Why?
I need git Large File System (LFS) fetching in a C++ project that prefers not to shell out to the real git-lfs CLI, or shipping the python runtime.
Brings me no joy to conclude bjarnelang is the best choice here.

# Usage
You need:
- an x86 linux
- ssh
- (ideally) a repo with lfs to test on.


`nix develop --impure` gets all your dev deps, build with `make`.
`make test` makes a small test binary under `test/main`
