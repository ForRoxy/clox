#pragma once

#include "memory.h"
#include <iostream>

template <class Value = double >
struct ValueArray {
	int capacity;
	int count;
	Value* values;

	ValueArray() :values(nullptr), capacity(), count(0) {}
	ValueArray(const Value& value);
	~ValueArray();

	void init();
	void addValue(const Value& value);
	int decCount();
	void printValue(const Value& value);

	friend std::ostream& operator<< (std::ostream& os,const ValueArray<Value>& valueArray);
};

template <class Value /*= double */>
void ValueArray<Value>::printValue(const Value& value)
{
	std::cout << value;
}

template <class Value /*= double */>
int ValueArray<Value>::decCount()
{
	return --count;
}

template <class Value /*= double */>
void ValueArray<Value>::addValue(const Value& value)
{
	if (capacity < count + 1) {
		int oldCapacity = capacity;
		capacity = GROW_CAPACITY(oldCapacity);
		values = GROW_ARRAY(Value, values, oldCapacity, capacity);
	}
	values[count] = value;
	count++;
}

template<class Value /*= double */>
ValueArray<Value>::ValueArray(const Value& value) :ValueArray() {
	addValue(value);
}

template <class Value /*= double */>
ValueArray<Value>::~ValueArray()
{
	FREE_ARRAY(Value, values, capacity);
	init();
}

template<class Value>
void ValueArray<Value>::init() {
	capacity = 0;
	count = 0;
	values = nullptr;
}


template <class Value /*= double */>
 std::ostream& operator<< (std::ostream& os,const ValueArray<Value>& valueArray){
	 os << "capacity: " << valueArray.capacity << std::endl;
	 os << "count: " <<  valueArray.count << std::endl;
	 os << "values";
	 for (auto it : valueArray.values) {
		 os << it << "  ";
	 }
	 os << std::endl;
}
