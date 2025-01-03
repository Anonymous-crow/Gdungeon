#include "catalog.hpp"
#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <stdexcept>
#include <sqlite3.h>

Catalog::~Catalog() {
    std::map<std::string, Card*>::iterator i;
    for (i = cardMap.begin(); i != cardMap.end(); i++) {
        delete i->second;
    }
    cardMap.clear();
}

Card* Catalog::getCardByID(const std::string& cardID) {
    Card* resp = searchForID(cardID);
    if (resp == nullptr) {
        return createCard(cardID);
    }
    return resp;
}

Card* Catalog::searchForID(const std::string& cardID) {
    std::map<std::string, Card*>::iterator i;
    for (i = cardMap.begin(); i != cardMap.end(); i++) {
        if (cardID == i->first) {
            return i->second;
        }
    }
    return nullptr;
}

Card* Catalog::createCard(const std::string& cardID) {
    return 0;
}