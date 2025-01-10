#include "catalog.hpp"
#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <stdexcept>
#include <cstring>
#include <filesystem>
#include <sqlite3.h>

namespace fs = std::filesystem;


Catalog::~Catalog() {
    std::map<std::string, Card*>::iterator i;
    for (i = cardMap.begin(); i != cardMap.end(); i++) {
        delete i->second;
    }
    cardMap.clear();
    closeDB();
}

Catalog::Catalog() {
    openDB();
}

Card* Catalog::getCardByID(const std::string& cardID) {
    Card* resp = searchForID(cardID);
    if (resp == nullptr) {
        return createCard(cardID);
    }
    return resp;
}

Card* Catalog::searchForID(const std::string& cardID) const {
    std::map<std::string, Card*>::const_iterator i;
    for (i = cardMap.begin(); i != cardMap.end(); i++) {
        if (cardID == i->first) {
            return i->second;
        }
    }
    return nullptr;
}

int callback(void *CatalogObj, int argc, char **argv, char **azColName){
        reinterpret_cast<Catalog*>(CatalogObj)->cardMap[argv[1]] = 
                                new Card(argv);
        return 0;
    }

Card* Catalog::createCard(const std::string& cardID) {
    
    char *errmsg = 0;
    Card* newCard;
    int rc = sqlite3_exec(db, 
                sqlite3_mprintf(
                    "SELECT * FROM playerCards WHERE cardID = \"%q\";", 
                    cardID.c_str()), 
                    callback, 
                    reinterpret_cast<void*>(this), 
                    &errmsg);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        throw std::runtime_error(errmsg);
        sqlite3_free(errmsg);
    }
    return searchForID(cardID);
}

void Catalog::openDB() {
    fs::path assets = fs::current_path() / "assets";
    fs::path filename = assets / "cards.db";
    // if (!fs::exists(assets))
    // {

    //     std::error_code ec;
    //     fs::create_directories(assets, ec);
    //     if (ec.value() != 0)
    //     {

    //         char errmsg[] = "Can't create folder: ";
    //         strcat(errmsg, ec.message().c_str());
    //         throw std::runtime_error(errmsg);
    //     }
    // }

    int rc = sqlite3_open(filename.c_str(), &db);

    if (rc)
    {

        char errmsg[] = "Can't open database: ";
        strcat(errmsg, sqlite3_errmsg(db));
        throw std::runtime_error(errmsg);
    }
}

void Catalog::closeDB()
{

    sqlite3_close(db);
    db = nullptr;
}