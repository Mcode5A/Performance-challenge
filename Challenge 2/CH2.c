#include <stdio.h>
#include <stdlib.h>

// Variable used to generate pseudo-random numbers
unsigned int seed;

// Function to generate pseudo-random numbers
int myRandom() {
	seed = (214013 * seed + 2531011);
	return (seed >> 13);
}


void LongNumInit(unsigned char *L, unsigned N)
{
	for (int i = 0; i < N; i++)
	{
		L[i] = myRandom() % 10;  // digito decimal
	}
}

void LongNumPrint(unsigned char *L, unsigned N, const char *Name)
{
	printf("%s:", Name);
	for (int i = N; i > 0; i--)
	{
		printf("%d", L[i - 1]);
	}
	printf("\n");
}


void LongNumSet(unsigned char *L, unsigned N, unsigned char digit)
{
	for (int i = 0; i < N; i++)
	{
		L[i] = digit;
	}
}


void LongNumCopy(unsigned char *Vin, unsigned char *Vout, unsigned N)
{
	for (int i = 0; i < N; i++)
	{
		Vout[i] = Vin[i];
	}
}

void LongNumAddition(unsigned char *Vin1, unsigned char *Vin2, unsigned char *Vout, unsigned N)
{
	unsigned char CARRY = 0;
	for (int i = 0; i < N; i++)
	{
		char R = Vin1[i] + Vin2[i] + CARRY;
		if (R <= 9)
		{
			Vout[i] = R; CARRY = 0;
		}
		else
		{
			Vout[i] = R - 10; CARRY = 1;
		}
	}

}


void LongNumAddDigit(unsigned char *V, unsigned char digit, unsigned N)
{
	int i = 0;
	char R = V[0] + digit;
	if (R < 10)
	{
		V[0] = R;// No carry
	}

	V[0] = R - 10;
	// add carry, maybe iteratively for all digits
	unsigned char CARRY = 1;
	i = 1;
	while (CARRY && i < N)
	{
		if (V[i] < 9)
		{
			V[i] = V[i] + 1; CARRY = 0;
		}
		else
		{
			V[i] = 0; i++;  // CARRY remains set to 1
		}
	}

}


void LongNumHorizAdd(unsigned char *Vin, unsigned char *Vout, unsigned N)
{
	unsigned char CARRY = 0;
	LongNumSet(Vout, N, 0);
	for (int i = 0; i < N; i++)
	{
		LongNumAddDigit(Vout, Vin[i], N);
	}
 // CARRY can never be set
}

void LongNumConstMult(unsigned char *V, unsigned N, unsigned char digit)
{
	unsigned char CARRY = 0;
	for (int i = 0; i < N; i++)
	{
		unsigned char R = V[i] * digit + CARRY;
		CARRY = R / 10;
		R = R - CARRY * 10;
		V[i] = R;
	}
// may be from 0 to 9
}

void LongNumMultiply(unsigned char *V3, unsigned char *V4, unsigned char *V2, unsigned char *V1, unsigned N)
{

	// Create Temporal Long Integer with double size
	unsigned char *TEMP = (unsigned char*)malloc(2 * N * sizeof(unsigned char));
	unsigned char *RES = (unsigned char*)calloc(2 * N , sizeof(unsigned char));

	//LongNumSet(RES, 2 * N, 0);    // Set RES to 0

	for (int i = 0; i < N; i++)
	{
		unsigned char *TEMP = (unsigned char*)calloc(2 * N ,sizeof(unsigned char));
		// Set TEMP to 0
		LongNumCopy(V3, TEMP + i, N);         // Copy Vin1 -> TEMP, with offset i
		LongNumConstMult(TEMP, 2 * N, V4[i]);  // TEMP * Vin2[i] -> TEMP
		LongNumAddition(TEMP, RES, RES, 2 * N); // TEMP + RES -> RES
		free(TEMP);
	}

	// Result goes to VoutH-VoutL
	LongNumCopy(RES, V1, N);  // Copy RES   -> VoutL
	LongNumCopy(RES + N, V2, N);  // Copy RES+N -> VoutH
}


int main(int argc, char **argv)
{
	int i, sum1, sum2, sum3, N = 10000, Rep = 50;

	seed = 12345;

	// obtain parameters at run time
	if (argc > 1) { N = atoi(argv[1]); }
	if (argc > 2) { Rep = atoi(argv[2]); }

	printf("Challenge #2: Vector size is %d. Repeat %d times\n", N, Rep);

	// Create Long Nums
	unsigned char *V1 = (unsigned char*)malloc(N * sizeof(unsigned char));
	unsigned char *V2 = (unsigned char*)malloc(N * sizeof(unsigned char));
	unsigned char *V3 = (unsigned char*)malloc(N * sizeof(unsigned char));
	unsigned char *V4 = (unsigned char*)malloc(N * sizeof(unsigned char));

	LongNumInit(V1, N); LongNumInit(V2, N); LongNumInit(V3, N);

	// Repeat
	for (i = 0; i < Rep; i++)
	{
		LongNumAddition(V1, V2, V4, N);

		// Create Temporal Long Integer with double size
		unsigned char *TEMP = (unsigned char*)malloc(2 * N * sizeof(unsigned char));
		unsigned char *RES = (unsigned char*)calloc(2 * N, sizeof(unsigned char));

		//LongNumSet(RES, 2 * N, 0);    // Set RES to 0

		for (int i = 0; i < N; i++)
		{
			unsigned char *TEMP = (unsigned char*)calloc(2 * N, sizeof(unsigned char));
			// Set TEMP to 0
			LongNumCopy(V3, TEMP + i, N);         // Copy Vin1 -> TEMP, with offset i
			LongNumConstMult(TEMP, 2 * N, V4[i]);  // TEMP * Vin2[i] -> TEMP
			LongNumAddition(TEMP, RES, RES, 2 * N); // TEMP + RES -> RES
			free(TEMP);
		} 

		// Result goes to VoutH-VoutL
		LongNumCopy(RES, V1, N);  // Copy RES   -> VoutL
		LongNumCopy(RES + N, V2, N);  // Copy RES+N -> VoutH
		LongNumHorizAdd(V1, V2, N);
		LongNumAddDigit(V3, V2[0], N);
	}
 
	// Print last 32 digits of Long Numbers
	LongNumPrint(V1, 32, "V1");
	LongNumPrint(V2, 32, "V2");
	LongNumPrint(V3, 32, "V3");
	LongNumPrint(V4, 32, "V4");

	free(V1); free(V2); free(V3); free(V4);
	return 0;
}