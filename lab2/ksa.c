#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*
	Implementacja RC4
*/

struct STATE{
	int i;
	int j;
	long* s;
}typedef state;

struct KEY{
	int l;
	char* k;
}typedef key;

struct RC4{
	state* s;
	key* k;
	int t;
	int n;
}typedef rc4;

FILE* f;

void swap(long *a,long *b)
{
	long t = *a;
	*a = *b;
	*b = t;
}

/*
	Allocate memory for the state of the permutation that will be used 
	for generting random numbers
*/
state* allocateMemoryForState(int n)
{
	
	state* s = NULL;
	
	s = (state*)malloc(sizeof(state));
	s->s = (long*)malloc(sizeof(long)*n);
	s->i = 0;
	s->j = 0;
	return s;
	
}

key* allocateMemoryForKey(char* s)
{
	int len = 0;
	int i;
	for(i=0;s[i]!='\0';i++)
	{
		len++;
	}
	
	key* k = (key*)malloc(sizeof(key));
	k->l = len;
	k->k = (char*)malloc(sizeof(char)*len);
	for(i=0;i<k->l;i++)
	{
		k->k[i] = s[i];
	}
	
	return k;
}

rc4* allocateMemoryForRC4(int n, int t, char* k)
{
	rc4* r = (rc4*)malloc(sizeof(rc4));
	r->s = allocateMemoryForState(n);
	r->k = allocateMemoryForKey(k);
	r->n = n;
	r->t = t;
	return r;
}

key* freeKeyMemory(key* k)
{
	if(k==NULL)
		return NULL;
		
	free(k->k);
	k->k = NULL;
	free(k);
	return NULL;
}
/*
	free memory allocated for state, to clean up program
*/
state* freeStateMemory(state* s)
{
	if(s==NULL)
		return NULL;
		
	free(s->s);
	free(s);
	s = NULL;
	
	return NULL;
}

rc4* freeRC4Memory(rc4* r)
{
	if(r==NULL)
		return NULL;
		
	r->s = freeStateMemory(r->s);
	r->k = freeKeyMemory(r->k);
	free(r);
	r=NULL;
	return r;
}

/*
	initialize the state of the permutation
*/
state* initializeState(state* s, int n)
{
	int i;
	
	for(i=0;i<n;i++)
	{
		s->s[i] = (long)i;
	}
	
	return s;
	
}



/*
	original KSA
*/
void KSA(rc4** r)
{
	int j = 0;
	int i;
	int t = (*r)->t;
	int n = (*r)->n;
	int l = (*r)->k->l;
	state* s = (*r)->s;
	for(i=0;i<t;i++)
	{
		j = (j+s->s[i] + (*r)->k->k[i%l] )%n;
		swap(&s->s[i%n],&s->s[j%n]);
	}
	(*r)->s = s;
	for(i=0;i<(*r)->n;i++)
	{
		printf("%ld\n",(*r)->s->s[i]);
	}
	
}
/*
	RS version of KS 
*/
void KSA_RS(rc4** r)
{
printf("here");
	int rc,i,ki,bi,bit,tcount,bcount;
	int t = (*r)->t;
	int n = (*r)->n;
	//bit size of key
	int lbits = (*r)->k->l*8;
	int l = (*r)->k->l;
	state* s = (*r)->s;
	long top[n];
	long bottom[n];
	long newS[n];
	char* key = (*r)->k->k;
	for(rc=0; rc<t;rc++)
	{
		
		tcount = 0;
		bcount = 0;
		for(i=0;i<n;i++)
		{	
			ki  = (rc*n+i)%lbits;
			bi = ki/l;
			bit = 1<<(bi%8);
			bi = key[bi]&bit;
			
			if(bi==0){
				top[tcount++] = i;
			}else
			{
				bottom[bcount++] = i;
			}
			
		}
		
		printf("top\n");
		for(i=0;i<tcount;i++)
		{
			
			newS[i] = s->s[top[i]];
			//printf("%ld ", s->s[top[i]]);
		}
		printf("\nbottom\n");
		for(i=0;i<bcount;i++){
			
			newS[i+tcount] = s->s[bottom[i]];
			//printf("%ld ", s->s[bottom[i]]);
		}
		
		for(i=0;i<n;i++)
		{
			s->s[i] = newS[i];
		}
	}
	
}

long PRGA(state** s, int n)
{
	int* i = &(*s)->i;
	int* j = &(*s)->j;
	//printf("1:%d,%d\n",*i,*j);
	*i = (*i+1)%n;
	//printf("2:%d\n",*i);
	*j = (*j+(*s)->s[*i])%n;
	
	swap(&(*s)->s[*i],&(*s)->s[*j]);
	//printf("3:%ld\n",(*s)->s[((*s)->s[*i]+(*s)->s[*j])%n]);
	return (*s)->s[((*s)->s[*i]+(*s)->s[*j])%n];
}

rc4* setUp(int n, int t, char* k, int type)
{
	printf("setup\n");
	rc4* r = allocateMemoryForRC4(n,t,k);
	r->s = initializeState(r->s,r->n);
	if(type==0)
	{
		KSA(&r);
	}else if(type==1)
	{
		printf("KSA_RS\n");
		KSA_RS(&r);
	}
	return r;
}

long getByte(rc4** r)
{
	long c = PRGA(&(*r)->s,(*r)->n);
	return c;
}

FILE* openFile(FILE *fp, char* filename)
{
	fp = fopen(filename,"wb");
	return fp;
}

void closeFile(FILE *fp)
{
	fclose(fp);
}

void writeToFile(rc4** r, FILE *fp)
{
	int byteCount = 0;
	long c;
	while(byteCount<16000000)
	{
		c = getByte(r);
		printf("%d",c);
		putc(c,fp);
		byteCount++;
	}
}

int main(int argc,char* argv[])
{
	int n = atoi(argv[1]);
	int t = atoi(argv[2]);
	int type = atoi(argv[4]);
	rc4* r = setUp(n,t,argv[3],type);
	f = openFile(f,"test.txt");
	writeToFile(&r,f);
	closeFile(f);	
	
	r = freeRC4Memory(r);
	return 0;
}
