
///
/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <asiopal/UTCTimeSource.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/outstation/IUpdateHandler.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include "/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DataStructs.h"
#include "/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DataStructs.cpp"
#include <asiodnp3/ConsoleLogger.h>
#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/PrintingChannelListener.h>
#include <asiodnp3/PrintingSOEHandler.h>
#include <asiodnp3/UpdateBuilder.h>

#include <iostream>
#include <string>
#include <thread>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;

void ConfigureDatabase(DatabaseConfig& config, int bSize, int aSize, int cSize, PointInit& points)
{
    // example of configuring analog index 0 for Class2 with floating point variations by default
    // std::cout << config.analog[0].clazz << std::endl;
	int x = 0;
	BinaryInput bi[107] = *points.getBinaryInputs();
	AnalogInput ai[569] = *points.getAnalogInputs();

	while (x < bSize) {
		config.binary[x].clazz = PointClass::Class2;
		config.binary[x].svariation = StaticBinaryVariation::Group1Var1;

		if (bi[x].DefEvtClass = 1) {
			config.binary[x].evariation = EventBinaryVariation::Group2Var2;
		}

		else if (bi[x].DefEvtClass = 2) {
			config.binary[x].evariation = EventBinaryVariation::Group2Var2;
		}

		else if (bi[x].DefEvtClass = 3) {
			config.binary[x].evariation = EventBinaryVariation::Group2Var3;
		}

		else {
			config.binary[x].evariation = EventBinaryVariation::Group2Var1;
		}
		x++;
	}
	x = 0;

	while (x < aSize) {
		config.analog[x].clazz = PointClass::Class2;
		config.analog[x].svariation = StaticAnalogVariation::Group30Var1;

		if (ai[x].DefEvtClass == 1) {
			config.analog[x].evariation = EventAnalogVariation::Group32Var1;
		}

		else if (ai[x].DefEvtClass == 2) {
			config.analog[x].evariation = EventAnalogVariation::Group32Var2;
		}

		else if (ai[x].DefEvtClass == 3) {
			config.analog[x].evariation = EventAnalogVariation::Group32Var3;
		}

		else {
			config.analog[x].evariation = EventAnalogVariation::Group32Var1;
		}

		x++;

	}
	x = 0;

	while (x < cSize) {
		config.counter[x].clazz = PointClass::Class2;
		config.counter[x].svariation = StaticCounterVariation::Group20Var1;
		config.counter[x].evariation = EventCounterVariation::Group22Var1;
		x++;
	}
}

struct State
{
    uint32_t count = 0;
    double value = 0;
    bool binary = false;
    DoubleBit dbit = DoubleBit::DETERMINED_OFF;
    uint8_t octetStringValue = 1;
};

void clearComms() {
	std::ofstream ofile;
	ofile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/CrobCall.txt", std::ios::out | std::ios::trunc);
	ofile.close();

	ofile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/AnalogCall.txt", std::ios::out | std::ios::trunc);
	ofile.close();

	ofile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/CrobCallLog.txt", std::ios::out | std::ios::trunc);
	ofile.close();

	ofile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/AnalogCallLog.txt", std::ios::out | std::ios::trunc);
	ofile.close();

}

void copyComms() {
	std::ifstream ifile;
	std::ofstream ofile;
	string hold;

	ofile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/CrobCallLog.txt", std::ios::out | std::ios::app);
	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/CrobCall.txt");
	while (!ifile.eof()) {
		getline(ifile, hold);
		ofile << hold;
	}
	ofile << std::endl;

	ifile.close();
	ofile.close();

	ofile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/AnalogCallLog.txt", std::ios::out | std::ios::app);
	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/AnalogCall.txt");
	while (!ifile.eof()) {
		getline(ifile, hold);
		ofile << hold;
	}
	ofile << std::endl;

	ifile.close();
	ofile.close();

	ofile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/AnalogCall.txt", std::ios::out | std::ios::trunc);
	ofile.close();

	ofile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/CrobCall.txt", std::ios::out | std::ios::trunc);
	ofile.close();

}

void commandHandler(UpdateBuilder& builder, State& state, PointInit& points) {
	std::ifstream ifile;
	string line;
	string chop;
	std::size_t found;
	int newInd;
	int outputInd;
	int newVal;

	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/AnalogCall.txt", ifstream::in);

	while (!ifile.eof()) {
		getline(ifile, line);
//		std::cout << "HEY LINE" << line << std::endl;
		found = line.find(":");
		if (found!=std::string::npos) {
//			std::cout << "THE LINE: " << line << std::endl;
			chop = line.substr(0, found);
			stringstream val(chop);
			val >> outputInd;
			newInd = points.getLink(outputInd, "a");
			line = line.substr(line.find(":") + 1, line.find("\n"));
			stringstream val3(line);
			val3 >> newVal;
//			std::cout << "Line: " << line << std::endl;
			std::cout << "AI: " << newInd << " - AO: " << outputInd << " - Val: " << line << std::endl;
			builder.Update(Analog(newVal), newInd);
		}
		else {
			std::cout << "This is a false line\n";
		}
	}

	ifile.close();

	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/CrobCall.txt", ifstream::in);

	while (!ifile.eof()) {
		getline(ifile, line);
//		std::cout << "HEY DUDE" << line << std::endl;
		found = line.find(":");

		if (found!=std::string::npos) {
//			std::cout << "THIS THE REALEST LINE I EVER SAW: " << line << std::endl;
			chop = line.substr(0, found);
			stringstream val2(chop);
			val2 >> outputInd;
			newInd = points.getLink(outputInd, "b");
			line = line.substr(line.find(":") + 1, line.find("\n"));
			stringstream val4(line);
			val4 >> newVal;
//			std::cout << "Line: " << line << std::endl;
			std::cout << "BI: " << newInd << " - BO: " << outputInd << " - Val: " << line << std::endl;
			builder.Update(Binary(newVal), newInd);
		}
		else {
//			std::cout << "This a really fake line\n";
		}
	}

	ifile.close();
}

bool sizeCheck() {
	std::ifstream ifile;
	int s1, s2;
	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/AnalogCall.txt", ifstream::in | ifstream::binary);

	if (!ifile.is_open()) {
		return false;
	}

	ifile.seekg(0, ios::end);
	s1 = ifile.tellg();
	ifile.close();

	ifile.open("/home/pi/opendnp3/cpp/examples/outstation/Logs/CrobCall.txt", ifstream::in | ifstream::binary);

	if (!ifile.is_open()) {
		return false;
	}

	ifile.seekg(0, ios::end);
	s2 = ifile.tellg();
	ifile.close();

	if (s1 > 0 || s2 > 0) {
		return true;
	}

	else {
		return false;
	}
}

void InitializeDataset(UpdateBuilder& builder, State& state, PointInit& points, int index, string flag) {
	if (flag == "A") {
//		std::cout << "ADDING  VALUE TO INDEX: " << index << "  of value: " << points.getAnalogIn(index).currentValue << std::endl;
		builder.Update(Analog(points.getAnalogIn(index).currentValue), index);
	}

	else if (flag == "B") {
		builder.Update(Binary(points.getBinaryIn(index).currentValue), index);
	}

	else if (flag == "C") {
		builder.Update(Counter(points.getCounter(index).currentValue), index);
	}
}


void AddUpdates(UpdateBuilder& builder, State& state, const std::string& arguments, PointInit& points);

int main(int argc, char* argv[])
{

    PointInit points;
    clearComms();
    int sz1 = 107;
    int sz2 = 43;
    int sz3 = 569;
    int sz4 = 461;
    int sz5 = 4;

    // Specify what log levels to use. NORMAL is warning and above
    // You can add all the comms logging by uncommenting below.
    const uint32_t FILTERS = levels::NORMAL | levels::ALL_COMMS;

    // This is the main point of interaction with the stack
    // Allocate a single thread to the pool since this is a single outstation
    // Log messages to the console
    DNP3Manager manager(4, ConsoleLogger::Create());

    // Create a TCP server (listener)
    auto channel = manager.AddTCPServer("server", FILTERS, ServerAcceptMode::CloseExisting, "192.168.20.58", 20000,
                                        PrintingChannelListener::Create());

    //Parameter Structure (Binary, Double Binary, AnalogIn, Counter, Frozen Counter, BinaryOut, AnalogOut, TimeInterval, Octets)
    DatabaseSizes size(sz1, 0, sz3, sz5, 0, sz2, sz4, 0, 0);
    // The main object for a outstation. The defaults are useable,
    // but understanding the options are important.
    OutstationStackConfig config(size);

    EventBufferConfig evsize = EventBufferConfig::AllTypes(sz4+sz5+sz2+sz4+sz1);

    // Specify the maximum size of the event buffers
    config.outstation.eventBufferConfig = evsize;

    // you can override an default outstation parameters here
    // in this example, we've enabled the oustation to use unsolicted reporting
    // if the master enables it
    config.outstation.params.allowUnsolicited = true;

    // You can override the default link layer settings here
    // in this example we've changed the default link layer addressing
    config.link.LocalAddr = 10;
    config.link.RemoteAddr = 1;

    config.link.KeepAliveTimeout = openpal::TimeDuration::Max();

    // You can optionally change the default reporting variations or class assignment prior to enabling the outstation
    ConfigureDatabase(config.dbConfig, sz1, sz3, sz5, points);

    // Create a new outstation with a log level, command handler, and
    // config info this	returns a thread-safe interface used for
    // updating the outstation's database.
    auto outstation = channel->AddOutstation("outstation", SuccessCommandHandler::Create(),
                                             DefaultOutstationApplication::Create(), config);

//    points.printValues();


    State st;

	for (int n = 0; n < sz1; n++) {
		UpdateBuilder build;
		InitializeDataset(build, st, points, n, "B");
		outstation->Apply(build.Build());
	}

	for (int m = 0; m < sz3; m++) {
		UpdateBuilder build2;
		InitializeDataset(build2, st, points, m, "A");
		outstation->Apply(build2.Build());
	}

	for (int o = 0; o < sz5; o++) {
		UpdateBuilder build3;
		InitializeDataset(build3, st, points, o, "C");
		outstation->Apply(build3.Build());
	}

    // Enable the outstation and start communications
    outstation->Enable();

    // variables used in example loop
    string input;
    State state;


    while (true)
    {
  //      std::cout << "Enter one or more measurement changes then press <enter>" << std::endl;
  //      std::cout << "c = counter, b = binary, d = doublebit, a = analog, o = octet string, 'quit' = exit" << std::endl;
//        std::cin >> input;

	if (sizeCheck()) {
		std::cout << "HEYO WE MADE IT YO \n\n";
		UpdateBuilder  b4;
		commandHandler(b4, state, points);
		outstation->Apply(b4.Build());
		std::cout << "Its been applied\n";
		copyComms();
	}
/*
        if (input == "quit")
            return 0; // DNP3Manager destructor cleanups up everything automatically
        else
        {
            // update measurement values based on input string
            UpdateBuilder builder;
            AddUpdates(builder, state, input, points);
            outstation->Apply(builder.Build());
        }

*/
    }

    return 0;
}

void AddUpdates(UpdateBuilder& builder, State& state, const std::string& arguments, PointInit& points)
{
	string user;
	int user2;

    for (const char& c : arguments)
    {
        switch (c)
        {
        case ('c'):
        {
            builder.Update(Counter(state.count), 0);
            ++state.count;
            break;
        }
        case ('a'):
        {
            builder.Update(Analog(state.value), 0);
            state.value += 1;
            break;
        }
        case ('b'):
        {
            builder.Update(Binary(state.binary), 0);
            state.binary = !state.binary;
            break;
        }
        case ('d'):
        {
            builder.Update(DoubleBitBinary(state.dbit), 0);
            state.dbit
                = (state.dbit == DoubleBit::DETERMINED_OFF) ? DoubleBit::DETERMINED_ON : DoubleBit::DETERMINED_OFF;
            break;
        }
	case ('p'):
	{
		std::cout <<  "Print all (a), names (n), values (v), type (t): ";
		std::cin >> user;

		if (user == "a") {
			points.printAllData();
		}

		else if (user == "n") {
			points.printNames();
		}

		else if (user == "v") {
			points.printValues();
		}

		else if (user == "t") {
			std::cout << "Print Binary Inputs (1), Binary Outputs (2), Analog Inputs (3)\n"
			<< "Analog Outputs (4), Counters (5), Device Attributes (6)\n";
			std::cin >> user2;

			points.printType(user2);
		}

		else {
			std::cout << "Invalid Option\n";
		}
		break;
	}

        case ('o'):
        {
            OctetString value(openpal::RSlice(&state.octetStringValue, 1));
            builder.Update(value, 0);
            state.octetStringValue += 1;
            break;
        }
        default:
            break;
        }
    }
}
