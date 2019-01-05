#pragma once

/*

Contains definition for a forward linked list template

*/

template <class _T>
class FLL;

template <class _T>
class node {
	friend FLL<_T>;

	_T value;
	node* next;
	node(_T v, node* n) {
		value = v;
		next = n;
	};

	//nodes should NEVER be copied or moved, they are purely internal to the creating list
	node(node&&) = delete;
	node(const node&) = delete;
public:
	_T getValue() const {
		return value;
	};

	void setValue(_T v) {
		value = v;
	};

	node* getNext() {
		return next;
	};
	const node* getNext() const {
		return next;
	};
};

template <class _T>
class FLL {

	node<_T>* head;
	node<_T>* tail;
	int length;

public:
	FLL() {
		head = nullptr;
		tail = nullptr;
		length = 0;
	};
	~FLL() {
		clear();
	};
	FLL(FLL<_T> &&refernce) {
		head = refernce.head;
		tail = refernce.tail;
		length = refernce.length;

		refernce.head = nullptr;
		refernce.tail = nullptr;
		refernce.length = 0;

	}
	FLL(FLL<_T> const &reference) {
		node<_T> * focus = reference.head;
		head = nullptr;
		tail = nullptr;
		length = 0;

		while (focus != nullptr) {
			append(focus->value);

			focus = focus->next;
		}
	};

	FLL<_T> & operator=(FLL<_T> const &reference) {
		node<_T> * focus = reference.head;
		head = nullptr;
		tail = nullptr;
		length = 0;

		while (focus != nullptr) {
			append(focus->value);

			focus = focus->next;
		}

		return *this;
	};

	void push(_T value) {
		head = new node<_T>(value, head);

		if (tail == nullptr) tail = head;

		length++;
	};

	void append(_T value) {
		if (head != nullptr) {
			tail->next = new node<_T>(value, nullptr);
			tail = tail->next;
		}
		else {
			head = new node<_T>(value, nullptr);
			tail = head;
		}

		length++;
	}

	//returns the element at head, undefined behavior if empty
	_T pop() {
		_T product = head->value;
		node<_T> * to_be = head->next;

		delete head;

		head = to_be;

		if (head == nullptr) tail = nullptr;

		length--;

		return product;
	};

	

	bool empty() {
		return head == nullptr;
	}
	bool contains(_T search) const {
		node<_T>* focus = head;

		while (focus != nullptr) {
			if (focus->value == search) return true;
			focus = focus->next;
		}

		return false;
	};

	node<_T>* getHead() {
		return head;
	};
	node<_T> const * getHead() const {
		return head;
	};

	node<_T>* getTail() {
		return tail;
	};
	node<_T> const * getTail() const {
		return tail;
	};

	FLL<_T> reverse() const {
		FLL<_T> product;

		node<_T>* focus = head;

		while (focus != nullptr) {
			product.push(focus->value);
			focus = focus->next;
		}

		return product;
	}

	bool remove(_T search) {
		node<_T>* focus = head;
		node<_T>* after;

		//special cases

		if (head == nullptr) return false;

		if (head->value == search) {
			head = head->next;
			if (head == nullptr) tail = nullptr;

			delete focus;
			length--;

			return true;
		}

		//general case

		after = head->next;

		while (after != nullptr) {

			if (after->value == search) {
				focus->next = after->next;

				delete after;
				length--;

				return true;
			}

			focus = after;
			after = after->next;
		}
		return false;
	};

	int removeAll(_T search) {
		node<_T>* focus = head;
		node<_T>* after;
		int count = 0;

		//special cases

		while (head != nullptr && head->value == search) {
			head = head->next;

			delete focus;
			count++;

			focus = head;
		}

		if (head == nullptr) {
			tail = nullptr;
			return count;
		}

		//general case

		after = head->next;

		while (after != nullptr) {

			if (after->value == search) {
				focus->next = after->next;

				delete after;
				count++;

				after = focus->next;
			}
			else {
				focus = after;
				after = after->next;
			}
		}

		if (head == nullptr) tail = nullptr;

		length -= count;

		return count;
	};

	//appends the target list, and empties it
	void absorb(FLL<_T> &target) {
		if (target.head != nullptr) {

			if (head == nullptr) {
				head = target.head;
			}
			else {
				tail->next = target.head;
			}

			tail = target.tail;

			length += target.length;

			target.head = nullptr;
			target.tail = nullptr;
			target.length = 0;
		}
	}

	int size() const {
		node<_T>* focus = head;
		int s = 0;
		
		while (focus != nullptr) {
			s++;
			focus = focus->next;
		}

		return s;
	}

	int l() const {
		return l;
	}

	void qInsert(_T value, bool (*compare)(_T, _T)) {
		node<_T> * focus = head;
		node<_T> * after;

		if (head == nullptr) {
			push(value);
			return;
		}

		if (compare(head->getValue(), value)) {
			push(value);
			return;
		}

		after = head->next;
		
		while (after != nullptr) {
			if (compare(after->getValue(), value)) {

				focus->next = new node<_T>(value, after);
				length++;
				return;
			}

			focus = after;
			after = after->next;
		}

		append(value);
	}
	void clear() {
		auto focus = head;

		while (head != nullptr) {
			focus = head->next;
			delete head;
			head = focus;
		}

		tail = nullptr;

		length = 0;
	};

	node<_T>* operator[](int index) {
		node<_T>* focus = head;

		while (focus != nullptr && index > 0) {
			focus = focus->next;
			index-=1;
		}

		return focus;
	}
};