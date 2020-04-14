# Data Structures

## Compiling

From the project root:

`> cmake --build build --config Release --target all`

or if you don't have cmake:

`> cd build` \
`> make`

## Running

Run `./build/[target]`

Available targets are: rb_bench, st_bench, bf_bench, tests

## References

The original xxHash: https://github.com/Cyan4973/xxHash

The port we're using: https://github.com/RedSpah/xxhash_cpp

FNV hash from: https://create.stephan-brumme.com/fnv-hash/