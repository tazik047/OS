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

	struct List::node* initNode(memPages, int);

	void deleteOddNode(struct List::node*);
	int getLength();
	bool contains(struct List::node*);
	void addNode(struct List::node*);
	void deleteNode(struct List::node*);
	void deleteList(struct List::node*);
	void changePages(struct List::node*);
	void printList();
};