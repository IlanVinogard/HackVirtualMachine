#pragma once
#include <iostream>
#include <fstream>
#include "Parser.h"
#include "CodeWriter.h"
#include "../VM/Utility/RAM.h"
#include <string>

using namespace std;

class VirtualMachine {
private:
	ifstream input;
	ofstream output;
	RAM ram;
public:
	VirtualMachine(const string& inputPath, const string& outputPath) {
	input.open(inputPath, ios::in);
	if (!input.is_open()) {
		cerr << "Error opening input file: " << inputPath << endl;
		throw runtime_error("Failed to open input file.");
	}

	output.open(outputPath, ios::out);
	if (!output.is_open()) {
		cerr << "Error opening output file: " << outputPath << endl;
		throw runtime_error("Failed to open output file.");
	}
}

	~VirtualMachine() {
		input.close();
		output.close();
	}

	void convertFile() {
		CodeWriter codeWriter(output, ram);
		string line;
		while (getline(input, line)) {
			if (Parser::isNotCommentLine(line)) {
				try {
					string validLine = Parser::cleanAndValidateLine(line);

					// Add comment to file.
					output << "// " + validLine << endl;

					// check command type.
					string commandType = Parser::commandType(validLine);
					
					// write command to file.
					if (commandType == "C_ARITHMEIC") {
						codeWriter.writeArithmetic(Parser::arg1(validLine));
					}
					else if (commandType == "C_PUSH" || commandType == "C_POP") {
						codeWriter.writePushPop(commandType, Parser::arg1(validLine), Parser::arg2(validLine));
					} // continue next chapter ...

				} catch (const runtime_error& e) {
					cerr << "Error: " << e.what() << endl;
				}
			}
		}
	}
};
