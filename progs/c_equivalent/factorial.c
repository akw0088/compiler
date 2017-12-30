
int main() 
{
    int I;
    for( I =  1; I <= 10; I++ )
    {
    	printi(I);
        prints(" Factorial = ");
    	printi(factorial(I));
        printLine();
    }
}

int factorial(int n) 
{
    if( n == 1 )
	return 1;
    return( n * factorial(n-1) );
}
