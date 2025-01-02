#include "catalog.hpp"
#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <stdexcept>
#include <sqlite3.h>

Catalog::~Catalog() {
    for (auto& [key, val] : cardMap) {
        delete val;
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
    for (auto const& [key, val] : cardMap) {
        if (cardID == key) {
            return val;
        }
    }
    return nullptr;
}

Card* Catalog::createCard(const std::string& cardID) {
    return 0;
}