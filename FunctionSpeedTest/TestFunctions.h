#pragma once

#include <iostream>

void test_target() {
	int i{};

	while (std::cin >> i)
		std::cout << i << " ";

}