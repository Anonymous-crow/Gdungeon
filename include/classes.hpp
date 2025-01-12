#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <vector>
#include <list>
#include <random>
#include <array>

#include <catalog.hpp>

class Catalog;
class Card;


class Entity {
    public:
        Entity() :  hp{5},
                    hpCap{5},
                    name{""},
                    id{""} {}

        Entity(int newHp, 
               const std::string& newName,
               const std::string& newID) :
                    hp(newHp),
                    hpCap(newHp),
                    name(newName),
                    id(newID) {}
        
        std::string getName() const;
        void setName(const std::string&);
        std::string getID() const;
        void setID(const std::string&);

        int getHp() const;
        void damage(int);
        void heal(int);
    protected:
        std::string name;
        std::string id;
        int hp;
        int hpCap;
};


class Player: public Entity {
    public:
        Player(Catalog* newCardCatalogPtr) 
                :   energy{3}, 
                    handsize{5},
                    cardCatalogPtr(newCardCatalogPtr),
                    Entity()
                    {}

        Player(int newHp, 
               const std::string& newName,
               const std::string& newID, 
               Catalog* newCardCatalogPtr) :
                            energy{3}, 
                            handsize{5},
                            cardCatalogPtr(newCardCatalogPtr),
                            Entity(newHp, newName, newID)
                            {}

        int getEnergy() const;
        int getHandSize() const;
        int getDeckSize() const;
        std::string getDeckString() const;
        std::map<std::string, int> getDeckContents() const;
        
        void addCardToDeck(Card*);
        void addCardToDeck(const std::string&, int = 1);
        void removeCardFromDeck(const std::string&, int = 1);

        ~Player();

    private:
        int energy;
        int handsize;
        std::list<Card*> deck;
        std::vector<Card*> draw;
        std::vector<Card*> hand;
        std::vector<Card*> discard;
        Catalog* cardCatalogPtr{nullptr};
};


class Party {
    public:
        Party(Catalog* newCardCatalogPtr) 
            :    cardCatalogPtr(newCardCatalogPtr) {};
        ~Party();
        void addMember(Player*);
        void addMember(int, 
                const std::string&, 
                const std::string&);
        int size();
        Player* operator[](int);
        std::string print();
    private:
        std::array<Player*, 4> partyList{nullptr};
        Catalog* cardCatalogPtr{nullptr};
};


#endif