/*
 *  file: element.h
 *  class: element
 *
 *   description:
 *
 *
 *    copyright:
*/


#ifndef ELEMENT_H
#define ELEMENT_H


using namespace std;

class element {
	public:
		element();
		~element();
		int setPtr(char *set);
		char *getPtr();
		int setValue(int set);
		int getValue();
	private:
		element *next;
		char *ptr;
		int value;
	protected:
}; /* element */

#endif /* ELEMENT_H */
