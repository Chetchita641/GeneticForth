/////////////////////////////////////////////////////////////////////////////
// genetic.cpp
// vim = ts=8 sts=8 sw=4 noet

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

const int mLen = 10;
const int randRange = 100;
const int numMethods = 10;
const int iterations = 500;

void InitMethods();
int Calculate(char* method, int a, int b);
string ForthTranslation(char* method, int a, int b);
float ValidSet(char* method, int result);
int RandomChoice(float sigmoids[10]);
void MakeChild(int father, int mother);

char methods[numMethods][mLen + 1];
char newMethods[numMethods][mLen + 1];
static int Coefficients[mLen];
int aPlace = -1;
int bPlace = -1;

const bool forthTranslation = false;

int main(int argc, char** argv)
{
	bool debug = false;
	int i;

	int a;
	int b;
	int result;
	float accuracy[numMethods];
	float accuracyTotal;
	float sigmoids[numMethods];
	int father;
	int mother;
	
	InitMethods();
	while (a != -1)
	{
		// Set seed for random number generator
		srand( rand() ^ time(NULL) );

		
		printf ("Please enter a value for a: ");
		scanf ("%d", &a);
		if (a == -1)
			break;

		printf ("Please enter a value for b: ");
		scanf ("%d", &b);

		printf ("Method key:\n"
			"a) Add\n"
			"b) Subtract\n"
			"c) Multiply\n"
			"d) Divide\n"
			"e) Modulo\n"
			"f) Swap\n"
			"g) Duplicate\n"
			"h) Over\n" 
			"i) Rotate\n");

		for (int iter = 0; iter < iterations; ++iter)
		{
			accuracyTotal = 0.0;
			for (i = 0; i < numMethods; ++i)
			{
				// Draw from generated methods (sequence of operations)
				if (debug) printf("Method[%d]: %s\n", i, methods[i]);
				aPlace = -1;
				bPlace = -1;
				result = Calculate(methods[i], a, b);
			
				if (forthTranslation) 
				{
				  string trans = ForthTranslation(methods[i], a, b);
				  cout << trans;
				}

				// Determine their accuracy compared to validation set
				accuracy[i] = ValidSet(methods[i], result);
				accuracyTotal += accuracy[i];
			}
			if (debug) printf("\n");
			
			// Convert methods into sigmoids
			for (i = 0; i < numMethods; ++i)
			{
				sigmoids[i] = accuracy[i] / accuracyTotal;
				if (debug) printf("Sigmoids[%d]: %f\n", i, sigmoids[i]);
			}

			// Create ten children randomly using the weights
			for (i = 0; i < numMethods; ++i)
			{
				father = RandomChoice(sigmoids);
				mother = RandomChoice(sigmoids);
				MakeChild(father, mother);
			}
			if (debug) printf ("\n");
			
		}
	}
	return 0;
}

void InitMethods()
{
	int r;
	char method[mLen+1];

	for (int i = 0; i < numMethods; ++i)
	{
		for (int j = 0; j < mLen; ++j)
		{
			r = rand() % 6;
			methods[i][j] = 'a' + r;
		}
	}	
}

int Calculate(char* method, int a, int b)
{
	bool debug = false;

	int result = 0;
	int prev;
	int coEff;

	int len = (int) strlen(method);
	
	if (aPlace == -1)
	  aPlace = rand() % len;
	if (bPlace == -1)
	  bPlace = aPlace;
	while (bPlace == aPlace)
	{
		bPlace = rand() % len;
	}

	if (aPlace == 0)
		result = a;
	else if (bPlace == 0)
		result = b;
	else 
		result = rand () % randRange;
	for (int i = 1; i < len; ++i)
	{
		if (i == aPlace) {
			coEff = a;
		} else if (i == bPlace) {
			coEff = b;
		} else {
			coEff = rand() % randRange;
		}

		if (method[i-1] == 'a') {
			prev = result;	
			result += coEff;
			Coefficients[i-1] = coEff;
			if (debug) printf ("%d + %d = %d\n", prev, coEff, result);
		}
		else if (method[i-1] == 'b') {
			prev = result;	
			result -= coEff;
			Coefficients[i-1] = coEff;
			if (debug) printf ("%d - %d = %d\n", prev, coEff, result);
		}
		else if (method[i-1] == 'c') {
			prev = result;	
			result *= coEff;
			Coefficients[i-1] = coEff;
			if (debug) printf ("%d * %d = %d\n", prev, coEff, result);
		}
		else if (method[i-1] == 'd') {
			if (coEff == 0)
			{
				printf ("Cannot divide by zero\n");
				continue;
			}	
			prev = result;	
			result /= coEff;
			Coefficients[i-1] = coEff;
			if (debug) printf ("%d / %d = %d\n", prev, coEff, result);
		}
		else if (method[i-1] == 'e') {
			if (coEff == 0)
			{
				printf ("Cannot divide by zero\n");
				continue;
			}	
			prev = result;	
			result %= coEff;
			Coefficients[i-1] = coEff;
			if (debug) printf ("%d %% %d = %d\n", prev, coEff, result);
		}
		else if (method[i-1] == 'f')
		{
			if (i == 0)
				printf ("%d", result);
			break;
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
		if (method[i] == 'a')  sstm << Coefficients[i] << " + "; 
		else if (method[i] == 'b')  sstm << Coefficients[i] << " - ";
		else if (method[i] == 'c')  sstm << Coefficients[i] << " * ";
		else if (method[i] == 'd')  sstm << Coefficients[i] << " / ";
		else if (method[i] == 'e')  sstm << Coefficients[i] << " /MOD ";
	}

	sstm << ". bye\n";
	string tmp = sstm.str();
	return tmp.c_str();
}

//Tests for y = a * 10 + b
float ValidSet(char* method, int result)
{
	bool debug = true;

	int testA[10] = { 5, 13, 8, 33, 7, 3, 82, 4, 37, 0 };
	int testB[10] = { 10, -2, 8, 2, 42, -8, -40, -18, 0, 0 };
	int testY[10] = { 60, 128, 88, 332, 112, 22, 780, 22, 370, 0 };

	int testResult;
	int resultTotal = 0;
	float averageError;
	int difference;
	for (int i = 0; i < 10; ++i)
	{
		testResult = Calculate(method, testA[i], testB[i]);
		difference = abs(testResult - testY[i]);
		resultTotal += difference;
		if (debug) printf ("Difference from Test Set %i: %i\n", i, difference);
	}
	averageError = (float) resultTotal / 10.0;
	if (debug) printf ("Method: %s\n", method);
	if (debug) printf ("Average error: %f\n\n", averageError);
	return 1 / averageError;
}

int RandomChoice(float sigmoids[10])
{
	bool debug = false;
	int r;
	float thresh = 1.0;
	while (thresh > 0.0)
	{
		r = rand() % numMethods;
		if (debug) printf ("Index: %d\n", r);
		if (thresh < sigmoids[r])
		{
			if (debug) printf ("\nReturning %d\n", r);
			return r;
		}
		thresh -= sigmoids[r];
	}

	return -1;
}

void MakeChild(int father, int mother)
{
	int i;
	int j;
	int median = mLen / 2;

	for (i = 0; i < numMethods; ++i)
	{
		for (j = 0; j < median-1; ++j)
		{
			methods[i][j] = methods[father][j]; 	
		}
		for (i = median; i < mLen; ++i)
		{
			methods[i][j] = methods[mother][j];
		}
	}
}

