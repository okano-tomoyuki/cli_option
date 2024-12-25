#include "cli_option.hpp"
 
int main(int argc, char** argv)
{
    auto cli_option = CliOption()
        .add<CliOption::ARG_OPTIONAL>('h', "help", "print help or manual.")
        .add<CliOption::ARG_OPTIONAL>('i', "input", "input file name.")
        .add<CliOption::ARG_OPTIONAL>('o', "output", "output file name.")
        .add<CliOption::ARG_OPTIONAL>('v', "version", "print application version.");
 
    cli_option.usage("hogehoge soft for your useful.\n", "\ncopyright aaaa.bbb");
    cli_option.parse(argc, argv);
    cli_option.print_value();
 
    if (!cli_option.found('v'))
    {
        std::cout << "version option not found." << std::endl;
    }
}