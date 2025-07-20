//
//  ilp.cpp
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/20/25.
//
#include "ilp.h"

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