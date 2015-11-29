#include <stdio.h>
#include <stdlib.h>
void generateKeys(int k, int l); // generuje klucze przy uzyciu k liczb pierwszych dlugosci l-bitow i zapisuje je w keys.txt
void encrypt(char msg[], char dec[]); // przyjmuje wiadomosc do zaszyfrowania msg, szyfruje ustalonym kluczem z keys.txt i zapisuje do pliku podanego w dec
void decrypt(char cip[], char dec[], int mode);

int eulfun (int primes[], int len); // funkcja Eulera: przyjmuje tablice liczb pierwszy i jej liczbe elementow, zwraca wynik tej funkcji
int getmod (int primes[], int len); // funcja oblicza modul ( iloczyn liczb z tablicy primes ) i go zwraca
int nwd (int a, int b); // zwraca najwiekszy wspolny dzielnik liczb a i b
int revmod (int a, int mod); // zwraca liczbe odwrotna ( co do modulu mod ) do liczy a. korzysta z rozszerzonego algorytmu Euklidesa
int pot_mod(int a, int w, int n); // oblicza potege a^w modulo n

void main (int argc, char *argv[])
{
	int c;
	printf("System szyfrowania danych RSA\nProsze wybrac dzialanie: \n1 - Generowanie klucza\n2 - Szyfrowanie danych\n3 - Odszyfrowanie danych\nInne - Koniec programu\n");
	scanf("%d", &c);
	switch (c)
	{
		case 1: 
			printf("Generowanie klucza!\n");
			// sprawdzenie liczby argumentow programow ( maja byc dwa: ilosc liczb pierwszych i ile bitow maja )
			if (argc-1 != 2)
			{	
				printf("Nieprawidlowa liczba parametrow\n");
				exit(1);
			}
			generateKeys((int) strtol(argv[1], (char **)NULL, 10)  ,  (int) strtol(argv[2], (char **)NULL, 10));
			printf("Wygenerowane klucze zapisano do keys.txt\n");
			break;
		case 2:
			printf("Szyfrowanie danych!\n");
			encrypt(argv[1], argv[2]);
    		break;
    	case 3:
    		printf("Odszyfrowanie danych!\n");
    		int c;
    		printf("Podaj tryb (0 - zwykly, 1 - CRT)\n");
    		scanf("%d", &c);
    		decrypt(argv[1], argv[2], c);
    		break;
   		default:
   			printf("Koniec programu!\n");
   			exit(1);
    		break;
	}
}

void encrypt(char msg[], char enc[])
{
	FILE *input, *keys, *output;
	int e, mod; // wykladnik klucza publicznego i modulo
	if ((input=fopen(msg, "r"))==NULL)
	{
		printf ("Nie mogę otworzyć pliku z tekstem do zaszyfrowania do odczytu!\n");
    	exit(1);
    }
    if ((keys=fopen("keys.txt", "r"))==NULL)
	{
		printf ("Nie mogę otworzyć pliku keys.txt do odczytu!\n");
    	exit(1);
    }
    if ((output=fopen(enc, "w"))==NULL)
	{
		printf ("Nie mogę otworzyć pliku keys.txt do odczytu!\n");
    	exit(1);
    }
    fscanf(keys, "%d %d\r\n", &mod, &e);
    printf("Klucz publiczny: modulo = %d, wykladnik = %d\n", mod, e);
    
    int c; // porcja danych z wiadomosci do zaszyfrowania ( domyslnie znak pliku tekstowego )    
    while ( (c = getc(input)) != EOF) 
    	putc(pot_mod(c, e, mod), output); //zwroc zaszyfrowany znak na "output"
    fclose (input);
    fclose (keys);
}

void decrypt(char cip[], char dec[], int mode)
{
	FILE *input, *keys, *output;
	int d, mod, p , q, dP, dQ, qInv; // wykladnik klucza prywatnego i modulo
	if ((input=fopen(cip, "r"))==NULL)
	{
		printf ("Nie mogę otworzyć pliku z tekstem do zaszyfrowania do odczytu!\n");
    	exit(1);
    }
    if ((keys=fopen("keys.txt", "r"))==NULL)
	{
		printf ("Nie mogę otworzyć pliku keys.txt do odczytu!\n");
    	exit(1);
    }
    if ((output=fopen(dec, "w"))==NULL)
	{
		printf ("Nie mogę otworzyć pliku keys.txt do odczytu!\n");
    	exit(1);
    }    
    
	fscanf(keys, "%d %d\r\n", &mod, &d);
    fscanf(keys, "%d %d\r\n", &mod, &d); // wczytaj dwa razy aby uzyskac dane klucza z drugiej linii pliku keys.txt ( czyli prywatnego )
    fscanf(keys, "%d %d %d %d %d\0", &p, &q, &dP, &dQ, &qInv); // dla trybu CRT
    int c; // c - porcja szyfrogramu ( znak ) 
    switch (mode)
	{
		case 0: // tryb zwykly
			printf("Klucz prywatny: modulo = %d, wykladnik = %d\n", mod, d); 
			while ( (c = getc(input)) != EOF)
    			putc(pot_mod(c, d, mod), output);   		
			break;
		case 1:
			printf("Klucz prywatny: p = %d, q = %d, dP = %d, dQ = %d, qInv = %d\n", p, q, dP, dQ, qInv);    
			int m1, m2, h;
			while ( (c = getc(input)) != EOF)
		    {
		    	m1 = pot_mod(c, dP,  p);
		    	m2 = pot_mod(c, dQ,  q);
		    	//if (m1 < m2) h = qInv * ((m1 + ((q + p - 1) / p)*p)%p);
				h = (qInv * (m1-m2)) % p; //else
		    	putc((m2 + h*q), output);
			}
    		break;
   		default:
   			printf("Bledny tryb - koniec programu!\n");
   			exit(1);
    		break;
	}
  
    fclose (input);
    fclose (keys);
}

void generateKeys(int k, int l)
{
	/*TODO : 
		- GENEROWANIE K KLUCZY DLUGOSCI D-BITOW
	*/
	printf("%d liczb pierwszych %d-bitowych \n", k, l);
	FILE *keys;
	if ((keys=fopen("keys.txt", "wb"))==NULL)
	{
		printf ("Nie mozna otworzyc pliku keys.txt do zapisu!\n");
    	exit(1);
    }
	int prms[2] = {13, 11};
	int len = 2;
	int eul, mod;
	eul = eulfun(prms, len);
	mod = getmod(prms, len);
	
	int e = 3; // wykladnik klucza publicznego, wstepnie zainicjalizowany
	while (nwd(e, eul) != 1) // wyznacza e (nieparzyste) wzglednie pierwsze z eul
		e += 2;
	
	int d = revmod(e, eul); // wykladnik klucza prywatnego, odwrotny co do modulu ( =eul ) do liczby e
	
	// dla wersji CRT
	int dP, dQ, qInv;
	dP = d % (prms[0] - 1);
	dQ = d % (prms[1] - 1);
	qInv = revmod(prms[1], prms[0]);
	

	fprintf(keys, "%d %d\r\n", mod, e); //zapis klucza publicznego
	fprintf(keys, "%d %d\r\n", mod, d); // zapis klucza prywatnego
	fprintf(keys, "%d %d %d %d %d\0", prms[0], prms[1], dP, dQ, qInv); // zapis klucza prywatnego CRT
	fclose (keys);
}

int eulfun (int primes[], int len)
{
	if (len < 2)
	{
		printf("Za malo liczb pierwszych!\n");
		return -1; // zwroc -1 w przypadku bledu
	}
	int i;
	int result = 1;
	for (i=0; i<len; i++)
		result *= (primes[i]-1);
		
	return result;
}

int getmod (int primes[], int len)
{
	if (len < 2)
	{
		printf("Za malo liczb pierwszych!\n");
		return -1; // zwroc -1 w przypadku bledu
	}
	int i;
	int result = 1;
	for (i=0; i<len; i++)
		result *= primes[i];
		
	return result;
}

int nwd (int a, int b)
{
	int temp;

	while(b != 0)
	{
    	temp = b;
    	b = a % b;
    	a = temp;
  	};
	return a;
}

int revmod (int a, int mod)
{
	int a0,n0,p0,p1,q,r,t;

	p0 = 0; p1 = 1; a0 = a; n0 = mod;
	q  = n0 / a0;
	r  = n0 % a0;
	while(r > 0)
	{
    	t = p0 - q * p1;
    	if(t >= 0)
			t = t % mod;
		else
      		t = mod - ((-t) % mod);
    	
		p0 = p1; p1 = t;
    	n0 = a0; a0 = r;
    	q  = n0 / a0;
    	r  = n0 % a0;
	}
  	return p1;
}

int pot_mod(int a, int w, int n) // wykladnik rozkladamy jako sume poteg 2 ( alg. Hornera ). Dla reszt niezerowych tworzymy iloczyn potęg a modulo n
{
  int pot,wyn,q;

  pot = a; wyn = 1;
  for(q = w; q > 0; q /= 2)
  {
    if(q % 2) wyn = (wyn * pot) % n;
    pot = (pot * pot) % n; // kolejna potęga
  }
  return wyn;
}
