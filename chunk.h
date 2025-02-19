#pragma once
#include "common.h"
#include "value.h"
#include <iostream>
#include <unordered_set>


enum OpCode {
	OP_RETURN,
	OP_CONSTANT,
}; 


template<class T = double>
struct Chunk {
	int count;
	int capacity;
	uint8_t* code;
	int* lines;                                                                                                      
	ValueArray<T> constants;
	                                 
	Chunk() :count(0), capacity(0), code(nullptr),lines(nullptr) {}
	~Chunk();

	void init();
	void writeChunk(uint8_t byte, int lines);
	int addConstant(const T& value);

	/*functions for testing*/
	void printCode(){
		std::cout << "code: ";
		for (int i = 0; i < count; ++i) {
			std::cout  << code[i] << " ";
		}
		std::cout << std::endl;
	}
	void printLines() {
		std::cout << "lines: ";
		for (int i = 0; i < count; ++i) {
			std::cout << code[i] << " ";
		}
		std::cout << std::endl;
	}
	void printConstants() {
		std::cout << constants << std::endl;
	}
};

template<class T /*= double*/>
int Chunk<T>::addConstant(const T& value)
{
	constants.addValue(value);
	return constants.decCount();
}

template<class T /*= double*/>
Chunk<T>::~Chunk()
{
	FREE_ARRAY(uint8_t, code, capacity);
	FREE_ARRAY(int, lines, capacity);
	init();
}

template<class T /*= double*/>
void Chunk<T>::init()
{
	count = 0;
	capacity = 0;
	code = nullptr;
	lines = nullptr;
}

template<class T /*= double*/>
void Chunk<T>::writeChunk(uint8_t byte, int line)
{
	if (capacity < count + 1) {
		int oldCapacity = capacity;
		capacity = GROW_CAPACITY(oldCapacity);
		code = GROW_ARRAY(uint8_t, code,
			oldCapacity, capacity);
		lines = GROW_ARRAY(int, lines,
			oldCapacity, capacity);
	}
	code[count] = byte;
	lines[count] = line;
	count++;
}
