#pragma once
#include "common.h"
#include "value.h"
#include <iostream>
#include <vector>


enum OpCode {
	OP_RETURN,
	OP_CONSTANT,
}; 

//Run-Length Encoding
struct LineInfo {
	int index;       //对应的字节码起始索引
	int lineNumber;  //行号

	bool operator==(const LineInfo& right) {
		return index == right.index && lineNumber == right.lineNumber;
	}
	friend std::ostream& operator<<(std::ostream& os, const LineInfo& lineInfo) {
		os << lineInfo.index << " " << lineInfo.lineNumber << std::endl;
		return os;
	}
};

template<class T = double>
struct Chunk {
	std::vector<uint8_t> code;
	std::vector<LineInfo> lines;                                                                                                      
	ValueArray<T> constants;
	                                 
	Chunk() = default;
	~Chunk() = default;

	void writeChunk(uint8_t byte, int lines);
	int addConstant(const T& value);
	int getLine(int instructionIndex) const;

	/*functions for testing*/
	void printCode(){
		std::cout << "code: ";
		for (const auto& it : code) {
			std::cout  << it << " ";
		}
		std::cout << std::endl;
	}
	void printLines() {
		std::cout << "lines: ";
		for (const auto& it:lines) {
			std::cout << it << " ";
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
void Chunk<T>::writeChunk(uint8_t byte, int line)
{
	code.push_back(byte);
	if (lines.empty() || lines.back().lineNumber != line) {
		lines.push_back(LineInfo({static_cast<int>(code.size())-1,line}));
	}
}

// 使用二分查找查找行号
template<class T>
int Chunk<T>::getLine(int instructionIndex) const {
	if (lines.empty()) return -1;
	int left = 0, right = static_cast<int>(lines.size()) - 1;
	while (left < right) {
		int mid = (left + right + 1) / 2;
		if (lines[mid].index <= instructionIndex) {
			left = mid;
		}
		else {
			right = mid - 1;
		}
	}
	return lines[left].lineNumber;
}