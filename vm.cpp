#include "vm.h"

void VM::initVM()
{

}

void VM::freeVM()
{

}

//��ȡ���鼰�ֽ��벢����
InterpretResult VM::interpret(Chunk<>* chunk)
{
	this->chunk = chunk;
	this->ip = this->chunk->getCodePtr();
	return run();
}

// **ָ�����**
void VM::handleOpReturn() {
	// OP_RETURN �����߼�
}

void VM::handleOpConstant() {
	auto constant = chunk->constants.values[*ip++];
	chunk->constants.printValue(constant);
	std::cout << std::endl;
}

// **���������**
InterpretResult VM::run() {
#define READ_BYTE() (*ip++)
#define READ_CONSTANT() (chunk->constants.values[READ_BYTE()])
	while (true) {
#ifdef DEBUG_TRACE_EXECUTION
		disassembleInstruction(chunk,
			(int)(ip - chunk->getCodePtr()));
#endif
		uint8_t instruction = READ_BYTE();

		// **���opcode�Ƿ���Ч**
		if (instruction >= sizeof(instructionTable) / sizeof(instructionTable[0])) {
			std::cerr << "Unknown opcode: " << static_cast<int>(instruction) << std::endl;
			return INTERPRET_RUNTIME_ERROR;
		}

		// **���ó�Ա����**
		(this->*instructionTable[instruction])();

		// **����� `OP_RETURN`����ֹ����**
		if (instruction == 0) {
			return INTERPRET_OK;
		}
	}

#undef READ_BYTE
#undef READ_CONSTANT
}