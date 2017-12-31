int up[15];
int down[15];
int rows[8];
int x[8];

function void queens()
	int i
{
	int r;
	r = 0;
        for 100 i = 0; 7; 1
	{
		    rows[r] = 0;
	} 100	continue
}

function void print()
	int k
{

	for 200 k = 0; 7
	{
		printc(x[k] + 49);
	} 200     continue
	printLine();
}

function void main()
{
	int i
	for 300 i = 0; 14
	{
		up[i] = 1;
		down[i] = 1;
	} 300	continue
	for 400 i = 0; 7
	{
		rows[i] = 1;
	} 400	continue
	queens(0);
}
