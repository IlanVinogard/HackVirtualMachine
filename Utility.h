#pragma once

#include <iostream>
#include <fstream>

using namespace std;

class Ui {
public:
    static void uiLogo() {
        cout << "\n\n";
        cout << " __     __  _          _                     _     __  __                  _       _                \n";
        cout << " \\ \\   / / (_)  _ __  | |_   _   _    __ _  | |   |  \\/  |   __ _    ___  | |__   (_)  _ __     ___ \n";
        cout << "  \\ \\ / /  | | | '__| | __| | | | |  / _` | | |   | |\\/| |  / _` |  / __| | '_ \\  | | | '_ \\   / _ \\\n";
        cout << "   \\ V /   | | | |    | |_  | |_| | | (_| | | |   | |  | | | (_| | | (__  | | | | | | | | | | |  __/\n";
        cout << "    \\_/    |_| |_|     \\__|  \\__,_|  \\__,_| |_|   |_|  |_|  \\__,_|  \\___| |_| |_| |_| |_| |_|  \\___|\n";
        cout << "                                                                                                    \n";
    }

    static void clear() {
        cout << "\x1B[2J\x1B[H";
    }
};

class File {
public:
    static string askFilePath() {
        cout << "Please insert file path\n";
        cout << "make sure the file ends with .vm\n";
        cout << "File path -> ";
        string path;
        cin >> path;
        return path;
    }

    static string askIfError(string& path) {
        cout << "\nInvalid path!\n";
        cout << "Example -> path.vm\n";
        cout << "Try again.\n";
        cout << "File path -> ";
        cin >> path;
        return path;
    }

    static bool endsWith(const string& path, string format) {
        if (path.size() - 1 < format.size()) return false;

        return path.compare(path.size() - format.size(), format.size(), format) == 0;
    }

    static void isValidFormat(string& path, bool& isFormat) {
        path = askIfError(path);
        isFormat = endsWith(path, ".vm");
    }
};