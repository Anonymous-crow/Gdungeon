#include <iostream>
#include <random>
#include <memory>
#include <string>
#include <classes.hpp>
#include <catalog.hpp>
#include <cli.hpp>

auto rng = std::default_random_engine {};

void createTheGuys(Party& party) {
    
    party.addMember(25, "Rudolph", "rudy");
    party.addMember(22, "Mikey", "mikey");
    party.addMember(18, "“Slick”", "slick");
    party.addMember(20, "Dr. Victor", "victor");
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
    party[0]->addCardToDeck("wingledonger");
    party[0]->addCardToDeck("rudy_Block", 7);
    party[0]->addCardToDeck("rudy_Cover", 7);
    party[0]->removeCardFromDeck("rudy_Punch", 2);
    party[0]->removeCardFromDeck("rudy_Cover", 2);
    // cardCatalog.clearUnused(party[0]);
    party.erase((party.begin() + 2));
    party.addMember(18, "“Slick”", "slick");
    cardCatalog.clearUnused(&party);
    party.erase(party.begin(), (party.begin() + 1));
    std::cout << std::endl;
    std::cout << "Hello, from Gdungeon!\n" 
    << party.print() << std::endl;
    // std::cout << cardCatalog.getUnusedCards() << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    return 0;
}