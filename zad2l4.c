#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define MAXPRM 20
int testFermat(unsigned long p);
unsigned long getRand(unsigned long a,unsigned long b);
unsigned long NWD(unsigned long a, unsigned long b);
unsigned long mulMod(unsigned long a, unsigned long b, unsigned long n);
unsigned long powMod(unsigned long a, unsigned long e, unsigned long n);
void *countIt(void *arg);

const unsigned long lp[] =
{
	  2,  3,  5,  7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
	 71, 73, 79, 83, 89, 97,101,103,107,109,113,127,131,137,139,149,151,157,163,
	167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,
	271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,
	389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,
	503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,
	631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,
	757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,
	883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997,1009
};
pthread_mutex_t mojmuteks=PTHREAD_MUTEX_INITIALIZER;
int id = 0; // id dla watkow
int prims[MAXPRM];
int main(int argc, char *argv[]) // argv1 - #l.pierwsze, argv2 - #bitow
{
	int primes = (int) strtol(argv[1], (char **)NULL, 10);
	int bits = (int) strtol(argv[2], (char **)NULL, 10);
	srand((unsigned) time(NULL));

	int min = pow(2, (bits-1)); // najmniejsza k bitowa liczba nieparzysta
	int max = pow(2, bits)-1; // najwieksza k bitowa liczba
	pthread_t watki[primes];

	int i;
	for(i=0; i<primes; i++)
	{
		if ( pthread_create( &watki[i], NULL, countIt,NULL) )
			printf("błąd przy tworzeniu wątku\n"); abort();
		if ( pthread_join ( watki[i], NULL ) )
		{
			printf("błąd w kończeniu wątku\n");
			exit();
		}
	}	
	
	printf("Wygenerowana liczba pierwsza - fermat %d\n", los);
	return 0; // potem zwroc tablice liczb pierwszych
}

void *countIt(void *arg)
{
	int los;
	int myid;
	while(1)
	{
		pthread_mutex_lock(&mojmuteks);
		myid = id;
		id++;
		pthread_mutex_unlock(&mojmuteks);
		printf("Watek nr: %d\n", myid);
		los = min + rand() / (RAND_MAX / (max - min + 1) + 1); // losowa z przedzialu [min, max]
		if(los % 2 == 0) los += 1; // niewykroczy poza zakres bo parzysta nie mogla byc maxem ( max jest nieparzysty - #bits jedynek w zapisie binarnym )
		if(1 == testFermat(los)) break;
		sleep(1);
	}
	prims[myid]= los;
	return NULL;
}

int testFermat(unsigned long p)
{
	unsigned long a;
	int i, t;
	
	t = 1;
  	for(i = 0; i < 169; i++)
    	if((p != lp[i]) && (p % lp[i] == 0))
    	{
      		return 0;
    	}
  	if(t && (p > 1009))
  	{
    	for(i = 1; i <= 10; i++)
    	{
      		a = getRand(2,p-1);
      		if((NWD(p,a) != 1) || (powMod(a,p-1,p) != 1))
      		{
        		return 0;
      		}
    	}
  	}
	return t;
}

unsigned long getRand(unsigned long a,unsigned long b)
{
  unsigned long w;
  int i;

  for(i = 1; i <= 8; i++)
  {
    w <<= 8;
    w &= rand() % 256;
  }
  return a + (w % (b - a));
}

// Funkcja oblicza NWD
//--------------------
unsigned long NWD(unsigned long a, unsigned long b)
{
  unsigned long t;

  while(b)
  {
    t = b; b = a % b; a = t;
  }
  return a;
}

// Funkcja mnoży a i b mod n
//--------------------------
unsigned long mulMod(unsigned long a, unsigned long b, unsigned long n)
{
  unsigned long m,w;

  w = 0;
  for(m = 1; m; m <<= 1)
  {
    if(b & m) w = (w + a) % n;
    a = (a << 1) % n;
  }
  return w;
}

// Funkcja oblicza a^e mod n
//--------------------------
unsigned long powMod(unsigned long a, unsigned long e, unsigned long n)
{
  unsigned long m,p,w;

  p = a; w = 1;
  for(m = 1; m; m <<= 1)
  {
    if(e & m) w = mulMod(w,p,n);
    p = mulMod(p,p,n);
  }
  return w;
}
