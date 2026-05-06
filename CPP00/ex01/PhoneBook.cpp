
#include "PhoneBook.hpp"
#include "Contact.hpp"
#include <iostream>
#include <cctype>
#include <cstdlib>

PhoneBook::PhoneBook()
{
    index = 0;
}

bool str_isalnum(std::string str)
{
    int i = 0;
    while(str[i])
    {
        if(!isalnum(str[i]))
            return(false);
        i++;
    }
    return(true);
}

std::string get_info(std::string info)
{
    std::string str;
    while (1)
    {
        std::cout << info;
        std::getline(std::cin , str);
        if(std::cin.eof())
            exit(1);
        if (str.empty() || !str_isalnum(str))
        {
            std::cout << "invalid info" << std::endl;
            continue;
        }
        return(str);
    }
}

void    PhoneBook::add_contact()
{
    std::string first;
    std::string last;
    std::string nick;
    std::string phone;
    std::string secret;
    first = get_info("First Name: ");
    last = get_info("Last Name: ");
    nick = get_info("NickName: ");
    phone = get_info("Phone Number: ");
    secret = get_info("Darkest Secret: ");
    contacts[index % 8].set_contact_info(first, last, nick, phone, secret);
    index++;
}

void    PhoneBook::list_contact()
{
    int i;
    std::string selected;
    int select;

    i = 0;
    select = -1;
    std::cout << "     index|first name| last name|  nickname" << std::endl;
    while(i < index)
    {
        if(index > 8 && i == 8)
            break ;
        contacts[i].print_contact(i);
        i++;
    }
    std::cout << "enter the index: ";
    std::getline(std::cin, selected);
    if(std::cin.eof())
        exit(1);
    select = std::atoi(selected.c_str());
    if((!isdigit(selected[0])) || (select > index - 1))
        std::cout << "Wrong inex size" << std::endl;
    else
        contacts[select].get_contact_info();
}
