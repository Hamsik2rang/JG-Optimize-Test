﻿//
//  main.cpp
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/15/25.
//
#include "Predefine.h"
#include "common.h"
#include "simd.h"
#include "particle.h"
#include "utility.h"
#include "monster.h"
#include "ilp.h"
#include "allocator.h"
#include "lock.h"

#include <iostream>
#include <chrono>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
	int testCaseIndex =4; // command args 사용하기 귀찮으면 이 값을 원하는 케이스 인덱스로 변경하세요.
	if (argc > 1)
	{
		testCaseIndex = std::stoi(argv[1]);
	}

	Random::Init();

	std::vector<const char*> name;
	std::vector<size_t> runtime;
	std::vector<time_t> result;

	size_t testSize = 0;
	volatile int returnIndex;

	switch (testCaseIndex)
	{
	case 0:
	case 1:
	case 2:
	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		testSize = 2;
		break;
	case 3:
	case 14:
		testSize = 3;
		break;
	default:
		std::cout << "지원하지 않는 테스트 인덱스입니다." << std::endl;
		return -1; // 프로세스 중료
	}

	assert(testSize > 0);
	name.resize(testSize);
	runtime.resize(testSize);
	result.resize(testSize);

	switch (testCaseIndex)
	{
	case 0:
	{
		name[0] = FUNC_NAME(sum_row_major);
		name[1] = FUNC_NAME(sum_col_major);

		const int N = 10000;
		const int M = 10000;

		int** arr = new int* [N];
		for (int i = 0; i < N; i++)
		{
			arr[i] = new int[M];
			for (int j = 0; j < M; j++)
			{
				arr[i][j] = Random::NextInt32(-512, 512);
			}
		}
		WARM_UP_CACHE_START();
		sum_row_major(arr, N, M);
		sum_col_major(arr, N, M);
		WARM_UP_CACHE_END();

		ELAPSE_START(0)
			sum_row_major(arr, N, M);
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1)
			sum_col_major(arr, N, M);
		ELAPSE_END(1, result[1]);
	}
	break;
	case 1:
	{
		name[0] = FUNC_NAME(multiply_matrix_simd);
		name[1] = FUNC_NAME(multiply_matrix_scalar);

		Matrix mat1(1024, 1024);
		Matrix mat2(1024, 1024);
		Matrix matResult1(1024, 1024);
		Matrix matResult2(1024, 1024);

		set_random_matrix_elements(mat1);
		set_random_matrix_elements(mat2);

		WARM_UP_CACHE_START();
		returnIndex = multiply_matrix_simd(mat1, mat2, matResult1);
		returnIndex = multiply_matrix_scalar(mat1, mat2, matResult2);
		WARM_UP_CACHE_END();

		ELAPSE_START(0);
		returnIndex = multiply_matrix_simd(mat1, mat2, matResult1);
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1);
		returnIndex = multiply_matrix_scalar(mat1, mat2, matResult2);
		ELAPSE_END(1, result[1]);

		VALIDATE(fltcmp(matResult1.data, matResult2.data, sizeof(float) * 1024 * 1024), 0);
		VALIDATE(returnIndex, testCaseIndex);
	}
	break;
	case 2:
	{
		name[0] = FUNC_NAME(multiply_many_matrix_simd);
		name[1] = FUNC_NAME(multiply_many_matrix_scalar);

		size_t matrixCount = 10'000'000;

		std::vector<Matrix> mat1(matrixCount);
		std::vector<Matrix> mat2(matrixCount);
		std::vector<Matrix> matResult1(matrixCount);
		std::vector<Matrix> matResult2(matrixCount);

		WARM_UP_CACHE_START();
		returnIndex = multiply_many_matrix_simd(mat1, mat2, matResult1);
		returnIndex = multiply_many_matrix_scalar(mat1, mat2, matResult2);
		WARM_UP_CACHE_END();

		ELAPSE_START(0);
		returnIndex = multiply_many_matrix_simd(mat1, mat2, matResult1);
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1);
		returnIndex = multiply_many_matrix_scalar(mat1, mat2, matResult2);
		ELAPSE_END(1, result[1]);

		VALIDATE(returnIndex, testCaseIndex);
		for (size_t i = 0; i < matrixCount; i++)
		{
			VALIDATE(fltcmp(matResult1[i].data, matResult2[i].data, 4 * 4), 0);
		}
	}
	break;
	case 3:
	{
		name[0] = FUNC_NAME(update_particle_position_aos_linked_list);
		name[1] = FUNC_NAME(update_particle_position_aos_array);
		name[2] = FUNC_NAME(update_particle_position_soa_array);

		const size_t particleCount = 10'000'000;
		const size_t frameCount = 100;
		std::list<Particle> particle_aos_linked_list(particleCount);
		Particle* particle_aos_array = new Particle[particleCount];
		ParticleSOA particle_soa_array(particleCount);

		float deltaTime = Random::NextFloat(0.001f, 0.015f);

		WARM_UP_CACHE_START();
		returnIndex = update_particle_position(particle_aos_linked_list, particleCount, deltaTime);
		returnIndex = update_particle_position(particle_aos_array, particleCount, deltaTime);
		returnIndex = update_particle_position(particle_soa_array, particleCount, deltaTime);
		WARM_UP_CACHE_END();

		ELAPSE_START(0);
		for (int i = 0; i < frameCount; i++)
			returnIndex = update_particle_position(particle_aos_linked_list, particleCount, deltaTime);
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1);
		for (int i = 0; i < frameCount; i++)
			returnIndex = update_particle_position(particle_aos_array, particleCount, deltaTime);
		ELAPSE_END(1, result[1]);

		ELAPSE_START(2)
			for (int i = 0; i < frameCount; i++)
				returnIndex = update_particle_position(particle_soa_array, particleCount, deltaTime);
		ELAPSE_END(2, result[2]);

		delete[] particle_aos_array;
		VALIDATE(returnIndex, testCaseIndex);
	}
	break;
	case 4:
	{
		name[0] = FUNC_NAME(update_monsters_allocate);
		name[1] = FUNC_NAME(update_monsters_pooling);

		const size_t monsterCount = 1'000'000;
		const size_t frameCount = 100;
		Monster** monsterPool1 = new Monster * [monsterCount] { nullptr };
		Monster** monsterPool2 = new Monster * [monsterCount] { nullptr };

		WARM_UP_CACHE_START();
		initialize_monsters(monsterPool1, monsterCount);
		initialize_monsters(monsterPool2, monsterCount);
		returnIndex = update_monsters_allocate(monsterPool1, monsterCount);
		initialize_monsters(monsterPool1, monsterCount);
		initialize_monsters(monsterPool2, monsterCount);
		returnIndex = update_monsters_pooling(monsterPool1, monsterCount);
		WARM_UP_CACHE_END();

		initialize_monsters(monsterPool1, monsterCount);
		ELAPSE_START(0);
		for (int i = 0; i < frameCount; i++)
		{
			returnIndex = update_monsters_allocate(monsterPool1, monsterCount);
		}
		ELAPSE_END(0, result[0]);

		initialize_monsters(monsterPool2, monsterCount);
		ELAPSE_START(1);
		for (int i = 0; i < frameCount; i++)
		{
			returnIndex = update_monsters_pooling(monsterPool2, monsterCount);
		}
		ELAPSE_END(1, result[1]);

		VALIDATE(returnIndex, testCaseIndex);
		release_monsters(monsterPool1, monsterCount);
		release_monsters(monsterPool2, monsterCount);
		delete[] monsterPool1;
		delete[] monsterPool2;
	}
	break;
	case 5:
	{
		name[0] = FUNC_NAME(count_monster_by_attributes);
		name[1] = FUNC_NAME(count_monster_by_hash);

		const size_t monsterCount = 1'000'000;
		const size_t frameCount = 100;
		Mesh* baseMesh = new Mesh();
		Material* baseMaterial = new Material();
		Texture* baseTexture = new Texture();

		Monster* baseMonster = new Monster("BaseMonster", Monster::Type::Orc, Monster::Immune::Fire, baseMesh, baseMaterial, baseTexture);
		Monster** monsterPool = new Monster * [monsterCount] { nullptr };
		initialize_monsters(monsterPool, monsterCount);

		int targetCount = 0;

		for (size_t i = 0; i < monsterCount; i++)
		{
			if (i % 250 == 0 || Random::NextUInt32(0, 100) < 24)
			{
				monsterPool[i] = baseMonster;
				targetCount++;
			}
			monsterPool[i]->isActive = false;
		}
		volatile int countFromAttr = 0;
		volatile int countFromHash = 0;
		WARM_UP_CACHE_START();
		countFromAttr = count_monster_by_attributes(monsterPool, monsterCount, baseMonster);
		countFromHash = count_monster_by_hash(monsterPool, monsterCount, baseMonster);
		WARM_UP_CACHE_END();

		ELAPSE_START(0);
		for (size_t i = 0; i < frameCount; i++)
		{
			countFromAttr = count_monster_by_attributes(monsterPool, monsterCount, baseMonster);
		}
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1);
		for (size_t i = 0; i < frameCount; i++)
		{
			countFromHash = count_monster_by_hash(monsterPool, monsterCount, baseMonster);
		}
		ELAPSE_END(1, result[1]);

		VALIDATE(countFromAttr, targetCount);
		VALIDATE(countFromHash, targetCount);
		for (size_t i = 0; i < monsterCount; i++)
		{
			if (baseMonster == monsterPool[i])
			{
				monsterPool[i] = nullptr;
			}
		}
		release_monsters(monsterPool, monsterCount);
		delete[] monsterPool;
		delete baseMonster;
	}
	break;
	case 6:
	{
		name[0] = FUNC_NAME(classify_monster_by_attributes);
		name[1] = FUNC_NAME(find_monster_by_attributes);

		const size_t monsterTypeCount = (size_t)Monster::Type::Count;
		const size_t monsterImmuneCount = (size_t)Monster::Immune::Count;

		const size_t bucketCount = monsterTypeCount * monsterImmuneCount;
		const size_t monsterCount = 1'000;

		Mesh* baseMesh[bucketCount];
		Material* baseMaterial[bucketCount];
		Texture* baseTexture[bucketCount];
		Monster* baseMonster[bucketCount];

		for (size_t i = 0; i < monsterTypeCount; i++)
		{
			for (size_t j = 0; j < monsterImmuneCount; j++)
			{
				size_t k = i * monsterImmuneCount + j;
				baseMesh[k] = new Mesh();
				baseMaterial[k] = new Material();
				baseTexture[k] = new Texture();
				baseMonster[k] = new Monster("BaseMonster", (Monster::Type)i, (Monster::Immune)j, baseMesh[k], baseMaterial[k], baseTexture[k]);
				baseMonster[k]->isActive = false;
			}
		}

		Monster** monsterPool = new Monster * [monsterCount] { nullptr };
		for (size_t i = 0; i < monsterCount; i++)
		{
			size_t index = Random::NextUInt32(0, bucketCount - 1);
			monsterPool[i] = baseMonster[index];
		}

		volatile int returnIndex[2] = { 0 };
		std::vector<std::vector<Monster*>> bucket[2];

		WARM_UP_CACHE_START();
		returnIndex[0] = classify_monster_by_attributes(monsterPool, monsterCount, bucket[0]);
		returnIndex[1] = classify_monster_by_hash(monsterPool, monsterCount, bucket[1]);
		WARM_UP_CACHE_END();

		ELAPSE_START(0);
		returnIndex[0] = classify_monster_by_attributes(monsterPool, monsterCount, bucket[0]);
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1);
		returnIndex[1] = classify_monster_by_hash(monsterPool, monsterCount, bucket[1]);
		ELAPSE_END(1, result[1]);

		for (size_t i = 0; i < bucketCount; i++)
		{
			// Mesh, Material, Texture는 Monster의 소멸자에서 자동 해제됩니다.
			delete baseMonster[i];
		}
		VALIDATE(returnIndex[0], testCaseIndex);
		VALIDATE(returnIndex[1], testCaseIndex);
	}
	break;
	case 9:
	{
		volatile int result = 0;
		result = branch_dependency();
		result = branch_dependency_none();
		return 0;
	}
	break;
	case 10:
	{
		StackAllocator* allocator = StackAllocator::GetInstance();
		name[0] = FUNC_NAME(default_allocator_allocate);
		name[1] = FUNC_NAME(stack_allocator_allocate);

		const size_t allocSize = 1024 * 1024 * 400; // 400MB
		const size_t frameCount = 1000;

		WARM_UP_CACHE_START();
		allocator->Allocate(1024 * 1024);
		allocator->Flush();
		char* p = new char[1024 * 1024];
		delete p;
		WARM_UP_CACHE_END();

		size_t allocatedSize[2]{ 0 };
		ELAPSE_START(0);
		for (size_t i = 0; i < frameCount; i++)
		{
			size_t remainSize = allocSize;
			while (remainSize)
			{
				size_t size = Random::NextUInt64(1, std::min((size_t)1024 * 32, remainSize));
				remainSize -= size;
				allocatedSize[0] += size;
				volatile char* p = new char[size];
				volatile int someResult = do_something_for_allocate();
				delete[] p;
			}
		}
		ELAPSE_END(0, result[0]);


		ELAPSE_START(1);
		for (size_t i = 0; i < frameCount; i++)
		{
			size_t remainSize = allocSize;
			while (remainSize)
			{
				size_t size = Random::NextUInt64(1, std::min((size_t)1024 * 32, remainSize));
				remainSize -= size;
				allocatedSize[1] += size;
				volatile char* p = (char*)allocator->Allocate(size);
				volatile int someResult = do_something_for_allocate();
			}
			allocator->Flush();
		}
		ELAPSE_END(1, result[1]);

		VALIDATE(allocatedSize[0], allocatedSize[1]);
	}
	break;
	case 11:
	{
		PoolAllocator* allocator = PoolAllocator::GetInstance(64);
		name[0] = FUNC_NAME(default_allocator_allocate);
		name[1] = FUNC_NAME(pool_allocator_allocate);

		WARM_UP_CACHE_START();
		char* p = (char*)allocator->Allocate();
		allocator->Deallocate(p);
		p = new char[64];
		delete p;
		WARM_UP_CACHE_END();


		size_t allocatedObjectCount[2]{ 0 };
		const size_t objectCount = 750'000;  // 64 * 750'000 = 48MiB
		const size_t frameCount = 10;
		ELAPSE_START(0);
		for (size_t i = 0; i < frameCount; i++)
		{
			size_t remainCount = objectCount;
			while (remainCount)
			{
				size_t count = Random::NextUInt64(1, std::min((size_t)1024, remainCount));
				remainCount -= count;
				allocatedObjectCount[0] += count;
				for (size_t j = 0; j < count; j++)
				{
					volatile char* p = new char[64];
					volatile int someResult = do_something_for_allocate();
					delete[] p;
				}
			}
		}
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1);
		for (size_t i = 0; i < frameCount; i++)
		{
			size_t remainCount = objectCount;
			while (remainCount)
			{
				size_t count = Random::NextUInt64(1, std::min((size_t)1024, remainCount));
				remainCount -= count;
				allocatedObjectCount[1] += count;
				for (size_t j = 0; j < count; j++)
				{
					volatile char* p = (char*)allocator->Allocate();
					volatile int someResult = do_something_for_allocate();
					allocator->Deallocate((void*)p);
				}
			}
		}
		ELAPSE_END(1, result[1]);
		allocator->Destroy();

		VALIDATE(allocatedObjectCount[0], allocatedObjectCount[1]);
	}
	break;
	case 12:
	{
		name[0] = FUNC_NAME(parallel_level_0_spinlock);
		name[1] = FUNC_NAME(parallel_level_0_mutex);

		const size_t threadCount = std::thread::hardware_concurrency();
		const size_t loopCount = 1'000'000;
		int64_t count[2]{ 0, 0 };
		WARM_UP_CACHE_START();
		do_something_in_parallel_with_spinlock(threadCount, loopCount, 0, count[0]);
		do_something_in_parallel_with_mutex(threadCount, loopCount, 0, count[1]);
		WARM_UP_CACHE_END();

		count[0] = 0;
		count[1] = 0;
		ELAPSE_START(0);
		volatile int returnValue0 = do_something_in_parallel_with_spinlock(threadCount, loopCount, 0, count[0]);
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1);
		volatile int returnValue1 = do_something_in_parallel_with_mutex(threadCount, loopCount, 0, count[1]);
		ELAPSE_END(1, result[1]);

		VALIDATE(returnValue0, returnValue1);
		VALIDATE(count[0], count[1]);
	}
	break;
	case 13:
	{
		name[0] = FUNC_NAME(parallel_pool_alloc_spinlock);
		name[1] = FUNC_NAME(parallel_pool_alloc_mutex);

		const size_t threadCount = std::thread::hardware_concurrency();

		size_t allocatedObjectCount[2]{ 0 };
		const size_t objectCount = 750'000;  // 64 * 750'000 = 48MiB
		const size_t frameCount = 100;

		WARM_UP_CACHE_START();
		do_allocate_in_parallel_mutex(threadCount, objectCount, frameCount);
		do_allocate_in_parallel_spinlock(threadCount, objectCount, frameCount);
		WARM_UP_CACHE_END();

		ELAPSE_START(0);
		allocatedObjectCount[0] = do_allocate_in_parallel_spinlock(threadCount, objectCount, frameCount);
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1);
		allocatedObjectCount[1] = do_allocate_in_parallel_mutex(threadCount, objectCount, frameCount);
		ELAPSE_END(1, result[1]);

		VALIDATE(allocatedObjectCount[0], allocatedObjectCount[1]);
	}
	break;
	case 14:
	{
		name[0] = FUNC_NAME(parallel_count_lock_free);
		name[1] = FUNC_NAME(parallel_count_spinlock);
		name[2] = FUNC_NAME(parallel_count_mutex);

		const size_t threadCount = std::thread::hardware_concurrency();
		const size_t loopCount = 1'0'00;
		int64_t count[3]{ 0, 0, 0 };
		WARM_UP_CACHE_START();
		do_something_in_parallel_with_spinlock(threadCount, loopCount, 0, count[0]);
		do_something_in_parallel_with_mutex(threadCount, loopCount, 0, count[1]);
		WARM_UP_CACHE_END();

		count[0] = 0;
		count[1] = 0;
		count[2] = 0;

		ELAPSE_START(0);
		volatile int returnValue0 = do_something_in_parallel_with_lock_free(threadCount, loopCount, 0, count[0]);
		ELAPSE_END(0, result[0]);

		ELAPSE_START(1);
		volatile int returnValue1 = do_something_in_parallel_with_spinlock(threadCount, loopCount, 0, count[1]);
		ELAPSE_END(1, result[1]);

		ELAPSE_START(2);
		volatile int returnValue2 = do_something_in_parallel_with_mutex(threadCount, loopCount, 0, count[2]);
		ELAPSE_END(2, result[2]);

		VALIDATE(returnValue0, testCaseIndex);
		VALIDATE(count[0], count[1]);
		VALIDATE(count[0], count[2]);
	}
	break;

	//...
	default:
		break;
	}

	int minIndex = 0;
	time_t minTime = std::numeric_limits<long long>::max();
	std::cout << "---------------------------------" << std::endl;
	for (int i = 0; i < testSize; i++)
	{
		std::wcout << "## " << i << "번 함수 " << name[i] << " 수행 시간 : " << result[i] / 1000 << " 밀리초" << std::endl;
		std::wcout << "---------------------------------" << std::endl;
		if (minTime > result[i])
		{
			minIndex = i;
			minTime = result[i];
		}
	}

	std::sort(result.begin(), result.end());

	std::wcout << "가장 최적화된 방식인 " << name[minIndex] << " 는 다른 방식(들)보다 각각 ";
	for (int i = 1; i < result.size(); i++)
	{
		std::wcout << (i > 1 ? ", " : "");
		if (result[0] == 0) result[0] = 1; // NaN 방지
		std::wcout << (float)(result[i]) / (float)result[0] << "배";
	}
	std::wcout << " 더 빠릅니다." << std::endl;
	std::wcout << "이는 다른 방식(들)의 자원 소모량을 각각 ";
	for (int i = 1; i < result.size(); i++)
	{
		std::wcout << (i > 1 ? ", " : "");
		if (result[0] == 0) result[0] = 1; // NaN 방지
		std::wcout << (float)(result[i] - result[0]) / (float)result[i] * 100.0f << "%";
	}
	std::wcout << " 절감합니다." << std::endl;

	return 0;
}
