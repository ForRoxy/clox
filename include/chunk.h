#pragma once
#include "common.h"
#include "value.h"
#include <iostream>
#include <vector>


enum OpCode {
	OP_RETURN,
	OP_CONSTANT,
	OP_CONSTANT_LONG,
}; 

// Run-Length Encoding
// 只存储每行的第一个字节码的索引，其他的字节码用二分查找的方式确定行号
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
	std::vector<uint8_t> code;    //存储字节操作码或常量在常量池中的索引
	std::vector<LineInfo> lines;  //存储该行第一个字节码在code中的索引以及行号                                                                                                   
	ValueArray<T> constants;      //常量池，存储该块的各个常量
	                   
	//默认构造函数和析构函数
	Chunk() = default;
	~Chunk() = default;

	//将字节码或常量索引 和 行号写入chunk
	void writeChunk(uint8_t byte, int line);
	//向常量池中添加常量，返回在常量池中的索引
	int addConstant(const T& value);
	//通过字节码索引获取源代码的行号
	int getLine(int instructionIndex) const;
	//写入常量，可以自适应索引大小
	void writeConstant(const T& value, int line);

	/*functions for testing*/
	void printCode(){
		std::cout << "code: ";
		for (const auto it : code) {
			std::cout  << static_cast<int>(it) << " ";
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

	// 获取原始指针
	uint8_t* getCodePtr() { return code.data(); }
	uint8_t* getCodeEnd() { return code.data() + code.size(); }

};

template<class T /*= double*/>
void Chunk<T>::writeConstant(const T& value, int line)
{
	// 获取常量池的索引
	int constantIndex = addConstant(value);

	// 判断常量池的索引大小，选择不同的操作码
	if (constantIndex <= 255) {
		// 使用 OP_CONSTANT，1字节操作数
		writeChunk(OP_CONSTANT, line);  // 写入 OP_CONSTANT
		writeChunk(static_cast<uint8_t>(constantIndex), line);  // 写入常量索引（1字节）
	}
	else {
		// 使用 OP_CONSTANT_LONG，3字节操作数表示3字节索引
		writeChunk(OP_CONSTANT_LONG, line);  // 写入 OP_CONSTANT_LONG
		writeChunk(static_cast<uint8_t>(constantIndex & 0xFF), line);  // 低字节
		writeChunk(static_cast<uint8_t>((constantIndex >> 8) & 0xFF), line);  // 中字节
		writeChunk(static_cast<uint8_t>((constantIndex >> 16) & 0xFF), line);  // 高字节
	}
}

template<class T /*= double*/>
int Chunk<T>::addConstant(const T& value)
{
	constants.addValue(value);
	//返回常量在常量池中的索引
	return constants.values.size()-1;
}

template<class T /*= double*/>
void Chunk<T>::writeChunk(uint8_t byte, int line)
{
	code.push_back(byte);
	if (lines.empty() || lines.back().lineNumber != line) {
		lines.push_back(LineInfo({static_cast<int>(code.size())-1,line}));
	}
}

// 使用二分查找确定字节码的行号
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