#pragma once

#include <string>
#include <map>
#include <algorithm>

#include "object.h"

class Environment {
public:
	std::map<std::string, Object*> data;
	std::map<std::string, Function*> data_function;

	Environment() {
		data = {};
	}

	bool exists(std::string identifier) {
		return data.find(identifier) != data.end();
	}

	Object* get(std::string identifier) {
		return data[identifier];
	}

	void set(std::string identifier, Object* value) {
		data[identifier] = value;
	}

	void set(std::string identifier, Function* value) {
		data_function[identifier] = value;
	}

	Function* get_function(std::string identifier) {
		return data_function[identifier];
	}
};