/*************************************************************************/
/* demo.cpp                                                              */
/*************************************************************************/
/* https://www.dandevlog.com/all/programming/1347/                       */
/* https://github.com/dandevlog0206/cpp-property                         */
/*************************************************************************/
/* Copyright (c) 2024 www.dandevlog.com                                  */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include <iostream>
#include <string>
#include <util/property.hpp>

using namespace std;

class MyClass {
	PROPERTY_INIT(MyClass);

public:
	MyClass() :
		prop_default_int1(20) // Auto implemented property can be initialized by member-initializer
	{
		// Normal properties can be initialized by initializing in constructor
		int_member    = 1;
		float_member  = 3.14;
		string_member = "hello";
	}

public:
	PROPERTY {
		PROPERTY_DEFAULT_GET(int, prop_default_int0) = 10; // Auto implemented property only with Getter
		PROPERTY_DEFAULT_SET(int, prop_default_int1);      // Auto implemented property only with Setter
		PROPERTY_DEFAULT_GET_SET(int, prop_default_int2);  // Auto implemented property with Getter/Setter
	};

	PROPERTY {
		PROPERTY_GET(int, prop_int);           // Property only with Getter
		PROPERTY_SET(float, prop_float);       // Property only with Setter
		PROPERTY_GET_SET(string, prop_string); // Property with Getter/Setter
	};

private:
	GET(prop_int) {
		return int_member;
	}

	SET(prop_float) {
		float_member = min(max(value, 0.f), 10.f);
	}

	GET(prop_string); // Declare Getter/Setter of prop_string property
	SET(prop_string);

private:
	int         int_member;
	float       float_member;
	std::string string_member;
};

GET_IMPL(MyClass, prop_string) {
	cout << "someone got string message!\n";
	return string_member;
}

SET_IMPL(MyClass, prop_string) {
	cout << "someone set string message!\n";
	cout << "from : " << string_member << endl;
	cout << "to   : " << value << endl;
	string_member = value;
}

int main() {
	MyClass inst;

	int int0 = inst.prop_default_int0;
	// int int1 = inst.prop_default_int1; Error, write-only property
	int int2 = inst.prop_default_int2;

	// inst.prop_default_int0 = 100; Error, read-only property
	inst.prop_default_int1 = 100;
	inst.prop_default_int2 = 100;

	int int_value = inst.prop_int;
	// inst.prop_int = 100; Error, read-only property
	
	// float float_value = inst.prop_float; Error
	inst.prop_float = 100.f; 

	string string_value = inst.prop_string;
	inst.prop_string = "Hello World!";
	cout << inst.prop_string.get() << endl;
	cout << ((string)inst.prop_string).front() << endl; // Can be acessed after casting(but, copied instance)
	
	/* Warnings(use inst.prop_string.get() instead)

	1. cout << inst.prop_string << endl; Error

	2. auto string_value = inst.prop_string; Error

	3. for (char ch : inst.prop_string) Error
	       cout << ch;
	*/


	return 0;
}
