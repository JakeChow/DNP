
#include "DataStructs.h"
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <sstream>

using namespace std;

typedef struct Link
{
	int input;
	int output;
} Link ;

void isInput(int x) {

}

int PointInit::getLink(int x, string type) {
	int size = 0;

	if (type == "a") {
		if (aop[x].linkedInput >= 0) {
			return aop[x].linkedInput;
		}
	}

	else if (type == "b") {
		if (bop[x].linkedInput >= 0) {
			return bop[x].linkedInput;
		}
	}

	else {
		return -1;
	}
}

void PointInit::setLinks() {
	std::ifstream ifile;
	string hold;
	string partial;
	string second;
	int input;
	int output;
	Link bLink[10];
	Link aLink[243];
	int count = 0;

	//Fills array with binary links
	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNP_BinaryLinks.csv");
	while (!ifile.eof()) {
		getline(ifile, hold);
		partial = hold.substr(0, hold.find(","));
		second = hold.substr(hold.find(",") + 1, hold.find("\n"));
		stringstream i1(partial);
		i1 >> output;
		//------------------------------TEMPORARY FIX TO SET FIRST VALUE CORRECTLY----------------------------------
		if (count == 0) {
			bLink[count].output = 3;
		}
		else {
			bLink[count].output = output;
		}

		stringstream i2(second);
		i2 >> input;
		bLink[count].input = input;
		count++;
	}
	count = 0;
	ifile.close();


	//Fills array with analog links
	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNP_AnalogLinks.csv");
	while (!ifile.eof()) {
		getline(ifile, hold);
		partial = hold.substr(0, hold.find(","));
		second = hold.substr(hold.find(",") + 1, hold.find("\n"));
		stringstream i3(partial);
		i3 >> output;

		if (count == 0) {
			aLink[count].output = 6;
		}
		else {
			aLink[count].output = output;
		}

		stringstream i4(second);
		i4 >> input;
		aLink[count].input = input;
		count++;
	}
	ifile.close();

	//Connects binary points to binary points instance array
	for (int x = 0; x < 10; x++) {
//		std::cout << "BLink[" << x << "] - Input: " << bLink[x].input << " - Output: " << bLink[x].output << std::endl;
		this->bop[bLink[x].output].linkedInput = bLink[x].input;
//		std::cout << "Value: " << bLink[x].input << "  - ArrVal: " << this->bop[bLink[x].output].linkedInput << std::endl;
	}

	//Connects analog points to analog points instance array
	for (int y = 0; y < 243; y++) {
//		std::cout << "ALink[" << y << "] - Input: " << aLink[y].input << " - Output: " << aLink[y].output << std::endl;
		this->aop[aLink[y].output].linkedInput = aLink[y].input;
//		std::cout << "Value: " << aLink[y].input << "  - ArrVal: " << this->aop[aLink[y].output].linkedInput << std::endl;
	}
}


void PointInit::setCurrentValues() {
	std::ifstream ifile;
	string x = "";
	bool binValue;
	int anaValue;
	int index = 0;

	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/BinaryDataSet.csv");
	while (!ifile.eof()  && index < 108) {
		getline(ifile, x);
		stringstream holdVal(x);
		holdVal >> binValue;
		this->bi[index].currentValue = binValue;
		index++;
	}

	ifile.close();
	index = 0;

	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/AnalogDataSet.csv");
	while (!ifile.eof() && index < 569) {
		getline(ifile, x);
		x = x.substr(0, x.find(','));
		stringstream holdVal(x);
		holdVal >> anaValue;
		this->ai[index].currentValue = anaValue;
//		std::cout << "Index : " << index << "  - Current Value: " << this->ai[index].currentValue << "  -  Real Val: " << anaValue << std::endl;
		index++;
	}

	for (int y = 0; y < 4; y++) {
		this->cou[y].currentValue = 0;
	}

	ifile.close();
}


void PointInit::printType(int x) {
	int skip;
	int sz = -1;
	string first;
	string second;
	string third;
	string fourth;

	//binary input
	if (x == 1) {
		std::cout << "Printing Binary Inputs \n";
		for (int x = 0; x < 54; x++) {
			skip = x + 54;

			if (skip < 107) {
				first = "[" + std::to_string(x) + "] - " + this->bi[x].Name + " ~ " + std::to_string(this->bi[x].currentValue);
				second = "[" + std::to_string(skip) + "] - " + this->bi[skip].Name + " ~ " + std::to_string(this->bi[skip].currentValue);

				third = first.substr(first.find("~") + 1);
				fourth = second.substr(second.find("~") + 1);
				//Formatting
				while (true) {
					if (first.size() < 55) {
						first = first + " ";
					}

					else if (first.size() > 55) {
						first = first.substr(0, first.find("~") + 1);
						while (first.size() > 40) {
							first = first.substr(0, first.size() - 1);
						}
						first = first + third;
					}

					if (second.size() < 55) {
						second = second + " ";
					}

					else if (second.size() > 55) {
						second = second.substr(0, second.find("~") + 1);
						while (second.size() > 40) {
							second = second.substr(0, second.size() - 1);
						}
						second = second + fourth;
					}

					if (first.size() == 55 && second.size() == 55) {
						break;
					}
				}

				std::cout << first + " | " + second << std::endl;
			}
		}
	}
	//binary output
	else if (x == 2) {
		std::cout << "Printing Binary Outputs\n";
		for (int a = 0; a < 21; a++) {
			skip = a + 21;

				first = "[" + std::to_string(a) + "] - " + this->bop[a].Name + " ~ " + std::to_string(this->bop[a].currentValue);
				second = "[" + std::to_string(skip) + "] - " + this->bop[skip].Name + " ~ " + std::to_string(this->bop[skip].currentValue);


				third = first.substr(first.find("~") + 1);
				fourth = second.substr(second.find("~") + 1);

				//Formatting
				while (true) {
					if (first.size() < 55) {
						first = first + " ";
					}
					else if (first.size() > 55) {
						first = first.substr(0, first.find("~") + 1);
						while (first.size() > 40) {
							first = first.substr(0, first.size() - 1);
						}
						first = first + third;
					}

					if (second.size() < 55) {
						second = second + " ";
					}

					else if (second.size() > 55) {
						second = second.substr(0, second.find("~") + 1);
						while (second.size() > 40) {
							second = second.substr(0, second.size() - 1);
						}
						second = second + fourth;
					}

					if (first.size() == 55 && second.size() == 55) {
						break;
					}
				}


				std::cout << first + " | " + second << std::endl;
		}
	}

	//analog input
	else if (x == 3) {
		std::cout << "Printing Analog Inputs\n";
		for (int b = 0; b < 285; b++) {
			skip = b + 285;

			if (skip < 569) {
				first = "[" + std::to_string(b) + "] - " + this->ai[b].Name + " ~ " + std::to_string(this->ai[b].currentValue);
				second = "[" + std::to_string(skip) + "] - " + this->ai[skip].Name + " ~ " + std::to_string(this->ai[skip].currentValue);

				third = first.substr(first.find("~") + 1);
				fourth = second.substr(second.find("~") + 1);

				//Formatting
				while (true) {
					if (first.size() < 55) {
						first = first + " ";
					}
					else if (first.size() > 55) {
						first = first.substr(0, first.find("~") + 1);
						while (first.size() > 40) {
							first = first.substr(0, first.size() - 1);
						}
						first = first + third;
					}

					if (second.size() < 55) {
						second = second + " ";
					}

					else if (second.size() > 55) {
						second = second.substr(0, second.find("~") + 1);
						while (second.size() > 40) {
							second = second.substr(0, second.size() - 1);
						}
						second = second + fourth;
					}

					if (first.size() == 55 && second.size() == 55) {
						break;
					}
				}



				std::cout << first + " | " + second << std::endl;

			}

		}
	}

	//analog output
	else if (x == 4) {
		std::cout << "Printing Analog Outputs\n";
		for (int c = 0; c < 231; c++) {
			skip = x + 231;

			if (skip < 461) {
				first = "[" + std::to_string(c) + "] - " + this->aop[c].Name + " ~ " + std::to_string(this->aop[c].currentValue);
				second = "[" + std::to_string(skip) + "] - " + this->aop[skip].Name + " ~ " + std::to_string(this->aop[skip].currentValue);

				third = first.substr(first.find("~") + 1);
				fourth = second.substr(second.find("~") + 1);

				//Formatting
				while (true) {
					if (first.size() < 55) {
						first = first + " ";
					}
					else if (first.size() > 55) {
						first = first.substr(0, first.find("~") + 1);
						while (first.size() > 40) {
							first = first.substr(0, first.size() - 1);
						}
						first = first + third;
					}

					if (second.size() < 55) {
						second = second + " ";
					}

					else if (second.size() > 55) {
						second = second.substr(0, second.find("~") + 1);
						while (second.size() > 40) {
							second = second.substr(0, second.size() - 1);
						}
						second = second + fourth;
					}

					if (first.size() == 55 && second.size() == 55) {
						break;
					}
				}



				std::cout << first + " | " + second << std::endl;

			}
		}
	}

	//counter
	else if (x == 5) {
		std::cout << "Printing Counters\n";
		for (int d = 0; d < 2; d++) {
			skip = d + 2;
			first = "[" + std::to_string(d) + "] - " + this->cou[d].Name + " ~ " + std::to_string(this->cou[d].currentValue);
			second = "[" + std::to_string(skip) + "] - " + this->cou[skip].Name + " ~ " + std::to_string(this->cou[skip].currentValue);
			if (skip < 4) {

				third = first.substr(first.find("~") + 1);
				fourth = second.substr(second.find("~") + 1);

				//Formatting
				while (true) {
					if (first.size() < 55) {
						first = first + " ";
					}
					else if (first.size() > 55) {
						first = first.substr(0, first.find("~") + 1);
						while (first.size() > 40) {
							first = first.substr(0, first.size() - 1);
						}
						first = first + third;
					}

					if (second.size() < 55) {
						second = second + " ";
					}

					else if (second.size() > 55) {
						second = second.substr(0, second.find("~") + 1);
						while (second.size() > 40) {
							second = second.substr(0, second.size() - 1);
						}
						second = second + fourth;
					}

					if (first.size() == 55 && second.size() == 55) {
						break;
					}
				}




				std::cout << first + " | " + second << std::endl;
			}
		}
	}

	//device attributes
	else if (x == 6) {
		std::cout << "Printing Device Attributes\n";
		for (int e = 0; e < 23; e++) {
			skip = e + 23;

			if (skip < 45) {


				first = "[" + std::to_string(e) + "] - " + this->cou[e].Name + " ~ " + std::to_string(this->cou[e].currentValue);
				second = "[" + std::to_string(skip) + "] - " + this->cou[skip].Name + " ~ " + std::to_string(this->cou[skip].currentValue);
				third = first.substr(first.find("~") + 1);
				fourth = second.substr(second.find("~") + 1);

				//Formatting
				while (true) {
					if (first.size() < 55) {
						first = first + " ";
					}
					else if (first.size() > 55) {
						first = first.substr(0, first.find("~") + 1);
						while (first.size() > 40) {
							first = first.substr(0, first.size() - 1);
						}
						first = first + third;
					}

					if (second.size() < 55) {
						second = second + " ";
					}

					else if (second.size() > 55) {
						second = second.substr(0, second.find("~") + 1);
						while (second.size() > 40) {
							second = second.substr(0, second.size() - 1);
						}
						second = second + fourth;
					}

					if (first.size() == 55 && second.size() == 55) {
						break;
					}
				}



				std::cout << first + " | " + second << std::endl;
			}
		}
	}

	else {
		std::cout << "Invalid Type Argument \n";
	}


}

void PointInit::printNames() {
	int x;

	std::cout << "------------------------------------------------BINARY INPUTS----------------------------------------------------\n";
	for (x = 0; x < 107; x++) {
		std::cout << "BI" << x << "  -  " << this->bi[x].Name << std::endl;
	}

	std::cout << "------------------------------------------------BINARY OUTPUTS----------------------------------------------------\n";
	for (x = 0; x < 42; x++) {
		std::cout << "BO" << x << "  -  " << this->bop[x].Name << std::endl;
	}

	std::cout << "------------------------------------------------ANALOG INPUTS----------------------------------------------------\n";
	for (x = 0; x < 569; x++) {
		std::cout << "AI" << x << "  -  " << this->ai[x].Name << std::endl;
	}

	std::cout << "------------------------------------------------ANALOG OUTPUTS----------------------------------------------------\n";
	for (x = 0; x < 461; x++) {
		std::cout << "AO" << x << "  -  " << this->aop[x].Name << std::endl;
	}

	std::cout << "------------------------------------------------COUNTERS----------------------------------------------------------\n";
	for (x = 0; x < 4; x++) {
		std::cout << "CO" << x << "  -  " << this->cou[x].Name << std::endl;
	}

	std::cout << "------------------------------------------------DEVICE ATTRIBUTES----------------------------------------------------\n";
	for (x = 0; x < 45; x++) {
		std::cout << "DA" << x << "  -  " << this->da[x].Name << std::endl;
	}
}

void PointInit::printValues() {
	int x;

	std::cout << "PRINTING POINTS WITH VALUES \n\n";

	std::cout << "------------------------------------------------BINARY INPUTS----------------------------------------------------\n";
	for (x = 0; x < 107; x++) {
		std::cout << "BI" << x << "  -  " << this->bi[x].currentValue << std::endl;
	}

	std::cout << "------------------------------------------------BINARY OUTPUTS----------------------------------------------------\n";
	for (x = 0; x < 42; x++) {
		std::cout << "BO" << x << "  -  " << this->bop[x].currentValue << std::endl;
	}

	std::cout << "------------------------------------------------ANALOG INPUTS----------------------------------------------------\n";
	for (x = 0; x < 569; x++) {
		std::cout << "AI" << x << "  -  " << this->ai[x].currentValue << std::endl;
	}

	std::cout << "------------------------------------------------ANALOG OUTPUTS----------------------------------------------------\n";
	for (x = 0; x < 461; x++) {
		std::cout << "AO" << x << "  -  " << this->aop[x].currentValue << std::endl;
	}

	std::cout << "------------------------------------------------COUNTERS----------------------------------------------------------\n";
	for (x = 0; x < 4; x++) {
		std::cout << "CO" << x << "  -  " << this->cou[x].currentValue << std::endl;
	}

}

void PointInit::printAllData() {
	BinaryInput arr1[107];
	BinaryOutput arr2[42];
	AnalogInput arr3[569];
	AnalogOutput arr4[461];
	CounterPoint arr5[4];
	DeviceAttribute arr6[45];

	for (int x = 0; x < 107; x++) {
		arr1[x] = this->bi[x];
//		std::cout << "arr1: " << x << std::endl;
	}

	for (int a = 0; a < 42; a++) {
		arr2[a] = this->bop[a];
//		std::cout << "arr2: " << a << std::endl;
	}

	for (int b = 0; b < 569; b++) {
		arr3[b] = this->ai[b];
//		std::cout << "arr3: " << b << std::endl;
	}

	for (int d = 0; d < 461; d++) {
		arr4[d] = this->aop[d];
//		std::cout << "arr4: " << d << std::endl;
	}

	for (int c = 0; c < 4; c++) {
		arr5[c] = this->cou[c];
//		std::cout << "arr5: " << c << std::endl;
	}

	for  (int j = 0; j < 45; j++) {
		arr6[j] = this->da[j];
	}

	std::cout << " THE ARRAYS ARE FILLED \n";
	int f = 0;
	int e = 0;
	int g = 0;
	int h = 0;
	int i = 0;
	int k = 0;
	int s1 = (sizeof(arr1)/sizeof(*arr1));
	int s2 = (sizeof(arr2)/sizeof(*arr2));
	int s3 = (sizeof(arr3)/sizeof(*arr3));
	int s4 = (sizeof(arr4)/sizeof(*arr4));
	int s5 = (sizeof(arr5)/sizeof(*arr5));
	int s6 = (sizeof(arr6)/sizeof(*arr6));

	std::cout << "------------------------------------------------BINARY INPUTS----------------------------------------------------\n";
	while (f < s1) {
		std::cout << "BI" << f << "  - Current Value: " << arr1[f].currentValue << std::endl
		<< "  Name: " << arr1[f].Name << std::endl;
		std::cout << "\t Default Event Class: " << arr1[f].DefEvtClass << "  - State0: " << arr1[f].State0 << "   - State1:  " << arr1[f].State1
		<< "\n\t LnClass: " << arr1[f].LnClass << "  - DataObj" << arr1[f].DataObj << "  - CDC: " << arr1[f].Cdc;
		std::cout << "\n\t Unique: " << arr1[f].Unique << "  - Ref: " << arr1[f].Reference << std::endl << std::endl;
		f++;
	}

	std::cout << "------------------------------------------------BINARY OUTPUTS----------------------------------------------------\n";
	while (e < s2) {
		std::cout << "BO" << e << "  - Current Value: " << arr2[e].currentValue << std::endl
		<< "  Name: " << arr2[e].Name << std::endl;
		std::cout << "\t Default Event Class: " << arr2[e].DefEvtClass << "  - DefCmdClass: " << arr2[e].DefCmdClass
		<< "\n\t State0: " << arr2[e].State0 << "   - State1:  " << arr2[e].State1;
		std::cout << "\n\t LnClass: " << arr2[e].LnClass << "  - DataObj" << arr2[e].DataObj << "  - CDC: " << arr2[e].Cdc
		<< "\n\t Unique: " << arr2[e].Unique << "  - Ref: " << arr2[e].Reference << std::endl;
		std::cout << "\t LinkedInput: " << arr2[e].linkedInput << std::endl << std::endl;
		e++;
	}

	std::cout << "------------------------------------------------ANALOG INPUTS----------------------------------------------------\n";
	while  (g < s3) {
		std::cout << "AI" << g <<  "  - Current Value: " << arr3[g].currentValue << std::endl
		<< "  Name: " << arr3[g].Name << std::endl;
		std::cout << "\t Default Event Class: " << arr3[g].DefEvtClass << "  - Units: " << arr3[g].units
		<< "\n\t MinTx: " << arr3[g].MinTx << "  - MaxTx: " << arr3[g].MaxTx;
		std::cout << "\n\t Scaling Multiplier: " << arr3[g].ScalingMult << "  - Scaling Offset: " << arr3[g].ScalingOffset
		<< "\n\t DataObj: " << arr3[g].DataObj << "  - CDC: " << arr3[g].Cdc << "  - Reference: " << arr3[g].Reference << std::endl << std::endl;
		g++;
	}

	std::cout << "------------------------------------------------ANALOG OUTPUTS----------------------------------------------------\n";
	while (h < s4) {
		std::cout << "AO" << h << "  - Current Value: " << arr4[h].currentValue << std::endl
		<< "  Name: " << arr4[h].Name
		<< "\n\t MinTx: " << arr4[h].MinTx << "  - MaxTx: " << arr4[h].MaxTx
		<< "\n\t Scaling Multiplier: " << arr4[h].ScalingMult << "  - Scaling Offset: " << arr4[h].ScalingOffset << "  - Units: " << arr4[h].Units
		<< "\n\t DataObj: " << arr4[h].DataObj << "  - CDC: " << arr4[h].Cdc << "  - Reference: " << arr4[h].Reference << std::endl;
		std::cout << "\t LinkedInput: " << arr4[h].linkedInput << std::endl << std::endl;
		h++;
	}

	std::cout << "------------------------------------------------COUNTERS----------------------------------------------------------\n";
	while (i < s5) {
		std::cout << "CO" << i << "  - Current Value: " << arr5[i].currentValue << std::endl
		<< "  Name: " << arr5[i].Name << std::endl;
		std::cout << "\t Default Class: " << arr5[i].DefClass << "  - Frozen Counter: " << arr5[i].FrozenCounter
		<< "\n\t Default Frozen Counter Assigned: " << arr5[i].DefFrozenCountAssigned << "  - LnClass: " << arr5[i].LnClass;
		std::cout << "\n\t LnInstance: " << arr5[i].LnInstance << "  - DataObj: " << arr5[i].DataObj
		<< "\n\t Cdc: " << arr5[i].Cdc << "  - Reference: " << arr5[i].Reference << std::endl << std::endl;
		i++;
	}

	std::cout << "------------------------------------------------DEVICE ATTRIBUTES----------------------------------------------------\n";
	while (k < s6) {
		std::cout << "DA" << k+211 << " : " << arr6[k].Name << std::endl
		<< "\t LnClass: " << arr6[k].LnClass << "  - LnInstance: " << arr6[k].LnInst
		<< "\n\t DataObj: " << arr6[k].LnClass << "  - Cdc: " << arr6[k].Cdc << std::endl << std::endl;
		k++;
	}
}

BinaryInput PointInit::getBinaryIn(int x) {
	int s1 = 107;
	BinaryInput b;

	if (x < s1 && x >= 0) {
		return this->bi[x];
	}

	else {
		b.Name = "FAILED VALUE";
		return b;
	}
}

BinaryOutput PointInit::getBinaryOut(int x) {
	int s1 = 43;
	BinaryOutput b;

	if (x < s1 && x >= 0) {
		return this->bop[x];
	}

	else {
		b.Name = "FAILED VALUE";
		return b;
	}
}

AnalogInput PointInit::getAnalogIn(int x) {
	int s1 = 569;
	AnalogInput a;

	if (x < s1 && x >= 0) {
		return this->ai[x];
	}

	else {
		a.Name = "FAILED VALUE";
		return a;
	}
}

AnalogOutput PointInit::getAnalogOut(int x) {
	int s1 = 461;
	AnalogOutput a;

	if (x < s1 && x >= 0) {
		return this->aop[x];
	}

	else {
		a.Name = "FAILED VALUE";
		return a;
	}
}

CounterPoint PointInit::getCounter(int x) {
	int s1 = 4;
	CounterPoint c;

	if (x < s1 && x >= 0) {
		return this->cou[x];
	}

	else {
		c.Name = "FAILED VALUE";
		return c;
	}
}

DeviceAttribute PointInit::getDeviceAttribute(int x) {
	int s1 = 45;
	DeviceAttribute d;

	if (x < s1 && x >= 0) {
		return this->da[x];
	}

	else {
		d.Name = "FAILED VALUE";
		return d;
	}
}

BinaryInput * PointInit::getBinaryInputs() {
	return this->bi;
}

BinaryOutput * PointInit::getBinaryOutputs() {
	return this->bop;
}

CounterPoint * PointInit::getCounters() {
	return this->cou;
}

AnalogInput * PointInit::getAnalogInputs() {
	return this->ai;
}

AnalogOutput * PointInit::getAnalogOutputs() {
	return this->aop;
}

DeviceAttribute * PointInit::getDeviceAttributes() {
	return this->da;
}

/*
A method to initialize the array variables for each type of data.
*/
void PointInit::setPoint(string type, string point, int index) {
	string holdstring = point;
	string hold;
	string pointHold[25];
	int x = 0;
	int y = 0;
	string check = "";

//	std::cout << point << std::endl << std::endl;
	while (holdstring.size() > 1 && holdstring != check) {
		pointHold[x] = holdstring.substr(0, holdstring.find(','));
//		std::cout << type << " [" << x << "]: " << pointHold[x] << std::endl;
		check = holdstring;
		holdstring = holdstring.substr(holdstring.find(',') + 1);
		x++;
	}

	stringstream in1(pointHold[1]);
	stringstream in2(pointHold[2]);
	stringstream in3(pointHold[3]);
	stringstream in4(pointHold[4]);
	stringstream in5(pointHold[5]);
	stringstream in6(pointHold[6]);

	if (type == "bi") {
		BinaryInput bIn;
		bIn.Name = pointHold[1];
		in1 >> y;
		bIn.DefEvtClass = y;
		bIn.State1 = pointHold[2];
		bIn.State0 = pointHold[3];
		bIn.LnClass = pointHold[4];
		bIn.DataObj = pointHold[5];
		bIn.Cdc = pointHold[6];
		bIn.Unique = pointHold[7];
		bIn.Reference = pointHold[8];
		bIn.currentValue = NULL;

		this->bi[index] = bIn;
	}

	else if (type == "bo") {
		BinaryOutput bOp;
		bOp.Name = pointHold[1];
		bOp.State0 = pointHold[2];
		bOp.State1 = pointHold[3];
		in4 >> y;
		bOp.DefCmdClass = y;
		in5 >> y;
		bOp.DefEvtClass = y;
		bOp.LnClass = pointHold[6];
		bOp.DataObj = pointHold[7];
		bOp.Cdc = pointHold[8];
		bOp.Unique = pointHold[9];
		bOp.Reference = pointHold[10];
		bOp.currentValue = NULL;
		bOp.linkedInput = -1;

//		std::cout << pointHold[9] << ": hold[9]" << std::endl;
		this->bop[index] = bOp;
//		std::cout << "IS THIS THE BREAK" << std::endl;
	}

	else if (type == "ai") {
		AnalogInput aIn;
		aIn.Name =  pointHold[1];
		in2 >> y;
		aIn.DefEvtClass = y;
		in3 >> y;
		aIn.MinTx = y;
		in4 >> y;
		aIn.MaxTx = y;
		in5 >> y;
		aIn.ScalingMult = y;
		in6 >> y;
		aIn.ScalingOffset = y;
		aIn.units = pointHold[7];
		aIn.DataObj = pointHold[8];
		aIn.Cdc = pointHold[9];
		aIn.Reference = pointHold[10];
		aIn.currentValue = NULL;
		this->ai[index] = aIn;
	}


	else if (type == "ao") {
		AnalogOutput aOp;
		aOp.Name = pointHold[1];
		in2 >> y;
		aOp.MinTx = y;
		in3 >> y;
		aOp.MaxTx = y;
		in4 >> y;
		aOp.ScalingMult = y;
		in5 >> y;
		aOp.ScalingOffset = y;
		aOp.Units = pointHold[6];
		aOp.LnClass = pointHold[7];
		aOp.DataObj = pointHold[8];
		aOp.Cdc = pointHold[9];
		aOp.Reference = pointHold[10];
		aOp.currentValue = NULL;
		aOp.linkedInput = -1;

		this->aop[index] = aOp;
	}

	else if (type == "co") {
		CounterPoint cPo;
		cPo.Name = pointHold[1];
		cPo.DefClass = pointHold[2];
		cPo.FrozenCounter = pointHold[3];
		in4 >> y;
		cPo.DefFrozenCountAssigned = y;
		cPo.LnClass = pointHold[5];
		in6 >> y;
		cPo.LnInstance = y;
		cPo.DataObj = pointHold[7];
		cPo.Cdc = pointHold[8];
		cPo.Reference = pointHold[9];
		cPo.currentValue = NULL;
		this->cou[index] = cPo;


//		this->cop[index] = cPo;
	}

	else if (type == "da") {
		DeviceAttribute dAt;
		dAt.Name = pointHold[1];
		dAt.LnClass = pointHold[2];
		in3 >> y;
		dAt.LnInst = y;
		dAt.DataObj = pointHold[4];
		dAt.Cdc = pointHold[5];
		this->da[index] = dAt;
	}
}

/*
A method to parse csv files and initialize the DNP type variable arrays for this instance of PointInit.
*/
void PointInit::fillDataSets() {
	string line, line2, line3, line4, line5, line6;
	std::ifstream file, file2, file3, file4, file5, file6;
	int x = 0;

	file.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNPPointerList_BinaryInputs_SC.csv");
	while (!file.eof() && x < 108) {
		getline(file, line, '\n');
		setPoint("bi", line, x);
		x++;
	}
	file.close();
	x = 0;

	file2.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNPPointerList_BinaryOutputs_SC.csv");
	while (!file2.eof() && x < 42) {
		getline(file2, line2, '\n');
		setPoint("bo", line2, x);
		x++;
	}
	file2.close();
	x = 0;


	file3.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNPPointerList_AnalogInputs_SC.csv");
	while (!file3.eof() && x < 569) {
		getline(file3, line3, '\n');
		setPoint("ai", line3, x);
		x++;
	}
	file3.close();
	x = 0;

	file4.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNPPointerList_AnalogOutputs_SC.csv");
	while (!file4.eof() && x < 461) {
		getline(file4, line4, '\n');
		setPoint("ao", line4, x);
		x++;
	}
	file4.close();
	x = 0;

	file5.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNPPointerList_Counters_SC.csv");
	while (!file5.eof() && x < 4) {
		getline(file5, line5, '\n');
		setPoint("co", line5, x);
		x++;
	}
	file5.close();
	x = 0;

	file6.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNPPointer_DeviceAttributes.csv");
	while (!file6.eof() && x < 45) {
		getline(file6, line6, '\n');
		setPoint("da", line6, x);
		x++;
	}
	file6.close();

	std::cout << "Setting current values\n";

	setCurrentValues();

	std::cout << "\n FINISHED ASSIGNING VALUES \n";
}


/*
The default construcror for PointInit
*/
PointInit::PointInit() {
	fillDataSets();
	setLinks();
}
