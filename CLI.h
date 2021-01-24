

#ifndef CLI_H_
#define CLI_H_
/*
 * CLI.h
 *
 * Author: 322060187 Gali Seregin
 */
#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	inf* infor=new inf();
	//HybridAnomalyDetector *detector= new HybridAnomalyDetector();
	vector<Command *> comma;
	DefaultIO* dio;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
