#pragma once

#include <string>
#include <fstream>


class Object {
public:
	virtual ~Object() {};
	virtual std::string toString() = 0;
};

class Integer : public Object {
public:
	int value;

	Integer(int value) {
		this->value = value;
	}

	std::string toString() {
		return std::to_string(value);
	}

	Integer operator+ (const Integer &rhs) const {
		return Integer(this->value + rhs.value);
	}

	Integer operator- (const Integer& rhs) const {
		return Integer(this->value - rhs.value);
	}

	Integer operator/ (const Integer& rhs) const {
		return Integer(this->value / rhs.value);
	}

	Integer operator* (const Integer& rhs) const {
		return Integer(this->value * rhs.value);
	}

	bool operator!=(const Integer& rhs) {
		return (this->value != rhs.value);
	}

	bool operator==(const Integer& rhs) {
		return (this->value == rhs.value);
	}

	bool operator>=(const Integer& rhs) {
		return (this->value >= rhs.value);
	}

	bool operator<=(const Integer& rhs) {
		std::cout << "WORKING";
		return (this->value <= rhs.value);
	}

	bool operator>(const Integer& rhs) {
		return (this->value > rhs.value);
	}

	bool operator<(const Integer& rhs) {
		return (this->value < rhs.value);
	}

	friend std::ostream& operator<< (std::ostream& out, const Integer& obj) {
		return out << obj.value;
	}

};

class String : public Object {
public:
	std::string value;

	String(std::string value) {
		this->value = value;
	}

	std::string toString() {
		return value;
	}

	friend std::ostream& operator<< (std::ostream& out, const String& obj) {
		return out << obj.value;
	}
};

class Boolean : public Object {
public:
	bool value;

	Boolean(bool value) {
		this->value = value;
	}

	std::string toString() {
		return std::to_string(value);
	}

	bool operator!=(const Boolean& rhs) {
		return this->value != rhs.value;
	}

	bool operator==(const Boolean& rhs) {
		return this->value == rhs.value;
	}


	friend std::ostream& operator<< (std::ostream& out, const Boolean& obj) {
		if (obj.value) {
			return out << "True";
		}
		else {
			return out << "False";
		}
	}
};

class None : public Object {
public:

	std::string toString() {
		return "None";
	}

};
