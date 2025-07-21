#include "allocator.h"
#include "utility.h"

StackAllocator* StackAllocator::s_instance = nullptr;

void StackAllocator::Init()
{
	if (isInitialized) return;
	buffer = new char[blockSize];
	ptr = 0;
	isInitialized = true;
}

void StackAllocator::Destroy()
{
	delete[] buffer;
	buffer = nullptr;
	isInitialized = false;
}

void* StackAllocator::Allocate(size_t size)
{
	if (ptr + size > blockSize) return nullptr; // Out of memory
	void* result = buffer + ptr;
	ptr += size;
	return result;
}

void StackAllocator::Flush()
{
	ptr = 0;
}

StackAllocator::Marker StackAllocator::Mark()
{
	Marker marker(this, ptr);
	return marker;
}

void StackAllocator::Release(const Marker& marker)
{
	if (ptr < marker.ptr) return;

	ptr -= marker.ptr;
}


PoolAllocator* PoolAllocator::s_instance;

void PoolAllocator::Destroy()
{
	delete[] buffer;
}

void* PoolAllocator::Allocate()
{
	// FreeList에 남은 청크가 없으면 할당 실패
	if (!freeListHead)
	{
		return nullptr;
	}

	// 1. FreeList의 헤드를 떼어냄
	Chunk* chunkToAllocate = freeListHead;
	// 2. 헤드를 다음 노드로 이동
	freeListHead = freeListHead->next;

	return chunkToAllocate;
}

void PoolAllocator::Deallocate(void* ptr)
{
	if (!ptr) return;

	// 1. 청크를 FreeList의 헤드로 추가
	Chunk* chunkToFree = static_cast<Chunk*>(ptr);
	chunkToFree->next = freeListHead;
	freeListHead = chunkToFree;
}

int do_something_for_allocate()
{
	return Random::NextInt32(0, 100);
}

















void PrepareFrame() {}
void OnEvent() {}
void OnUpdate() {}
void OnGUI() {}
void OnRender() {}
void Submit() {}










void foo()
{
	StackAllocator* allocator = StackAllocator::GetInstance();
	StackAllocator::Marker marker = allocator->Mark();

	void* p = allocator->Allocate(1024 * 1024); // 1MB 할당

	//...

	int* pp = (int*)allocator->Allocate(328);

	//...

	return;
}





void bar()
{


	 

	// Game Loop...
	while (true)
	{
		PrepareFrame(); // 프레임 준비

		OnEvent(); // 이벤트 핸들링

		OnUpdate(); // 게임 업데이트

		OnGUI(); // GUI 업데이트

		OnRender(); // 렌더링

		Submit(); // 프레임 제출

		//...

		StackAllocator::GetInstance()->Flush(); // 이번 프레임 할당 해제!
	}












}