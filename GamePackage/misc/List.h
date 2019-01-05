
#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

template<class T>
struct Node {
	Node(Node<T>*prev, Node<T>*next,T data) 
		:prev(prev),next(next),data(data){}
		
	Node<T>*next;
	Node<T>*prev;
	T data;
};

//list implemented by doubly linked list
template<class T>
class List {
public:
	List() {
		head = new Node<T>(NULL, NULL, (T)0);
		tail = new Node<T>(head, NULL, (T)0);
		head->next = tail;
		n = 0;
	}
	~List() {
		//what if type of data is a dynamic pointer
		clear();
	
		delete head;
		delete tail;
		head = NULL;
		tail = NULL;
		n = 0;
	}

	Node<T>* erase(Node<T>*pointer) {
		
		Node<T>*temp = pointer->prev;
		
		pointer->next->prev = pointer->prev;
		pointer->prev->next = pointer->next;
		
		pointer->next = NULL;
		pointer->prev = NULL;
		
		delete pointer;
		pointer = NULL;
		n--;
		return temp;	
	}
		//insert after a given node
	void insert(Node<T>*pointer, T data) {
		Node<T>*newNode = new Node<T>(pointer, pointer->next, data);
		newNode->next->prev = newNode;
		newNode->prev->next = newNode;
		n++;
	}
	
	void push_back(T data) {
		insert(tail->prev, data);
	}
	void push_front(T data) {
		insert(head, data);
	}
	void pop_front() {
		if (head->next != tail)
			erase(head->next);
	}
	void pop_back() {
		if (tail->prev != head)
			erase(tail->prev);
	}

	bool clear() {
		for (Node<T>*iter = head->next; iter != tail; iter = iter->next)
			iter = erase(iter);
		return true;
	}
	bool empty()const {
		return(n == 0);
	}
	int size()const {
		return this->n;
	}
	Node<T>*first() {
		return head->next;
	}
	Node<T>*last(){
		return tail->prev;
	}
	//two controling nodes
	Node<T>*head;
	Node<T>*tail;
	int n;
};
#endif // !DOUBLY_LINKED_LIST_H
