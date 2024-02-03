#include "FunctionSpeedTest.h"

FunctionSpeedTest::FunctionSpeedTest(void (*target_ptr)(), int count)
	: m_target_ptr{ target_ptr }
	, m_count{ count }
{
}

int FunctionSpeedTest::test(std::vector<chr::microseconds> *times_ptr) const
{
	// �Է� ���ϰ� ��� ������ path
	fs::path input_path{ "./Input" };
	fs::path output_path{ "./Output" };

	// Input, Output ���丮�� �������� ������ ����
	if (!fs::exists(input_path))
		fs::create_directory(input_path);
	if (!fs::exists(output_path))
		fs::create_directory(output_path);

	// Input ���丮�� �ִ� ������ ��ȸ�ϱ� ���� �ݺ���
	fs::directory_iterator input_dir_itr{ input_path };

	if (fs::begin(input_dir_itr) == fs::end(input_dir_itr)) {	// Input�� ������ ���� ���
		// output.txt�� ��� ���� ��Ʈ��
		std::ofstream output_file_stream{ output_path / "output.txt"};

		// cout�� output.txt�� �����̷�Ʈ�� �� ���� cout ��Ʈ�� ���� ����
		std::streambuf *cout_stream_buffer{ std::cout.rdbuf(output_file_stream.rdbuf()) };

		// target �Լ��� microseconds ���� ���� �ð��� ��´�
		chr::microseconds execution_time{ 
			get_execution_time(times_ptr, &output_file_stream) };

		// ��� ���� ��Ʈ���� �ݴ´�
		output_file_stream.close();

		// cout�� ���۸� ���� ��Ʈ�� ���۷� ����
		std::cout.rdbuf(cout_stream_buffer);

		// ���� �ð� ���
		std::cout << "Execution time: " << get_ms_str(execution_time) << std::endl;
	}
	else {
		for (const auto &entry : input_dir_itr) {	// Input�� �ִ� ���� ��ȸ
			// entry ������ ����� ���� ��Ʈ��, �Է� ���ϰ� ��� ���� �̸� ����
			std::ifstream input_file_stream{ entry.path() };
			std::ofstream output_file_stream{ output_path / entry.path().filename() };

			// cin, cout�� entry ���Ϸ� �����̷�Ʈ�� �� ���� ��Ʈ�� ���� ����
			std::streambuf *cin_stream_buffer{ std::cin.rdbuf(input_file_stream.rdbuf()) };
			std::streambuf *cout_stream_buffer{ std::cout.rdbuf(output_file_stream.rdbuf()) };

			// target �Լ��� microseconds ���� ���� �ð��� ��´�
			chr::microseconds execution_time{ 
				get_execution_time(times_ptr, &output_file_stream, &input_file_stream) };

			// ����� ���� ��Ʈ���� �ݴ´�
			input_file_stream.close();
			output_file_stream.close();
			
			// cin, cout�� ���۸� ���� ��Ʈ�� ���۷� ����
			std::cin.rdbuf(cin_stream_buffer);
			std::cout.rdbuf(cout_stream_buffer);

			// ���� �ð� ���
			std::cout << "Execution time of " << entry.path().filename()
					  << ": " << get_ms_str(execution_time) << std::endl;
		}
	}

	return 0;
}

chr::microseconds FunctionSpeedTest::get_execution_time(
	std::vector<chr::microseconds> *&times_ptr_ref,
	std::ofstream *ofs_ptr, std::ifstream *ifs_ptr) const
{
	// microseconds�� ���� ������ ����
	chr::microseconds sum_of_mcs{ 0 };

	for (int i = 0; i < m_count; ++i) {
		// �ð� ���� ����
		auto start{ chr::high_resolution_clock::now() };

		// target �Լ� ȣ��
		m_target_ptr();

		// �ð� ���� ����
		auto stop{ chr::high_resolution_clock::now() };

		// ������ �ð��� sum_of_mcs�� ���Ѵ�
		sum_of_mcs += chr::duration_cast<chr::microseconds>(stop - start);

		// ���� ����� ��ġ, ���� �÷��� �ʱ�ȭ 
		ofs_ptr->seekp(0);
		if (ifs_ptr) {
			std::cin.clear();
			ifs_ptr->seekg(0);
		}
	}

	// ���� �ð��� ��� ���
	chr::microseconds execution_time{ sum_of_mcs / m_count };

	// times ���Ͱ� nullptr�� �ƴϸ� ���� �ð� push
	if (times_ptr_ref != nullptr)
		times_ptr_ref->push_back(execution_time);

	return execution_time;
}

std::string FunctionSpeedTest::get_ms_str(chr::microseconds &execution_time) const
{
	// execution time�� ���ڿ��� ��ȯ
	std::string time_str{ std::to_string(execution_time.count()) };

	// 1ms ���� ���� ��� 0���� �е�
	if (time_str.length() < 4)
		time_str = std::string(4 - time_str.length(), '0') + time_str;

	// �ݺ� ���� �ε���
	auto beg_idx{ time_str.length() % 3 };
	if (beg_idx == 0) beg_idx = 3;

	// �ݺ� ���� �ε���
	auto end_idx{ time_str.length() - 3 };

	// "." ����
	time_str.insert(end_idx, ".");

	// "," ����
	for (auto i{ beg_idx }; i != end_idx; i += 4, ++end_idx)
		time_str.insert(i, ",");
	
	return time_str + " ms";
}