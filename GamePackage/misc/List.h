#include<iostream>
#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#define LIST_FOR(it,list) for(auto it = list.first(); it!=list.tail; it = it->next)

template<class T>
struct Node {
	Node(Node<T>*prev, Node<T>*next, T data)
		:prev(prev), next(next), data(data) {}
	Node(Node<T>*prev = NULL, Node<T>*next = NULL)
		:prev(prev), next(next) {}
	Node<T>*next;
	Node<T>*prev;
	T data;
};

//doubly linked list supporting capacity declaration -> highly recommend declaring the amount of
//elements to be store from the beginning in order to avoid the problem of memory fragmentation
//due to lonely new operator.
template<class T>
class List {

	int m_size;
	int m_capacity;
	Node<T>*m_memory;

public:
	List(int capacity = 0) :m_capacity(capacity) {
		if (m_capacity > 0) {
			m_memory = new Node<T>[m_capacity + 2];
			head = &m_memory[0];// new Node<T>(NULL, NULL);
			tail = &m_memory[1];// new Node<T>(head, NULL);
			tail->prev = head;
		}
		else {
			m_memory = NULL;
			head = new Node<T>(NULL, NULL);
			tail = new Node<T>(head, NULL);
		}
		head->next = tail;
		m_size = 0;
	}
	~List() {
		//what if type of data is a dynamic pointer
		if (m_capacity > 0) {
			if (m_size > m_capacity) {
				int outOfMemoryNodeNum = m_size - m_capacity;
				Node<T>*iter = tail->prev;
				for (int i = 0; i < outOfMemoryNodeNum; i++) {
					auto temp = iter->prev;
					delete iter;
					iter = temp;
				}
			}
			delete[] m_memory;
		}
		else {
			LIST_FOR(iter, (*this)) iter = erase(iter);
			delete head;
			delete tail;
		}
		head = NULL;
		tail = NULL;
		m_capacity = 0;
		m_size = 0;
	}

	Node<T>* erase(Node<T>*pointer) {

		Node<T>*temp = pointer->prev;
		pointer->next->prev = pointer->prev;
		pointer->prev->next = pointer->next;

		if (m_size < m_capacity) {
			if (pointer != &m_memory[m_size + 1]) {
				//copy the last element in m_memory
				pointer->next = m_memory[m_size + 1].next;
				pointer->prev = m_memory[m_size + 1].prev;
				pointer->data = m_memory[m_size + 1].data;
				//point the next and prev to this new element  
				m_memory[m_size + 1].next->prev = pointer;
				m_memory[m_size + 1].prev->next = pointer;
			}
		}
		else delete pointer;


		pointer = NULL;
		m_size--;
		return temp;
	}
	//insert after a given node
	void insert(Node<T>*pointer, T data) {
		Node<T>*newNode = NULL;
		if (m_size < m_capacity) {
			newNode = &m_memory[m_size + 2];
			newNode->prev = pointer;
			newNode->next = pointer->next;
			newNode->data = data;
			std::cout << "Inserting under capacity " << m_size << " " << m_capacity << '\n';
		}
		else {
			std::cout << "Inserting above capacity " << m_size << " " << m_capacity << '\n';
			newNode = new Node<T>(pointer, pointer->next, data);
		}
		newNode->next->prev = newNode;
		newNode->prev->next = newNode;
		m_size++;
	}
	bool clear() {
		if (m_capacity > 0) {
			if (m_size > m_capacity) {
				int outOfMemoryNodeNum = m_size - m_capacity;
				Node<T>*iter = tail->prev;
				for (int i = 0; i < outOfMemoryNodeNum; i++) {
					auto temp = iter->prev;
					delete iter;
					iter = temp;
				}
			}
			m_size = 0;
			head->next = tail;
			tail->prev = head;
		}
		else
			LIST_FOR(iter, (*this)) iter = erase(iter);

	}
	void push_back(T data) { insert(tail->prev, data); }
	void push_front(T data) { insert(head, data); }
	void pop_front() { if (head->next != tail) erase(head->next); }
	void pop_back() { if (tail->prev != head) erase(tail->prev); }

	bool empty() { return(m_size == 0); }
	int size()const { return this->m_size; }
	Node<T>*first() { return head->next; }
	Node<T>*last() { return tail->prev; }

	//two controling nodes
	Node<T>*head;
	Node<T>*tail;
};

//This old version of double linked list does not support capacity declaration
//template<class T>
//struct Node {
//	Node(Node<T>*prev, Node<T>*next,T data) 
//		:prev(prev),next(next),data(data){}
//	Node(Node<T>*prev = NULL, Node<T>*next = NULL)
//		:prev(prev), next(next) {}
//
//	Node<T>*next;
//	Node<T>*prev;
//	T data;
//};
//
////list implemented by doubly linked list
//template<class T>
//class List {
//
//	int m_size;
//	int m_capacity;
//	Node<T>*m_memory;
//
//public:
//	List() {
//
//		m_capacity = 0;
//		m_memory = NULL;
//		head = new Node<T>(NULL, NULL);
//		tail = new Node<T>(head, NULL);
//		head->next = tail;
//		m_size = 0;
//	}
//	List(int capacity):m_capacity(capacity) {
//		m_memory = new Node<T>[m_capacity+2];
//		head = m_memory[0];// new Node<T>(NULL, NULL);
//		tail = m_memory[1];// new Node<T>(head, NULL);
//		head->next = tail;
//		tail->prev = head;
//		m_size = 0;
//	}
//	~List() {
//		//what if type of data is a dynamic pointer
//		clear();
//		delete head;
//		delete tail;
//		head = NULL;
//		tail = NULL;
//		m_size = 0;
//	}
//
//	Node<T>* erase(Node<T>*pointer) {
//		Node<T>*temp = pointer->prev;
//		pointer->next->prev = pointer->prev;
//		pointer->prev->next = pointer->next;
//		
//		pointer->next = NULL;
//		pointer->prev = NULL;
//		
//		delete pointer;
//		pointer = NULL;
//		m_size--;
//		return temp;	
//	}
//		//insert after a given node
//	void insert(Node<T>*pointer, T data) {
//		if (m_size < m_capacity - 2) {
//			Node<T>*newNode = m_memory[m_size];
//			newNode->next->prev = newNode;
//			newNode->prev->next = newNode;
//		}
//		else {
//			Node<T>*newNode = new Node<T>(pointer, pointer->next, data);
//			newNode->next->prev = newNode;
//			newNode->prev->next = newNode;
//		}
//		m_size++;
//	}
//	
//	void push_back(T data)	{ insert(tail->prev, data);}
//	void push_front(T data) { insert(head, data);}
//	void pop_front()		{ if (head->next != tail) erase(head->next);}
//	void pop_back()			{ if (tail->prev != head) erase(tail->prev);}
//	bool clear()			{ for(iter, this) iter = erase(iter);return true;}
//	bool empty()			{ return(m_size == 0);}
//	int size()const			{ return this->m_size;}
//	Node<T>*first()			{ return head->next; }
//	Node<T>*last()			{ return tail->prev; }
//
//	//two controling nodes
//	Node<T>*head;
//	Node<T>*tail;
//};
#endif // !DOUBLY_LINKED_LIST_H
