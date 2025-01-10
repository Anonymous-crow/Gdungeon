#ifndef CATALOG_H
#define CATALOG_H

#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <sqlite3.h>


enum EffectType {
    SHIELD,
    STRENGTH,
    FORTITUDE,
    GLINT,
    IMMUNITY,
    TAUNT,
    WEAK,
    VULNERABLE,
    STUN,
};

struct Effect {
    EffectType name;
    int stack;
};

struct Card {
    public:
        Card(char **argv)
             :  name(argv[0]),
                cardID(argv[1]),
                tagline(argv[2]),
                description(argv[3]),
                position(argv[4]),
                barrage(argv[5] == "1"),
                team(argv[6] == "1"),
                energyCost(std::stoi(argv[7])),
                token(argv[8] == "1"),
                tokenCost(std::stoi(argv[9])),
                tokenType(argv[10]),
                damage(std::stoi(argv[11])),
                copies(std::stoi(argv[12])),
                repeat(std::stoi(argv[13])),
                exhaust(argv[14] == "1")
                {};
        
        Card(const char* newName,
             const char* newID,
             const char* newTagline,
             const char* newDescription,
             const std::bitset<4>& newPosition,
             bool newBarrage,
             bool newTeam,
             int newEnergyCost,
             bool newToken,
             int newTokenCost,
             const char* newTokenType,
             int newDamage,
             int newCopies,
             int newRepeat,
             bool newExhaust,
             const std::vector<Effect>& newEffectList) 
             :  name(newName),
                cardID(newID),
                tagline(newTagline),
                description(newDescription),
                position(newPosition),
                barrage(newBarrage),
                team(newTeam),
                energyCost(newEnergyCost),
                token(newToken),
                tokenCost(newTokenCost),
                tokenType(newTokenType),
                damage(newDamage),
                copies(newCopies),
                repeat(newRepeat),
                exhaust(newExhaust),
                EffectList(newEffectList)
                {};
        std::string name;
        std::string cardID;
        std::string tagline;
        std::string description;
        std::bitset<4> position;
        bool barrage{false};
        bool team{false};
        int energyCost{0};
        bool token{false};
        int tokenCost{2};
        int damage{0};
        int copies{1};
        int repeat{0};
        bool exhaust;
        std::string tokenType;
        std::vector<Effect> EffectList;
};

struct EnemyCard {

};

struct Intention {
    EnemyCard Type;
    bool hasPosition{true};
    std::bitset<4> position;
    bool barrage{false};
};

class Catalog {
    public:
        Card* getCardByID(const std::string&);
        Catalog();
        ~Catalog();
    private:
        Card* searchForID(const std::string&) const;
        Card* createCard(const std::string&);
        std::map<std::string, Card*> cardMap;
        void openDB();
        void closeDB();
        sqlite3* db;
    friend int callback(void *, int, char **, char **);
};

#endif