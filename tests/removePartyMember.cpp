#include <catalog.hpp>
#include <classes.hpp>
#include <iostream>

int main(int argLen, char** args) {
	Catalog cardCatalog;
	Party party(&cardCatalog);
	party.addMember(25, "Rudolph", "rudy");
	party.addMember(22, "Mikey", "mikey");
	party.addMember(18, "“Slick”", "slick");
	party.addMember(new Player(20, "Dr. Victor", "victor", &cardCatalog));
	party.erase(party.begin() + 3);
	bool test1{false}, test2{false}, test3{false};
	if (party.size() == 3) {
		std::string ids[3] = {"rudy", "mikey", "slick"};
		bool match{true};
		for (size_t i{0}; i < 3; ++i) {
			if (ids[i] != party[i]->getID())
				match = false;
		}
		if (match)
			test1 = true;
		else
			std::cout << "Erase 1 Fail" << std::endl;
	} else {
		throw std::runtime_error("Erase 1 Fail");
	}
	party.erase(party.begin(), party.begin() + 2);
	if (party.size() == 1) {
		if (party[0]->getID() == "slick")
			test2 = true;
		else
			std::cout << "Erase 2 Fail" << std::endl;
	} else {
		throw std::runtime_error("Erase 2 Fail");
	}
	party.remove("slick");
	if (party.size() == 0 && party.begin() == party.end())
		test3 = true;
	else
		std::cout << "Remove Fail" << std::endl;

	if (test1 && test2 && test3)
		std::cout << "All Tests Passed" << std::endl;
}