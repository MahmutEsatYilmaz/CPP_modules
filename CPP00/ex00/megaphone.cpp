
#include <iostream>

int main(int ac , char **av)
{
    int i;
    int j;

    i = 1;
    j = 0;
    if(ac < 2)
    {
        std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
        return (1);
    }
    while(av[i])
    {
        j = 0;
        while (av[i][j])
        {
            av[i][j] = toupper(av[i][j]);
            j++;
        }
        std::cout << av[i];
        if(av[i + 1])
            std::cout << " ";
        else
            std::cout << std::endl;
        i++;
    }
    return (0);
}