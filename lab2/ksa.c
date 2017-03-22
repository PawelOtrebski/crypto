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
		s->s[i] = i;
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

long PRGA(state** s, int n)
{
	int* i = &(*s)->i;
	int* j = &(*s)->j;
	
	*i = (*i+1)%n;
	*j = (*j+(*s)->s[*i])%n;
	swap(&(*s)->s[*i],&(*s)->s[*j]);
	return (*s)->s[((*s)->s[*i]+(*s)->s[*j])%n]&0xFF;
}

rc4* setUp(int n, int t, char* k)
{
	rc4* r = allocateMemoryForRC4(n,t,k);
	r->s = initializeState(r->s,r->n);
	KSA(&r);
	return r;
}

char getByte(rc4** r)
{
	long c = PRGA(&(*r)->s,(*r)->n);
	return c;
}


int main(int argc,char* argv[])
{
	rc4* r = setUp(16,16,"john");
	r = freeRC4Memory(r);
	return 0;
}
