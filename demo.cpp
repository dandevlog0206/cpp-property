#include <iostream>
#include <vector>
#include <chrono>
#include "property.h"

#define BENCHMARK_N 1000000

using namespace std;

struct vec2 {
	float x, y;
};

vec2 operator+(const vec2& lhs, const vec2& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

ostream& operator<<(ostream& os, const vec2& v) {
	os << "[" << v.x << ", " << v.y << "]";
	return os;
}

ostream& operator<<(ostream& os, const vector<int>& v) {
	os << "[ " << v[0];
	for (size_t i = 1; i < v.size(); ++i)
		os << ", " << v[i];
	os << " ]"; 
	return os;
}

class MyClass {
	PROPERTY_INIT(MyClass);
	
public:
	MyClass() :
		Float0(2.f),
		m_num_list({ 1, 2, 3 ,4 ,5 }),
		m_float0(1.f),
		m_vector0({ 1, 2 })
	{
		Float0 = 3.f;

		// float dummy = Float2;

		// Float1 = 3.f; // readonly property can be written by the owner(same to writeonly property)

		// you can omit declaring getter for writeonly property(same to readonly property)
		// float dummy = Float2; *error*
	}

	// property members don't use any memory space => *zero* abstraction cost in memory
	// property and getter
	PROPERTY(vector<int>, NumberList);
	PROPERTY(vec2, BenchVector);
	PROPERTY(vec2, Vector0);
	PROPERTY(float, Float0);
	PROPERTY(float, Float1, readonly);  // new attribute readonly
	PROPERTY(float, Float2, writeonly); // new attribute writeonly
	
	PROPERTY(float, Float3, default); // new attribute default, Float3 behaves almost same as float
	PROPERTY(float, Float4, default, readonly); // mix two attributes
	PROPERTY(float, Float5, default, writeonly);
	PROPERTY(float, Float6, readonly, default); // attribute order is not matter 

private:
	PROPERTY_GET_DECL(NumberList) {
		return m_num_list;
	}

	PROPERTY_SET_DECL(NumberList) {
		m_num_list = value;
	}

	PROPERTY_GET_DECL(BenchVector) {
		return m_bench_vector;
	}

	PROPERTY_SET_DECL(BenchVector) {
		m_bench_vector = value;
	}

	// or you can use PROPERTY_GET_SET_DECL(Vector0); instead of below two lines
	PROPERTY_GET_DECL(Vector0); // separate declaration and implementation
	PROPERTY_SET_DECL(Vector0);

	PROPERTY_GET_DECL(Float0) { // declare and implement at the same time
		cout << "get Float0: " << m_float0 << endl;
		return m_float0;
	}
	
	PROPERTY_SET_DECL(Float0) {
		cout << "set Float0: " << (m_float0 = value) << endl;
	}

	PROPERTY_GET_DECL(Float1) {
		cout << "get Float1: " << m_float1 << endl;
		return m_float1;
	}

	PROPERTY_SET_DECL(Float1) {
		cout << "set Float1: " << (m_float1 = value) << endl;
	}

	// getter is optional to writeonly property
	// PROPERTY_GET_DECL(Float2) {
	// 	cout << "get Float2: " << m_float2 << endl;
	// 	return m_float2;
	// }

	PROPERTY_SET_DECL(Float2) {
		cout << "set Float2: " << (m_float2 = value) << endl;
	}

private:
	vector<int> m_num_list;
	vec2 m_bench_vector;
	float m_float0;
	float m_float1;
	float m_float2;
	vec2 m_vector0; 
};

PROPERTY_GET_IMPL(MyClass, Vector0) {
	cout << "get Vector0: " << m_vector0 << endl;
	return m_vector0;
}

PROPERTY_SET_IMPL(MyClass, Vector0) {
	cout << "set Vector0: " << (m_vector0 = value) << endl;
}

int main() {
	float dummy;
	(void)dummy;
 
	MyClass cls;

	cout << sizeof(cls) << endl;
	cout << cls.NumberList << endl; // expect [ 1, 2, 3, 4, 5 ]
	cls.NumberList->push_back(6);   // access to member(requires both get and set)
	cout << cls.NumberList << endl; // expect [ 1, 2, 3, 4, 5, 6 ]

	dummy = cls.Float0.get(); // read1
	dummy = cls.Float0();     // read2
	dummy = cls.Float0;       // read3

	cls.Float0.set(1.f); // write1 (less abstracted)
	cls.Float0(2.f);     // write2
	cls.Float0 = 3.f;    // write3 (most abstracted)

	cout << cls.Vector0 + vec2{ 3, 4 } << endl; // automatic casting
	cls.Vector0->x += 1.f; // read and write member variable 

	// Simple Benchmark (low abstraction cost at speed in -O3 optimization)
	{
		vec2 BenchVector = { 0.f, 0.f };

		auto begin = chrono::high_resolution_clock::now();
		
		for (volatile int i = 0; i < BENCHMARK_N; ++i)
			BenchVector.x += i;

		auto end = chrono::high_resolution_clock::now();

		auto us = chrono::duration_cast<chrono::microseconds>(end - begin).count();
		cout << "benchmark: normal access(no abstraction): " << us / 1e3f << "ms\n"; // about 0.46ms 
	}

	{
		auto begin = chrono::high_resolution_clock::now();
		
		for (volatile int i = 0; i < BENCHMARK_N; ++i)
			cls.BenchVector->x += i;

		auto end = chrono::high_resolution_clock::now();

		auto us = chrono::duration_cast<chrono::microseconds>(end - begin).count();
		cout << "benchmark: access by property: " << us / 1e3f << "ms\n"; // about 0.68ms 
	}

	return 0;
}