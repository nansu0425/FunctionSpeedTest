#include "FunctionSpeedTest.h"
#include "TestFunctions.h"

int main()
{
	// test target funtion 
	FunctionSpeedTest fst(target_function, 100);
	fst.test();

	return 0;
}