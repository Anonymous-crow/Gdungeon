#include "classes.hpp"
#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <sstream>

#define MELEE 1
#define RANGED 2
#define SUMMONER 4
#define SUPPORT 8

std::string Entity::getName() const {
    return name;
}

std::string Entity::getID() const {
    return id;
}

inline int Entity::getHp() const {
    return hp;
}

inline int Entity::getHpCap() const {
    return hpCap;
}

inline void Entity::setName(const std::string& newName) {
    name = newName;
}

inline void Entity::setID(const std::string& newID) {
    name = newID;
}

void Entity::damage(int damageAmt) {
    hp -= damageAmt;
    if (hp < 0) {
        hp = 0;
    }
}

void Entity::heal(int healAmt) {
    hp += healAmt;
    if (hp > hpCap) {
        hp = hpCap;
    }
}

int Player::getEnergy() const {
    return energy;
}

int Player::getHandSize() const {
    return static_cast<int>(hand.size());
}

int Player::getDeckSize() const {
    return static_cast<int>(deck.size());
}

std::string Player::getDeckString() const {
    std::ostringstream out;
    std::map<std::string, int> deckMap;
    for (Card* e : deck) {
        ++deckMap[e->name];
    }
    bool first{false};
    for (std::map<std::string, int>::const_iterator 
         i = deckMap.begin(); 
         i != deckMap.end(); 
         ++i) {
        out << "\t   ";
        if (!first) {
            out << "- ";
            first = true;
        } else {
            out << "  ";
        }
        out << i->first;
        if (i->second > 1) {
            out << " x" << i->second;
        }
        out << "\n";
    }
    return out.str();
}

std::map<std::string, int> Player::getDeckContents() const {
    std::map<std::string, int> deckMap;
    for (Card* e : deck) {
        ++deckMap[e->cardID];
    }
    return deckMap;
}

void Player::addCardToDeck(Card* newCard) {
    int found{0};
    for (Card* e : deck) {
        if (e == newCard) {
            ++found;
        }
    }
    if (found == 0) {
        deck.push_back(newCard);
    } else if (newCard->copies > found) {
        deck.push_back(newCard);
    }
}

void Player::addCardToDeck(const std::string& cardID, int copies) {
    int found{0};
    Card* newCard = nullptr;
    for (Card* e : deck) {
        if (e->cardID == cardID) {
            ++found;
            newCard = e;
        }
    }
    if (found == 0) {
        newCard = cardCatalogPtr->getCardByID(cardID);
        if (newCard != nullptr) {
            for (int i{0}; i < newCard->copies && i < copies; ++i) {
                deck.push_back(newCard);
            }
        } else {
            std::cerr << "Could not load card "
            << cardID << "! Card not found."
            << std::endl;
        }
    } else if (newCard->copies > found) {
        for (int i{0}; 
             found < newCard->copies && i < copies; 
             ++i) {
            deck.push_back(newCard);
            ++found;
        }
    }
}

void Player::removeCardFromDeck(const std::string& cardID, 
                                int toRemove) {
    int deleted{0};
    std::list<Card*>::iterator i = deck.begin();
    while(deleted < toRemove && i != deck.end()) {
        if (cardID == (*i)->cardID) {
            i = deck.erase(i);
            ++deleted;
        } else {
            ++i;
        }
    }
} 

Player::~Player() {
    deck.clear();
    draw.clear();
    hand.clear();
    discard.clear();
    cardCatalogPtr->clearUnused(this);
}

Player* const Party::operator[](size_t index) const {
    if (index >= size() || index < 0) {
        throw std::out_of_range("Index is out of range.");
    }
    return partyList[index];
}

void Enemy::initIntentionDeck() {
	std::cout << "Range:\n";
	std::uint8_t mask{1};
	for (std::uint8_t i{0}; i < 4; ++i) {
		switch(rangeType & mask) {
			case MELEE:
				std::cout << "\tMELEE\n";
				break;
			case RANGED:
				std::cout << "\tRANGED\n";
				break;
			case SUMMONER:
				std::cout << "\tSUMMONER\n";
				break;
			case SUPPORT:
				std::cout << "\tSUPPORT\n";
				break;
		}
		mask <<= 1;
	}
}


Party::iterator Party::begin() {
    return iterator(&partyList[0]);
}

Party::iterator Party::end() {
    int nullPos{0};
    for (int i{0}; i < 4; ++i) {
        if (partyList[i] == nullptr) {
            break;
        } else {
            ++nullPos;
        }
    }
    return iterator(&partyList[nullPos]);
}

inline void Party::swap(iterator& a, iterator& b) {
    Player* tmp = *a;
    *a = *b;
    *b = tmp;
}

Party::iterator Party::erase(Party::iterator it) {
    Party::iterator end = this->end();
    if (it == end) {
        return end;
    }
    delete *it;
    *it = nullptr;
    Party::iterator nullIt = it++;
    if (it == end) {
        return nullIt;
    }
    while (it != end) {
        if (*it != nullptr) {
            *nullIt = *it;
            *it = nullptr;
            ++nullIt;
        }
        ++it;
    }
    return --nullIt;
}

Party::iterator Party::erase(Party::iterator itStart, 
                             Party::iterator itEnd) {
    if (itStart == itEnd) {
        return itStart;
    }
    Party::iterator end = this->end();
    if (itStart == end) {
        return end;
    }
    for (Party::iterator it = itStart; 
         (it != itEnd) && (it != end); 
         ++it) {
        delete *it;
        *it = nullptr;
    }
    if (itEnd == end) {
        return itStart;
    }
    Party::iterator current = itStart;
    while (*itEnd != *end) {
        if (*itEnd != nullptr) {
            *current = *itEnd;
            *itEnd = nullptr;
            ++current;
        }
        ++itEnd;
    }
    return itStart;
}

Party::iterator Party::remove(const std::string& id, int limit) {
    return remove(begin(), end(), id, limit);
}

Party::iterator Party::remove(iterator itb,
                              iterator ite,
                              const std::string& id,
                              int limit) {
    int i{0};
    while (itb != ite && i < limit) {
        if ((*itb)->getID() == id) {
            itb = erase(itb);
            --ite;
            ++i;
        } else {
            ++itb;
        }
    }
    return ite;
}

void Party::addMember(Player* newPlayer) {
    bool idNotFound{true};
    int nullPos{4};
    for (int i{3}; i > -1; --i) {
        if (partyList[i] != nullptr) {
            if(partyList[i]->getID() == newPlayer->getID()) {
                idNotFound = false;
                break;
            }
        } else {
            nullPos = i;
        }
    }
    if (idNotFound && nullPos < 4) {
        partyList[nullPos] = newPlayer;
    }
}

void Party::addMember(int newHp, 
                      const std::string& newName,
                      const std::string& newID) {
    bool idNotFound{true};
    int nullPos{4};
    for (int i{3}; i > -1; --i) {
        if (partyList[i] != nullptr) {
            if(partyList[i]->getID() == newID) {
                idNotFound = false;
                break;
            }
        } else {
            nullPos = i;
        }
    }
    if (idNotFound && nullPos < 4) {
        partyList[nullPos] = new Player(newHp, 
                                          newName, 
                                          newID,
                                          cardCatalogPtr);
    }
}

size_t Party::size() const{
    int resp{0}; 
    for (int i{0}; i < 4; ++i){
        if (partyList[i] != nullptr) {
            ++resp;
        };
    };
    return resp;
}

std::string Party::print() {
    std::ostringstream out;
    std::ostringstream cardOut;
    out << "Members:\n";
    for (Player* e : partyList) {
        if (e != nullptr) {
            out << "\t* " << e->getName() << "\n"
            << e->getDeckString();
        }
    }
    return out.str();
}

Party::~Party() {
    for (int i{0}; i < 4; ++i) {
        delete partyList[i];
        partyList[i] = nullptr;
    }
}

