#ifndef ARENA_H
#define ARENA_H
#include <catalog.hpp>
#include <classes.hpp>

#include <queue>

class Player;
class Party;
class EnemyParty;
class EntityParty;
class Catalog;
struct Card;
struct Effect;

class Arena {
    public:
        Arena(Party* newParty) : playerParty(newParty) {}
        inline void setOpps(EnemyParty*);
        inline EnemyParty* getOppParty();
        inline void queueOpps (EnemyParty*);
    private:
        std::queue<EnemyParty*> partyQueue;
        Party* playerParty{nullptr};
        EnemyParty* oppParty{nullptr};
        EntityParty* dummyParty{nullptr};
};

#endif