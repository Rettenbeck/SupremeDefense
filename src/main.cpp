#define INCLUDE_CPP

#include <iostream>
#include <include.hpp>



int main() {
    std::make_unique<SupDef::App>()->run();
    return 0;
}
