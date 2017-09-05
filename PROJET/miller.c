#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*int miller_rabin(int nb){
	int rangInf=nb-1;
	int s,d;
	s=0;
	while((rangInf%2)==0){
		rangInf/=2;
		s++;
	}
	
	int puissanceDeux=1;
	puissanceDeux<<=s;
	d=(nb-1)/puissanceDeux;
	printf("%d=2^%dx%d\n",nb-1,s,d);
	
	int i,nbAlea;
	for(i=0;i<s;i++){
		nbAlea=rand()%nb;
		
	}
	
	
	return 1;
}*/

long long exponentielle_modulo(long long a, long long e, long long n)
{
	long long x=1;
	while(e){
		a=a%n;
		if ((e&1) == 1) x = ((x%n)*a)%n;
		e>>=1;
		a = (a*a)%n;
	}
	return x;
}

//renvoie 0 si il est possiblement premier
int temoin_miller(int a, int n){
	long long rangInf=n-1;
	long long s,d;
	s=0;
	while((rangInf&1)==0){
		rangInf/=2;
		s++;
	}

	long long puissanceDeux=1;
	puissanceDeux<<=s;
	d=(n-1)/puissanceDeux;
	//printf("%d=2^%dx%d\n",n-1,s,d);
	printf("s=%lld, d=%lld\n",s,d);
	
	long long x=exponentielle_modulo(a,d,n);
	printf("x=%lld\n",x);
	if((x==1)||(x==n-1)) return 0;
	while(s>1){
		x=exponentielle_modulo(x,2,n);
		//printf("x=%d\n",x);
		if(x==n-1) return 0;
		s=s-1;
	}
	return 1;
}

int miller_rabin(long long n,long long k){
	long long bit;
	long long temp=n;
	if(temp<=0) return 0;
	else if(temp==1) return 0;
	else if(temp==2) return 1;
	else if(n&1==0) return 0;
	long long i;
	long long alea;
	for(i=0;i<k;i++){
		alea=1;
		while(alea<2){
			alea=rand()%(n-2);
		}
		printf("alea: %lld, ",alea);
		if(temoin_miller(alea,n)) return 0;
	}
	return 1;
}

int main(int argc,char** argv){
	if(argc==1){
		printf("Manque d'arguments\n");
		exit(1);
	}
	srand(time(NULL));
	long long nombre=atoi(argv[1]);
	//int nombre=rand();
	printf("Le nombre à tester est %lld\n",nombre);
	int reponse=miller_rabin(nombre,5);
	if(reponse==0) printf("NON PREMIER\n");
	else if(reponse==1) printf("PREMIER\n");
	else printf("??\n");
	//printf("REPONSE: %d\n",miller_rabin(nombre,12));
	//temoin_miller(12,nombre);
	//printf("Resultat: %d\n",miller_rabin(nombre));
	
	return 0;
}
