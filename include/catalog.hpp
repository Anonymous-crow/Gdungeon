#ifndef CATALOG_H
#define CATALOG_H

#include <bitset>
#include <cstring>
#include <list>
#include <map>
#include <sqlite3.h>
#include <string>

#include <classes.hpp>

class Player;
class Party;

enum EffectType {
  SHIELD = 1,
  STRENGTH,
  FORTITUDE,
  GLINT,
  MARK,
  IMMUNITY,
  RESTORE,
  CLEAR,
  TAUNT,
  WEAK,
  VULNERABLE,
  STUN,
};

//! Represents Card effects
struct Effect {
  Effect(EffectType newType, int newStack) : name(newType), stack(newStack) {}

  EffectType name; //!< effect type
  int stack; //!< amount of effects on this Card
};

//! This will be used to represent Player cards
struct Card {
public:
  //! A constructor designed for use with SQLite
  Card(char **argv)
      : name(argv[0]), cardID(argv[1]), tagline(argv[2]), description(argv[3]),
        position(argv[4]), barrage(std::strcmp(argv[5], "1") == 0),
        team(std::strcmp(argv[6], "1") == 0), energyCost(std::stoi(argv[7])),
        token(std::strcmp(argv[8], "1") == 0), tokenCost(std::stoi(argv[9])),
        tokenType(argv[10]), damage(std::stoi(argv[11])),
        copies(std::stoi(argv[12])), repeat(std::stoi(argv[13])),
        exhaust(std::strcmp(argv[14], "1") == 0) {};

  //! A general-use constructor
  Card(const char *newName, const char *newID, const char *newTagline,
       const char *newDescription, const std::bitset<4> &newPosition,
       bool newBarrage, bool newTeam, int newEnergyCost, bool newToken,
       int newTokenCost, const char *newTokenType, int newDamage, int newCopies,
       int newRepeat, bool newExhaust, const std::list<Effect> &newEffectList)
      : name(newName), cardID(newID), tagline(newTagline),
        description(newDescription), position(newPosition), barrage(newBarrage),
        team(newTeam), energyCost(newEnergyCost), token(newToken),
        tokenCost(newTokenCost), tokenType(newTokenType), damage(newDamage),
        copies(newCopies), repeat(newRepeat), exhaust(newExhaust),
        EffectList(newEffectList) {};
  std::string name;             //!< Name of the Card.
  std::string cardID;           //!< Unique Identifier for the Card, usually also has player information
  std::string tagline;          //!< Tagline to be shown on the card face.
  std::string description;      //!< The description to be shown on the card face for the user
  std::bitset<4> position;      //!< Which position can be affected by this Card.
  bool barrage{false};          //!< Does this affect all positions denoted by position
  bool team{false};             //!< Does this affect the Player team
  int energyCost{0};            //!< The amount of energy the Card costs to play.
  bool token{false};            //!< Does this Card require a token other than energy to play.
  int tokenCost{2};             //!< How many additional tokens does this cost to play
  int damage{0};                //!< How much damage does this Card deal
  int copies{1};                //!< Maximum allowed copies of this Card
  int repeat{0};                //!< How many times this Card repeats its effects in a turn
  bool exhaust;                 //!< Does this Card delete iteslf after being played
  std::string tokenType;        //!< Which token is used to play this Card
  std::list<Effect> EffectList; //!< A list of Effect structs that will be put into effect when the card is played
};

//! The struct to be used in enemy intentions
struct EnemyCard {
  // TODO implement enemy cards
};

//! used by Enemies to attack and is shown to the player ahead of time
struct Intention {
  EnemyCard Type;
  bool hasPosition{true};
  std::bitset<4> position;
  bool barrage{false};
};

/*************************************************************//**
* \brief Used to fetch cards from an SQLite database.
*
* The Catalog class uses cardIDs in order to fetch cards from the 
* sqlite database, and store pointers to them in a map, so they 
* can be used again without another fetch.  It also deletes any
* cards from memory when they are no longer needed by the player
* classes.
*
* ***************************************************************/
class Catalog {
public:
  /*********************************//**
  * \brief fetch and return Cards by 
  *        cardID
  *
  * This function first searches cards
  * already in memory, then creates it
  * from the database if it does not.
  *
  * @param cardID The ID to search for
  * and create if not found.
  *
  * @return A pointer to the Card with
  * the matching cardID. Returns
  * nullptr when the card with cardID
  * is not found.
  * ***********************************/
  Card *getCardByID(const std::string &);

  /*********************************//**
  * \brief Returns amout of times a 
  *        certian Card appears.
  *
  * This function searches the cardmap 
  * for the card, and then returns its 
  * copies field.  If the card has 
  * not been created, then it searches
  * the SQLite database, and returns 
  * the amount of copies a card has 
  * as an integer.
  *
  * @param cardID The ID of the Card to
  * fetch the amount of copies of.
  *
  * @return The amount of copies a Card
  * has.
  * ***********************************/
  int getCopiesByID(const std::string &);
  
  /*********************//**
  * \brief Removes any Cards 
  * not being used by the 
  * Party from cache.
  *
  * Searches through the 
  * decks of all players in 
  * the Party, and deletes 
  * cached Cards no longer 
  * being used by them.
  *
  * @param partyClear A 
  * pointer to the Party 
  * of Players to be 
  * cleared.
  * ***********************/
  void clearUnused(Party *);

  /****************************//**
  * \brief Removes any Cards not 
  * being used by the Party from 
  * cache.
  * 
  * @param playerClear A pointer 
  * to the player that will have 
  * its unused Cards removed from 
  * cache.
  ********************************/
  void clearUnused(const Player *);

  /******************************//**
  * \brief Check if a card has been 
  * cached.
  * 
  * @param cardID The cardID of the 
  * Card to be searched for in cache
  **********************************/
  bool isCached(const std::string &);
  Catalog();
  ~Catalog();

private:
  Card *searchForID(const std::string &) const;
  int searchForCopies(const std::string &) const;
  std::list<std::string> searchForPlayerCards(const std::string &) const;
  std::list<std::string> searchForAllCards() const;
  Card *createCard(const std::string &);
  std::map<std::string, Card *> cardMap;
  void openDB();
  void closeDB();
  sqlite3 *db;
  friend int intCallBack(void *, int, char **, char **);
  friend int cardCallback(void *, int, char **, char **);
  friend int cardMapCallback(void *, int, char **, char **);
  friend int stringListCallback(void *, int, char **, char **);
  friend int effectListCallback(void *, int, char **, char **);
};

#endif
