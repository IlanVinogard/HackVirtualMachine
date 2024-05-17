#pragma once
#include <string>
#include <fstream>
#include "../VM/Utility/RAM.h"
#include "../VM/VirtualMachine.h"

class CodeWriter {
private:
	ofstream& output;
	RAM& ram;

public:
	CodeWriter(ofstream& output, RAM& ram) : output(output), ram(ram) {};

	static void writeArithmetic(const string& command) {
		if (command == "add") {

		}
		else if (command == "sub") {

		}
		else if (command == "neg") {

		}
		else if (command == "eq") {

		}
		else if (command == "gt") {

		}
		else if (command == "lt") {

		}
		else if (command == "and") {

		}
		else if (command == "or") {

		}
		else if (command == "not") {

		}
	}

	static void writePushPop(const string& command, const string& segment, const int& index) {

	}
};
