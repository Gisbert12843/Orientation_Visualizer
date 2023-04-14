#pragma once
#include <chrono>
#include <iostream>
#include <windows.h>

namespace AutoDrones
{
	class Benchmark
	{
		std::chrono::steady_clock::time_point start;
		std::chrono::steady_clock::time_point end;

	public:
		Benchmark()
		{
			start = std::chrono::high_resolution_clock::now();
		}

		virtual ~Benchmark()
		{
			end = std::chrono::high_resolution_clock::now();

			std::cout << "Benchmark: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds\n";
			std::cout.flush();
		}
	};

}