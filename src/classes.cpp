#include "classes.hpp"
#include <string>
#include <map>
#include <iostream>

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

Player::~Player() {
    for (auto& e : deck) {
        delete e;
    }
    deck.clear();
    for (auto& e : draw) {
        delete e;
    }
    draw.clear();
    for (auto& e : hand) {
        delete e;
    }
    hand.clear();
    for (auto& e : discard) {
        delete e;
    }
    discard.clear();
}


void Party::addMember(Player* newPlayer) {
    for (int i{0}; i < 4; i++) {
        if (partyList[i] == nullptr) {
            partyList[i] = newPlayer;
            break;
        }
    }
}

void Party::addMember(const std::string& newName, int newHp) {
    for (int i{0}; i < 4; i++) {
        if (partyList[i] == nullptr) {
            partyList[i] = new Player(newHp, newName);
            break;
        }
    }
}

int Party::size() {
    int resp{0}; 
    for (auto e: partyList){
        if (e != nullptr) {
            resp++;
        };
    };
    return resp;
}

Party::~Party() {
    for (int i{0}; i < 4; i++) {
        delete partyList[i];
        partyList[i] = nullptr;
    }
}

