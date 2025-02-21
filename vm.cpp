#include "vm.h"

void VM::initVM()
{

}

void VM::freeVM()
{

}

//获取语句块及字节码并运行
InterpretResult VM::interpret(Chunk<>* chunk)
{
	this->chunk = chunk;
	this->ip = this->chunk->getCodePtr();
	return run();
}

// **指令处理函数**
void VM::handleOpReturn() {
	// OP_RETURN 处理逻辑
}

void VM::handleOpConstant() {
	auto constant = chunk->constants.values[*ip++];
	chunk->constants.printValue(constant);
	std::cout << std::endl;
}

// **运行虚拟机**
InterpretResult VM::run() {
#define READ_BYTE() (*ip++)
#define READ_CONSTANT() (chunk->constants.values[READ_BYTE()])
	while (true) {
#ifdef DEBUG_TRACE_EXECUTION
		disassembleInstruction(chunk,
			(int)(ip - chunk->getCodePtr()));
#endif
		uint8_t instruction = READ_BYTE();

		// **检查opcode是否有效**
		if (instruction >= sizeof(instructionTable) / sizeof(instructionTable[0])) {
			std::cerr << "Unknown opcode: " << static_cast<int>(instruction) << std::endl;
			return INTERPRET_RUNTIME_ERROR;
		}

		// **调用成员函数**
		(this->*instructionTable[instruction])();

		// **如果是 `OP_RETURN`，终止运行**
		if (instruction == 0) {
			return INTERPRET_OK;
		}
	}

#undef READ_BYTE
#undef READ_CONSTANT
}