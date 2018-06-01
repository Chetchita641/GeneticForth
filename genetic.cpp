/////////////////////////////////////////////////////////////////////////////
// genetic.cpp
// vim = ts=8 sts=8 sw=4 noet

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

int ChooseMethod(char* method, int a, int b);
string ForthTranslation(char* method, int a, int b);

int main(int argc, char** argv)
{

	int a;
	int b;
	char method[50]; 

	while (a != -1)
	{
		printf ("Please enter in a value for 'a': ");
		scanf("%i", &a);
		if (a == -1)
			break;

		printf ("Please enter in a value for 'b': ");
		scanf("%i", &b);
		
		printf ("Please choose a sequence of  methods\n" 
			"1) Add\n"
			"2) Subtract\n"
			"3) Multiply\n"
			"4) Divide\n"
			"5) Modulo\n"
			"6) Swap\n"
			"7) Duplicate\n"
			"8) Over\n" 
			"9) Rotate\n"
			"? "); 
		scanf("%s", method);
		int result = ChooseMethod(method, a, b);
		printf ("Result: %d\n\n", result);
		string trans = ForthTranslation(method, a, b);
		cout << trans << "\n";
	}
	return 0;
}

int ChooseMethod(char* method, int a, int b)
{
	int result = a;
	int len = (int) strlen(method);
	printf ("\n");
	for (int i = 0; i < len; ++i)
	{
		if (method[i] == '1') {
			a = result;
			result += b;
			printf ("%d + %d = %d\n", a, b, result);
		}
		else if (method[i] == '2') {
			a = result;
			result -= b;
			printf ("%d - %d = %d\n", a, b, result);
		}
		else if (method[i] == '3') {
			a = result;
			result *= b;
			printf ("%d * %d = %d\n", a, b, result);
		}
		else if (method[i] == '4') {
			a = result;
			result /= b;
			printf ("%d / %d = %d\n", a, b, result);
		}
		else if (method[i] == '5') {
			a = result;
			result %= b;	
			printf ("%d % %d = %d\n", a, b, result);
		}
	}
	return result;
}

string ForthTranslation(char* method, int a, int b)
{
	stringstream sstm;
	sstm << "sf " << a << " ";
	int len = (int) strlen(method);
	for (int i = 0; i < len; ++i)
	{
		if (method[i] == '1')  sstm << b << " + "; 
		else if (method[i] == '2')  sstm << b << " - ";
		else if (method[i] == '3')  sstm << b << " * ";
		else if (method[i] == '4')  sstm << b << " / ";
		else if (method[i] == '5')  sstm << b << " /MOD ";
	}

	sstm << ". bye";
	string tmp = sstm.str();
	return tmp.c_str();
}


