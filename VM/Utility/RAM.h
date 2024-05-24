#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

class RAM {
private:
    unordered_map<string, int> addresses;
    unordered_map<int, int> memory;
    stack<int> myStack;
    string currentFile;
    vector<string> fileStack;

public:
    RAM() {
        // Initialize special registers with appropriate values
        addresses = {
            {"SP", 256}, {"LCL", 300}, {"ARG", 400}, {"THIS", 3000}, {"THAT", 3010},
            {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4},
            {"R5", 5}, {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9},
            {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15}
        };

        // Initialize stack pointer (SP) in memory
        setMemory(256, 0); // Initialize the stack pointer to 256
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

    void ensureMemory(int address) {
        if (memory.find(address) == memory.end()) {
            memory[address] = 0;
        }
    }

    void pushStack(int value) {
        if (addresses.at("SP") >= 0 && addresses.at("SP") < 24576) {
            myStack.push(value);
            addresses["SP"]++;
        }
        else {
            throw runtime_error("Stack overflow.");
        }
    }

    int popStack() {
        if (!myStack.empty()) {
            int value = myStack.top();
            myStack.pop();
            addresses["SP"]--;
            return value;
        }
        throw runtime_error("Stack underflow.");
    }

    int getStackSize() const {
        return myStack.size();
    }

    void setCurrentFile(const string& fileName) {
        currentFile = fileName;
    }

    void pushFile(const string& fileName) {
        fileStack.push_back(fileName);
    }

    void popFile() {
        if (!fileStack.empty()) {
            fileStack.pop_back();
        }
    }

    void printRAM() const {
        // Clear the console (platform-specific)
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        // Print RAM addresses
        cout << "=== Special Registers ===" << endl;
        for (const auto& pair : addresses) {
            cout << setw(5) << left << pair.first << ": " << pair.second << endl;
        }

        cout << "\n=== Stack ===" << endl;
        stack<int> tempStack = myStack;
        int stackIndex = addresses.at("SP") - 1;
        while (!tempStack.empty()) {
            cout << "SP[" << stackIndex << "]: " << tempStack.top() << endl;
            tempStack.pop();
            stackIndex--;
        }

        cout << "\n=== Memory ===" << endl;
        for (const auto& pair : memory) {
            cout << "RAM[" << pair.first << "]: " << pair.second << endl;
        }

        cout << "\n=== Current File ===" << endl;
        cout << currentFile << endl;

        cout << "\n=== File Stack ===" << endl;
        for (const auto& file : fileStack) {
            cout << file << endl;
        }

        // Pause for a while to allow the user to read
        this_thread::sleep_for(chrono::milliseconds(500));

        // Clear the console again for the next output
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
};