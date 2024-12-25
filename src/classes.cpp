#include "classes.hpp"
#include <string>
#include <map>
#include <iostream>

int Player::getHp() const {
    return hp;
}

int Player::getEnergy() const {
    return energy;
}

int Player::getHandSize() const {
    return hand.size();
}

int Player::getDeckSize() const {
    return deck.size();
}

void Player::damage(int damageAmt) {
    hp -= damageAmt;
    if (hp < 0) {
        hp = 0;
    }
}

void Player::heal(int healAmt) {
    hp += healAmt;
    if (hp > hpCap) {
        hp = hpCap;
    }
}

Player::~Player() {
    for (auto& e : hand) {
        delete e;
    }
    hand.clear();
    for (auto& e : deck) {
        delete e;
    }
    deck.clear();
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

int Party::partyLegnth() {
    return partyList.size();
}

Party::~Party() {
    for (int i{0}; i < 4; i++) {
        delete partyList[i];
        partyList[i] = nullptr;
    }
}

