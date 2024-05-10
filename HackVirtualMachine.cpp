#include <iostream>
#include "../HackVirtualMachine/Utility.h"
#include "../HackVirtualMachine/Parser.h"
#include "../HackVirtualMachine/VirtualMachine.h"

int main()
{
    try {
        while (true) {
            Ui::uiLogo();

            string path = File::askFilePath();

            // check for suffix '.vm' and clear console.
            bool isFormat = File::endsWith(path, ".vm");
            Ui::clear();

            // if not vaild format force him to be :D.
            while (!isFormat) {
                Ui::uiLogo();
                File::isValidFormat(path, isFormat);
                Ui::clear();
            }

            VirtualMachine virtualMachine(path, "Prog.asm");

            try {
                cout << "File successfully opened" << endl;
                // converting file to new file named XXX.asm.
                virtualMachine.convertFile();

                cout << "Finished converting!\n" << endl;

            } catch (const runtime_error& e) {
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