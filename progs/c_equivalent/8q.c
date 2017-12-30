int up[15];
int down[15];
int rows[8];
int x[8];

void print() 
{
	int k;

	for( k = 0; k <= 7; k++ )
		printc(x[k] + 49);
	printLine();
}

void queens(int c) 
{
	int r;

        for( r = 0;  r <= 7; r++ )
	    if (   rows[r] != 0 
		      && 
		   up[r-c+7] != 0
		      && 
		   down[r+c] != 0
	       )
 
		{
		    rows[r] = 0;
		    up[r-c+7] = 0;
		    down[r+c] = 0;
		    x[c] = r;
		    if (c == 7)
		    	print();
		    else
		    	queens(c + 1);
		    rows[r] = 1;
		    up[r-c+7] = 1;
		    down[r+c] = 1;
		}
}

int main() 
{
	int i;
	for( i = 0;  i <= 14; i++ )
        {
		up[i] = 1;
		down[i] = 1;
        }
	for( i = 0;  i <= 7; i++ )
		rows[i] = 1;
	queens(0);
	return 0;
}
