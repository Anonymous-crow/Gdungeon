#include "classes.hpp"
#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <sstream>

std::string Entity::getName() const {
    return name;
}

std::string Entity::getID() const {
    return id;
}

int Entity::getHp() const {
    return hp;
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
    // for (auto& e : deck) {
    //     delete e;
    // }
    deck.clear();
    // for (auto& e : draw) {
    //     delete e;
    // }
    draw.clear();
    // for (auto& e : hand) {
    //     delete e;
    // }
    hand.clear();
    // for (auto& e : discard) {
    //     delete e;
    // }
    discard.clear();
}

Player* const Party::operator[](int index) const {
    if (index >= size() || index < 0) {
        throw std::out_of_range("Index is out of range.");
    }
    return partyList[index];
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

int Party::size() const{
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
            out << "\t* " << e->getName() << "\n";
            out << e->getDeckString();
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

