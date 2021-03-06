#pragma once

#ifndef _LINKEDLIST_h
#define _LINKEDLIST_h

template <typename T>
struct LinkedListItem
{
	LinkedListItem<T>* PreviousItem = nullptr;
	LinkedListItem<T>* NextItem = nullptr;
	T Value = nullptr;

	LinkedListItem(T item, LinkedListItem<T>* previous = nullptr)
	{
		Value = item;

		if (previous != nullptr)
		{
			previous->NextItem = this;
			PreviousItem = previous;
		}
	}
};

template <typename T>
class LinkedList
{
	LinkedListItem<T>* _root = nullptr;
	LinkedListItem<T>* _top = nullptr;
	static T _nullValue;

public:
	LinkedList(bool deleteItemsOnRemoval = false)
	{

	}
	void AddItem(T item);
	T RemoveFirstItem();
	T RemoveFirstItem(T item);

	LinkedListItem<T>* FirstItem();
	bool IsEmpty() { return _root == nullptr; }
};

template<typename T>
void LinkedList<T>::AddItem(T item)
{
	auto link = new LinkedListItem<T>(item, _top);

	if (_root == nullptr)
		_root = link;

	_top = link;
}

template<typename T>
inline T LinkedList<T>::RemoveFirstItem()
{
	auto link = this->_root;
	
	T value = static_cast<T>(nullptr);// = this->_nullValue;
	if (link != nullptr)
	{
		value = link->Value;

		if (link == this->_root)
			this->_root = link->NextItem;

		delete link;
	}

	return value;
}

template<typename T>
T LinkedList<T>::RemoveFirstItem(T item)
{
	LinkedListItem<T>* link = this->_root;

	while (link != nullptr && link->Value != item)
		link = link->NextItem;

	T value = static_cast<T>(nullptr);// = this->_nullValue;
	if (link != nullptr)
	{
		value = link->Value;
		
		if (link == this->_root)
			this->_root = link->NextItem;

		delete link;
	}

	return value;
}

template<typename T>
LinkedListItem<T>* LinkedList<T>::FirstItem()
{
	return this->_root;
}

#endif // _LINKEDLIST_h
