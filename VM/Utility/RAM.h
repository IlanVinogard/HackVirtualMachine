#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>

using namespace std;

class RAM {
private:
	unordered_map<string, int> addresses;
	stack<int> myStack;

public:
	RAM() {
		addresses = {
			{"SP", 256}, {"LCL", 0}, {"ARG", 0}, {"THIS", 0}, {"THAT", 0}, {"Temp0", 0},
			{"Temp1", 0}, {"Temp2", 0}, {"Temp3", 0}, {"Temp4", 0}, {"Temp5", 0},
			{"Temp6", 0}, {"Temp7", 0}, {"R13", 0}, {"R14", 0}, {"R15", 0}
		};
	}

	bool contains(const string& symbol) const {
		return addresses.find(symbol) != addresses.end();
	}

	void addEntry(const string& symbol, int& SP) {
		if (contains(symbol)) {
			throw runtime_error("Symbol already exists in the table.");
		}
		addresses[symbol] = SP;
	}

	int getValueAddress(const string& symbol) const {
		auto it = addresses.find(symbol);
		if (it != addresses.end()) {
			return it->second;
		}
		throw runtime_error("Symbol not found in the table.");
	}

	unordered_map<string, int>& getAddress() {
		return addresses;
	}

	void addStack(const int& value) {
		myStack.push(value);
	}

	void popStack() {
		myStack.pop();
	}
};
