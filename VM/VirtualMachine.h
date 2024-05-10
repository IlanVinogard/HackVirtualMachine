#pragma once
#include <iostream>
#include <fstream>
#include "Parser.h"
#include "CodeWriter.h"

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
		// code.
	}
};
