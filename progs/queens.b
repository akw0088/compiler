int up[15];
int down[15];
int rows[8];
int x[8]

queens() returns void
	int i
begin
	int r;
	r = 0
        do 100 i = 0, 7, 1
		    rows[r] = 0
100	continue
end

print() returns void
	int k
begin

	do 200 k = 0, 7
		printc(x[k] + 49)
200     continue
	printLine()
end

main() returns void
begin
	int i
	do 300 i = 0, 14
		up[i] = 1
		down[i] = 1
300	continue
	do 400 i = 0, 7
		rows[i] = 1
400	continue
	queens(0)
end