#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <vector>
#include <random>
#include <array>

#include "catalog.hpp"


class Entity {
    public:
        Entity() :  hp{5},
                    hpCap{5},
                    name{""} {}

        Entity(int newHp, const std::string& newName) :
                    hp(newHp),
                    hpCap(newHp),
                    name{newName} {}
        
        std::string getName() const;
        void setName(const std::string&);

        int getHp() const;
        void damage(int);
        void heal(int);
    protected:
        std::string name;
        int hp;
        int hpCap;
};


class Player: public Entity {
    public:
        Player() :  energy{3}, 
                    handsize{5},
                    Entity()
                    {}

        Player(int newHp, const std::string& newName) :
                            energy{3}, 
                            handsize{5},
                            Entity(newHp, newName)
                            {}

        int getEnergy() const;
        int getHandSize() const;
        int getDeckSize() const;
        
        int addCardToDeck(const Card&);

        ~Player();

    private:
        int energy;
        int handsize;
        std::vector<Card*> deck;
        std::vector<Card*> draw;
        std::vector<Card*> hand;
        std::vector<Card*> discard;
};


class Party {
    public:
        ~Party();
        void addMember(Player*);
        void addMember(const std::string&, int);
        int size();
    private:
        std::array<Player*, 4> partyList{nullptr};
};


#endif