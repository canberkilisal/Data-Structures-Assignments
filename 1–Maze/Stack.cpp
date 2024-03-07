#include <iostream>
#include "Stack.h"

using namespace std;


struct Cell {

	int x_cord;
	int y_cord;
	int left;
	int right;
	int up;
	int down;
	bool visited;

	Cell() {}

	Cell(int x, int y) {

		x_cord = x;
		y_cord = y;
		left = 1;
		right = 1;
		up = 1;
		down = 1;
		visited = false;
	}

	Cell(const Cell & c) {

		x_cord = c.x_cord;
		y_cord = c.y_cord;
		left = c.left;
		right = c.right;
		up = c.up;
		down = c.down;
		visited = c.visited;

	}
};


template <typename T>
Stack<T>::Stack() { 
	
	head = nullptr; 
}

template <typename T>
Stack<T>::Stack(const Stack & rhs) {
	
	head = nullptr;
	*this = rhs;
}

template <typename T>
Stack<T>::~Stack() {

	while (!isEmpty()) { 
		pop(); 
	}
}

template <typename T>
bool Stack<T>::isEmpty() const { 
	
	return (head == nullptr); 
}

template <typename T>
void Stack<T>::pop() {

	Node *tempHead = head;
	head = head->next;
	delete tempHead;
}

template <typename T>
void Stack<T>::push(const T & x) {

	head = new Node(x, head);
}

template<typename T>
T Stack<T>::topAndPop()
{
	T top(top());
	pop();
	return top;
}

template <typename T>
const T & Stack<T>::top() const { 
	
	return head->data; 
}

template<typename T>
const Stack<T> & Stack<T>::operator=(const Stack<T> & rhs){
	
	if (this != &rhs)
	{
		while (!isEmpty()) { 
			pop(); 
		}
		if (rhs.isEmpty()) {

			return *this;
		}
			
		Node *rptr = rhs.head;
		Node *ptr = new Node(rptr->data);
		head = ptr;

		for (rptr = rptr->next; rptr != nullptr; rptr = rptr->next)
			ptr = ptr->next = new Node(rptr->data);
	}
	return *this;
}


