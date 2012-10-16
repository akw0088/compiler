int A[10][20][30]


main() returns void
begin
    int i;
    int j;
    int k
    do 300 i = 0, 9
    	do 200 j = 0, 19
    	    do 100 k = 0, 29
		A[i][j][k] = i * j * k
100 continue    
200 continue    
300 continue

    printi(A[3][2][4])
    printLine() 

end
