#ifndef CARD_H
#define CARD_H

#include <string>
#include <bitset>
#include <vector>

enum EffectType {
    SHIELD,
    STRENGTH,
    FORTITUDE,
    GLINT,
    IMMUNITY,
    TAUNT,
    WEAK,
    VULNERABLE,
    STUN,
};

struct Effect {
    EffectType name;
    int stack;
};

class Card {
    public:
        std::string name;
        std::string tagline;
        std::string description;
        std::bitset<4> position;
        bool barrage{false};
        bool team{false};
        int energyCost;
        bool token{false};
        int tokenCost{2};
        int damage{0};
        std::string tokenName;
        std::vector<Effect> EffectList;
};

#endif