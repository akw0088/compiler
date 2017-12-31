
function void main()
{
    int I;
    for 100 (I = 1; 10)
    {
    	printi(I);
        prints(" Factorial = ");
    	printi(factorial(I));
        printLine();
    } 100 end
}

function int factorial(n)
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
