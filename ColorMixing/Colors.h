#pragma once

#include "pch.h"

class Colors
{
private:
	double reflectionData[31] = { 0 };

public:
	// constructor
	Colors();
	Colors(double data[31]);

	// getter, setter
	void setData(double data[31]);
	void setDataAt(int index, double value);
	void getData(double data[31]);
	double getDataAt(int index);

	std::string getReflectionString(void);
};