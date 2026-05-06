
#include <iostream>

int main()
{
    std::string str = "HI THIS IS BRAIN";
    std::string *stringPTR = &str;
    std::string &stringREF = str;

    std::cout << "variable adress: " << &str << std::endl;
    std::cout << "referance adress: " << &stringREF << std::endl;
    std::cout << "pointer adress: " << &stringPTR << std::endl;

    std::cout << "variable value: " << str << std::endl;
    std::cout << "variable value: " << stringREF << std::endl;
    std::cout << "variable value: " << stringPTR << std::endl;
}