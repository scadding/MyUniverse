/*
 *  file: options.h
 *  class: options
 *
 *   description:
 *
 *
 *    copyright:
*/


#ifndef OPTIONS_H
#define OPTIONS_H

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

class options {
	public:
		options();
		options(char *filename);
		~options();
		options(int argc, char **argv);
		
		int readFile(char *filename);
		int addCmdLineOptions(int argc, char **argv);
		
		enum opType {
			opString,
			opInt,
			opStringArray,
			opFlag,
			opNull
		};
		
		struct option {
			char *name;
			char *shortName;
			char *data;
			enum opType type;
			struct option *next;
		} *op;
	private:
	protected:
}; /* options */

#endif /* OPTIONS_H */
