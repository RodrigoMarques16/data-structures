# Data Structures

## Compiling

From the project root:

`> cmake --build build --config Release --target all`

or

`> cd build`
`> make`

## Running

Run `./build/[target}`

Available targets are: rb_bench, st_bench, bf_bench

## References

The original xxHash: https://github.com/Cyan4973/xxHash

The port we're using: https://github.com/RedSpah/xxhash_cpp

FNV hash from: https://create.stephan-brumme.com/fnv-hash/