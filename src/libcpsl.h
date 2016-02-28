/*This file is part of libcpsl.
 * libcpsl is public domain software; you may use it however you please.
 * Refer to the file UNLICENSE.txt for more information.
 * 2016, by Subsentient, the engerbilled one.
 */

/** Note to any authors: Do NOT add C99/C11 constructs in this header. We must be able to use this from C89.**/

#ifndef __LIBCPSL_H__
#define __LIBCPSL_H__

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include <stddef.h> /*For size_t*/

typedef signed char CPSL_Bool;
typedef int CPSL_DynArrayHandle;
typedef enum
{ /*The best way to do this actually.*/
	LISTDEL_FAIL, /*Couldn't delete the element.*/
	LISTDEL_OK, /*Deleted normally and efficiently.*/
	LISTDEL_HEADMOVED, /*Element was Head and so the Head has changed.*/
	LISTDEL_LISTDESTROYED /*Was the only, the list is now destroyed.*/
} CPSL_ListDelStatus;

/**Types**/


/*Our list implementation can use 48 bytes on a 64-bit platform, but it's designed to be fast and versatile more than it is memory efficient.*/
struct CPSL_List
{
	void *Data; /*USERS: You'll need to use this if you are in C++ and want a list of non-POD types. Don't do the type punning for non-POD types.
	* The C++ standard says that's undefined behaviour and might not do what you want at all. The type punning could get you reading from the vtable.
	* If you DO want to do the type punning and it's safe, then this member can be ignored. It's just one pointer.
	* USERS: You are responsible for deleting anything you allocate and attach to Data. If you don't delete it yourself, memory leak.
	*/
	
	/**ATTENTION USERS: Don't change anything below here directly, use the list functions for that! You can read them though.**/
	struct CPSL_List *Next; /*Points to the next element in the list. Will be NULL if you are at the end.*/
	struct CPSL_List *Prev; /*Points to the previous element in the list. Will be NULL if used on Head.*/
	struct CPSL_List **Head; /*Points to a pointer that points to the list head.*/
	struct CPSL_List **End; /*Points to a pointer that points to the list end.*/
	const unsigned *PerElementSize; /*Points to a pointer that points to an unsigned int that holds the size per element.*/
};

struct CPSL_Allocator
{
	void *(*malloc)(const size_t Size);
	void (*free)(void *const Data);
};

/**Functions**/
void *(*CPSL_DynArray_Get)(CPSL_DynArrayHandle Handle, const unsigned Subscript);
CPSL_Bool *(*CPSL_DynArray_Set)(CPSL_DynArrayHandle Handle, const unsigned Subscript);


/*Linked list functions*/

/*With CPSL_List_NewList(), PerObjectSize should be sizeof(struct CPSL_List) unless you want to do the type punning trick,
 * (which is why we ask you), but make sure it's at least sizeof(struct CPSL_List) no matter what.
 * It'll return NULL if PerObjectSize is too small.
 */
struct CPSL_List *CPSL_List_NewList(const unsigned PerObjectSize);

/* Really, with both of these below functions, it looks up *->Head from any element you give it,
 * so you can just pass it any arbitrary member of a list and it'll figure Head out for itself.
 */
CPSL_Bool CPSL_List_DestroyList(struct CPSL_List *AnyListElement);
struct CPSL_List *CPSL_List_AddNode(struct CPSL_List *AnyListElement);


/*CPSL_List_DeleteNode() can change *->Head, so it's best not to rely on the value of *->Head, but instead ->Head.
 * It will delete the entire list if you are deleting the last element. You're warned.
 */
CPSL_ListDelStatus CPSL_List_DeleteNode(struct CPSL_List *NodeToDelete);

/**Globals**/


#ifdef __cplusplus
} /*extern "C"*/

/*We need this template for list next-ing because we can't do implicit conversion FROM void in C++.*/
template <typename T>
static inline T CPSL_LNEXT(T List)
{
	return reinterpret_cast<T>( (((struct CPSL_List*)List)->Next) );
}

#else /*If we are C, we use a simple macro instead of a template.*/
#define CPSL_LNEXT(x) ((void*)x->Next)
#endif /*__cplusplus*/

#endif /*__LIBCPSL_H__*/

