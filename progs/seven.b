
main() returns void
{
    int x;
    int B;
    int i


    B = 7
    i = 3
    printi(i)
    printLine()

    do 100 i = 3, 7, 1
        printi(i)
        printLine()
        B = B + 2 * i
100 continue    

    printi(B)
    printLine()
    do 200 i = 3, 7, 2
	x = i * 3
200 continue

    if( B < x  && B != 0)
    {
	if( x == 0 )
	{
            printLine()
	    printi(x)
	}
	else
	{
	    printi(x)
	}
    }
    printLine()     
}
