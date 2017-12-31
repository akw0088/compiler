int X[1000]

function void main()
{
    int i;

    for 10 (i = 0; 999)
    {
        X[i] = (i * 32767) % 10000;
    } 10  end

	prints("Some of the original values are:\n");
        printi(X[111]);
	prints(" ");
        printi(X[222]);
	prints(" ");
        printi(X[333]);
	prints(" ");
        printi(X[666]);
	prints(" ");
        printi(X[777]);
	prints(" ");
        printi(X[999]);
        printLine();

	printLine();
	printLine();
	printLine();

	sort(1000);
    
	prints("Some of the sorted values, in order are:\n");
        printi(X[111]);
	prints(" ");
        printi(X[222]);
	prints(" ");
        printi(X[333]);
	prints(" ");
        printi(X[666]);
	prints(" ");
        printi(X[777]);
	prints(" ");
        printi(X[999]);
        printLine();
}


function void sort(n)
    int n
{
    int i;
    int j;
    for 200 (i = 0; n-1; 1)
    {
        for 100 (j = 0; n-i-1; 1)
	{
	    if( X[j] > X[j+1] )
            {
		int temp;
		temp   = X[j];
		X[j]   = X[j+1];
		X[j+1] = temp;
	    }
	} 100 end
    } 200 end
}
