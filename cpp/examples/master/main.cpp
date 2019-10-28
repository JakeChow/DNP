
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

#include <asiodnp3/ConsoleLogger.h>
#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/DefaultMasterApplication.h>
#include <asiodnp3/PrintingChannelListener.h>
#include <asiodnp3/PrintingCommandCallback.h>
#include <asiodnp3/PrintingSOEHandler.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sstream>

using namespace std;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace opendnp3;

//RESETS THE TRANSFERRED DATA LOGS BETWEEN OUTSTATION AND MASTER
void clearLogs() {
    std::ofstream ofile;
    ofile.open("/home/pi/opendnp3/cpp/examples/master/AnalogValues.txt", std::ofstream::out | std::ofstream::trunc);
    ofile.close();
    ofile.open("/home/pi/opendnp3/cpp/examples/master/BinaryValues.txt", std::ofstream::out | std::ofstream::trunc);
    ofile.close();
    ofile.open("/home/pi/opendnp3/cpp/examples/master/CounterValues.txt", std::ofstream::out | std::ofstream::trunc);
    ofile.close();
}

void printData() {
	std::ifstream ifile;
	string hold;

	ifile.open("/home/pi/opendnp3/cpp/examples/master/AnalogValues.txt");
	std::cout << "------------------------------ANALOG--------------------------------------\n";
	while (!ifile.eof()) {
		getline(ifile, hold);
		std::cout << hold << std::endl;
	}
	ifile.close();

	std::cout << "------------------------------BINARY--------------------------------------\n";
	ifile.open("/home/pi/opendnp3/cpp/examples/master/BinaryValues.txt");

	while (!ifile.eof()) {
		getline(ifile, hold);
		std::cout << hold << std::endl;
	}

	ifile.close();
}

//A RANGE USED FOR ANALOG OUTPUT MIN AND MAX VALUES
typedef struct Range
{
	int index;
	int min;
	int max;
} Range ;

int main(int argc, char* argv[])
{
    int count = 0;
    int val;
    string line;
    Range analogRanges[461];
    std::ifstream ifile;
    ifile.open("/home/pi/opendnp3/cpp/examples/outstation/DNPInitHelper/DNPPointerList_AnalogOutputs_SC.csv");

//FILLS THE RANGE ARRAY
    while (!ifile.eof()) {
	getline(ifile, line);
	line = line.substr(line.find(",")+1);
	line = line.substr(line.find(",")+1);
	if (count < 461) {
		analogRanges[count].index = count;
		stringstream holdVal(line.substr(0, line.find(",")));
		holdVal >> val;
		analogRanges[count].min = val;
		line = line.substr(line.find(",")+1);
		stringstream holdVal2(line.substr(0, line.find(",")));
		holdVal2 >> val;
		analogRanges[count].max = val;
	}
	count++;
    }

    std::ofstream ofile;
    string choice1 = "";
    string choice3 = "";
    int choice2 = -1;
    int choice4 = -1;
    int rmin;
    int rmax;
    int contCode;

//AN ARRAY WITH EACH CROB CONTROL OPTION
    ControlCode crobCalls[6] = {
            	ControlCode::LATCH_ON,
		ControlCode::LATCH_OFF,
		ControlCode::PULSE_ON,
		ControlCode::PULSE_OFF,
		ControlCode::TRIP_PULSE_ON,
		ControlCode::CLOSE_PULSE_ON };


    // Specify what log levels to use. NORMAL is warning and above
    // You can add all the comms logging by uncommenting below
    const uint32_t FILTERS = levels::NORMAL | levels::ALL_APP_COMMS;

    // This is the main point of interaction with the stack
    DNP3Manager manager(1, ConsoleLogger::Create());

    // Connect via a TCPClient socket to a outstation
    auto channel = manager.AddTCPClient("tcpclient", FILTERS, ChannelRetry::Default(), {IPEndpoint("192.168.20.57", 20000)},
                                        "0.0.0.0", PrintingChannelListener::Create());

    // The master config object for a master. The default are
    // useable, but understanding the options are important.
    MasterStackConfig stackConfig;

    // you can override application layer settings for the master here
    // in this example, we've change the application layer timeout to 2 seconds
    stackConfig.master.responseTimeout = TimeDuration::Seconds(2);
    stackConfig.master.disableUnsolOnStartup = true;

    // You can override the default link layer settings here
    // in this example we've changed the default link layer addressing
    stackConfig.link.LocalAddr = 1;
    stackConfig.link.RemoteAddr = 10;

    // Create a new master on a previously declared port, with a
    // name, log level, command acceptor, and config info. This
    // returns a thread-safe interface used for sending commands.
    auto master = channel->AddMaster("master",                                     // id for logging
                                     PrintingSOEHandler::Create(),                 // callback for data processing
                                     asiodnp3::DefaultMasterApplication::Create(), // master application instance
                                     stackConfig                                   // stack configuration
    );

    // do an integrity poll (Class 3/2/1/0) once per minute
//    auto integrityScan = master->AddClassScan(ClassField::AllClasses(), TimeDuration::Minutes(1));
//    auto integrityScan1 = master->AddClassScan(ClassField::(ClassField::CLASS1), TimeDuration::Minutes(1));
    auto integrityScan = master->AddClassScan(PointClass::Class2, TimeDuration::Minutes(1));

    // do a Class 1 exception poll every 5 seconds
    auto exceptionScan = master->AddClassScan(ClassField::AllClasses(), TimeDuration::Seconds(60));

    // Enable the master. This will start communications.
    master->Enable();

    bool channelCommsLoggingEnabled = true;
    bool masterCommsLoggingEnabled = true;

    usleep(500000);
    clearLogs();
    usleep(500000);
    integrityScan->Demand();


    while (true)
    {
        std::cout << "Enter a command" << std::endl;
        std::cout << "x - exits program" << std::endl;
        std::cout << "a - performs an ad-hoc range scan" << std::endl;
        std::cout << "i - integrity demand scan" << std::endl;
        std::cout << "e - exception demand scan" << std::endl;
        std::cout << "d - disable unsolicited" << std::endl;
        std::cout << "r - cold restart" << std::endl;
        std::cout << "c - send command" << std::endl;
        std::cout << "t - toggle channel logging" << std::endl;
        std::cout << "u - toggle master logging" << std::endl;

        char cmd;
        std::cin >> cmd;
        switch (cmd)
        {
        case ('a'):
            master->ScanRange(GroupVariationID(1, 2), 0, 3);
            break;
        case ('d'):
            master->PerformFunction("disable unsol", FunctionCode::DISABLE_UNSOLICITED,
                                    {Header::AllObjects(60, 2), Header::AllObjects(60, 3), Header::AllObjects(60, 4)});
            break;
        case ('r'):
        {
            auto print = [](const RestartOperationResult& result) {
                if (result.summary == TaskCompletion::SUCCESS)
                {
                    std::cout << "Success, Time: " << result.restartTime.GetMilliseconds() << std::endl;
                }
                else
                {
                    std::cout << "Failure: " << TaskCompletionToString(result.summary) << std::endl;
                }
            };
            master->Restart(RestartType::COLD, print);
            break;
        }
        case ('x'):
            // C++ destructor on DNP3Manager cleans everything up for you
            return 0;
	case ('p'):
		printData();
		break;
        case ('i'):
	       	    clearLogs();
	            integrityScan->Demand();
        	    break;
        case ('e'):
	            exceptionScan->Demand();
        	    break;

//////////////////////////START OF CROBS////////////////////////////////////////

        case ('c'):
        {
		choice1 = "";
		choice2 = -1;
		choice3 = "";
		choice4 = -1;
		rmin = NULL;
		rmax = NULL;
		contCode = -1;

		//DATA TYPE SELECTION
		while (choice1 != "a" || choice1 != "b") {
			std::cout << "Analog (a) or Binary (b) input: ";
			std::cin >> choice1;

			//INDEX SELECTION
			if (choice1 == "a") {
				while (choice2 < 0 || choice2 > 460) {
					std::cout << "Analog Range is 0-460\n What is the index: ";
					std::cin >> choice2;

					if (choice2 < 0 || choice2 > 460) {
						std::cout << "That index is out of range for binary inputs\n";
					}

					//ANALOG VALUE SELECTION W RANGES
					else {
						while (true) {
							rmin = analogRanges[choice2].min;
							rmax = analogRanges[choice2].max;
							std::cout << "Values can range between " << rmin << " - " << rmax << std::endl;
							std::cout << "What value would you like it set to: ";
							std::cin >> choice4;

							if (choice4 >= rmin && choice4 <= rmax) {
								break;
							}

							else {
								std::cout << "Selected Value is out of range\n";
								choice4 = NULL;
							}
						}
					}
				}
			}
			//BINARY INDEX SELECTION
			else if (choice1 == "b") {
				while (choice2 < 0 || choice2 > 42) {
					std::cout << "Binary Range is 0-42\n What is the index: ";
					std::cin >> choice2;

					if (choice2 < 0 || choice2 > 42) {
						std::cout << "That index is out of range for analog inputs\n";
					}
				}
			}

			if (choice1 == "a" || choice1 == "b") {
				break;
			}

			else if (choice1 != "a" || choice1 != "b") {
				std::cout << "Invalid input selection\n";
				std::cout << "Choice1: " << choice1 << "  - Choice2: " << choice2 << std::endl;
				choice1 = "";
				choice2 = -1;
			}
		}


	//OPERATION TYPE
	std::cout << "Direct Operate (d) or Select and Operate (s): ";
	std::cin >> choice3;

	if (choice3 != "d" || choice3 != "s") {
		std::cout << "Invalid operation type, defaulting to select and operate\n";
		choice3 = "s";
	}

	CommandSet comms;

	//ANALOG COMMAND CALL
	if (choice1  == "a") {
    		std::cout << "SENDING AO COMMAND CALL TO INDEX: " << choice2 << std::endl;
		AnalogOutputFloat32 ana(choice4);
		auto& header = comms.StartHeader<AnalogOutputFloat32>();

		header.Add(ana, choice2);

		if (choice3 == "d") {
			master->DirectOperate(std::move(comms), PrintingCommandCallback::Get());
			break;
		}

		else {
			master->SelectAndOperate(std::move(comms), PrintingCommandCallback::Get());
			break;
		}
	}

	//BINARY COMMAND CALL
	else if (choice1 == "b") {
		//CROB OPERATION SELECTION
		while (true) {
			std::cout << "Control Codes\n Pulse On (1), Pulse Off (2)\n Latch On (3), Latch Off (4)\n    Trip (5), Close (6)\n";
			std::cout << "Control Code: ";
			std::cin >> contCode;

			if (contCode > 0 && contCode < 7) {
				break;
			}

			else {
				std::cout << "Invalid Control Code \n";
			}
		}

    		std::cout << "SENDING CROB CALL TO INDEX: " << choice2 << std::endl;
		auto& header = comms.StartHeader<ControlRelayOutputBlock>();
		ControlRelayOutputBlock crob(crobCalls[contCode-1]);


		header.Add(crob, choice2);

		//BINARY COMMAND CALLS
		if (choice3 == "d") {
			master->DirectOperate(std::move(comms), PrintingCommandCallback::Get());
			break;
		}

		else {
        		master->SelectAndOperate(std::move(comms), PrintingCommandCallback::Get());
	        	break;
	        }
	}

	else {
		std::cout << "System error\n";
	}

	clearLogs();
	usleep(2000000);
	integrityScan->Demand();
	usleep(1000000);
	}

////////////////////////////END OF CROBS/////////////////////////////////////////////////

        case ('t'):
        {
            channelCommsLoggingEnabled = !channelCommsLoggingEnabled;
            auto levels = channelCommsLoggingEnabled ? levels::ALL_COMMS : levels::NORMAL;
            channel->SetLogFilters(levels);
            std::cout << "Channel logging set to: " << levels << std::endl;
            break;
        }
        case ('u'):
        {
            masterCommsLoggingEnabled = !masterCommsLoggingEnabled;
            auto levels = masterCommsLoggingEnabled ? levels::ALL_COMMS : levels::NORMAL;
            master->SetLogFilters(levels);
            std::cout << "Master logging set to: " << levels << std::endl;
            break;
        }
        default:
            std::cout << "Unknown action: " << cmd << std::endl;
            break;
        }
    }

    return 0;
}
