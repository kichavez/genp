// main.cpp : Defines the entry point for the application.

#include <iostream>
#include <fstream>
#include "genp.h"
#include "typedefs.h"

using namespace std;

void initEmulator();

int main(int argc, char* argv[]) {

	if (argc != 2) {
		cerr << "Invalid number of arguments";
		return 1;
	}

	byte* romBuffer;

	int size = initEmulator(argv[2], romBuffer);

	Genp emulator;

	emulator.loadRom(romBuffer, size);

	cout << "Hello CMake." << endl;
	return 0;
}

int initEmulator(char* file_path, byte*& buf) {
	ifstream input(file_path, ios::binary);
}
