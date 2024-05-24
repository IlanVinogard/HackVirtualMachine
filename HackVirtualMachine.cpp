#include <iostream>
#include "../HackVirtualMachine/VM/Utility/Utility.h"
#include "../HackVirtualMachine/VM/Parser.h"
#include "../HackVirtualMachine/VM/VirtualMachine.h"

int main()
{
    try {
        while (true) {
            Ui::uiLogo();

            string directoryPath = File::askDirectoryPath();

            // check for valid directory and clear console.
            bool isValidDir = File::isValidDirectory(directoryPath);
            Ui::clear();

            // if not valid directory force user to re-enter.
            while (!isValidDir) {
                Ui::uiLogo();
                cout << "Invalid directory!" << endl;
                directoryPath = File::askDirectoryPath();
                isValidDir = File::isValidDirectory(directoryPath);
                Ui::clear();
            }

            vector<string> vmFiles = File::getVMFiles(directoryPath);

            if (vmFiles.empty()) {
                cerr << "No .vm files found in the directory!" << endl;
                continue;
            }

            // Prioritize Sys.vm if it exists
            vector<string> orderedFiles;
            for (const auto& file : vmFiles) {
                if (fs::path(file).filename() == "Sys.vm") {
                    orderedFiles.insert(orderedFiles.begin(), file);
                }
                else {
                    orderedFiles.push_back(file);
                }
            }

            RAM ram;
            VirtualMachine virtualMachine("Prog.asm", ram);

            try {
                cout << "Files successfully opened" << endl;
                // converting files to new file named Prog.asm.
                virtualMachine.convertFiles(orderedFiles);

                cout << "Finished converting!\n" << endl;

            }
            catch (const runtime_error& e) {
                cerr << "Error during translating: " << e.what() << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Unhandled exception: " << e.what() << '\n';
        return 1;
    }

    return 0;
}