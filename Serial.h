#pragma once
#include "wholeinclude_orientation_visualizer.h"
#include "logic_data.h"
static std::exception global_exception;


namespace AutoDrones
{
	logic_data* interpretData(std::size_t bytes_transferred,char* buffer);
	void print_screen(std::size_t bytes_transferred, char* buffer);
	void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred, char* buffer, boost::asio::serial_port& serial_port, logic_data* _logic_data);
	void readSerial(boost::asio::io_service& io, boost::asio::serial_port& serialport, logic_data* _logic_data);
	std::string choose_serial(boost::asio::io_service& io);
}