#include "ScalarConverter.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        ScalarConverter::convert(argv[1]);
        return 0;
    }

    /* No argv[1]: run a battery of representative literals instead. */
    const char *tests[] = {
        "0", "42", "-42", "99999999999999",
        "0.0f", "42.0f", "-4.2f", "4.2f",
        "0.0", "-4.2", "4.2",
        "'a'", "'*'",
        "nan", "+inf", "-inf",
        "nanf", "+inff", "-inff"
    };
    size_t count = sizeof(tests) / sizeof(tests[0]);

    for (size_t i = 0; i < count; ++i)
    {
        std::cout << "--- convert(\"" << tests[i] << "\") ---\n";
        ScalarConverter::convert(tests[i]);
    }
    return 0;
}
