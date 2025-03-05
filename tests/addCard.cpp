#include <catalog.hpp>
#include <classes.hpp>
#include <iostream>

int main(int argLen, char** args) {
	Catalog cardCatalog;
	Party party(&cardCatalog);
	party.addMember(25, "Rudolph", "rudy");
	party.addMember(22, "Mikey", "mikey");
	party.addMember(18, "“Slick”", "slick");
	party.addMember(20, "Dr. Victor", "victor");

	bool test1{false}, test2{false}, test3{false};

	int copies = cardCatalog.getCopiesByID("rudy_Punch");

	party[0]->addCardToDeck("rudy_Punch");
	party[0]->addCardToDeck("rudy_Punch");
	party[0]->addCardToDeck("rudy_Punch");
	party[0]->addCardToDeck("rudy_Punch");
	party[0]->addCardToDeck("rudy_Punch");
	std::map<std::string, int> contents = party[0]->getDeckContents();
	copies = copies > 5 ? 5 : copies;
	int size = static_cast<int>(contents.size());
	if (size == 1 && contents["rudy_Punch"] == copies)
		test1 = true;
	else
		std::cout << "Test 1 Failed" << std::endl;
	party[0]->addCardToDeck("rudy_Block");
	party[0]->addCardToDeck("wingledonger");
	party[0]->addCardToDeck("rudy_Block", 7);
	contents = party[0]->getDeckContents();
	copies = cardCatalog.getCopiesByID("rudy_Block");
	copies = copies > 8 ? 8 : copies;
	size = static_cast<int>(contents.size());
	if (size == 2 && contents["rudy_Block"] == copies)
		test2 = true;
	else
		std::cout << "Test 2 Failed" << std::endl;
	party[0]->addCardToDeck("rudy_Cover", 7);
	contents = party[0]->getDeckContents();
	copies = cardCatalog.getCopiesByID("rudy_Cover");
	copies = copies > 7 ? 7 : copies;
	size = static_cast<int>(contents.size());
	if (size == 3 && contents["rudy_Cover"] == copies)
		test3 = true;
	else
		std::cout << "Test 3 Failed" << std::endl;
	if (test1 && test2 && test3)
		std::cout << "All Tests Passed" << std::endl;
}
