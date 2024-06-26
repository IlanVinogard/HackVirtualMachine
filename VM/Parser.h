#pragma once
#include <regex>
#include <string>

class Parser {
public:
    static string cleanAndValidateLine(const string& line) {
        regex pattern("\\s+");
        string validLine = regex_replace(line, pattern, " ");

        // Remove leading and trailing spaces
        size_t firstNonSpace = validLine.find_first_not_of(" ");
        size_t lastNonSpace = validLine.find_last_not_of(" ");

        if (firstNonSpace != std::string::npos && lastNonSpace != std::string::npos) {
            validLine = validLine.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
        } else {
            validLine = "";
        }

        return validLine;
    }

    static bool isNotCommentLine(const string& line) {
        string trimmedLine = line.substr(0, line.find("//"));
        trimmedLine = cleanAndValidateLine(trimmedLine);
        return !trimmedLine.empty();
    }

    static string removeComments(const string& line) {
        size_t commentStart = line.find("//");
        if (commentStart != string::npos) {
            return line.substr(0, commentStart);
        }
        return line;
    }

    static string commandType(const string& line) {
        string command = line.substr(0, line.find_first_of(' '));

        // push/pop C_COMMANDS.
        if (command == "push") return "C_PUSH";
        else if (command == "pop") return "C_POP";

        // Arithmetics/logic C_COMMANDS.
        else if (command == "add") return "C_ARITHMETIC";
        else if (command == "sub") return "C_ARITHMETIC";
        else if (command == "neg") return "C_ARITHMETIC";
        else if (command == "eq") return "C_ARITHMETIC";
        else if (command == "gt") return "C_ARITHMETIC";
        else if (command == "lt") return "C_ARITHMETIC";
        else if (command == "and") return "C_ARITHMETIC";
        else if (command == "or") return "C_ARITHMETIC";
        else if (command == "not") return "C_ARITHMETIC";

        // labels C_LABEL.
        else if (command == "label") return "C_LABEL";

        // goto C_GOTO.
        else if (command == "goto") return "C_GOTO";

        // if-goto C_IF.
        else if (command == "if-goto") return "C_IF";

        // functions C_FUNCTION.
        else if (command == "function") return "C_FUNCTION";

        // return C_RETURN.
        else if (command == "return") return "C_RETURN";

        // call C_CALL.
        else if (command == "call") return "C_CALL";

        // exception.
        else {
            throw runtime_error("Unrecognized command: " + command);
        }
    }

    static string updateValidLine(const string& line) {
        size_t firstSpace = line.find_first_of(' ');
        if (firstSpace == string::npos) return "";

        return line.substr(firstSpace + 1);
    }

    static string arg1(const string& commandType, const string& line) {
        if (commandType == "C_ARITHMETIC") return line;

        string remaining = updateValidLine(line);
        size_t firstSpace = remaining.find_first_of(' ');
        if (firstSpace == string::npos) return remaining;

        return remaining.substr(0, firstSpace);
    }

    static int arg2(const string& line) {
        string remaining = updateValidLine(line);
        size_t firstSpace = remaining.find_first_of(' ');
        if (firstSpace == string::npos) {
            throw runtime_error("No second argument found in line: " + line);
        }

        return stoi(remaining.substr(firstSpace + 1));
    }
};