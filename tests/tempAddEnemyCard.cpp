#include <catalog.hpp>
#include <classes.hpp>
#include <iostream>

int main(int argc, char** argv) {

	bool test1{false}, test2{false};

	Catalog catalog;

	EnemyCard* bash = catalog.getEnemyCard("barbatos_bash");
	if (bash != nullptr && (bash->name == "Cymbal Bash")) {
		test1 = true;
	} else {
		std::cout << "Test 1 Failed\n";
	}

	EnemyCard* fail = catalog.getEnemyCard("weebledeelb");
	if (fail == nullptr) {
		test2 = true;
	} else {
		std::cout << "Test 2 Failed\n";
	}

	if (test1 && test2) {
		std::cout << "All Tests Passed";
	}
	std::cout << std::endl;

	return 0;
}
