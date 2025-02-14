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

/*************************************************************//**
* \brief The Entity class is used to represent Players and Enimies
* 
* A small class that holds the hp, hpcap, and id of an Entity, 
* and some simple functions to retrieve and set these variables.
* 
*****************************************************************/
class Entity {
    public:
        //! Default Constructor
        Entity() :  hp{5},
                    hpCap{5},
                    name{""},
                    id{""} {};
        
        /// @brief The standard Entity constructor. 
        /// Sets an int hp, and strings name and id.
        Entity(int newHp, 
               const std::string& newName,
               const std::string& newID) :
                    hp(newHp),
                    hpCap(newHp),
                    name(newName),
                    id(newID) {};
        
        /// @brief Returns the name of the Entity.
        /// @return a string containing the name of the Entity.
        std::string getName() const;

        /// @brief Sets the name of the Entity.
        /// @param newName A string containing 
        /// the new name to be set.
        inline void setName(const std::string& newName);

        /// @brief Returns the ID of the Entity.
        /// @return a string containing the ID of the Entity.
        std::string getID() const;

        /// @brief Sets the ID of the Entity
        /// @param newID A string containing 
        /// the new ID to be set.
        inline void setID(const std::string&);
        
        /// @brief Returns the hp of an Entity.
        /// @return An int containg the hp of the Entity.
        inline int getHp() const;

        /// @brief Returns the hpCap of an Entity.
        /// @return An int containg the hpCap of the Entity.
        inline int getHpCap() const;

        /// @brief Reduce Entity's HP
        /// @param damageAmt amount of damage to reduce HP by.
        void damage(int damageAmt);

        /// @brief increase Entity's HP
        /// @param damageAmt amount of damage to increase HP by.
        void heal(int healAmt);
    protected:
        std::string name;
        std::string id;
        int hp;
        int hpCap;
};

/*************************************************************//**
* \brief The class that will represent and be controlled by the 
* player.
* 
* This class inherits Entity and adds some player specific 
* aspects.
*****************************************************************/
class Player : public Entity {
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

        /// @brief Returns the Players Energy
        /// @return int representing players Energy
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


class Enemy : public Entity {

};


template<class T>
struct ptrIterator {
    using iterator_category 
        = std::contiguous_iterator_tag;
    using difference_type = std::ptrdiff_t; 
    using value_type = T*;
    using pointer = T**;
    using reference = T*&;
    public:
        ptrIterator() : m_ptr{nullptr} {};
        ptrIterator(pointer ptr) : m_ptr(ptr) {};

        inline reference operator*() const {
            return *m_ptr;
        }
        inline pointer operator->() {return m_ptr;}

        inline reference operator[]
                (difference_type rhs) {
            return m_ptr[rhs];
        }

        inline ptrIterator<T>& operator++() {
            ++m_ptr; 
            return *this;
        }
        inline ptrIterator<T> operator++(int) {
            ptrIterator<T> tmp = *this; 
            ++m_ptr;
            return tmp; 
        }

        inline ptrIterator<T>& operator--() {
            --m_ptr; 
            return *this;
        }
        inline ptrIterator<T> operator--(int) {
            ptrIterator<T> tmp = *this; 
            --m_ptr;
            return tmp; 
        }

        inline ptrIterator<T> operator+
                (difference_type rhs) {
            return ptrIterator<T>(m_ptr + rhs);
        }

        inline ptrIterator<T> operator-
                (difference_type rhs) {
            return ptrIterator<T>(m_ptr - rhs);
        }

        inline friend ptrIterator<T> operator+
                (difference_type lhs, ptrIterator<T>& rhs) {
            return ptrIterator<T>(rhs.m_ptr + lhs);
        };

        inline friend ptrIterator<T> operator-
                (difference_type lhs, ptrIterator<T>& rhs) {
            return ptrIterator<T>(rhs.m_ptr - lhs);
        };

        inline friend bool operator==
            (const ptrIterator<T>& a, const ptrIterator<T>& b) {
                return a.m_ptr == b.m_ptr;
        };

        inline friend bool operator!=
            (const ptrIterator<T>& a, const ptrIterator<T>& b) {
                return a.m_ptr != b.m_ptr;
        };

        friend class Party;


    private:
        pointer m_ptr;
};


class Party : ptrIterator<Player>{
    public:
        using iterator = ptrIterator<Player>;

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

        inline void swap(iterator&, 
                         iterator&);

        
        iterator erase(iterator);
        iterator erase(iterator, 
                       iterator);
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

class EnemyParty : ptrIterator<Enemy> {
    public:
        using iterator = ptrIterator<Enemy>;
    
};

#endif