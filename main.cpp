#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
	Chunk<double> chunk;
	int constant = chunk.addConstant(6.2);
	chunk.writeChunk(OP_CONSTANT,123);
	chunk.writeChunk(constant,123);
	chunk.writeChunk(OP_RETURN,123);
	chunk.printCode();

	disassembleChunk(&chunk, "test chunk");
	return 0;
}