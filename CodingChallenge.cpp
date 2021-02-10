#include "Supervision.h"
#include "Interface.h"

#include <unistd.h>
#include <iostream>

int main(int argc, char **argv) {
	std::cout << "Staring" << std::endl;
	srand (time(NULL));
	Interface* interface = new Interface;
	Supervision supervision;
	supervision.setInterfaceImpl(interface);
	supervision.start();
	sleep(120);
	delete interface;
	return 0;
}
