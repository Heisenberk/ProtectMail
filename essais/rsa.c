#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
//typedef unsigned long long bigInt;
//ULONG_MAX
int alea(){
	double q=sqrt(INT_MAX);
	//double q=INT_MAX;
	int n=q; 
	int i=rand()%n;
	return i;
}

int teste_premier(int a){
	if(a==2) return 1;
	else if(a%2==0) return 0;
	int n;
	int test=0;
	double b=a;
	double aa=sqrt(b);
	for(n=1;n<=a;n=n+2){
		if(a%n==0){
			test++;
		}
	}
	if(test==2) return 1;
	return 0;
}

int alea_premier(){
	int i=alea();
	//printf("->%d\n",i);
	if(teste_premier(i)) return i;
	else {
		return alea_premier();
	}
}

int pgcd(int a,int b){
	if (a % b == 0) return b;
	return pgcd(b, a % b);
}

int genere_e(int z){
	double q=sqrt(INT_MAX);
	int n=q; 
	int e=rand()%n;
	if(pgcd(e,z)==1) return e;
	return genere_e(z);
}

int genere_d(int e,int z){
	int d=0;
	while((e*d)%z!=1){
		d=d+1;
	}
	return d;
}

int main(){
	srand(time(NULL));
	int p,q,n,z,e,d;
	p=alea_premier();
	q=alea_premier();
	//p=11;
	//q=5;
	while(p==q){
		p=alea_premier();
		q=alea_premier();
	}
	printf("\np=%u\n",p);
	printf("q=%u\n",q);
	n=p*q;
	printf("n=%d\n",n);
	z=(p-1)*(q-1);
	printf("z=%d\n",z);
	e=genere_e(z);
	printf("e=%d\n",e);
	d=genere_d(e,z);
	printf("d=%d\n",d);
	printf("\n");
	return 0;
}
