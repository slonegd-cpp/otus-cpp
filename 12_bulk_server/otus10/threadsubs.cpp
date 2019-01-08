#include "threadsubs.h"

std::map<std::thread::id, std::size_t> ThreadFileExporter::blocksQty;
std::map<std::thread::id, std::size_t> ThreadFileExporter::cmdQty;
std::size_t ThreadFileExporter::loadCPU  {0};
bool        ThreadFileExporter::failTest {false};

std::size_t ThreadConsoleExporter::blocksQty {0};
std::size_t ThreadConsoleExporter::cmdQty {0};