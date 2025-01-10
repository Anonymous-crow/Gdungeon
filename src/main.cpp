#include <iostream>
#include <random>
#include <memory>
#include <string>
#include "classes.hpp"
#include "catalog.hpp"
#include "cli.hpp"

auto rng = std::default_random_engine {};

void createTheGuys(Party& party) {
    
    party.addMember(25, "Rudolph");
    party.addMember(22, "Mikey");
    party.addMember(18, "“Slick”");
    party.addMember(20, "Dr. Victor");
}

int main(int argLen, char** args){
    Catalog cardCatalog;
    Party party(&cardCatalog);
    Cli interface(argLen, args);
    createTheGuys(party);
    party[0]->addCardToDeck("rudy_Punch");
    party[0]->addCardToDeck("rudy_Punch");
    party[0]->addCardToDeck("rudy_Punch");
    party[0]->addCardToDeck("rudy_Punch");
    party[0]->addCardToDeck("rudy_Punch");
    party[0]->addCardToDeck("rudy_Block");
    party[0]->addCardToDeck("rudy_Block", 7);
    party[0]->addCardToDeck("rudy_Cover", 7);
    std::cout << std::endl;
    std::cout << "Hello, from Gdungeon!" << std::endl;
    std::cout << party.print() << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    return 0;
}