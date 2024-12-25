#include <iostream>
#include <random>
#include <memory>
#include "classes.hpp"

auto rng = std::default_random_engine {};

void createTheGuys(Party& party) {
    
    party.addMember(new Player(25, "Rudolph"));
    party.addMember(new Player(22, "Mikey"));
    party.addMember(new Player(18, "“Slick”"));
    party.addMember(new Player(20, "Dr. Victor"));
}

int main(int, char**){
    Party party;
    createTheGuys(party);
    std::cout << "Hello, from Gdungeon!" << std::endl;
}
