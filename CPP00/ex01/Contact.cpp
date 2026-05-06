
#include "Contact.hpp"

std::string fit_str(std::string info)
{
    if(info.length() < 10)
        info.insert(0, (10 - info.length()), ' ');
    else if(info.length() > 10)
    {
        info = info.substr(0, 9) + ".";
    }
    return(info);
}

void    Contact::set_contact_info(std::string first, std::string last, std::string nick, std::string phone, std::string secret)
{
    Contact::first_name = first;
    Contact::last_name = last;
    Contact::nickname = nick;
    Contact::phone_number = phone;
    Contact::darkest_secret = secret;
}

void    Contact::get_contact_info()
{
    std::cout << "First Name:     " << first_name << std::endl;
    std::cout << "Last Name:      " << last_name << std::endl;
    std::cout << "Nickname:       " << nickname << std::endl;
    std::cout << "Phone Number:   " << phone_number << std::endl;
    std::cout << "Darkest Secret: " << darkest_secret << std::endl;
}

void    Contact::print_contact(int index)
{
    std::cout << "         " << index << "|";
    std::cout << fit_str(first_name) << "|";
    std::cout << fit_str(last_name) << "|";
    std::cout << fit_str(nickname) << std::endl;
}