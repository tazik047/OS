#include "stdafx.h";
#include <Windows.h>;
#include "List.h";

int size = 0;
List::~List() {
	node *current, *temp;
	current = head;
	temp = head;
	while (current != NULL) {
		current = current->next;
		delete temp;
		temp = current;
	}
}
int List::getLength() {
	return size;
}
struct List::node* List::initNode(memPages s, int id) {
	struct node *ptr = new node;

	// error? then just return
	if (ptr == NULL)
		return static_cast< struct node *> (NULL);
	// assign it 
	// then return pointer to ne node
	else {
		ptr->name = s;
		ptr->id = id;
		return ptr;
	}
}

// adding to the end of List  
void List::addNode(struct node *newNode)  {
	// if there is no node, put it to head
	if (head == NULL) {
		head = newNode;
		tail = newNode;
	}

	// link in the new_node to the tail of the List
	// then mark the next field as the end of the List
	// adjust tail to point to the last node

	tail->next = newNode;
	newNode->next = NULL;
	tail = newNode;
	size++;
}

bool List::contains(struct node* nd) {
	if (head == NULL) {                          
		return false;
	}
	struct node *temp, *prev;
	temp = nd;    
	prev = head;   
	if (head->name.sizePages == nd->name.sizePages 
		&& head->name.address == nd->name.address) {
		return true;
	}
	while (prev->next != NULL) {
		if (prev->next->name.sizePages == nd->name.sizePages
			&& prev->next->name.address == nd->name.address) {
			return true;
		}
		prev = prev->next;
	}
	return false;
}
void List::deleteOddNode(struct List::node *nd) {
	if (head == NULL) {
		return;
	}
	head = head->next;
	size--;
	addNode(nd);
}
void List::changePages(struct List::node *nd) {
	if (head == NULL) {
		return;
	}
	struct node *temp, *prev, *Tail;
	temp = nd;   
	prev = head;  
	if (head->name.sizePages == nd->name.sizePages
		&& head->name.address == nd->name.address) {
		head = head->next;
		size--;
		addNode(temp);
		return;
	}
	deleteNode(nd);
	addNode(nd);
		/*if (head->next->name.sizePages == nd->name.sizePages
			&& head->next->name.address == nd->name.address) {
			head->next = head->next->next;
			size--;
			addNode(nd);
			return;
		}
		prev = prev->next;*/
}

void List::deleteNode(struct List::node *ptr)
{
		struct node *pPre = NULL, *pDel = NULL;
		if (head==ptr ) {
			/* point to the node to be deleted */
			pDel = head;
			/* update */
			head = pDel->next;
			delete pDel;
			return;
		}
		pPre = head;
		pDel = head->next;
		while (pDel != NULL) {
			if (pDel->name.sizePages == ptr->name.sizePages && pDel->name.address == ptr->name.address) {
				/* Update the list */
				pPre->next= pDel->next;
				/* If it is the last node, update the tail */
				if (pDel == tail) {
					tail = pPre;
				}
				delete pDel; /* Here only remove the first node with the given value */
				break; /* break and return */
			}
			pPre = pDel;
			pDel = pDel->next;
		}
}

void List::deleteList(struct node *ptr)
{
	struct node *temp;

	if (head == NULL) return;   	// don't try to delete an empty List      

	if (ptr == head) {			// if we are deleting the entire List    
		head = NULL;			// then reset head and    
		tail = NULL;			// end to empty                                     
	}
	else {
		temp = head;			// if it's not the entire List, readjust end   
		while (temp->next != ptr)       // locate previous node to ptr   
			temp = temp->next;
		tail = temp;                     // set end to node before ptr   
	}

	while (ptr != NULL) {		// whilst there are still nodes to delete      
		temp = ptr->next;			// record address of next node                 
		delete ptr;			// free this node                             
		ptr = temp;			// point to next node to be deleted           
	}
	size = 0;
}

void List::printList() {
	node *current;
	current = head;
	if (current == NULL) {
		_tprintf(_T("The list is empty"));
		return;
	}
	int i = 0;
	while (current != NULL) {
		_tprintf(_T("%d. Address: %d Size: %d\n"), i, current->name.address, current->name.sizePages);
		current = current->next;
		i++;
	}
}