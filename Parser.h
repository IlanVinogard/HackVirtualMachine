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
        }
        
        return validLine;
	}

	static bool isNotCommentLine(const string line) {
		if (line.empty() || line.find("//") != string::npos) return false;
		return true;
	}
};