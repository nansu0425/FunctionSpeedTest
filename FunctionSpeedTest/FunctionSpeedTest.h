#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;
namespace chr = std::chrono;

class FunctionSpeedTest
{
private:
	void (*m_target_ptr)(){};
	int m_count{};
public:
	FunctionSpeedTest(void (*target_ptr)(), int count = 1);

	int test(std::vector<chr::microseconds> *times_ptr = nullptr) const;
private:
	chr::microseconds get_execution_time(
		std::vector<chr::microseconds> *&times_ptr_ref, 
		std::ofstream *ofs_ptr, 
		std::ifstream *ifs_ptr = nullptr) const;
	std::string get_ms_str(chr::microseconds &execution_time) const;
};

