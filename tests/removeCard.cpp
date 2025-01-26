#include <iostream>
#include <classes.hpp>
#include <catalog.hpp>

int main(int argLen, char** args){
    Catalog cardCatalog;
    Party party(&cardCatalog);
    party.addMember(25, "Rudolph", "rudy");
    party.addMember(22, "Mikey", "mikey");
    party.addMember(18, "“Slick”", "slick");
    party.addMember(20, "Dr. Victor", "victor");

    bool test1{false}, test2{false}, test3{false};

    int punchCopies = cardCatalog.getCopiesByID("rudy_Punch");
    int blockCopies = cardCatalog.getCopiesByID("rudy_Block");
    party[0]->addCardToDeck("rudy_Punch", punchCopies);
    party[0]->addCardToDeck("rudy_Block", blockCopies);
    party[0]->addCardToDeck("rudy_Cover", 1);

    std::map<std::string, int> contents = party[0]->getDeckContents();
    punchCopies = contents["rudy_Punch"];

    party[0]->removeCardFromDeck("rudy_Punch", 2);

    contents = party[0]->getDeckContents();
    if (punchCopies - 2 == contents["rudy_Punch"]) test1 = true;
    else std::cout << "Test 1 Failed" << std::endl;
    int size = static_cast<int>(contents.size());

    party[0]->removeCardFromDeck("rudy_Cover", 2);

    contents = party[0]->getDeckContents();
    if (static_cast<int>(contents.size()) == size - 1) 
        test2 = true;
    else std::cout << "Test 2 Failed" << std::endl;

    cardCatalog.clearUnused(party[0]);
    if (!cardCatalog.isCached("rudy_Cover")) test3 = true;
    else std::cout << "Test 3 Failed" << std::endl;

    if (test1 && test2 && test3) std::cout 
        << "All Tests Passed" << std::endl;
}