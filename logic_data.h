#pragma once
#include "wholeinclude_orientation_visualizer.h"

//this represents the Orientation of the SerialDevice taken that it provides its SensorData to the Softwares
class logic_data
{
	double magneto_x=0.0, magneto_y = 0.0, magneto_z = 0.0;
	double gyro_x = 0.0, gyro_y = 0.0, gyro_z = 0.0;


	// static pointer which will points
	//to the instance of this class

	

public:

	//static std::string line;

	double getGyroX()
	{
		return gyro_x;
	}
	double getGyroY()
	{
		return gyro_y;
	}
	double getGyroZ()
	{
		return gyro_z;
	}
	double getMagnetoX()
	{
		return magneto_x;
	}
	double getMagnetoY()
	{
		return magneto_y;
	}
	double getMagnetoZ()
	{
		return magneto_z;
	}

	void setGyroX(double x)
	{
		gyro_x = x;
	}
	void setGyroY(double y)
	{
		gyro_y = y;
	}
	void setGyroZ(double z)
	{
		gyro_z = z;
	}
	void setMagnetoX(double x)
	{
		magneto_x = x;
	}
	void setMagnetoY(double y)
	{
		magneto_y = y;
	}
	void setMagnetoZ(double z)
	{
		magneto_z = z;
	}

	void set_Magneto(double x, double y, double z)
	{
		magneto_x = x;
		magneto_y = y;
		magneto_z = z;
	}
	void set_Gyro(double x, double y, double z)
	{
		gyro_x = x;
		gyro_y = y;
		gyro_z = z;
	}

	void print()
	{
		const std::string format = "--------------------";
		std::cout << format << "\n";
		std::cout << "gyro_x: " << gyro_x << "   gyro_y: " << gyro_y << "   gyro_z: " << gyro_z << "\n";
		std::cout << "magneto_x: " << magneto_x << "   magneto_y: " << magneto_y << "   magneto_z: " << magneto_z << "\n";
		std::cout << format << "\n";

	}
};

