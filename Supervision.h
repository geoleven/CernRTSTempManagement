/*
 * Supervision.h
 *
 *  Created on: Feb 9, 2021
 *      Author: george
 */

#ifndef SUPERVISION_H_
#define SUPERVISION_H_

#include "Interface.h"
//#include <thread>

class Supervision {
public:
	Supervision();
	virtual ~Supervision();

	void start(void);
	void setInterfaceImpl(Interface *  interface);

private:
	void * mainThreadPointer = NULL;
	Interface * interface = NULL;
};

#endif /* SUPERVISION_H_ */
