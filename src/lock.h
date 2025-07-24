//
//  lock.h
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/22/25.
//
#pragma once
#ifndef __LOCK_H__
#define __LOCK_H__

#include "Predefine.h"
#include <mutex>
#include <thread>

#include <atomic>

class SpinLock
{
public:
	void Lock()
	{
		// Busy-wait
		while (flag.test_and_set(std::memory_order_acquire))
		{
			std::this_thread::yield();
		}
	}

	void Unlock()
	{
		flag.clear(std::memory_order_release);
	}

private:
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

int do_something_in_parallel_with_lock_free(size_t threadCount, size_t loopCount, int workLevel, int64_t& result);
int do_something_in_parallel_with_spinlock(size_t threadCount, size_t loopCount, int workLevel, int64_t& result);
int do_something_in_parallel_with_mutex(size_t threadCount, size_t loopCount, int workLevel, int64_t& result);

size_t do_allocate_in_parallel_spinlock(size_t threadCount, size_t objectCount, size_t frameCount);
size_t do_allocate_in_parallel_mutex(size_t threadCount, size_t objectCount, size_t frameCount);

class LockFreeQueue
{
public:


private:
	struct Node
	{
		void* data;
		void* node;
	};
};

#endif