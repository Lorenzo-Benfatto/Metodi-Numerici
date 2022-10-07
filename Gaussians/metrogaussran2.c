// sto coso usa il ran2 e funge ma il seed è dato col culo

#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran2(long *idum)
{
	int j;
	long k;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	float temp;

	if (*idum <= 0) {
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		idum2=(*idum);
		for (j=NTAB+7;j>=0;j--) {
			k=(*idum)/IQ1;
			*idum=IA1*(*idum-k*IQ1)-k*IR1;
			if (*idum < 0) *idum += IM1;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ1;
	*idum=IA1*(*idum-k*IQ1)-k*IR1;
	if (*idum < 0) *idum += IM1;
	k=idum2/IQ2;
	idum2=IA2*(idum2-k*IQ2)-k*IR2;
	if (idum2 < 0) idum2 += IM2;
	j=iy/NDIV;
	iy=iv[j]-idum2;
	iv[j] = *idum;
	if (iy < 1) iy += IMM1;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}
#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(void){
    int a, start=0, N=100000;
    long int z,j;
    float mu=5, sigma2=1, d=2.93495;
    float xp, y, r, p, q; //y è il xk+1
    FILE *f;
    f=fopen("metrogauss.txt","w");
    if(f==NULL){
     perror("File non correttamente aperto");
     exit(1);
    }

    y=start;
    for (int i = 0; i < N; i++){
        j=i;
        xp=ran2(&j);
        xp=y+d*(1-2*xp);
        r=1;
        p=exp((-pow((xp-mu),2)+pow((y-mu),2))/(2*sigma2));
        if(r>p){
            r=p;
        }
        z=i+N;
        q=ran2(&z);
        a=0;
        if(q<r){
            y=xp;
            a=1;
        }
        fprintf(f, "%f  %d  %d \n", y,i,a);
    }
    fclose(f);
    return 0;
}