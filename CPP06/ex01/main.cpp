#include "Serializer.hpp"
#include <iostream>

int main()
{
    Data original;
    original.id = 42;
    original.name = "Bender";
    original.score = 3.14;

    uintptr_t raw = Serializer::serialize(&original);
    Data *back = Serializer::deserialize(raw);

    std::cout << "original address: " << &original << "\n";
    std::cout << "serialized value: " << raw << "\n";
    std::cout << "deserialized address: " << back << "\n";

    if (back == &original)
        std::cout << "OK: deserialize(serialize(ptr)) == ptr\n";
    else
        std::cout << "KO: pointers differ\n";

    std::cout << "back->id=" << back->id << " back->name=" << back->name
              << " back->score=" << back->score << "\n";
    return 0;
}
