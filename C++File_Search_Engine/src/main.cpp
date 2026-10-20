#include <iostream>
#include <string>

#include "Cli.h"

using std::cout;
using std::cin;
using std::string;

int main()
{
    Cli cli{ std::cin, std::cout };
    return cli.run();
}

