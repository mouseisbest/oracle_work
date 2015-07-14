#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define COUNTS 6000000

char *memcpy2(char *dest, char *src, size_t n)
{
        int i = 0;
    while (i < n)
    {
        dest[i] = src[i];
        i ++;
    }
    return dest;

}

char *memcpy1(char *to, char *from, size_t n)
{
    long esi, edi;
    int ecx;
    esi = (long)from;
    edi = (long)to;
    asm volatile("rep ; movsl"
        : "=&c" (ecx), "=&D" (edi), "=&S" (esi)
        : "0" (n / 4), "1" (edi), "2" (esi)
        : "memory"
        );
    return to;
}

long long getcyclenow()
{
	unsigned int h, l;
	long long bs, cc;
	asm("rdtsc /n/t");
	asm("movl %%eax, %0/n/t":"=g"(l));
	asm("movl %%edx, %0/n/t":"=g"(h));
	bs = h;
	cc = (bs << 32)|l;
	return cc;
}
int main(int argc, char **argv)
{
	time_t st0, st1, st2, st3;
	long long l0, l1, l2, l3;
	int i = 0;
	char *src = (char *)calloc(1, 109);
	strcpy(src, "iiiiabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
	char *dest0 = (char *)calloc(1, 109);
	char *dest1 = (char *)calloc(1, 109);
	char *dest2 = (char *)calloc(1, 109);

	l0 = getcyclenow();
	dest0 = (char*)memcpy(dest0, src, 108);
	l1 = getcyclenow();
	dest2 = memcpy2(dest2, src, 108);
	l2 = getcyclenow();
	dest1 = memcpy1(dest1, src, 108);
	l3 = getcyclenow();
	printf("%X/t%X/t%X/n", l1-l0, l2-l1, l3-l2);

	memset(dest0, 0, 109);
	memset(dest1, 0, 109);
	memset(dest2, 0, 109);

	st0 = time(NULL);
	for (i = 0; i < COUNTS; i++)
		dest0 = (char*)(dest0, src, 108);
	st1 = time(NULL);
	for (i = 0; i < COUNTS; i++)
		dest2 = memcpy2(dest2, src, 108);
	st2 = time(NULL);
	for (i = 0; i < COUNTS; i++)
		dest1 = memcpy1(dest1, src, 108);
	st3 = time(NULL);
	printf("t0:%f, t1:%f, %f/n", difftime(st1, st0), difftime(st2, st1), difftime(st3,st2));

	/*
	memset(dest0, 0, 109);
	memset(dest1, 0, 109);
	memset(dest2, 0, 109);

	l0 = getcyclenow();
	dest0 = memcpy(dest0, src, 108);
	l1 = getcyclenow();
	dest2 = memcpy2(dest2, src, 108);
	l2 = getcyclenow();
	dest1 = memcpy1(dest1, src, 108);
	l3 = getcyclenow();
	printf("%X/t%X/t%X/n", l1-l0, l2-l1, l3-l2);
	*/
	return 0;
}
