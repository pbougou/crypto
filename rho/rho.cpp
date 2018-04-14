#include <bits/stdc++.h>

using namespace std;
 
// function for modular exponentiation.
// It returns: result = (a^d) % p
long long int mod_expo(long long int a, unsigned int d, long long int p)
{
    long long int result = 1;
    unsigned int i;
    a %= p;
               
    for(i=d; i>0; i>>=1)
    {
        if (i & 1)
            result = (result*a) % p;

        a = (a*a) % p;
    }
    return result;
}

// function for miller rabin probabilistic prime test
// returns true if p is probably prime
// and false if p is not a prime for sure
bool miller_rabin(long long int t, long long int p)
{
    srand(time(NULL));
    long long int b = 2 + rand() % (p - 4);
 
    long long int x = mod_expo(b, t, p);
 
    if (x == 1  || x == p-1) return true;
 
    while (t != p-1)
    {
        x = (x * x) % p;
        t *= 2;

        if (x == 1)
		return false;
        else if (x == p-1)    
		return true;
    }
 
    return false;
}

// make miller rabin test more accurate
// repeat it k times where k accuracy parameter 
// and make input parameter t 
// where t is n = (2^s)*t
bool isPrime(long long int n, int k)
{
    if (n <= 1 || n == 4)  return false;
    if (n <= 3) return true;
    if (n%2 == 0) return false;
    
    long long int t = n - 1;
    while (t % 2 == 0)
        t /= 2;
 
    for (int i = 0; i < k; i++)
         if (miller_rabin(t, n) == false)
              return false;
 
    return true;
}

int step; 

// Pollard rho probabilistic factorization method
long long int rho(long long int n)
{
    srand (time(NULL));

    if (n==1) return n;
 
    if (n % 2 == 0) return 2;
 
    long long int x = (rand()%(n-2))+2;
    long long int y = x;
    long long int k = 2;
 
    long long int d;  
 
    step = 0;
    while (true)
    {
		step++;

		x = (((x-1)%n)*((x+1)%n))%n;
		d = __gcd(abs(x-y),n);

		if(d!=1 && d!=n)
			return d;

		if(step==k) {
			y = x;
			k *= 2;
		}
    }
 
    return d;
}

// Main: In vector of primes there are all primes between lower and upper
// We take at random two of them and we construct N and then
// N=p*q where p,q primes is factorized with rho method
vector<long long int> primes(11);
int main(int argc,char **argv)
{
    long long int d,N;
    clock_t t3,t4;
   
    if(argc!=3) {
	fprintf(stderr,"Usage: ./exec Low High\nRun with ./exec 1000 10000 for our exercise.\n"); 
	exit(1);
    }
    
    long long int lower = atoi(argv[1]);
    long long int upper = atoi(argv[2]);

    long long int n,j=0;
    
    for(n=lower; n <= upper; ++n) {
    	if(isPrime(n,5)) {
    		primes.push_back(n);
    	}
    }
  
    vector<long long int>::size_type sz = primes.size();
    long long int p1,p2; 
    srand (time(NULL));	
    long long int randomIndex = rand() % sz;
    p1 = primes[randomIndex];
    srand (time(NULL));
    randomIndex = rand() % sz;
    p2 = primes[randomIndex];
    N = p1 * p2;

    t3 = clock();
    d = rho(N);
    t4 = clock(); 

    clock_t t = t4-t3;
    if(d== p1 || d==p2)
    	printf("Pollard Rho with N=pq,where N=%lld,p=%lld,q=%lld took %fsec in %d steps with d=%lld\n",N,p1,p2,((double)t)/CLOCKS_PER_SEC,step,d);
    					 
    return 0;
}

