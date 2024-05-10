#include <iostream>
#include "../HackVirtualMachine/Utility.h"
#include "../HackVirtualMachine/Parser.h"

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


        }
    }
    catch (const exception& e) {
        cerr << "Unhandled exception: " << e.what() << '\n';
        return 1;
    }

    return 0;
}