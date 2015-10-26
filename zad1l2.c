#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXLINE 1000

int getline(char s[], int lim, FILE *fp, char s1[]);

main(int argc, char *argv[])
{
	FILE *fp;
	if ((fp=fopen(argv[1], "r"))==NULL)
	{
		printf ("Nie mogę otworzyć pliku do odczytu!\n");
    	exit(1);
    }
    int quantityOfCpt = strtol(argv[2], NULL, 10); // liczba szyfrogramow
    char a[quantityOfCpt][MAXLINE]; // tablica stringow dla kazdego szyfrogramu
	char line[MAXLINE]; // biezacy szyfrogram
	int i=0;
	int quantityOfChars = strtol(argv[3], NULL, 10);
	
	while( getline(line, MAXLINE, fp, a[i]) > 0 && i < quantityOfCpt)
	{
		printf("Pobrano szyfrogram %s\n", line);
		i++;
	}
	
	//TODO: pobrac w args liczbe kryptogramow(quantityOfCpt) i  liczbe znakow(quantityOfChars), 
	//zainicjalizować tablicę ptrs na poczatki kolejnych szyfrogramow ( czyli tez zapisac kolejne szyfrogramy jako stringi, do nich ptr przypisac )
	int maxkey = (pow(2, 8)-1); // maximal 8bit key
	int key = 0; // actual cheking key
	int mainkey[quantityOfCpt]; //ostatni poprawny klucz dla szyfrogramu
	char *ptrs[quantityOfCpt]; // wskazuje koniec ostatnich wczytanych danych ( czyli wczytuje 8cyfr 0-1 do spacji gdy dane poprawne ) [tablica wskaznikow]
	long int enc[quantityOfCpt][quantityOfChars]; // przechowuje dla danego wiersza zaszyfrowana wartosc dziesietna danego znaku ( podana w argv[3] )
	int decrypted = 0;
	int character = 0; //actual position analizing char
	int cryptogram = 0; // wskazuje index aktualnie sprawdzanego szyfrogramu
	int j = 0; //index for loop
	
	for (i=0; i < quantityOfCpt; i++) //prepare array enc with dec reprezentation of each char 
	{
		ptrs[i] = a[i];
		//printf("Ptrs[%d]: %s\n", i, ptrs[i]);
		for(j=0; j<quantityOfChars; j++)
		{
			enc[i][j] = strtol(ptrs[i], &ptrs[i], 2); // gdy dany szyfrogram ma mniej znakow niz szukany, na brakujacych pozycjach wartosc 0
		}
	}
	
	for (i=0; i < quantityOfCpt; i++) //wyswietl zaszyfrowane znaki dziesietnie dla kazdego szyfrogramu
	{
		printf("Szyfrogram %d dziesietnie: ", i);
		for(j=0; j<quantityOfChars; j++)
		{
			printf("%d ", enc[i][j]);
		}
		printf("\n");
	}
	
	int passes[quantityOfChars]; //przechowuje liczbe szyfrogramow dla ktorych biezacy klucz spelnia warunki dla znaku x ( przy passes[x] )
	for(character=0; character<quantityOfChars; character++) // inicjalizacja
		passes[character] = 0;
	
	for(character=0; character<quantityOfChars; character++)
	{
		for(key=0; key<=maxkey; key++)
		{
			for(cryptogram=0; cryptogram<quantityOfCpt; cryptogram++)
			{
				//enc[cryptogram][character] = strtol(a[cryptogram], %(ptrs[cryptogram]), 2); //save in decimal encrypted char ( remember int ptr next char )
				if (enc[cryptogram][character] == 0) continue; // this case can be if actual cryptogram is too short than main text
				decrypted = enc[cryptogram][character] ^ key;
				if(  0 == ((decrypted>=65 && decrypted<=90) || (decrypted>=97 && decrypted<=122) || decrypted == 44 || decrypted == 46 ||
					decrypted == 33 || decrypted == 58 || decrypted == 59 || decrypted == 32 || decrypted == 63) 
				  ) break;		//if decrypted char has not sense check next key
				if (passes[character] <= cryptogram) //jesli znalazles klucz ktory spelnia warunek dla wiecej szyfrogramow niz aktualnie najlepszy dla danego znaku
				{
					//printf("decrypted %c\n", decrypted);
					printf("Old potential key")
					mainkey[character] = key; //last key whos give good value for some(maybe for all) cryptogram
					passes[character] = cryptogram;
				}
			}
		}
	}
	
	// wyswietl wyniki
	printf("Key: ");
	for(character=0; character<quantityOfChars; character++)
		printf("%d ", mainkey[character]);
	printf("\n");
	for(cryptogram=0; cryptogram<quantityOfCpt; cryptogram++)
	{
		printf("Text %d: ", cryptogram);
		for(character=0; character<quantityOfChars; character++)
		{
			decrypted = enc[cryptogram][character] ^ mainkey[character];
			printf("%c", decrypted);
		}
		printf("\n");
	}	
}

/*pobiera wiersz ze pliku (fp i zapisuje w s1*/
int getline(char s[], int lim, FILE *fp, char s1[])
{
	int c, i;
	
	for (i=0; i < lim-1 && (c=getc(fp))!=EOF && c!='\n'; ++i)
		s[i] = c;
	if (c == '\n')
	{
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	strcpy(s1, s);
	return i;
}
