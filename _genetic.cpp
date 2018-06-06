/////////////////////////////////////////////////////////////////////////////
// genetic.cpp
// vim = ts=8 sts=8 sw=4 noet

#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <cstdlib>

#define M_LENGTH 10 
#define ITERATIONS 500
#define M_NUM 10
#define CO_RANGE 100

//Functions
char* NewMethods();
int* NewCoefficients();
int Calculate(char* method, int* coefficients, int &a, int &b);
float ValidateSet(char* method, int* coefficients, int result);
int RandomChoice(float weightedAccuracy[]);
char* MakeChildMethod(char* fatherMethod, char* motherMethod);
int* MakeChildCoefficients(int* fatherCoefficient, int* motherCoefficient);

using namespace std;

int main(int argc, char** argv)
{
	srand (time(NULL));
	bool debug = false;
	int i;

	int a;
	int b;
	int result;
	int aPlace;
	int bPlace;

	float validAccuracy[M_NUM];
	float rawAcc = 0;
	float weightedAccuracy[M_NUM];

	int father;
	int mother;
	char* fatherMethod;
	char* motherMethod;
	char childMethod[M_LENGTH+1];
	int childCoefficients[M_LENGTH+1];

	char* methodsArray[M_NUM];
	int* coefficientsArray[M_NUM];

	//Draw from generated methods and calcuate their values. Then, validate.
	cout << "Please enter in a value for a: ";
	cin >> a;

	cout << "Please enter in a value for b: ";
	cin >> b;

	cout << "Method key:\n" <<
			"a) Add\n" <<
			"b) Subtract\n" <<
			"c) Multiply\n" <<
			"d) Divide\n" <<
			"e) Modulo\n" <<
			"f) Halt\n";
	
	for (i = 0; i < M_NUM; ++i)
	{
		methodsArray[i] = NewMethods();
		coefficientsArray[i] = NewCoefficients();
	}

	for (i = 0; i < M_NUM; ++i)
	{
		cout << "Method#" << i << ": " << methodsArray[i] << "\n";
		result = Calculate(methodsArray[i], coefficientsArray[i], a, b);

		cout << "\nValidation Set\n";
		validAccuracy[i] = ValidateSet(methodsArray[i], coefficientsArray[i], result);
		rawAcc += validAccuracy[i];
	}

	//Create a weighted accuracy to rank methods
	for (i = 0; i < M_NUM; ++i)
	{
		weightedAccuracy[i] = validAccuracy[i] / rawAcc;
		cout << "Weighted Accuracy for method #" << i << ": " << weightedAccuracy[i] << "\n";
	}
	
	//Make arrays of children and then replace the old arrays
	
	for (i = 0; i < M_NUM; ++i)
	{
		father = RandomChoice(weightedAccuracy);
		mother = RandomChoice(weightedAccuracy);
	
		cout << "Father method: " << methodsArray[father] << "\n";
		cout << "Mother method: " << methodsArray[mother] << "\n\n";

		fatherMethod = methodsArray[father];
		motherMethod = methodsArray[mother];

		childMethod[i] = MakeChildMethod(fatherMethod, motherMethod);

		cout << "Child method: " << childMethod[i] << "\n\n";
	}

	return 0;
}	

char* NewMethods()
{
	int r;
	int i;

	char* method = new char[M_LENGTH+1];

	for (i = 0; i < M_LENGTH; ++i)
	{
		r = rand() % 6;
		method[i] = 'a' + r;
	}
	method[i] = '\0';
	return method;
}

int* NewCoefficients()
{
	int* coefficients = new int[M_LENGTH+1];
	int aPlace;
	int bPlace;

	aPlace = rand() % M_LENGTH;
	bPlace = rand() % M_LENGTH;
	while (aPlace == bPlace)
		bPlace = rand() % M_LENGTH;

	for (int i = 0; i < M_LENGTH+1; ++i)
	{
		if (i == aPlace)
			coefficients[i] = CO_RANGE + 1;
		else if (i == bPlace)
			coefficients[i] = CO_RANGE + 2;
		else
			coefficients[i] = rand() % CO_RANGE;
	}

	return coefficients;
}

int Calculate(char* method, int* coefficients, int &a, int &b)
{
	bool debug = false;

	int result = *coefficients;
	int prev;
	int c;


	//Insert the first number
	if (*coefficients == CO_RANGE + 1)
		c = a;
	else if (*coefficients == CO_RANGE + 2)
		c = b;
	else
		c = *coefficients;
	
	result = c;
	coefficients++;

	for (int i = 0; i < M_LENGTH; ++i)
	{
		if (*coefficients == CO_RANGE + 1)
			c = a;
		else if (*coefficients == CO_RANGE + 2)
			c = b;
		else
			c = *coefficients;
		
		if (*method == 'a')
		{
			prev = result;
			result += c;
			if (debug) cout << prev << " + " << c << " = " << result << "\n";
		}
		else if (*method == 'b')
		{
			prev = result;
			result -= c;
			if (debug) cout << prev << " - " << c << " = " << result << "\n";
		}
		else if (*method == 'c')
		{
			prev = result;
			result *= c;
			if (debug) cout << prev << " * " << c << " = " << result << "\n";
		}
		else if (*method == 'd')
		{
			if (c == 0)
				cout << "Cannot divide by zero\n";
			else
			{
				prev = result;
				result /= c;
				if (debug) cout << prev << " / " << c << " = " << result << "\n";
			}
		}
		else if (*method == 'e')
		{
			if (c == 0)
				cout << "Cannot divide by zero\n";
			else
			{
				prev = result;
				result %= c;
				if (debug) cout << prev << " % " << c << " = " << result << "\n";
			}
		}
		else if (*method == 'f')
		{
			if (i == 0)
				cout << result << "\n";
			break;
		}

		method++;
		coefficients++;
	}

	cout << "Calculated result: " << result << "\n";
	return result;

}

float ValidateSet(char* method, int* coefficients, int result)
{
	int validResult;
	int diff = 0;
	float accuracy;

	int validA[10] = { 5, 13, 8, 33, 7, 3, 82, 4, 37, 0 };
	int validB[10] = { 10, -2, 8, 2, 42, -8, -40, -18, 0, 0 };
	int validY[10] = { 60, 128, 88, 332, 112, 22, 780, 22, 370, 0 };

	for (int i = 0; i < M_NUM; ++i)
	{
		validResult = Calculate(method, coefficients, validA[i], validB[i]);
		diff += abs(validResult - validY[i]);
	}

	cout << "Total difference from validation: " << diff << "\n\n";
	accuracy = 1.0 / (float) diff;
	return accuracy;
}

int RandomChoice(float weightedAccuracy[])
{
	int r;
	float thresh = 1.0;
	r = rand() % M_NUM;

	while (thresh > weightedAccuracy[r])
	{
		thresh -= weightedAccuracy[r];
		r = rand() % M_NUM;
	}
	return r;
}

char* MakeChildMethod(char* fatherMethod, char* motherMethod)
{
	return fatherMethod;
}

int* MakeChildCoefficients(int* fatherCoefficient, int* motherCoefficient)
{
	return new int[M_LENGTH+1];
}

