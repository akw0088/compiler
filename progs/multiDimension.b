int A[10][20][30]


function void main()
{
    int i;
    int j;
    int k;
    for 300 (i = 0; 9)
    {
    	for 200 (j = 0; 19)
	{
    	    for 100 (k = 0; 29)
	    {
		A[i][j][k] = i * j * k;
	    } 100 end    
	} 200 end 
    } 300 end

   


    printi(A[3][2][4]);
    printLine();

}
