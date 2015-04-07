#include "stdafx.h";
#include <Windows.h>
class List
{
public:
	struct memPages {
		LPVOID address;
		SIZE_T sizePages;
	};
	struct node {
		int id;
		memPages name;
		struct node *next;
	} *head, *tail, *ptr;

	List() :head(NULL), tail(NULL){}	// constructor	
	~List();			// destructor

	struct List::node* searchName(struct List::node*, memPages);
	struct List::node* searchId(struct List::node*, int);
	struct List::node* initNode(memPages, int);

	void reverse();
	void addNode(struct List::node*);
	void insertNode(struct List::node*);
	void deleteNode(struct List::node*);
	void deleteList(struct List::node*);
};