#pragma once
#include <iostream>
#include <fstream>
#include "Parser.h"
#include "CodeWriter.h"
#include <string>

using namespace std;

class VirtualMachine {
private:
	ifstream input;
	ofstream output;
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
		string line;
		while (getline(input, line)) {
			if (Parser::isNotCommentLine(line)) {
				try {
					string validLine = Parser::cleanAndValidateLine(line);

					// Add comment to file.
					output << "//" + validLine << endl;

					// Logic Code In Process :D
				} catch (const runtime_error& e) {
					cerr << "Error: " << e.what() << endl;
				}
			}
		}
	}
};