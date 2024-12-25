#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <map>
#include <bitset>
#include <vector>
#include <random>
#include <array>


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
        std::string tokenName;
        std::vector<Effect> EffectList;
        ~Card() {}
};

class Player {
    public:
        Player() :  hp{5}, 
                    hpCap{5}, 
                    energy{3}, 
                    handsize{5},
                    name{""} {}

        Player(int newHp, const std::string& newName) 
                        :   hp(newHp),
                            hpCap(hp),
                            name(newName), 
                            energy{3}, 
                            handsize{5} {}

        int getHp() const;
        int getEnergy() const;
        int getHandSize() const;
        int getDeckSize() const;
        std::string getName() const;
        
        void setName(const std::string&);
        void damage(int);
        void heal(int);
        int addCardToDeck(const Card&);

        ~Player();

    private:
        std::string name;
        int hp;
        int hpCap;
        int energy;
        int handsize;
        std::vector<Card*> hand;
        std::vector<Card*> deck;
};

class Party {
    public:
        ~Party();
        void addMember(Player*);
        void addMember(const std::string&, int);
        int partyLegnth();
    private:
        std::array<Player*, 4> partyList{nullptr};
};

// #include "classes.cpp"


#endif