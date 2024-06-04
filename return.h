#pragma once

#include <exception>
#include "object.h"

class ReturnException : public std::exception {
public:
	Object* return_object;

	char* what() {
		char* msg = new char[10];
		return msg;
	}

	ReturnException(Object* obj) {
		return_object = obj;
	}

	ReturnException() {
		return_object = new None();
	}
};

