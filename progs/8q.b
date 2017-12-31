int up[15];
int down[15];
int rows[8];
int x[8];

function void queens(int c)
{
	int r;

        for 100 (r = 0; 7; 1)
	{
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
		    {
		    	print();
                    }
		    else
		    {
		    	queens(c + 1);
                    }
		    rows[r] = 1;
		    up[r-c+7] = 1;
		    down[r+c] = 1;
		}
	} 100	end
}

function void print()
{
	int k;

	for 200 (k = 0; 7)
	{
		printc(x[k] + 49);
	} 200     end
	printLine();
}

function void main()
{
	int i;
	for 300 (i = 0; 14)
	{
		up[i] = 1;
		down[i] = 1;
	} 300	end
	for 400 (i = 0; 7)
	{
		rows[i] = 1;
	} 400	end
	queens(0);
}
