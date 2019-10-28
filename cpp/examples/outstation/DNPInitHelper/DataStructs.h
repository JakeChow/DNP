#ifndef OPENDNP3_DATASTRUCTS_H
#define OPENDNP3_DATASTRUCTS_H

#include <iostream>
#include <string>
#include <thread>

using  namespace std;

typedef struct BinaryInput
{
	string Name;
	bool DefEvtClass;
	string  State1;
	string State0;
	string LnClass;
	string DataObj;
	string Cdc;
	string Unique;
	string Reference;
	bool currentValue;
} BinaryInput ;

typedef struct BinaryOutput
{
	string Name;
	string State0;
	string State1;
	int DefCmdClass;
	int DefEvtClass;
	string LnClass;
	string DataObj;
	string Cdc;
	string Unique;
	string Reference;
	bool currentValue;
	int linkedInput;
} BinaryOutput ;

typedef struct CounterPoint
{
	string Name;
	string DefClass;
	string FrozenCounter;
	int DefFrozenCountAssigned;
	string LnClass;
	int LnInstance;
	string DataObj;
	string Cdc;
	string Reference;
	int currentValue;
} CounterPoint ;

typedef struct AnalogInput
{
	string Name;
	int DefEvtClass;
	int MinTx;
	int MaxTx;
	int ScalingMult;
	int ScalingOffset;
	string units;
	string DataObj;
	string Cdc;
	string Reference;
	int currentValue;
} AnalogInput ;

typedef struct AnalogOutput
{
	string Name;
	int MinTx;
	int MaxTx;
	int ScalingMult;
	int ScalingOffset;
	string Units;
	string LnClass;
	string DataObj;
	string Cdc;
	string Reference;
	int currentValue;
	int linkedInput;
} AnalogOutput ;

typedef struct DeviceAttribute
{
	string Name;
	string LnClass;
	int LnInst;
	string DataObj;
	string Cdc;
} DeviceAttribute ;

class PointInit {
public:
	PointInit();
	void setPoint(string type, string point, int index);
	void fillDataSets();
	void printAllData();
	void printValues();
	void printNames();
	void printType(int x);
	void setCurrentValues();
	void setLinks();
	int getLink(int x, string type);

	BinaryInput getBinaryIn(int x);
	BinaryOutput getBinaryOut(int x);
	AnalogInput getAnalogIn(int x);
	AnalogOutput getAnalogOut(int x);
	CounterPoint getCounter(int x);
	DeviceAttribute getDeviceAttribute(int x);

	BinaryInput * getBinaryInputs();
	BinaryOutput * getBinaryOutputs();
	CounterPoint * getCounters();
	AnalogInput * getAnalogInputs();
	AnalogOutput * getAnalogOutputs();
	DeviceAttribute * getDeviceAttributes();
private:
	BinaryInput bi[107];
	BinaryOutput bo[42];
	BinaryOutput bop[43];
	DeviceAttribute da[45];
	CounterPoint cou[4];
	AnalogInput ai[569];
	AnalogOutput aop[461];
};

#endif
