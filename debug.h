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
	uint8_t constant = chunk->code[offset + 1];
	printf("%-16s %4d '", name, constant);
	chunk->constants.printValue(chunk->constants.values[constant]);
	printf("'\n");
	return offset + 2;
}

// Disassemble all of the instructions in the entire chunk
template <class T>
void disassembleChunk(Chunk<T>* chunk, const char* name)
{
	// Print a little header
	std::cout << "== " << name << " ==" << std::endl;
	for (int offset = 0; offset < chunk->count;) {
		offset = disassembleInstruction(chunk, offset);
	}
}

// Disassembling a single instruction
template <class T>
int disassembleInstruction(Chunk<T>* chunk, int offset)
{
	// Print the byte offset of the given instruction
	printf("%04d ", offset);
	if (offset > 0 &&
		chunk->lines[offset] == chunk->lines[offset - 1]) {
		printf("   | ");
	}
	else {
		printf("%4d ", chunk->lines[offset]);
	}
	// Read a single byte from the byte code at the given offset
	uint8_t instruction = chunk->code[offset];
	switch (instruction) {
	case OP_RETURN:
		return simpleInstruction("OP_RETURN", offset);
	case OP_CONSTANT:
		return constantInstruction("OP_CONSTANT", chunk, offset);
	default:
		printf("Unknown opcode %d\n", instruction);
		return offset + 1;
	}
	return 0;
}


