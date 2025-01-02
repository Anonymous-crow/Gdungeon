#ifndef CLI_H
#define CLI_H

#include <string>
#include <vector>

class Cli {
    public:
        Cli(int argl, char** newArgs);
    private:
        std::vector<char*> args;
};

#endif

