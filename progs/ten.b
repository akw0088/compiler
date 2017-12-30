main() returns void
begin
    int A[100];
    int k;
    int j;
    int i
    i = 13
    j = 7
    k = 73
    printi(i)
    printLine()

    do 100 j = 0, 99
        A[j] = j
100 continue

    if( A[k-3+i] > j && i != 0)
    begin
	printi(k)   
        printLine()  
	if( k >= i * j )
	begin
	    printi(k)
            printLine()  
	end
	else
	begin
	    printi(j)
            printLine()  
	end
    end     
end
