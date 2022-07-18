#pragma once

namespace Memory
{
	extern HANDLE defaultHeap;
	extern size_t pageSize;

	void init();

	template<typename T>
	T* allocate(size_t count = 1)
	{
		return static_cast<T*>(HeapAlloc(defaultHeap, HEAP_ZERO_MEMORY, sizeof(T) * count));
	}

	template<typename T>
	void deallocate(T* memory)
	{
		HeapFree(defaultHeap, 0, memory);
	}
}