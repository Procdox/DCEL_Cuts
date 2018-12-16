#pragma once

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

public:
	FLL() {
		head = nullptr;
		tail = nullptr;
	};
	~FLL() {
		head = nullptr;
		tail = nullptr;
	};

	void push(_T value) {
		head = new node<_T>(value, head);

		if (tail == nullptr) tail = head;
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
	}

	//returns the element at head, undefined behavior if empty
	_T pop() {
		_T product = head->value;
		node<_T> * to_be = head->next;

		delete head;

		head = to_be;

		if (head == nullptr) tail = nullptr;

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
	const node<_T>* getHead() const {
		return head;
	};

	node<_T>* getTail() {
		return tail;
	};
	const node<_T>* getTail() const {
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

			return true;
		}

		//general case

		after = head->next;

		while (after != nullptr) {

			if (after->value == search) {
				focus->next = after->next;

				delete after;

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

		return count;
	};
};