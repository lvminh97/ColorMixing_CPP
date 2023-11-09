#include "pch.h"
#include "Colors.h"
#include "ColorParams.h"

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

CIE_XYZ Colors::getXYZ(void)
{
	if (isConverted == FALSE)
		convert();

	return XYZ;
}

CIE_LAB Colors::getLAB(void)
{
	if (isConverted == FALSE)
		convert();

	return LAB;
}

CIE_LCH Colors::getLCH(void)
{
	if (isConverted == FALSE)
		convert();

	return LCH;
}

unsigned int Colors::getRGB(void)
{
	if (isConverted == FALSE)
		convert();

	return RGB;
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

double Colors::rgbFx(double x)
{
	if (x > 0.008856)
		return pow(x, 0.3333);
	else
		return (903.3 * x + 16) / 116;
}

double Colors::rgbAdjust(double x)
{
	double res = 0;
	if (x <= -0.0031308)
		res = 0;
	else if (x <= 0.0031308)
		res = 12.92 * x;
	else
		res = 1.055 * pow(x, 0.41666) - 0.055;

	if (res >= 0 && res <= 1)
		return res;
	else if (res < 0)
		return 0;
	else
		return 1;
}

void Colors::reset(void)
{
	for (int i = 0; i < 31; i++)
	{
		reflectionData[i] = 0;
	}
	isConverted = FALSE;
}

void Colors::convert(void)
{
	if (isConverted)
		return;

	// convert CIEXYZ
	XYZ = {0, 0, 0};
	double K = 0;
	for (int i = 0; i < 31; i++)
	{
		K += D65_ILL[i] * OBSVR_2[1][i];
		XYZ.X += D65_ILL[i] * OBSVR_2[0][i] * reflectionData[i];
		XYZ.Y += D65_ILL[i] * OBSVR_2[1][i] * reflectionData[i];
		XYZ.Z += D65_ILL[i] * OBSVR_2[2][i] * reflectionData[i];
	}
	XYZ.X /= K;
	XYZ.Y /= K;
	XYZ.Z /= K;

	// convert CIELAB
	LAB.L = 116 * rgbFx(XYZ.Y / D65_REF[1]) - 16;
	LAB.a = 500 * (rgbFx(XYZ.X / D65_REF[0]) - rgbFx(XYZ.Y / D65_REF[1]));
	LAB.b = 200 * (rgbFx(XYZ.Y / D65_REF[1]) - rgbFx(XYZ.Z / D65_REF[2]));

	// convert CIELCH
	LCH.L = LAB.L;
	LCH.C = pow(LAB.a * LAB.a + LAB.b * LAB.b, 0.5);
	if (LAB.a != 0)
		LCH.H = atan(LAB.b / LAB.a);
	else if (LAB.b >= 0)
		LCH.H = 3.14159 / 2;
	else
		LCH.H = 3 * 3.14159 / 2;
	if (LAB.a < 0)
		LCH.H += 3.14159;
	// convert RGB
	double r, g, b;
	r = rgbAdjust(3.2404542 * XYZ.X - 1.5371385 * XYZ.Y - 0.4985314 * XYZ.Z);
	g = rgbAdjust(-0.969266 * XYZ.X + 1.8760108 * XYZ.Y + 0.0415560 * XYZ.Z);
	b = rgbAdjust(0.0556434 * XYZ.X - 0.2040259 * XYZ.Y + 1.0572252 * XYZ.Z);
	RGB = ((int) (r * 255)) | (((int) (g * 255)) << 8) | (((int) (b * 255)) << 16);

	isConverted = TRUE;
}