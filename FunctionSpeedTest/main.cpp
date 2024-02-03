#include "FunctionSpeedTest.h"
#include "TestFunctions.h"

int main()
{
	FunctionSpeedTest fst(test_target, 10);
	fst.test();

	return 0;
}