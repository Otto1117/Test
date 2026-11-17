#include <iostream>

#include "test/greeter.h"

int main() {
    std::cout << test::greet("world") << '\n';
    return 0;
}
