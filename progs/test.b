main() returns void
begin
    int n;
    int i;
    int j;
    int temp

	n = 1000
    do 200 i = 0, n-1, 1
        do 100 j = 0, n-i-1, 1

		printi(i)
		printi(j)
		printLine()

	100 continue
    200 continue
end