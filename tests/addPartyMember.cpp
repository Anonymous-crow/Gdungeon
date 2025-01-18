#include <iostream>
#include <classes.hpp>
#include <catalog.hpp>


int main(int argLen, char** args){
    Catalog cardCatalog;
    Party party(&cardCatalog);
    party.addMember(25, "Rudolph", "rudy");
    party.addMember(22, "Mikey", "mikey");
    party.addMember(18, "“Slick”", "slick");
    party.addMember(new Player(20, 
                               "Dr. Victor", 
                               "victor", 
                               &cardCatalog));
    std::cout << "Number of players: " << party.size() << std::endl;
    for (Player* e : party) {
        std::cout << e->getID() << " ";
    }
    std::cout << std::endl;
}