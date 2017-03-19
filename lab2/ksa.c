#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*
  Implementation of the RC4 encryption scheme
*/

/*
  Return the size of the string in bytes
*/



int sizeInBytes(char* str)
{
  return sizeof(char)*strlen(str);
}
/*
  return the size of a given string in bits
*/
int sizeInBits(char* str)
{
  return sizeInBytes(str)*8;
}

/*
  swap two elements pointers
*/

void swap(int* a,int* b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

/*
  allocate memory
*/

int* allocate(int n)
{
  return (int*)malloc(sizeof(int)*n);
}

/*
  free memory
*/
int* freeArray(int* p){
  if(p!=NULL)
  {
    free(p);
    p=NULL;
  }
  return p;
}


int* KSA(char* key, int n, int t)
{
  int* s = (int*)malloc(sizeof(int)*n);
  int i,j,l;

  for(i=0;i<n;i++)
  {
    s[i] = i;
  }

  j=0;
  l = sizeInBytes(key);
  int temp;
  for(i=0;i<t;i++)
  {
    temp = key[i%l];
    j = (j+s[i]+temp)%n;
    swap(&s[i%n],&s[j%n]);
  }
  return s;
}

int* KSA_RS(char* key,int n, int t)
{


  int* s = allocate(n);
  int* top = NULL;
  int* bottom = NULL;
  int* newS = NULL;
  int i,j,r,l,temp,index,k,v,tin,bin,lt;

  for(i=0;i<n;i++)
  {
    s[i] = i;
  }

  lt=sizeInBytes(key);
  l = lt*8;
  top = allocate(n);
  bottom = allocate(n);
  newS = allocate(n);
  for(r=0;r<t;r++)
  {

    tin = 0;
    bin = 0;

    for(i=0;i<n;i++)
    {
      index = (r*n+i)%l;
      v = key[index/8];
      k = (index%8);
      k = v&(1<<k);

      if(k==0)
      {
          top[tin++] = i;
      }else
      {
          bottom[bin++] = i;
      }

    }

    for(j=0;j<tin;j++)
    {
      newS[j] = s[top[j]];
    }

    for(j=0;j<bin;j++)
    {
      newS[j+tin] = s[bottom[j]];
    }


    for(j=0;j<n;j++)
    {
      s[j] = newS[j];
    }

  }

  top = freeArray(top);
  bottom = freeArray(bottom);
  newS = freeArray(newS);
  return s;
}

void PRNGA(int* s, int n)
{
  int i=0;
  int j=0;
  int looper = 0;
  int k;
  while(++looper!=10000)
    {
      i = (i+1)%n;
      j = (j+s[i])%n;
      swap(&s[i],&s[j]);
      k = s[(s[i]+s[j])%n];
      printf("%d\n",k);
    }
}

char* RC4(char*key,int n, int t)
{
  int* s = KSA(key,n,t);
  PRNGA(s,n);
  s = freeArray(s);
  return NULL;
}

char* RC4_RS(char*key, int n, int t)
{
  int* s = KSA(key,n,t);
  PRNGA(s,n);
  s = freeArray(s);
  return NULL;
}

int main(int argc,char* argv[])
{
  if(argc<5)
  {
    printf("not enough arguments");
    return 0;
  }
  int n = atoi(argv[2]);
  int t = atoi(argv[3]);
  int test = atoi(argv[4]);
  //char* s = RC4(argv[1],n,t);
  char* k = RC4_RS(argv[1],n,t);
  switch(test)
  {
    case 0: RC4(argv[1],n,t);
            break;
    case 1: RC4_RS(argv[1],n,t);
    default: printf("no valid option selected\n");
  }

  return 0;
}
