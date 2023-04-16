#pragma once
#include "wholeinclude_orientation_visualizer.h"
#include "logic_data.h"
#include "Benchmark.h"

static std::exception global_exception;


namespace AutoDrones
{
	//this interprets the buffer data into the passed logic_data object which's pointer is then also returned
	logic_data* interpretData(std::size_t bytes_transferred,char* buffer, logic_data* _logic_data);

	//this updates the console screen with the updated data from the buffer, totally flicker free and does support user moving of the cursor
	void print_screen(std::size_t bytes_transferred, char* buffer);

	//this continously reads data from the serialport into a buffer and prints it to the console
	void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred, char* buffer, boost::asio::serial_port& serial_port, logic_data* _logic_data);
	
	//this initiates the reading of data from the serialport into a buffer, continued by the handle_read() function
	void readSerial(boost::asio::io_service& io, boost::asio::serial_port& serialport, logic_data* _logic_data);

	//lists all available COM Ports and returns the users choice in string format e.g. std::string("COM9")
	std::string choose_serial(boost::asio::io_service& io);
}