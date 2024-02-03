#include "FunctionSpeedTest.h"
#include "TargetFunctions.h"

int main()
{
	// 각 입력 파일에 대해 target 함수 100번 테스트
	FunctionSpeedTest fst(target_function, 100);
	fst.test();

	return 0;
}