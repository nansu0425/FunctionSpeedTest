#include "FunctionSpeedTest.h"

FunctionSpeedTest::FunctionSpeedTest(void (*target_ptr)(), int count)
	: m_target_ptr{ target_ptr }
	, m_count{ count }
{
}

int FunctionSpeedTest::test(std::vector<chr::microseconds> *times_ptr) const
{
	// 입력 파일과 출력 파일의 path
	fs::path input_path{ "./Input" };
	fs::path output_path{ "./Output" };

	// Input, Output 디렉토리가 존재하지 않으면 생성
	if (!fs::exists(input_path))
		fs::create_directory(input_path);
	if (!fs::exists(output_path))
		fs::create_directory(output_path);

	// Input 디렉토리에 있는 파일을 순회하기 위한 반복자
	fs::directory_iterator input_dir_itr{ input_path };

	if (fs::begin(input_dir_itr) == fs::end(input_dir_itr)) {	// Input에 파일이 없는 경우
		// output.txt의 출력 파일 스트림
		std::ofstream output_file_stream{ output_path / "output.txt"};

		// cout을 output.txt로 리다이렉트한 후 이전 cout 스트림 버퍼 저장
		std::streambuf *cout_stream_buffer{ std::cout.rdbuf(output_file_stream.rdbuf()) };

		// target 함수의 microseconds 단위 실행 시간을 얻는다
		chr::microseconds execution_time{ 
			get_execution_time(times_ptr, &output_file_stream) };

		// 출력 파일 스트림을 닫는다
		output_file_stream.close();

		// cout의 버퍼를 이전 스트림 버퍼로 변경
		std::cout.rdbuf(cout_stream_buffer);

		// 실행 시간 출력
		std::cout << "Execution time: " << get_ms_str(execution_time) << std::endl;
	}
	else {
		for (const auto &entry : input_dir_itr) {	// Input에 있는 파일 순회
			// entry 파일의 입출력 파일 스트림, 입력 파일과 출력 파일 이름 동일
			std::ifstream input_file_stream{ entry.path() };
			std::ofstream output_file_stream{ output_path / entry.path().filename() };

			// cin, cout을 entry 파일로 리다이렉트한 후 이전 스트림 버퍼 저장
			std::streambuf *cin_stream_buffer{ std::cin.rdbuf(input_file_stream.rdbuf()) };
			std::streambuf *cout_stream_buffer{ std::cout.rdbuf(output_file_stream.rdbuf()) };

			// target 함수의 microseconds 단위 실행 시간을 얻는다
			chr::microseconds execution_time{ 
				get_execution_time(times_ptr, &output_file_stream, &input_file_stream) };

			// 입출력 파일 스트림을 닫는다
			input_file_stream.close();
			output_file_stream.close();
			
			// cin, cout의 버퍼를 이전 스트림 버퍼로 변경
			std::cin.rdbuf(cin_stream_buffer);
			std::cout.rdbuf(cout_stream_buffer);

			// 실행 시간 출력
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
	// microseconds의 합을 저장할 변수
	chr::microseconds sum_of_mcs{ 0 };

	for (int i = 0; i < m_count; ++i) {
		// 시간 측정 시작
		auto start{ chr::high_resolution_clock::now() };

		// target 함수 호출
		m_target_ptr();

		// 시간 측정 종료
		auto stop{ chr::high_resolution_clock::now() };

		// 측정한 시간을 sum_of_mcs에 더한다
		sum_of_mcs += chr::duration_cast<chr::microseconds>(stop - start);

		// 파일 입출력 위치, 에러 플래그 초기화 
		ofs_ptr->seekp(0);
		if (ifs_ptr) {
			std::cin.clear();
			ifs_ptr->seekg(0);
		}
	}

	// 실행 시간의 평균 계산
	chr::microseconds execution_time{ sum_of_mcs / m_count };

	// times 벡터가 nullptr이 아니면 실행 시간 push
	if (times_ptr_ref != nullptr)
		times_ptr_ref->push_back(execution_time);

	return execution_time;
}

std::string FunctionSpeedTest::get_ms_str(chr::microseconds &execution_time) const
{
	// execution time을 문자열로 변환
	std::string time_str{ std::to_string(execution_time.count()) };

	// 1ms 보다 작을 경우 0으로 패딩
	if (time_str.length() < 4)
		time_str = std::string(4 - time_str.length(), '0') + time_str;

	// 반복 시작 인덱스
	auto beg_idx{ time_str.length() % 3 };
	if (beg_idx == 0) beg_idx = 3;

	// 반복 종료 인덱스
	auto end_idx{ time_str.length() - 3 };

	// "." 삽입
	time_str.insert(end_idx, ".");

	// "," 삽입
	for (auto i{ beg_idx }; i != end_idx; i += 4, ++end_idx)
		time_str.insert(i, ",");
	
	return time_str + " ms";
}