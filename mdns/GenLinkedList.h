/* ========================================================================
 * Copyright (c) 2005-2026, OPC Federation AISBL, All rights reserved.
 *
 * OPC Foundation MIT License 1.00
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * The complete license agreement can be found here:
 * http://opcfoundation.org/License/MIT/1.00/
 * ======================================================================*/

#ifndef __GenLinkedList__
#define __GenLinkedList__


#include <stddef.h>


struct  GenLinkedList
{
    void        *Head,
    *Tail;
    size_t LinkOffset;
};
typedef struct GenLinkedList GenLinkedList;


void        InitLinkedList( GenLinkedList *pList, size_t linkOffset);

void        AddToHead( GenLinkedList *pList, void *elem);
void        AddToTail( GenLinkedList *pList, void *elem);

int     RemoveFromList( GenLinkedList *pList, void *elem);

int     ReplaceElem( GenLinkedList *pList, void *elemInList, void *newElem);



struct  GenDoubleLinkedList
{
    void        *Head,
    *Tail;
    size_t FwdLinkOffset,
           BackLinkOffset;
};
typedef struct GenDoubleLinkedList GenDoubleLinkedList;


void        InitDoubleLinkedList( GenDoubleLinkedList *pList, size_t fwdLinkOffset,
                                  size_t backLinkOffset);

void        DLLAddToHead( GenDoubleLinkedList *pList, void *elem);

void        DLLRemoveFromList( GenDoubleLinkedList *pList, void *elem);



/* A GenLinkedOffsetList is like a GenLinkedList that stores the *Next field as a signed */
/* offset from the address of the beginning of the element, rather than as a pointer. */

struct  GenLinkedOffsetList
{
    size_t Head,
           Tail;
    size_t LinkOffset;
};
typedef struct GenLinkedOffsetList GenLinkedOffsetList;


void        InitLinkedOffsetList( GenLinkedOffsetList *pList, size_t linkOffset);

void        *GetHeadPtr( GenLinkedOffsetList *pList);
void        *GetTailPtr( GenLinkedOffsetList *pList);
void        *GetOffsetLink( GenLinkedOffsetList *pList, void *elem);

void        OffsetAddToHead( GenLinkedOffsetList *pList, void *elem);
void        OffsetAddToTail( GenLinkedOffsetList *pList, void *elem);

int     OffsetRemoveFromList( GenLinkedOffsetList *pList, void *elem);

int     OffsetReplaceElem( GenLinkedOffsetList *pList, void *elemInList, void *newElem);


#endif //	__GenLinkedList__
