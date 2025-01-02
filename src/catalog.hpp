#ifndef CATALOG_H
#define CATALOG_H

#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <sqlite3.h>


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

struct Card {
    public:
        std::string name;
        std::string cardID;
        std::string tagline;
        std::string description;
        std::bitset<4> position;
        bool barrage{false};
        bool team{false};
        int energyCost{0};
        bool token{false};
        int tokenCost{2};
        int damage{0};
        int copies{1};
        int repeat{0};
        bool exhaust;
        std::string tokenName;
        std::vector<Effect> EffectList;
        ~Card(){}
};

struct EnemyCard {

};

struct Intention {
    EnemyCard Type;
    bool hasPosition{true};
    std::bitset<4> position;
    bool barrage{false};
};

class Catalog {
    public:
        Card* getCardByID(const std::string&);
    private:
        Card* searchForID(const std::string&);
        Card* createCard(const std::string&);
        std::map<std::string, Card*> cardMap;
        ~Catalog();
};

#endif