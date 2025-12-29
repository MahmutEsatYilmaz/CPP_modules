
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongCat.hpp"

int main()
{
    const Animal* meta = new Animal();
    const Animal* j = new Dog();
    const Animal* i = new Cat();

    std::cout << "Type j: " << j->getType() << " " << std::endl;
    std::cout << "Type i: " << i->getType() << " " << std::endl;
    
    std::cout << "Sound i (Cat): "; 
    i->makeSound();
    std::cout << "Sound j (Dog): "; 
    j->makeSound();
    std::cout << "Sound meta: "; 
    meta->makeSound();
    delete j;
    delete i;
    delete meta;

    const WrongAnimal* wrongMeta = new WrongAnimal();
    const WrongAnimal* wrongCat = new WrongCat();

    std::cout << "Type wrongCat: " << wrongCat->getType() << " " << std::endl;
    std::cout << "Sound wrongCat: ";
    wrongCat->makeSound();
    std::cout << "Type wrongMeta: " << wrongMeta->getType() << " " << std::endl;
    std::cout << "Sound wrongMeta: ";
    wrongMeta->makeSound();

    delete wrongCat;
    delete wrongMeta;

    return 0;
}