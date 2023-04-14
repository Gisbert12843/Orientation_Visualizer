#include "wholeinclude_orientation_visualizer.h"


#include "Serial.h"
#include "Graphics.h"
#include "logic_data.h"
#include "Benchmark.h"



void logic_handler(boost::asio::io_service& io, boost::asio::serial_port& port)
{
	logic_data* _logic_data;
	//char buffer[16] = "~12 11 10 9 8 7";
	//char* p_buffer = buffer;
	//size_t size = 16;
	{
		AutoDrones::readSerial(io, port, _logic_data);
	}

	_logic_data->print();
	std::cin.ignore();
	//delete(_logic_data);
}




int main(int argc, char** argv)
{
	boost::asio::io_service io;

	//userinput regarding the Port to be chosen
	std::string init_answer;
	do {
		init_answer = AutoDrones::choose_serial(io);
	} while (init_answer == "");
	//

	//inits the serial_port with the port chosen by the user
	boost::asio::serial_port port(io, init_answer);


	//boost::asio::serial_port port(io, "COM9");

	system("CLS");

	while(1)
	{
		logic_handler(io,port);
	}


	//AutoDrones_Graphics::init_GLFW_GLEW(window, argc, argv);
	
	//while (true)
	//	AutoDrones_Graphics::drawRectangle(window);

	//glfwDestroyWindow(window);
	//glfwTerminate();

	std::cout << std::endl << std::endl << "END";
	std::cin.ignore();
	return 0;
}