#include <iostream>
#include "Cli.h"

int main()
{
    Cli cli{ std::cin, std::cout };
    return cli.run();
}

