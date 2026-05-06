
#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <iostream>
class Contact
{
private:
    std::string first_name;
    std::string last_name;
    std::string nickname;
    std::string phone_number;
    std::string darkest_secret;
public:
    void    set_contact_info(std::string first, std::string last, std::string nick, std::string phone, std::string secret);
    void    get_contact_info();
    void    print_contact(int index);

};

#endif