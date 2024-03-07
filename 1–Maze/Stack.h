#ifndef _MAZE_H
#define _MAZE_H

using namespace std;


template <typename T>
class Stack {

private:
	struct Node {
		T data;
		Node* next;
		Node(const T & val, Node * n = nullptr) : data(val), next(n) {}
	};

	Node* head;

public:
	
	Stack();
	Stack(const Stack &);
	~Stack();

	bool isEmpty() const;

	void pop();
	void push(const T &);
	const T & top() const;
	T topAndPop();

	const Stack & operator=(const Stack &);

};

#endif _MAZE_H
