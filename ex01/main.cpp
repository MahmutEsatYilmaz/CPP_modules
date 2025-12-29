
#include "Dog.hpp"
#include "Cat.hpp"

int main()
{
    std::cout << "--- SUBJECT GEREKSINIMI: ARRAY TESTI ---" << std::endl;
    
    const int arraySize = 4;
    Animal* animals[arraySize];

    for (int i = 0; i < arraySize / 2; i++)
        animals[i] = new Dog();
    
    for (int i = arraySize / 2; i < arraySize; i++)
        animals[i] = new Cat();
    for (int i = 0; i < arraySize; i++)
        delete animals[i];


    std::cout << "\n--- DEEP COPY KANITI ---" << std::endl;
    Dog basic;
    Dog tmp = basic;
    return 0;
}