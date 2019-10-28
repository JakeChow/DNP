#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {
	std::ofstream ofile;
	std::ifstream ifile;
	string hold = "";
	string min = "";
	string max = "";

//Used to build AnalogRanges.csv which is a file with each analog input's optional range
	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNPPointerList_AnalogInputs_SC.csv");
	ofile.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/AnalogRanges.csv");

	while (!ifile.eof()) {
		getline(ifile, hold);
		hold = hold.substr(hold.find(',') + 1);
		hold = hold.substr(hold.find(',') + 1);
		hold = hold.substr(hold.find(',') + 1);
		min = hold.substr(0, hold.find(','));
		hold = hold.substr(hold.find(',') + 1);
		max = hold.substr(0, hold.find(','));

		ofile << min + ", " + max + "\n";
	}

	ifile.close();
	ofile.close();


	return 0;
}

