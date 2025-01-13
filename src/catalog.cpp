#include "catalog.hpp"
#include <string>
#include <bitset>
#include <list>
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

int Catalog::getCopiesByID(const std::string& cardID) {
    Card* resp = searchForID(cardID);
    if (resp == nullptr) {
        return searchForCopies(cardID);
    }
    return resp->copies;
}

Card* Catalog::searchForID(const std::string& cardID) const {
    std::map<std::string, Card*>::const_iterator 
        it = cardMap.find(cardID);
    if (it != cardMap.cend())
        return it->second;
    return nullptr;
}

int intCallBack(void *intPtr, 
             int argc, 
             char **argv, 
             char **azColName){
    *reinterpret_cast<int*>(intPtr) = std::stoi(argv[0]);
    return 0;
}

int stringListCallback(void *listPtr, 
             int argc, 
             char **argv, 
             char **azColName){
    reinterpret_cast<std::list<std::string>*>(listPtr)
        ->push_back(argv[0]);
    return 0;
}

int effectListCallback(void *listPtr, 
             int argc, 
             char **argv, 
             char **azColName){
    for (int i{1}; i < argc; ++i) {
        int stack = std::stoi(argv[i]);
        if (stack != 0) {
            reinterpret_cast<std::list<Effect>*>(listPtr)
                ->push_back(
                    Effect(EffectType(i), stack));
        }
    }
    return 0;
}

int cardCallback(void *CatalogObj, 
             int argc, 
             char **argv, 
             char **azColName){
    reinterpret_cast<Catalog*>(CatalogObj)->cardMap[argv[1]] 
            = new Card(argv);
    return 0;
}

int Catalog::searchForCopies(const std::string& cardID) const {
    char* errmsg = nullptr;
    char* sqlQuery = sqlite3_mprintf(
                    "SELECT copies FROM playerCards WHERE cardID = \"%q\";", 
                    cardID.c_str());
    int copies{0};
    int rc = sqlite3_exec(db, 
                          sqlQuery,
                          intCallBack, 
                          reinterpret_cast<void*>(&copies), 
                          &errmsg);

    sqlite3_free(sqlQuery);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        throw std::runtime_error(errmsg);
        sqlite3_free(errmsg);
    }
    return copies;
}

Card* Catalog::createCard(const std::string& cardID) {
    
    char* errmsg = nullptr;
    char* sqlQuery = sqlite3_mprintf(
                    "SELECT * FROM playerCards WHERE cardID = \"%q\";", 
                    cardID.c_str());
    int rc = sqlite3_exec(db, 
                          sqlQuery,
                          cardCallback, 
                          reinterpret_cast<void*>(this), 
                          &errmsg);

    sqlite3_free(sqlQuery);

    Card* resp = searchForID(cardID);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        throw std::runtime_error(errmsg);
        sqlite3_free(errmsg);
    }

    sqlQuery = sqlite3_mprintf(
                    "SELECT * FROM cardEffects WHERE cardID = \"%q\";", 
                    cardID.c_str());

    rc = sqlite3_exec(db, 
                          sqlQuery,
                          effectListCallback, 
                          reinterpret_cast<void*>(&(resp->EffectList)), 
                          &errmsg);

    sqlite3_free(sqlQuery);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        throw std::runtime_error(errmsg);
        sqlite3_free(errmsg);
    }

    return resp;
}

std::list<std::string> Catalog::searchForPlayerCards
            (const std::string& playerID) const {
    char* errmsg = nullptr;
    char* sqlQuery = sqlite3_mprintf(
            "SELECT cardID FROM playerCards WHERE cardID LIKE \"%q_%%\";", 
            playerID.c_str());
    std::list<std::string> cardList;
    int rc = sqlite3_exec(db, 
                          sqlQuery,
                          stringListCallback, 
                          reinterpret_cast<void*>(&cardList), 
                          &errmsg);

    sqlite3_free(sqlQuery);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        throw std::runtime_error(errmsg);
        sqlite3_free(errmsg);
    }
    return cardList;
}

std::list<std::string> Catalog::searchForAllCards() const {
    char* errmsg = nullptr;
    std::list<std::string> cardList;
    int rc = sqlite3_exec(db, 
                          "SELECT cardID FROM playerCards;",
                          stringListCallback, 
                          reinterpret_cast<void*>(&cardList), 
                          &errmsg);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        throw std::runtime_error(errmsg);
        sqlite3_free(errmsg);
    }
    return cardList;
}

void Catalog::clearUnused(Party* partyClear) {
    for (Player* e : *partyClear) {
        clearUnused(e);
    }
}

void Catalog::clearUnused(const Player* playerClear) {
    std::map<std::string, int> inUse = playerClear->getDeckContents();
    std::list<std::string> totalPlayerCards = searchForPlayerCards
                (playerClear->getID());
    for (std::string e : totalPlayerCards) {
        std::map<std::string, int>::iterator it = inUse.find(e);
        if (it == inUse.end()) {
            std::map<std::string, Card*>::iterator 
                jt = cardMap.find(e);
            if (jt != cardMap.end()) {
                delete jt->second;
                cardMap.erase(jt);
            }
        }
    }
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
    #ifdef _WIN32
    std::string FUCKWINDOWS = filename.string();
    const char * filepathCStringBecauseWindowsIsAnOperatingSystem = FUCKWINDOWS.c_str();
    int rc = sqlite3_open(filepathCStringBecauseWindowsIsAnOperatingSystem, 
                          &db);
    #else 
    int rc = sqlite3_open(filename.c_str(), &db);
    #endif

    if (rc)
    {

        char errmsg[] = "Can't open database: ";
        std::strcat(errmsg, sqlite3_errmsg(db));
        throw std::runtime_error(errmsg);
    }
}

void Catalog::closeDB()
{

    sqlite3_close(db);
    db = nullptr;
}