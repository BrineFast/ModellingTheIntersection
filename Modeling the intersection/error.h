#include<string>
#include<iostream>

using namespace std;

class Exception {
public:
	Exception();
	Exception(string str);

private:
	string str;
};

Exception::Exception() {

}

Exception::Exception(string str) {
	cerr << str << endl;
}

class InputException : public Exception {
public:
	InputException(string str);
private:
	string str;
};

InputException::InputException(string str) {
	cerr << str << endl;
}

class MemoryException : public Exception {
public:
	MemoryException(string str);
private:
	string str;
};

MemoryException::MemoryException(string str) {
	cerr << str << endl;
}

