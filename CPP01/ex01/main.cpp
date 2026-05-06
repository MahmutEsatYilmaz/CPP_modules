
#include "Zombie.hpp"

int main()
{
    Zombie *zombie;
    int i = 0;
    zombie = zombieHorde(5, "Zack");
    while(i < 6)
    {
        zombie[i].announce();
        i++;
    }
    i = 0;
        delete zombie;
}