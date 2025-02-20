#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
	Chunk<double> chunk;
	chunk.writeConstant(2.2, 111);
	chunk.writeConstant(6.2, 112);
	chunk.writeChunk(OP_RETURN,123);
	chunk.printCode();

	disassembleChunk(&chunk, "test chunk");
	return 0;
}