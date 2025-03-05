#include <arena.hpp>

inline void Arena::setOpps(EnemyParty* newParty) {
	oppParty = newParty;
}

inline EnemyParty* Arena::getOppParty() {
	return oppParty;
}

inline void Arena::queueOpps(EnemyParty* newParty) {
	partyQueue.push(newParty);
}
