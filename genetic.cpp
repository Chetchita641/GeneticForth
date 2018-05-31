/////////////////////////////////////////////////////////////////////////////
// genetic.cpp
// vim = ts=8 sts=8 sw=4 noet

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{

	int a;
	int b;
	char method; 

	while (a != -1)
	{
		cout << "Please enter in a value for 'a': ";
		cin >> a;
		if (a == -1)
			break;

		cout << "Please enter in a value for 'b': ";
		cin >> b;
		
		cout << "Please select a method\n" << 
			"1) Add\n" <<
			"2) Subtract\n" <<
			"3) Multiply\n" <<
			"4) Divide\n" <<
			"5) Modulo\n" <<
			"6) Swap\n" <<
			"7) Duplicate\n" <<
			"8) Over\n" << 
			"9) Rotate\n"; 
		cin >> method;
		int result = 5;
		cout >> result;
		cout << "\n\n";
	}
	return 0;
}

int ChooseMethod(char method, int &a, int &b)
{
	int result;
	switch (method) 
	{
		case '1':
			return a + b;
		case '2':
			return a - b;
		case '3':
			return a * b;
		case '4':
			return a / b;
		case '5':
			return a % b;
	}
	return -1;	
}


