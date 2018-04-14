#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <limits.h>
 
int frequency[26] = { 0 };
char *buffer = NULL;  // Ciphertext
size_t size = 0;      // Size of ciphertext 

double e_frq[26]= {
        0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
        0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
        0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
        0.00978, 0.02360, 0.00150, 0.01974, 0.00074};

int chi_distr()
{
	int i;
	double min = 1000000.0,sum=0;
	for(i=0; i<26; i++)
		sum += frequency[i];

	int key, probableKey;
	double e_i, diafora, chi_i,chiSum=0.0, c_i;
	double chiS[26];
	for(key = 0; key < 26; key++)
	{
		chiSum = 0; diafora = 0;
		for(i=0; i<26; i++)
		{
			c_i = frequency[(i+key)%26];
			e_i = sum * e_frq[i];
			diafora = c_i - e_i;
			diafora = diafora*diafora;
			chi_i = diafora/e_i;
			chiSum += chi_i;
		}

		chiS[key] = chiSum;

		if(chiSum < min)
		{
			min = chiSum;
			probableKey = key;
		}
	}

	return probableKey;			
}

double ic(double n)
{
	double sum = 0;
	int i=0;
	for(i=0; i<26; i++)
		sum += frequency[i] * (frequency[i] - 1);

	double ic = sum / (n*(n-1));
	return ic;
	 	
}


void readAndStoreText(char *arg)
{
	
	FILE *fp = fopen(arg, "r");
	fseek(fp, 0, SEEK_END); 
	size = ftell(fp); 
	rewind(fp);
	buffer = malloc((size + 1) * sizeof(*buffer)); 
	fread(buffer, size, 1, fp);
	buffer[size] = '\0';
}

void decrypt(char *key, int length)
{
 	int i,j;
	char c;
	for(i = 0, j = 0; i < size-1; ++i)
	{
		c = buffer[i];
		
		buffer[i] = ((c - key[j] + 26) % 26) + 65; 
		j = (j + 1) % length;
	}
}
 
int main(int argc, char *argv[])
{

 	if(argc==1 || argc>=3) {
		printf("Usage: ./exec FileName\n");
		exit(1);
	}

	readAndStoreText(argv[1]);
	size = strlen(buffer);
	
	
	int r = 1,i=0,j=0,index,k=0;
	double upper;
	double ioc = 0;

	/* Find key length */
	for(r=1; r < size; r++)
	{	
		upper = size/(double) r;		
		upper = ceil(upper);
	

		for(k=0; k<26; k++)		
			frequency[k] = 0;	
		ioc = 0;

		for(i=0;i < r;i++)
		{

			for(k=0; k<26; k++)		
				frequency[k] = 0;

			for(j=0; j < upper-1; j++)
			{
				index = buffer[i+j*r] - 65;
				frequency[index]++;
			}
		
			ioc += ic(upper);          // average index of coincidence
		}

		if(ioc/r > 0.06)
		{		
			break;
		}
		
	}

	char key[6];
	
	for(k=0; k<26; k++)
		frequency[k] = 0;	

	for(i=0;i < r;i++)
	{

		for(k=0; k<26; k++)
			frequency[k] = 0;

		for(j=0; j < upper-1; j++)
		{
			index = buffer[i+j*r] - 65;
			frequency[index]++;
		}
		
		key[i] = chi_distr() + 65;
		
	}

	key[i+1] = '\0';
	decrypt(key,r);
	printf("%s",buffer);
	return 0;
}
