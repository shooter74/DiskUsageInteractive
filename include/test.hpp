#ifndef H_Test
#define H_Test

#include <iostream>
#include <TreeNodeDiskUsage.hpp>
#include <Display.hpp>
#include <AnsiTerminal.hpp>

using std::cout;
using std::cerr;
using std::endl;

#ifdef D_UNIT_TESTS
void runTests(int argc, char *argv[]);
#endif

#endif