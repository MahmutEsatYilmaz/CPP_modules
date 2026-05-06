
#include <iostream>
#include "PhoneBook.hpp"

int main()
{
    std::string str;
    PhoneBook phonebook;
    while (1)
    {
        std::cout << "PhoneBook: ";
        std::getline(std::cin, str);

        if(std::cin.eof())
            break ;
        if(str == "ADD")
            phonebook.add_contact();
        else if(str == "SEARCH")
            phonebook.list_contact();
        else if(str == "EXIT")
            break;
        else if(!str.empty())
            std::cout << "WRONG COMMAND" << std::endl << "USAGE: ADD | SEARCH | EXIT" << std::endl;
    }
}
