//
//  ilp.cpp
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/20/25.
//
#include "ilp.h"
#include "common.h"

#define SOMETHING_DATA_0 0x0
#define SOMETHING_DATA_1 0x1


int war_dependency()
{
	int a = SOMETHING_DATA_0;
	int b = SOMETHING_DATA_1;

	int x;
	x = a * 2; // x는 Withe-And-Read 의존성을 가집니다.
	a = b + 1;

	return a;
}

int raw_dependency()
{
	int a = SOMETHING_DATA_0;

	int x, y;
	x = a + 1;
	y = x * 2; // x는 Read-And-Write 의존성을 가집니다.

	return y;
}

int waw_dependency()
{
	int a = SOMETHING_DATA_0;
	int b = SOMETHING_DATA_1;

	int x;
	x = a * 2;
	x = b + 1; // x는 Write-And-Write 의존성을 가집니다.

	return x;
}

int branch_dependency(int* arr, size_t size)
{
	int sum = 0;
	for (int i = 0; i < size; i++)
	{
		if (size > 1024)
		{
			sum += 255;
		}
	}

	return 9;
}

int branch_dependency()
{
	int arr[334];
	int sum = 0;
	__debugbreak();
	for (int i = 0; i < 334; i++)
	{
		if (Random::NextBool())
		{
			arr[i] = 255;
		}
		else
		{
			arr[i] = 0;
		}
		sum += arr[i];
	}

	return sum;
}

int branch_dependency_none()
{
	int arr[334];
	int sum = 0;
	__debugbreak();
	for (int i = 0; i < 334; i++)
	{
		sum += (int)Random::NextBool() * 255;
	}

	return sum;
}

int memory_dependency()
{
	/// BEFORE>>>
	{
		int sum = 0;
		for (int i = 0; i < get_size(); i++)
		{
			sum += Random::NextInt32(0, 41);
		}
	}

	/// AFTER>>>
	{
		int sum = 0;
		size_t testSize = get_size();
		for (int i = 0; i < testSize; i++)
		{
			sum += Random::NextInt32(0, 41);
		}
	}

	return 9;
}