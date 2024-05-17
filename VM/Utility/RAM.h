#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <stdexcept>

using namespace std;

class RAM {
private:
    unordered_map<string, int> addresses;
    unordered_map<int, int> memory; // ????????? ??? ???????? ???????? ?? ???????? ???????
    stack<int> myStack;

public:
    RAM() {
        // Initialize special registers with appropriate values
        addresses = {
            {"SP", 256}, {"LCL", 300}, {"ARG", 400}, {"THIS", 3000}, {"THAT", 3010},
            {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4},
            {"R5", 5}, {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9},
            {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15}
        };
    }

    bool contains(const string& symbol) const {
        return addresses.find(symbol) != addresses.end();
    }

    void setAddress(const string& symbol, int value) {
        addresses[symbol] = value;
    }

    int getAddress(const string& symbol) const {
        auto it = addresses.find(symbol);
        if (it != addresses.end()) {
            return it->second;
        }
        throw runtime_error("Symbol not found in the table: " + symbol);
    }

    void setMemory(int address, int value) {
        memory[address] = value;
    }

    int getMemory(int address) const {
        auto it = memory.find(address);
        if (it != memory.end()) {
            return it->second;
        }
        throw runtime_error("Address not found in memory: " + to_string(address));
    }

    void pushStack(int value) {
        myStack.push(value);
        addresses["SP"]++;
    }

    int popStack() {
        if (myStack.empty()) {
            throw runtime_error("Stack underflow.");
        }
        int value = myStack.top();
        myStack.pop();
        addresses["SP"]--;
        return value;
    }

    void printRAM() const {
        for (const auto& pair : addresses) {
            cout << pair.first << ": " << pair.second << endl;
        }
        for (const auto& pair : memory) {
            cout << pair.first << ": " << pair.second << endl;
        }
    }
};
