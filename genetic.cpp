/////////////////////////////////////////////////////////////////////////////
// genetic.cpp
// vim = ts=8 sts=4 sw=4 noet

#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>

#define METHOD_NUMBER 100
#define METHOD_LENGTH 10
#define METHOD_COUNT 6
#define CO_RANGE 100
#define MUTATION_COUNT 2

using namespace std;

struct Method
{
    char op[METHOD_LENGTH+1];
    int co[METHOD_LENGTH+1];
};

//Virtual Functions
Method* InitMethods();
char GetOperation();
int GetCoefficient();
int Calculate(Method method, int &a, int &b);
float Validate(Method method, int result);
void ShowBest(Method method, float &maxAccuracy, int &a, int &b);
Method* NextGeneration(Method* oldMethods, float* pSigmoids);
int ParentChoice(float* pSigmoids);
void Mutate(Method &method, float sigmoid);

int main(int argc, char** argv)
{
    int i;
    srand(time(NULL));

    int a;
    int b;
    int maxIterations;
    int result;
    float accuracies[METHOD_NUMBER];
    float maxAccuracy;
    int maxAccuracyIndex;
    float differenceTotal;
    float sigmoids[METHOD_NUMBER];
    float* pSigmoids = sigmoids;

    Method* methods = InitMethods();
    Method* oldMethods = methods;
    
    while (a != -1)
    {
	printf ("Please enter the number of iterations, or 0 to quit: ");
	scanf ("%i", &maxIterations);
	if (maxIterations <= 0)
	    break;
	printf ("Please enter in a value for a, or 0 for random: ");
	scanf ("%i", &a);
	if (a == 0)
	    a = rand() % CO_RANGE;
	printf ("Please enter in a value for b, or 0 for random: ");
	if (b == 0)
	    b = rand() % CO_RANGE;
	scanf ("%i", &b);
	printf ("Method key:\n"
		"a) Add\n" 
		"b) Subtract\n" 
		"c) Multiply\n" 
		"d) Divide\n" 
		"e) Modulo\n" 
		"f) Halt\n");

	for (int iterations = 0; iterations < maxIterations; ++iterations)
	{
	    printf ("\nIteration #%i:\n", iterations);
	    differenceTotal = 0;
	    maxAccuracy = 0.0;
	    for (i = 0; i < METHOD_NUMBER; ++i)
	    {
		result = Calculate(methods[i], a, b);
		accuracies[i] = Validate(methods[i], result);
		if (accuracies[i] > maxAccuracy)
		{
		    maxAccuracy = accuracies[i];
		    maxAccuracyIndex = i;
		}
		differenceTotal += accuracies[i];
	    }

	    ShowBest(methods[maxAccuracyIndex], maxAccuracy, a, b);

	    for (i = 0; i < METHOD_NUMBER; ++i)
	    {
		sigmoids[i] = accuracies[i] / differenceTotal;
	    }

	    oldMethods = methods;
	    methods = NextGeneration(oldMethods, pSigmoids);
	    delete oldMethods;
	}
    }
    return 0;
}

Method* InitMethods()
{
    int aPlace;
    int bPlace;
    int currentMethodLength;
    while (bPlace == aPlace)
	bPlace = rand() % METHOD_NUMBER;

    Method* methods = new Method[METHOD_NUMBER];
    for (int i = 0; i < METHOD_NUMBER; ++i)
    {
	methods[i].op[0] = '-';
	methods[i].co[0] = GetCoefficient();

	for (int j = 1; j < METHOD_LENGTH; ++j)
	{
	    methods[i].op[j] = GetOperation();
	    methods[i].co[j] = GetCoefficient();
	}

	aPlace = rand() % METHOD_LENGTH;
	bPlace = aPlace;
	while (bPlace == aPlace)
	    bPlace = rand() % METHOD_LENGTH;

	methods[i].co[aPlace] = CO_RANGE + 1;
	methods[i].co[bPlace] = CO_RANGE + 2;
    }

    return methods;
}

char GetOperation()
{
    int r = rand() % METHOD_COUNT;
    if (r == METHOD_COUNT - 1)
	return '-';
    else
	return r + 'a';
}

int GetCoefficient()
{ return rand() % CO_RANGE; }

int Calculate(Method method, int &a, int &b)
{
    int result = method.co[0];
    int prev;
    int c;

    for (int i = 1; i < METHOD_LENGTH; ++i)
    {
	c = method.co[i];
	if (c == CO_RANGE + 1)
	    c = a;
	else if (c == CO_RANGE + 2)
	    c = b;

	if (method.op[i] == 'a')
	    result += c;
	else if (method.op[i] == 'b')
	    result -= c;
	else if (method.op[i] == 'c')
	    result *= c;
	else if (method.op[i] == 'd' && c != 0)
	    result /= c;
	else if (method.op[i] == 'e' && c != 0)
	    result %= c;
		    
    }
    
    return result;
}

float Validate(Method method, int result)
{
    int validA[10] = { 5, 13, 8, 33, 7, 3, 82, 4, 37, 0 };
    int validB[10] = { 10, -2, 8, 2, 42, -8, -40, -18, 0, 0 };
    int validY[10] = { 60, 128, 88, 332, 112, 22, 780, 22, 370, 0 };
    
    float difference = 0.0;
    for (int i = 0; i < 10; ++i)
    {
	difference += (float) abs(Calculate(method, validA[i], validB[i]) - validY[i]);
    }

    
    return 1 / difference;
}

void ShowBest(Method method, float &maxAccuracy, int &a, int &b)
{
    stringstream sstm;
    char op;
    int co;

    sstm << "sf " << method.co[0] << " ";
    for (int i = 1; i < METHOD_LENGTH; ++i)
    {
	if (method.op[i] == '-')
	    continue;

	if (method.co[i] == CO_RANGE + 1)
	    co = a;
	else if (method.co[i] == CO_RANGE + 2)
	    co = b;
	else
	    co = method.co[i];

	if (method.op[i] == 'a')
	    op = '+';
	else if (method.op[i] == 'b')
	    op = '-';
	else if (method.op[i] == 'c')
	    op = '*';
	else if (method.op[i] == 'd')
	    op = '/';
	else if (method.op[i] == 'e')
	    op = '%';
	sstm << co << " " << op << " ";
    }
    sstm << ". bye";

    printf ("Best Method: %s\n", method.op);
    printf ("Accuracy: %f", maxAccuracy);
    printf ("\n%s\n", sstm.str().c_str());
    return;
}


Method* NextGeneration(Method* oldMethods, float* pSigmoids)
{
    int i;
    int w;
    int father;
    int mother;

    Method* newMethods = new Method[METHOD_NUMBER];

    for (i = 0; i < METHOD_NUMBER; ++i)
    {
	father = ParentChoice(pSigmoids);
	mother = ParentChoice(pSigmoids);

	w = 0;
	while (w < METHOD_LENGTH/2)
	{
	    newMethods[i].op[w] = oldMethods[father].op[w];
	    newMethods[i].co[w] = oldMethods[father].co[w];
	    ++w;
	}
	while (w < METHOD_LENGTH)
	{
	    newMethods[i].op[w] = oldMethods[mother].op[w];
	    newMethods[i].co[w] = oldMethods[mother].co[w];
	    ++w;
	}

	Mutate(newMethods[i], pSigmoids[i]);
    }

    return newMethods;
}

int ParentChoice(float* pSigmoids)
{
    int r;
    float thresh = 1.0;
    while (thresh > 0)
    {
	r = rand() % METHOD_NUMBER;
	if (thresh < pSigmoids[r])
	    return r;
	thresh -= pSigmoids[r];
    }
    return -1;
}

void Mutate(Method &method, float sigmoid)
{
    int r;
    char op;
    int co;

    for (int i = 0; i < MUTATION_COUNT; ++i)
    {
	r = rand() % METHOD_LENGTH;
	op = method.op[r];
	while (method.op[r] == op)
	    method.op[r] = rand() % METHOD_COUNT + 'a';

	co = method.co[r];
	while (co == method.co[r])
	    method.co[r] = rand() % CO_RANGE;
    }
    return;
}
