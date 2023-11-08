#include "Colors.h"

Colors::Colors() {}

Colors::Colors(double data[31])
{
	setData(data);
}

void Colors::setData(double data[31])
{
	for (int i = 0; i < 31; i++)
	{
		reflectionData[i] = data[i];
	}
}

void Colors::setDataAt(int index, double value)
{
	reflectionData[index] = value;
}

void Colors::getData(double data[31])
{
	for (int i = 0; i < 31; i++)
	{
		data[i] = reflectionData[i];
	}
}

double Colors::getDataAt(int index)
{
	return reflectionData[index];
}

std::string Colors::getReflectionString(void)
{
	std::string res = "Wavelength(nm)\t|\tReflection\r\n";
	res += "------------------------------------------------------------------------------------";
	char tmpBuf[30];

	for (int i = 0; i < 31; i++)
	{
		res += "\r\n";
		sprintf_s(tmpBuf, "\t%d\t|\t%.4lf", 400 + i * 10, reflectionData[i]);
		res += tmpBuf;
	}
	return res;
}