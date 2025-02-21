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
// ֻ�洢ÿ�еĵ�һ���ֽ�����������������ֽ����ö��ֲ��ҵķ�ʽȷ���к�
struct LineInfo {
	int index;       //��Ӧ���ֽ�����ʼ����
	int lineNumber;  //�к�

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
	std::vector<uint8_t> code;    //�洢�ֽڲ���������ڳ������е�����
	std::vector<LineInfo> lines;  //�洢���е�һ���ֽ�����code�е������Լ��к�                                                                                                   
	ValueArray<T> constants;      //�����أ��洢�ÿ�ĸ�������
	                   
	//Ĭ�Ϲ��캯������������
	Chunk() = default;
	~Chunk() = default;

	//���ֽ���������� �� �к�д��chunk
	void writeChunk(uint8_t byte, int line);
	//����������ӳ����������ڳ������е�����
	int addConstant(const T& value);
	//ͨ���ֽ���������ȡԴ������к�
	int getLine(int instructionIndex) const;
	//д�볣������������Ӧ������С
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

	// ��ȡԭʼָ��
	uint8_t* getCodePtr() { return code.data(); }
	uint8_t* getCodeEnd() { return code.data() + code.size(); }

};

template<class T /*= double*/>
void Chunk<T>::writeConstant(const T& value, int line)
{
	// ��ȡ�����ص�����
	int constantIndex = addConstant(value);

	// �жϳ����ص�������С��ѡ��ͬ�Ĳ�����
	if (constantIndex <= 255) {
		// ʹ�� OP_CONSTANT��1�ֽڲ�����
		writeChunk(OP_CONSTANT, line);  // д�� OP_CONSTANT
		writeChunk(static_cast<uint8_t>(constantIndex), line);  // д�볣��������1�ֽڣ�
	}
	else {
		// ʹ�� OP_CONSTANT_LONG��3�ֽڲ�������ʾ3�ֽ�����
		writeChunk(OP_CONSTANT_LONG, line);  // д�� OP_CONSTANT_LONG
		writeChunk(static_cast<uint8_t>(constantIndex & 0xFF), line);  // ���ֽ�
		writeChunk(static_cast<uint8_t>((constantIndex >> 8) & 0xFF), line);  // ���ֽ�
		writeChunk(static_cast<uint8_t>((constantIndex >> 16) & 0xFF), line);  // ���ֽ�
	}
}

template<class T /*= double*/>
int Chunk<T>::addConstant(const T& value)
{
	constants.addValue(value);
	//���س����ڳ������е�����
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

// ʹ�ö��ֲ���ȷ���ֽ�����к�
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