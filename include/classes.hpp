#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <vector>
#include <list>
#include <random>
#include <iterator>
#include <cstddef>

#include <catalog.hpp>

class Catalog;
struct Card;
struct Effect;


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
                iterator() : m_ptr{nullptr} {};
                iterator(pointer ptr) : m_ptr(ptr) {};

                inline reference operator*() const {
                    return *m_ptr;
                }
                inline pointer operator->() {return m_ptr;}

                inline reference operator[]
                        (difference_type rhs) {
                    return m_ptr[rhs];
                }

                inline iterator& operator++() {
                    ++m_ptr; 
                    return *this;
                }
                inline iterator operator++(int) {
                    iterator tmp = *this; 
                    ++m_ptr;
                    return tmp; 
                }

                inline iterator& operator--() {
                    --m_ptr; 
                    return *this;
                }
                inline iterator operator--(int) {
                    iterator tmp = *this; 
                    --m_ptr;
                    return tmp; 
                }

                inline iterator operator+
                        (difference_type rhs) {
                    return iterator(m_ptr + rhs);
                }

                inline iterator operator-
                        (difference_type rhs) {
                    return iterator(m_ptr - rhs);
                }

                inline friend iterator operator+
                        (difference_type lhs, iterator& rhs) {
                    return iterator(rhs.m_ptr + lhs);
                };

                inline friend iterator operator-
                        (difference_type lhs, iterator& rhs) {
                    return iterator(rhs.m_ptr - lhs);
                };

                inline friend bool operator==
                    (const iterator& a, const iterator& b) {
                        return a.m_ptr == b.m_ptr;
                };

                inline friend bool operator!=
                    (const iterator& a, const iterator& b) {
                        return a.m_ptr != b.m_ptr;
                };

                friend class Party;


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
        size_t size() const;
        Player* const operator[](size_t) const;
        std::string print();

        iterator begin();
        iterator end();

        inline void swap(iterator&, iterator&);

        
        iterator erase(iterator);
        iterator erase(iterator, iterator);
        iterator remove(const std::string&, int = 1);
        iterator remove(iterator, 
                        iterator, 
                        const std::string&,
                        int = 1);

    private:
        Player* partyList[4] {nullptr, 
                              nullptr, 
                              nullptr, 
                              nullptr};
        Catalog* cardCatalogPtr{nullptr};
};


#endif