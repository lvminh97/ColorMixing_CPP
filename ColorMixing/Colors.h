#pragma once

#include "pch.h"
#include "types.h"

class Colors
{
private:
	double reflectionData[31] = { 0 };
	unsigned int RGB = 0;
	CIE_XYZ XYZ;
	CIE_LAB LAB;
	CIE_LCH LCH;
	bool isConverted = false;

private:
	double rgbFx(double x);
	double rgbAdjust(double x);

public:
	// constructor
	Colors();
	Colors(double data[31]);

	// getter, setter
	void setData(double data[31]);
	void setDataAt(int index, double value);
	void getData(double data[31]);
	double getDataAt(int index);

	// color params
	CIE_XYZ getXYZ(void);
	CIE_LAB getLAB(void);
	CIE_LCH getLCH(void);
	unsigned int getRGB(void);

	void reset(void);
	void convert(void);

	std::string getReflectionString(void);
};