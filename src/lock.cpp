#include "lock.h"
#include "utility.h"

#include <memory>
#include <tuple>
#include "allocator.h"

class SpinLockPoolAllocator
{
public:
	union Chunk
	{
		Chunk* next;
		char data[0]; // 가변 크기 배열
	};

	static SpinLockPoolAllocator* GetInstance(size_t chunkSize = 64)
	{
		if (!s_instance)
		{
			s_instance = new SpinLockPoolAllocator(chunkSize);
		}
		return s_instance;
	}

	void* Allocate()
	{
		lock.Lock();
		// FreeList에 남은 청크가 없으면 할당 실패
		if (!freeListHead)
		{
			lock.Unlock();
			return nullptr;
		}
		// 1. FreeList의 헤드를 떼어냄
		Chunk* chunkToAllocate = freeListHead;
		// 2. 헤드를 다음 노드로 이동
		freeListHead = freeListHead->next;
		lock.Unlock();
		return chunkToAllocate;
	}

	void Deallocate(void* ptr)
	{
		lock.Lock();
		if (!ptr)
		{
			lock.Unlock();
			return;
		}

		// 1. 청크를 FreeList의 헤드로 추가
		Chunk* chunkToFree = static_cast<Chunk*>(ptr);
		chunkToFree->next = freeListHead;
		freeListHead = chunkToFree;
		lock.Unlock();
	}
private:
	SpinLockPoolAllocator(size_t chunkSize)
		: chunkSize(chunkSize)
		, freeListHead(nullptr)
	{
		buffer = new char[blockSize];
		for (size_t i = 0; i < blockSize / chunkSize; ++i)
		{
			Chunk* chunk = reinterpret_cast<Chunk*>(buffer + i * chunkSize);
			chunk->next = freeListHead;
			freeListHead = chunk;
		}
	}

	char* buffer;
	Chunk* freeListHead;
	const size_t chunkSize;
	const size_t blockSize = 1024 * 1024 * 48;
	SpinLock lock;

	static SpinLockPoolAllocator* s_instance;
};

class  MutexPoolAllocator
{
public:
	union Chunk
	{
		Chunk* next;
		char data[0]; // 가변 크기 배열
	};

	static MutexPoolAllocator* GetInstance(size_t chunkSize = 64)
	{
		if (!s_instance)
		{
			s_instance = new MutexPoolAllocator(chunkSize);
		}
		return s_instance;
	}

	void* Allocate()
	{
		lock.lock();
		// FreeList에 남은 청크가 없으면 할당 실패
		if (!freeListHead)
		{
			lock.unlock();
			return nullptr;
		}
		// 1. FreeList의 헤드를 떼어냄
		Chunk* chunkToAllocate = freeListHead;
		// 2. 헤드를 다음 노드로 이동
		freeListHead = freeListHead->next;
		lock.unlock();
		return chunkToAllocate;
	}

	void Deallocate(void* ptr)
	{
		lock.lock();
		if (!ptr)
		{
			lock.unlock();
			return;
		}

		// 1. 청크를 FreeList의 헤드로 추가
		Chunk* chunkToFree = static_cast<Chunk*>(ptr);
		chunkToFree->next = freeListHead;
		freeListHead = chunkToFree;
		lock.unlock();
	}
private:
	MutexPoolAllocator(size_t chunkSize)
		: chunkSize(chunkSize)
		, freeListHead(nullptr)
	{
		buffer = new char[blockSize];
		for (size_t i = 0; i < blockSize / chunkSize; ++i)
		{
			Chunk* chunk = reinterpret_cast<Chunk*>(buffer + i * chunkSize);
			chunk->next = freeListHead;
			freeListHead = chunk;
		}
	}

	char* buffer;
	Chunk* freeListHead;
	const size_t chunkSize;
	const size_t blockSize = 1024 * 1024 * 48;
	std::mutex lock;

	static MutexPoolAllocator* s_instance;
};

SpinLockPoolAllocator* SpinLockPoolAllocator::s_instance;
MutexPoolAllocator* MutexPoolAllocator::s_instance;



void work_lock_free_level_0(std::atomic<int64_t>& counter, size_t loopCount)
{
	for (size_t i = 0; i < loopCount; i++)
	{
		size_t another_count = Random::NextUInt64(100, 5500);
		for (volatile size_t j = 0; j < another_count; j++)
		{
			volatile size_t dummy = j * 2;
		}
		__debugbreak();
		counter++;
	}
}

void work_spinlock_level_0(SpinLock& lock, int64_t& counter, size_t loopCount)
{
	for (size_t i = 0; i < loopCount; i++)
	{
		size_t another_count = Random::NextUInt64(100, 5500);
		for (volatile size_t j = 0; j < another_count; j++)
		{
			volatile size_t dummy = j * 2;
		}

		lock.Lock();
		counter++;
		lock.Unlock();
	}
}

void work_mutex_level_0(std::mutex& lock, int64_t& counter, size_t loopCount)
{
	for (size_t i = 0; i < loopCount; i++)
	{
		size_t another_count = Random::NextUInt64(100, 5500);
		for (volatile size_t j = 0; j < another_count; j++)
		{
			volatile int dummy = j * 2;
		}

		lock.lock();
		counter++;
		lock.unlock();
	}
}

int do_something_in_parallel_with_lock_free(size_t threadCount, size_t loopCount, int workLevel, int64_t& result)
{
	std::vector<std::thread> threads;
	std::atomic<int64_t> count;
	for (int i = 0; i < threadCount; i++)
	{
		threads.emplace_back(work_lock_free_level_0, std::ref(count), loopCount);
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	result = count;

	return 14;
}

int do_something_in_parallel_with_spinlock(size_t threadCount, size_t loopCount, int workLevel, int64_t& result)
{
	std::vector<std::thread> threads;
	SpinLock lock;
	for (int i = 0; i < threadCount; i++)
	{
		threads.emplace_back(work_spinlock_level_0, std::ref(lock), std::ref(result), loopCount);
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	return 12;
}

int do_something_in_parallel_with_mutex(size_t threadCount, size_t loopCount, int workLevel, int64_t& result)
{
	std::vector<std::thread> threads;
	std::mutex lock;
	for (int i = 0; i < threadCount; i++)
	{
		threads.emplace_back(work_mutex_level_0, std::ref(lock), std::ref(result), loopCount);
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	return 12;
}

void allocate_random_count_spinlock(SpinLock& lock, size_t& remainCount, size_t& allocatedCount)
{
	SpinLockPoolAllocator* allocator = SpinLockPoolAllocator::GetInstance(64); // 64바이트 크기의 객체를 할당하는 풀 할당기
	while (true)
	{
		lock.Lock();
		if (remainCount == 0)
		{
			lock.Unlock();
			break;
		}
		size_t count = Random::NextUInt64(1, std::min((size_t)1024, remainCount));
		remainCount -= count;
		allocatedCount += count;
		lock.Unlock();

		for (size_t k = 0; k < count; k++)
		{
			void* p = allocator->Allocate();
			for (int i = 0; i < 10; i++)
				volatile int someResult = do_something_for_allocate();
			allocator->Deallocate(p);
		}
	}
}

void allocate_random_count_mutex(std::mutex& lock, size_t& remainCount, size_t& allocatedCount)
{
	MutexPoolAllocator* allocator = MutexPoolAllocator::GetInstance(64); // 64바이트 크기의 객체를 할당하는 풀 할당기
	while (true)
	{
		lock.lock();
		if (remainCount == 0)
		{
			lock.unlock();
			break;
		}
		size_t count = Random::NextUInt64(1, std::min((size_t)1024, remainCount));
		remainCount -= count;
		allocatedCount += count;
		lock.unlock();

		for (size_t k = 0; k < count; k++)
		{
			void* p = allocator->Allocate();
			for (int i = 0; i < 10; i++)
				volatile int someResult = do_something_for_allocate();
			allocator->Deallocate(p);
		}
	}
}

size_t do_allocate_in_parallel_spinlock(size_t threadCount, size_t objectCount, size_t frameCount)
{
	SpinLockPoolAllocator* allocator = SpinLockPoolAllocator::GetInstance();
	SpinLock lock;
	size_t allocatedObjectCount = 0;
	for (size_t i = 0; i < frameCount; i++)
	{
		std::vector<std::thread> threads;
		for (size_t j = 0; j < threadCount; j++)
		{
			threads.emplace_back(allocate_random_count_spinlock, std::ref(lock), std::ref(objectCount), std::ref(allocatedObjectCount));
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}

	return allocatedObjectCount;
}
size_t do_allocate_in_parallel_mutex(size_t threadCount, size_t objectCount, size_t frameCount)
{
	MutexPoolAllocator* allocator = MutexPoolAllocator::GetInstance();

	std::mutex lock;
	size_t allocatedObjectCount = 0;
	for (size_t i = 0; i < frameCount; i++)
	{
		std::vector<std::thread> threads;
		for (size_t j = 0; j < threadCount; j++)
		{
			threads.emplace_back(allocate_random_count_mutex, std::ref(lock), std::ref(objectCount), std::ref(allocatedObjectCount));
		}

		for (auto& thread : threads)
		{
			thread.join();
		}
	}

	return allocatedObjectCount;
}
