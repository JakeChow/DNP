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
#include "asiodnp3/PrintingSOEHandler.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace opendnp3;

void ClearLog(string log) {
	std::ofstream l;
	l.open(log, std::ios::out | std::ios::trunc);
	l.close();
}

void BinaryLog(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values)
{
	std::ofstream log;
	string hold = "";
	log.open("/home/pi/opendnp3/cpp/examples/master/BinaryValues.txt", std::ios::out | std::ios::app);

	auto print = [&](const Indexed<Binary>& pair) {
		hold = hold + "[BI" + std::to_string(pair.index) + "] : " + std::to_string(pair.value.value) + "\n";
	};

	values.ForeachItem(print);
	log << hold;
	log.close();
}

void AnalogLog(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values)
{
	std::ofstream log;
	string hold = "";
	log.open("/home/pi/opendnp3/cpp/examples/master/AnalogValues.txt", std::ios::out | std::ios::app);

	auto print = [&](const Indexed<Analog>& pair) {
		hold = hold + "[AI" + std::to_string(pair.index) + "] : " + std::to_string(pair.value.value) + "\n";
	};

	values.ForeachItem(print);
	log << hold;
	log.close();
}

void CounterLog(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values)
{
	std::ofstream log;
	string hold = "";
	log.open("/home/pi/opendnp3/cpp/examples/master/CounterValues.txt", std::ios::out | std::ios::app);

	auto print = [&](const Indexed<Counter>& pair) {
		hold = hold + "[CO" + std::to_string(pair.index) + "] : " + std::to_string(pair.value.value) + "\n";
	};

	values.ForeachItem(print);
	log << hold;
	log.close();
}

namespace asiodnp3
{


void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values)
{
    ClearLog("/home/pi/opendnp3/cpp/examples/master/BinaryValues.txt");
    BinaryLog(info, values);
    return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values)
{
    return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values)
{
//    ClearLog("/home/pi/opendnp3/cpp/examples/master/AnalogValues.txt");
    AnalogLog(info, values);
    return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values)
{
    ClearLog("/home/pi/opendnp3/cpp/examples/master/CounterValues.txt");
    CounterLog(info, values);
    return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values)
{
    return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
    return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
    return PrintAll(info, values);
}

void PrintingSOEHandler::Process(const HeaderInfo& /*info*/, const ICollection<Indexed<OctetString>>& values)
{
    auto print = [](const Indexed<OctetString>& pair) {
        std::cout << "OctetString "
                  << " [" << pair.index << "] : Size : " << pair.value.ToRSlice().Size() << std::endl;
    };

    values.ForeachItem(print);
}

void PrintingSOEHandler::Process(const HeaderInfo& /*info*/, const ICollection<Indexed<TimeAndInterval>>& values)
{
    auto print = [](const Indexed<TimeAndInterval>& pair) {
        std::cout << "TimeAndInterval: "
                  << "[" << pair.index << "] : " << pair.value.time << " : " << pair.value.interval << " : "
                  << IntervalUnitsToString(pair.value.GetUnitsEnum()) << std::endl;
    };

    values.ForeachItem(print);
}

void PrintingSOEHandler::Process(const HeaderInfo& /*info*/, const ICollection<Indexed<BinaryCommandEvent>>& values)
{
    auto print = [](const Indexed<BinaryCommandEvent>& pair) {
        std::cout << "BinaryCommandEvent: "
                  << "[" << pair.index << "] : " << pair.value.time << " : " << pair.value.value << " : "
                  << CommandStatusToString(pair.value.status) << std::endl;
    };

    values.ForeachItem(print);
}

void PrintingSOEHandler::Process(const HeaderInfo& /*info*/, const ICollection<Indexed<AnalogCommandEvent>>& values)
{
    auto print = [](const Indexed<AnalogCommandEvent>& pair) {
        std::cout << "AnalogCommandEvent: "
                  << "[" << pair.index << "] : " << pair.value.time << " : " << pair.value.value << " : "
                  << CommandStatusToString(pair.value.status) << std::endl;
    };

    values.ForeachItem(print);
}

void PrintingSOEHandler::Process(const HeaderInfo& /*info*/, const ICollection<Indexed<SecurityStat>>& values)
{
    auto print = [](const Indexed<SecurityStat>& pair) {
        std::cout << "SecurityStat: "
                  << "[" << pair.index << "] : " << pair.value.time << " : " << pair.value.value.count << " : "
                  << static_cast<int>(pair.value.quality) << " : " << pair.value.value.assocId << std::endl;
    };

    values.ForeachItem(print);
}

void PrintingSOEHandler::Process(const opendnp3::HeaderInfo& /*info*/,
                                 const opendnp3::ICollection<opendnp3::DNPTime>& values)
{
    auto print = [](const DNPTime& value) { std::cout << "DNPTime: " << value.value << std::endl; };

    values.ForeachItem(print);
}

} // namespace asiodnp3
