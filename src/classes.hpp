#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <map>
#include <vector>
#include <random>

#include "card.hpp"

class Player {
    public:
        int getHp() const;
        void damage();
        void heal();
        int getEnergy() const;
        int handsize{5};
    private:
        int hp;
        int energy{3};
        std::vector<Card> hand;
        std::vector<Card> deck;
};


#endif