#include "cli.hpp"
#include <string>
#include <vector>

Cli::Cli(int argl, char** newArgs) {
	for (int i{0}; i < argl; i++) {
		args.push_back(newArgs[i]);
	}
}
