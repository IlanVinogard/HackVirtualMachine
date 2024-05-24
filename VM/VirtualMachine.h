#pragma once
#include <iostream>
#include <fstream>
#include "Parser.h"
#include "CodeWriter.h"
#include "../VM/Utility/RAM.h"
#include <string>
#include <vector>

using namespace std;

class VirtualMachine {
private:
    ifstream input;
    ofstream output;
    RAM& ram;

public:
    VirtualMachine(const string& outputPath, RAM& ram) : ram(ram) {
        output.open(outputPath, ios::out);
        if (!output.is_open()) {
            cerr << "Error opening output file: " << outputPath << endl;
            throw runtime_error("Failed to open output file.");
        }
    }

    ~VirtualMachine() {
        output.close();
    }

    void convertFiles(const vector<string>& filePaths) {
        CodeWriter codeWriter(output, ram);

        for (const auto& path : filePaths) {
            ram.setCurrentFile(path);
            ram.pushFile(path);

            input.open(path, ios::in);
            if (!input.is_open()) {
                cerr << "Error opening input file: " << path << endl;
                throw runtime_error("Failed to open input file.");
            }

            string line;
            while (getline(input, line)) {
                line = Parser::removeComments(line);
                if (Parser::isNotCommentLine(line)) {
                    try {
                        string validLine = Parser::cleanAndValidateLine(line);

                        // Add comment to file.
                        output << "// " + validLine << endl;

                        // Check command type.
                        string commandType = Parser::commandType(validLine);

                        // Write command to file.
                        if (commandType == "C_ARITHMETIC") {
                            codeWriter.writeArithmetic(Parser::arg1(commandType, validLine));
                        }
                        else if (commandType == "C_PUSH" || commandType == "C_POP") {
                            codeWriter.writePushPop(commandType, Parser::arg1(commandType, validLine), Parser::arg2(validLine));
                        }
                        else if (commandType == "C_LABEL") {
                            codeWriter.writeLabel(Parser::arg1(commandType, validLine));
                        }
                        else if (commandType == "C_GOTO") {
                            codeWriter.writeGoto(Parser::arg1(commandType, validLine));
                        }
                        else if (commandType == "C_IF") {
                            codeWriter.writeIf(Parser::arg1(commandType, validLine));
                        }
                        else if (commandType == "C_FUNCTION") {
                            codeWriter.writeFunction(Parser::arg1(commandType, validLine), Parser::arg2(validLine));
                        }
                        else if (commandType == "C_CALL") {
                            codeWriter.writeCall(Parser::arg1(commandType, validLine), Parser::arg2(validLine));
                        }
                        else if (commandType == "C_RETURN") {
                            codeWriter.writeReturn();
                        }

                        ram.printRAM();
                    }
                    catch (const runtime_error& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                }
            }
            input.close();
            ram.popFile();
        }
    }
};