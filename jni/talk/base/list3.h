#include <android/log.h>    
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "P2P", __VA_ARGS__)

// #include "talk/base/scoped_ptr.h"
namespace sigslot{
	
template<class T> class Link;
template<class T> class List_iterator;

template<class T>
class List {
public:
	typedef List_iterator<T> iterator;

	List();
	List(const List<T> & l);
	~List();

	bool empty() const;
	unsigned int size() const;
	T & back() const;
	T & front() const;
	void push_front(const T & x);
	void push_back(const T & x);
	void pop_front();
	void pop_back();
	iterator begin() const;
	iterator end() const;
	void insert(iterator pos, const T & x);
	void erase(iterator & pos);
	// void erase(iterator & first_pos, iterator & end_pos);
	List<T> & operator=(const List<T> & l);

protected:
	Link<T> * first_link;
	Link<T> * last_link;
	unsigned int my_size;
};

template<class T>
List<T>::List() {   
	first_link = 0;
	last_link = 0;
	my_size = 0;
}

template<class T>
List<T>::List(const List & l) {  
	first_link = 0;
	last_link = 0;
	my_size = 0;
	for (Link<T> * current = l.first_link; current != 0;
			current = current->next_link)
		push_back(current->value);
}

template<class T>
typename List<T>::iterator List<T>::begin() const {
	return iterator(first_link);
}

template<class T>
class Link {
public: 
	typedef Link<T> link;
	
	Link(const T & x): value(x), next_link(0), prev_link(0) {}

	T value;
	Link<T> * next_link;
	Link<T> * prev_link;
   
	bool operator==(const link & rlink) const;
	
	friend class List<T> ;
	friend class List_iterator<T> ;
};

template<class T>
bool Link<T>::operator==(const link & rlink) const { 
	return this->value == rlink.value;
}

template<class T> class List_iterator //pg.207
{
public:
	typedef List_iterator<T> iterator;

	List_iterator(Link<T> * source_link) :
			current_link(source_link) {
	}
	List_iterator() :
			current_link(0) {
	}
	List_iterator(List_iterator<T> * source_iterator) :
			current_link(source_iterator->current_link) {
	}

	T & operator*(); // dereferencing operator
	iterator & operator=(const iterator & rhs);
	bool operator==(const iterator & rhs) const;
	bool operator!=(const iterator & rhs) const;
	iterator & operator++();
	//iterator operator++(int);
	iterator & operator--();
	//iterator operator--(int);

protected:
	Link<T> * current_link;

	friend class List<T> ;
};

template<class T>
T & List_iterator<T>::operator*() {
	return current_link->value;
}

template<class T>
List_iterator<T> & List_iterator<T>::operator++() {
	current_link = current_link->next_link;
	return *this;
}

template<class T>
List_iterator<T> & List_iterator<T>::operator--() {
	current_link = current_link->prev_link;
	return *this;
}  
  
template<class T>
List_iterator<T> & List_iterator<T>::operator=(const iterator & rhs){
	current_link = rhs.current_link;
	return *this;  
}    

template<class T>
void List<T>::push_back(const T & x) {
	Link<T> * new_link = new Link<T>(x);
	LOGD("try to push new-link");
	if (0 == first_link){
		first_link = last_link = new_link;    
	}	
	else {
		new_link->prev_link = last_link;
		last_link->next_link = new_link;
		last_link = new_link;
	}
	my_size++;
}

template<class T>
void List<T>::erase(iterator & pos) {
	Link<T> * curLink = pos.current_link;
	Link<T> * nextLink = curLink->next_link;
	Link<T> * preLink = curLink->prev_link;

	if (0 == preLink) {
		nextLink->prev_link = 0;
		first_link = nextLink;
	} else if (0 == nextLink) {
		preLink->next_link = 0;
		last_link = preLink;
	} else {
		preLink->next_link = nextLink;
	}
	my_size--;
	delete curLink;
}  

// template<class T>
// void List<T>::erase(iterator & first_pos, iterator & end_pos) {
// 	Link<T> * firstLink = first_pos.current_link;
// 	Link<T> * endLink = end_pos.current_link;
// 
// 	while (firstLink != endLink) {
// 		firstLink = firstLink->next_link;
// 		my_size--;
// 	}
// 	//delete firstLink;
// 	//delete endLink;
// }  

template<class T>
typename List<T>::iterator List<T>::end() const {
	return iterator(last_link);
}

template<class T>
List<T>::~List() {
	Link<T> * first = first_link;
	while (first != 0) {
		Link<T> * next = first->next_link;
		delete first;
		first = next;
	}            
	delete first_link;
	delete last_link; 
}

template<class T>
bool List_iterator<T>::operator==(const iterator & rhs) const {
	return (this->current_link == rhs.current_link);
}

template<class T>
bool List_iterator<T>::operator!=(const iterator & rhs) const {
	return !(*this == rhs);
}      

};
