#include <iostream>
#include "Cli.h"

//Example user filepath (Do not copy) => C:\SearchEngineBenchmarkCorpus
int main()
{
    Cli cli{ std::cin, std::cout };
    return cli.run();
}
