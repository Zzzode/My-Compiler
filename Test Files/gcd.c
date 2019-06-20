#include <stdio.h>

int gcd(int a, int b)
{
    if(b == 0)
        return a;
    else
        return gcd(b,a % b);
}

int main()
{
    printf("%d\n",gcd(6,36)*gcd(3,6));
    return 0;
}