//
//  allocator.h
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/18/25.
//
#pragma once
#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "Predefine.h"


class StackAllocator
{
public:
	struct Marker
	{
		Marker(StackAllocator* allocator, size_t ptr)
			: ptr(ptr), allocator(allocator)
		{
			
		}
		~Marker()
		{
			allocator->Release(*this);
		}
		size_t ptr;
		StackAllocator* allocator = nullptr;
	};

	void Init();
	void Destroy();

	static StackAllocator* GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new StackAllocator();
			s_instance->Init();
		}
		return s_instance;
	}
	void* Allocate(size_t size);
	void Flush();
	Marker Mark();

	void Release(const Marker& marker);

private:
	StackAllocator() = default;
	~StackAllocator() { Destroy(); }

	char* buffer;
	size_t ptr;
	bool isInitialized;

	const size_t blockSize = 1024 * 1024 * 48;

	static StackAllocator* s_instance;
};

class PoolAllocator
{
public:
	union Chunk
	{
		Chunk* next;
		char data[0]; // 가변 크기 배열
	};

	void Destroy();

	// 실제로는 C++의 Template등으로 크기 지정
	static PoolAllocator* GetInstance(size_t chunkSize)
	{
		if (!s_instance)
		{
			s_instance = new PoolAllocator(chunkSize);
		}
		return s_instance;
	}

	void* Allocate();
	void Deallocate(void* ptr);

private:
	PoolAllocator(size_t chunkSize)
		: chunkSize(chunkSize)
	{}
	~PoolAllocator() { Destroy(); }

	char* buffer;
	Chunk* freeListHead;
	const size_t chunkSize; 
	const size_t blockSize = 1024 * 1024 * 48; 
	static PoolAllocator* s_instance;
};




int do_something_for_allocate();

#endif