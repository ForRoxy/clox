#pragma once
#include <vector>
#include <iostream>
#include "chunk.h"
#include "debug.h"

enum InterpretResult {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
};

class VM
{
private:
	Chunk<>* chunk;
	uint8_t* ip;
	using Value = double;

	// **ָ�����**
	void handleOpReturn();
	void handleOpConstant();

	// **ָ���Function Pointer Table��**
	using InstructionHandler = void (VM::*)();
	static constexpr InstructionHandler instructionTable[] = {
		&VM::handleOpReturn,  // OP_RETURN = 0
		&VM::handleOpConstant // OP_CONSTANT = 1
	};

public:
	VM() = default;
	~VM() = default;

	void initVM();
	void freeVM();

	InterpretResult interpret(Chunk<>* chunk);
	InterpretResult run();
};

