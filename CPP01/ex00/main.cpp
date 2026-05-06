
#include <iostream>
#include "Zombie.hpp"

int main()
{
    Zombie *heap;
    randonChump("Allie");
    heap = newZombie("Zack");

    delete heap;
    return (0);
}