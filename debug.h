#pragma once
#include "chunk.h"
  
// The display function for OP_RETURN
static int simpleInstruction(const char* name, int offset) {
	std::cout << name << std::endl;
	return offset + 1;
}
// The display function for OP_CONSTANT
template <class T>
static int constantInstruction(const char* name, Chunk<T>* chunk, int offset) {
	//�ҵ������ڳ������е���������code�е���һ���ֽ�ֵ
	uint8_t constant = chunk->code[offset + 1];
	//����������ƺͳ�������
	printf("%-16s %4d '", name, constant);
	//���������ֵ
	chunk->constants.printValue(chunk->constants.values[constant]);
	printf("'\n");
	//ƫ��ֵ+2����constantָ��ֻ�������ֽ�
	return offset + 2;
}
// The display function for OP_CONSTANT_LONG
template <class T>
static int longConstantInstruction(const char* name, Chunk<T>* chunk, int offset) {
		// Handle OP_CONSTANT_LONG
		uint8_t byte1 = chunk->code[offset + 1];
		uint8_t byte2 = chunk->code[offset + 2];
		uint8_t byte3 = chunk->code[offset + 3];
		uint32_t constantIndex = (byte1 | (byte2 << 8) | (byte3 << 16));  // Combine the 3 bytes into a 24-bit index

		printf("%-16s %6d '", name, constantIndex);
		chunk->constants.printValue(chunk->constants.values[constantIndex]);
		printf("'\n");
		return offset + 4;  // OP_CONSTANT_LONG takes 4 bytes (1 byte for the instruction, 3 bytes for the constant index)

}

// Disassemble all of the instructions in the entire chunk
template <class T>
void disassembleChunk(Chunk<T>* chunk, const char* name)
{
	// Print a little header
	std::cout << "== " << name << " ==" << std::endl;
	int sz = chunk->code.size();
	for (int offset = 0; offset < sz;) {
		offset = disassembleInstruction(chunk, offset);
	}
}

// Disassembling a single instruction
template <class T>
int disassembleInstruction(Chunk<T>* chunk, int offset)
{
	// Print the byte offset of the given instruction
	printf("%04d ", offset);

	// ��ȡ�к�
	int line = chunk->getLine(offset);

	// ������һ��ָ���к���ͬ��ʡ���к�
	if (offset > 0 && line == chunk->getLine(offset - 1)) {
		printf("  | ");
	}
	else {
		std::cout << line<<" ";
	}

	// Read a single byte from the byte code at the given offset
	uint8_t instruction = chunk->code[offset];
	switch (instruction) {
	case OP_RETURN:
		return simpleInstruction("OP_RETURN", offset);
	case OP_CONSTANT:
		return constantInstruction("OP_CONSTANT", chunk, offset);
	case OP_CONSTANT_LONG:
		return longConstantInstruction("OP_CONSTANT_LONG", chunk, offset);
	default:
		printf("Unknown opcode %d\n", instruction);
		return offset + 1;
	}
	return 0;
}



