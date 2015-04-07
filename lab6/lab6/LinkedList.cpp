#include "stdafx.h"
#include "LinkedList.h"
#include "Methods.h"

struct memPages {
	LPVOID address;
	SIZE_T sizePages;
};

typedef struct LinkedListItem
{
	memPages Number;
	LinkedListItem* Next;


	LinkedListItem( memPages mp)
	{
		Number = mp;
		Next = NULL;
	}
} LinkedListItem;

class LinkedList
{
public:
	LinkedListItem* Head;
	LinkedListItem* End;

	LinkedList();

	void Add(memPages number);
	
	LinkedListItem* ElementAt(int index);

	bool Insert(int index, memPages number);

	bool Remove(memPages number);

	void Clear();

	~LinkedList();
	
};

void LinkedList::Add(memPages number)
{
	LinkedListItem* item = new LinkedListItem(number);
	if (Head == NULL)
	{
		Head = item;
		End = item;
	}
	else
	{
		End->Next = item;
		End = End->Next;
	}
}

LinkedListItem* LinkedList::ElementAt(int index)
{
	int idx = 0;
	LinkedListItem* curr = Head;
	while (curr != NULL && idx < index)
	{
		curr = curr->Next;
		idx++;
	}

	if (idx != index)
	{
		curr = NULL;
	}

	return curr;
}

bool LinkedList::Insert(int index, memPages number)
{
	LinkedListItem* item = new LinkedListItem(number);

	if (index == 0)
	{
		LinkedListItem* tmp = Head;
		Head = item;
		item->Next = tmp;

		if (End == NULL)
		{
			End = item;
		}

		return true;
	}
	else
	{
		LinkedListItem* prev = ElementAt(index - 1);
		if (prev != NULL)
		{
			LinkedListItem* tmp = prev->Next;
			prev->Next = item;
			item->Next = tmp;

			if (prev == End)
			{
				End = item;
			}

			return true;
		}

		return false;
	}
}

void LinkedList::Clear()
{
	LinkedListItem* curr = Head;
	while (curr != NULL)
	{
		LinkedListItem* tmp = curr;
		curr = curr->Next;
		delete tmp;
	}

	Head = NULL;
	End = NULL;
}

bool LinkedList::Remove(memPages number)
{
	LinkedListItem* prev = NULL;
	LinkedListItem* curr = Head;
	while (curr != NULL)
	{
		if (curr->Number.address == number.address)
		{
			if (prev == NULL)
			{
				Head = curr->Next;

				if (End == curr)
				{
					End = NULL;
				}

				delete curr;
			}
			else
			{
				prev->Next = curr->Next;

				if (End == curr)
				{
					End = prev;
				}

				delete curr;
			}

			return true;
		}

		prev = curr;
		curr = curr->Next;
	}

	return false;
}

LinkedList::~LinkedList()
{
	Clear();
}
