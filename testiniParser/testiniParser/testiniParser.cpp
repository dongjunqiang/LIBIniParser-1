#include <iostream>
using namespace std;
#include "IniParser.h"


int main(int argc, char* argv[])
{
    CIniParser iniParser;
    iniParser.load(".gitconfig");
    string value;
    iniParser.GetValue("difftool \"bc3\"", "cmd", value);
    cout << value << endl;

    iniParser.Save();

    return 0;
}

