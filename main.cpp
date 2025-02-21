#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
	VM vm;
	vm.initVM();
	Chunk<double> chunk;
	chunk.writeConstant(2.2, 111);
	chunk.writeConstant(6.2, 112);
	chunk.writeChunk(OP_RETURN,123);
	vm.interpret(&chunk);
	vm.freeVM();
	return 0;
}