#include "Serial.h"
#include "Benchmark.h"
#include "logic_data.h"

//this interprets the buffer data into a logic_data object which is then returned as a pointer
logic_data* AutoDrones::interpretData(std::size_t bytes_transferred,char* buffer)
{
	AutoDrones::Benchmark benchmark;
	try {

		if (*buffer != '~')
			throw std::exception("Recieved SerialData  is incomplete.");

		std::vector<double> vec_logic_data = {};

		char* next_token = nullptr;
		char* token = strtok_s(buffer+1, " ", &next_token);

		//this loop splits the buffer data after every space character and inserts it into the vector
		while (token != nullptr) {
			vec_logic_data.push_back(std::atof(token));
			token = strtok_s(nullptr, " ", &next_token);
		}


		//this will eventually check for coherent input data inside the vector
		for (auto it : vec_logic_data)
		{
			/*if(!isdigit(it))
				throw std::exception("logic_data inside of vector isnt coherent.");*/
		}

		//creates a new logic_data object using the intrepeted data
		{
			logic_data* _logic_data = new logic_data;

			_logic_data->setGyroX(vec_logic_data[0]);
			_logic_data->setGyroY(vec_logic_data[1]);
			_logic_data->setGyroZ(vec_logic_data[2]);
			_logic_data->setMagnetoX(vec_logic_data[3]);
			_logic_data->setMagnetoY(vec_logic_data[4]);
			_logic_data->setMagnetoZ(vec_logic_data[5]);



			return _logic_data;
		}
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception: " << ex.what() << "\n" << std::endl;
		system("pause");
	}
}

//this updates the console screen with the updated data from the buffer, totally flicker free and does support user moving of the cursor
void AutoDrones::print_screen(std::size_t bytes_transferred, char* buffer)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout.flush();

	SetConsoleCursorPosition(hConsole, COORD(0, 0));

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	COORD cursorPos = csbi.dwCursorPosition;

	DWORD bytesWritten = 0;
	for (std::size_t i = 0; i < bytes_transferred; i++) {
		if (buffer[i] == '\n') {
			// Move cursor to the start of the next line
			cursorPos.X = 0;
			cursorPos.Y++;
		}
		else {
			// Write the character to the console screen buffer
			WriteConsoleOutputCharacterA(hConsole, &buffer[i], 1, cursorPos, &bytesWritten);
			cursorPos.X++;
			if (cursorPos.X == csbi.dwSize.X) {
				// If we've reached the end of the line, move to the next line
				cursorPos.X = 0;
				cursorPos.Y++;
			}
		}
	}

	// Set the cursor position to the end of the output
	SetConsoleCursorPosition(hConsole, cursorPos);


	std::cout.flush();
}

//this continously reads data from the serialport into a buffer and prints it to the console
void AutoDrones::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred, char* buffer, boost::asio::serial_port& serial_port, logic_data* _logic_data)
{

	if (!error) {
		AutoDrones::Benchmark();
		//prints recieved data to screen in a non flickering way

		_logic_data = AutoDrones::interpretData(bytes_transferred, buffer);

		//AutoDrones::print_screen(bytes_transferred, buffer);

		// Call async_read again to continue reading data
		boost::asio::async_read(serial_port, boost::asio::mutable_buffers_1(buffer, 2048), boost::asio::transfer_at_least(1),
			boost::bind(handle_read, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, buffer, boost::ref(serial_port)));

	}
	else
	{
		std::cout << "Error occured during handle_read().\n" << error.message() << "\n";
		std::cin.ignore();
	}
}

//this initiates the reading of data from the serialport into a buffer, continued by the handle_read() function
void AutoDrones::readSerial(boost::asio::io_service& io, boost::asio::serial_port& serialport, logic_data* _logic_data)
{
	//std::cout << "called readSerial()\n";

	try {
		//Port Options
		serialport.set_option(boost::asio::serial_port_base::baud_rate(9600));
		serialport.set_option(boost::asio::serial_port_base::character_size(8));
		serialport.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
		serialport.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
		serialport.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
		//

		char buffer[1024];
		std::string result_line = "";

		//reading SerialData Block
		boost::asio::async_read(serialport, boost::asio::mutable_buffers_1(buffer, 1024), boost::asio::transfer_at_least(1),
			[&](const boost::system::error_code& error, std::size_t bytes_transferred)
			{
				//transfers buffer data to the handle function which further decides how to handle the data
				handle_read(error, bytes_transferred, buffer, serialport, _logic_data);
			});
		//

		//wait for sequence finnish
		io.run();
	}
	catch (const std::exception& ex) {

		//if the same exception occurs again stop the output
		//this is needed to stop the Error to be printed continously when e.g. the Serial Device disconnects

		if (std::string(ex.what()) != "" && std::string(global_exception.what()) == std::string(ex.what()))
		{
			std::cin.ignore();
			return;
		}
		//sets the current exception as the latest one to the global scope
		global_exception = ex;
		std::cerr << "Exception: " << ex.what() << std::endl << std::endl;
	}
}


std::string AutoDrones::choose_serial(boost::asio::io_service& io)
{
	boost::asio::serial_port serial(io);
	boost::system::error_code ec;

	std::vector<std::string> vec_port;

	system("CLS");
	std::cout << "Please choose a Port to connect to." << std::endl;

	//iterates over every possible Port and checks if it is available
	for (auto i = 1; i <= 256; ++i) {
		std::string port = "COM" + std::to_string(i);
		serial.open(port, ec);
		if (!ec) {
			vec_port.push_back(port);
			serial.close();
		}
	}

	//prints the stored Ports to the console
	for (int i = 0; i < vec_port.size(); i++)
	{
		std::cout << i << ":  " << vec_port[i].c_str() << std::endl;
	}
	while (true)
	{

		std::string user_input;
		std::getline(std::cin, user_input);

		//int x = (user_input[0]);
		//bool y = isdigit(user_input[0]);
		//int z = stoi(user_input);

		//if userinput exceeds 1 char or is bigger/smaller than the number of available Ports
		if (user_input.length() > 1 || !isdigit(user_input[0]) || stoi(user_input) >= vec_port.size())
		{
			//clears screen
			system("CLS");

			std::cout << "Please choose a Port to connect to." << std::endl;
			for (int i = 0; i < vec_port.size(); i++)
			{
				std::cout << i << ":  " << vec_port[i].c_str() << std::endl;
			}
			continue;
		}
		else //if userinput is correct
		{
			//returns the name of the Port chosen by the User
			return vec_port[std::stoi(user_input)];
		}
	}
}
