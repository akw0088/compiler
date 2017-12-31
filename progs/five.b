function void main()
{
    int x;
    int B;
    int i;

    x = 1;
    B = 1;
    for 100 (i = 3; 7)
    {
        printi(i);
        B = B + 2 * i;
    } 100 end    
    printLine();
    printi(B);
    printLine();

    for 200 (i = 3; 7; 2)
    {
	x = x * i;
    } 200 end    
    printi(x);
    printLine();
}
