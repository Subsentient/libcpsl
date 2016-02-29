/*This file is part of libcpsl.
 * libcpsl is public domain software; you may use it however you please.
 * Refer to the file UNLICENSE.txt for more information.
 * 2016, by Subsentient, the white rat hiding under your chair. Don't look.
 */

#ifndef __CPSL_INTERNALHEADER__
#define __CPSL_INTERNALHEADER__

#ifndef NULL
#define NULL ((void*)0)
#endif //NULL
enum { false, true };

#include <stddef.h> /*For size_t*/
#include <stdint.h>

struct CPSL_Allocator
{
	void *(*malloc)(const size_t Size);
	void (*free)(void *const Data);
	void *(*realloc)(void *const Ptr, size_t NewSize);
};

struct CPSL_DynArray
{
	unsigned PerElementSize;
	unsigned AllocatedElements;
};


//Static functions.
static inline void MemCopy(void *Dest, const void *Src, size_t ByteCount)
{
	const uint8_t *const Stopper = (uint8_t*)Src + ByteCount;
	const uint8_t *SrcWorker = Src;
	uint8_t *DestWorker = Dest;
	
	for (; SrcWorker != Stopper; ++SrcWorker, ++DestWorker)
	{
		*DestWorker = *SrcWorker;
	}
}

extern struct CPSL_Allocator Alloc;

#endif //__CPSL_INTERNALHEADER__
