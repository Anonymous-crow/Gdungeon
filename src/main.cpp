#include <iostream>
#include <random>
#include <memory>
#include <string>
#include "classes.hpp"
#include "catalog.hpp"
#include "cli.hpp"

auto rng = std::default_random_engine {};

void createTheGuys(Party& party) {
    
    party.addMember(new Player(25, "Rudolph"));
    party.addMember(new Player(22, "Mikey"));
    party.addMember(new Player(18, "“Slick”"));
    party.addMember(new Player(20, "Dr. Victor"));
}

int main(int argLen, char** args){
    Party party;
    Cli interface(argLen, args);
    createTheGuys(party);
    std::cout << std::endl;
    std::cout << "Hello, from Gdungeon!" << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    return 0;
}