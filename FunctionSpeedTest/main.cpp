#include "FunctionSpeedTest.h"
#include "TargetFunctions.h"

int main()
{
	// �� �Է� ���Ͽ� ���� target �Լ� 100�� �׽�Ʈ
	FunctionSpeedTest fst(target_function, 100);
	fst.test();

	return 0;
}