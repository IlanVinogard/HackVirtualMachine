#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

using namespace std;
namespace fs = std::filesystem;

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
    static string askDirectoryPath() {
        cout << "Please insert directory path containing .vm files\n";
        cout << "Directory path -> ";
        string path;
        cin >> path;
        return path;
    }

    static bool isValidDirectory(const string& path) {
        return fs::is_directory(path);
    }

    static vector<string> getVMFiles(const string& directoryPath) {
        vector<string> vmFiles;
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.path().extension() == ".vm") {
                vmFiles.push_back(entry.path().string());
            }
        }
        return vmFiles;
    }
};