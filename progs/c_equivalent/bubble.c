int X[1000];

void sort(int n) 
{
    int i;
    int j;
    for( i = 0; i < n; i++ )
        for( j = 0; j < n-i; j++ )
	    if( X[j] > X[j+1] )
            {
		int temp;
		temp   = X[j];
		X[j]   = X[j+1];
		X[j+1] = temp;
	    }
}

int main() 
{
    int i;

    for( i = 0; i < 1000; i++ )
        X[i] = (i * 32767) % 10000;

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
