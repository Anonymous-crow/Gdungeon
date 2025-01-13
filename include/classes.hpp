#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <vector>
#include <list>
#include <random>
#include <array>
#include <iterator>
#include <cstddef>

#include <catalog.hpp>

class Catalog;
struct Card;


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
        struct iterator {
            using iterator_category 
                = std::contiguous_iterator_tag;
            using difference_type = std::ptrdiff_t; 
            using value_type = Player*;
            using pointer = Player**;
            using reference = Player*&;
            public:
                iterator(pointer ptr) : m_ptr(ptr) {};

                reference operator*() const {return *m_ptr;}
                pointer operator->() {return m_ptr;}

                iterator& operator++() {
                    ++m_ptr; 
                    return *this;
                    }
                iterator operator++(int) {
                    iterator tmp = *this; 
                    ++m_ptr;
                    return tmp; 
                }

                friend bool operator==
                    (const iterator& a, const iterator& b) {
                        return a.m_ptr == b.m_ptr;
                };

                friend bool operator!=
                    (const iterator& a, const iterator& b) {
                        return a.m_ptr != b.m_ptr;
                };


            private:
                pointer m_ptr;
        };
        
        Party(Catalog* newCardCatalogPtr) 
            :    cardCatalogPtr(newCardCatalogPtr) {};
        ~Party();
        void addMember(Player*);
        void addMember(int, 
                const std::string&, 
                const std::string&);
        int size() const;
        Player* const operator[](int) const;
        std::string print();

        iterator begin();
        iterator end();

    private:
        Player* partyList[4] {nullptr, 
                              nullptr, 
                              nullptr, 
                              nullptr};
        Catalog* cardCatalogPtr{nullptr};
};


#endif