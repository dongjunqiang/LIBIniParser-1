#include <iostream>
using namespace std;
#include "IniParser.h"


int main(int argc, char* argv[])
{
	CIniParser iniParser;
	iniParser.load("aspnet_state_perf.ini");
	string value;
	iniParser.GetValue("languages", "01D", value);
	cout << value << endl;

	return 0;
}

