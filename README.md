# uint256_t

An unsigned 256 bit integer type for C++

Copyright (c) 2013 - 2018 Jason Lee @ calccrypto at gmail.com

Please see LICENSE file for license.

## Acknowledgements
Forked from https://github.com/calccrypto/uint256_t

### Main changes are
- Use cmake build system.
- Replace `uint128_t` implementation with build in `__uint128_t` type. This increase performance alot.
- Use C++20 language freatures.

## Usage
This is simple implementation of an unsigned 256 bit
integer type in C++. It's meant to be used like a standard
`uintX_t`, except with a larger bit size than those provided
by C/C++.

All that needs to be done in code is `#include "uint256_t.h"`

```c++
#include <iostream>
#include "uint256_t.h"

int main() {
    uint256_t a = 1;
    uint256_t b = 2;
    std::cout << (a | b) << std::endl;
    return 0;
}
```

### Compilation
A C++ compiler supporting at least C++20 is required. And cmake version 3.17.


