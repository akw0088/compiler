
main() returns void
{
    int I;
    do 100 I = 1, 10
    	printi(I);
        prints(" Factorial = ");
    	printi(factorial(I));
        printLine();
100 continue
}

factorial(n) returns int
    int n 
{
    if( n == 1 )
    {
	factorial = 1;
    }
    else
    {
	factorial = n * factorial(n-1);
    }
}
