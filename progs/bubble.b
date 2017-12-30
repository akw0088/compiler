int X[1000]

main() returns void
{
    int i

    do 10 i = 0, 999
        X[i] = (i * 32767) % 10000
10  continue

	prints("Some of the original values are:\n")
        printi(X[111])
	prints(" ")
        printi(X[222])
	prints(" ")
        printi(X[333])
	prints(" ")
        printi(X[666])
	prints(" ")
        printi(X[777])
	prints(" ")
        printi(X[999])
        printLine()

    printLine()
    printLine()
    printLine()

    sort(1000)
    
	prints("Some of the sorted values, in order are:\n")
        printi(X[111])
	prints(" ")
        printi(X[222])
	prints(" ")
        printi(X[333])
	prints(" ")
        printi(X[666])
	prints(" ")
        printi(X[777])
	prints(" ")
        printi(X[999])
        printLine()
}


sort(n) returns void
    int n
{
    int i;
    int j
    do 200 i = 0, n-1, 1
        do 100 j = 0, n-i-1, 1
	    if( X[j] > X[j+1] )
            {
		int temp
		temp   = X[j]
		X[j]   = X[j+1]
		X[j+1] = temp
	    }
	100 continue
    200 continue
}
