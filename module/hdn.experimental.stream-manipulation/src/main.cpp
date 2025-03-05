#include <iostream>

#include "hdef_map.h"

int main() {
	using namespace hdn;

	DynamicMemoryBuffer memBuffer;
	StreamWriter out{ &memBuffer };
	StreamReader in{ &memBuffer };

	// Write data
	HDefMap mapOut;
	mapOut.SetTest( 7 );
	mapOut.SetVersion( 4 );
	mapOut.AddEntity( 2 );
	mapOut.AddEntity( 6 );
	mapOut.AddEntity( 3 );
	out << mapOut;
	out << mapOut;
	out << mapOut;
	out << mapOut;
	out << mapOut;

	memBuffer.ResetHead();

	// Read data
	HObject* mapIn = &mapOut;
	in >> *mapIn;

	std::cout << memBuffer.getBuffer().size() << std::endl;
	std::cout << mapIn->m_Test << std::endl;

	return 0;
}