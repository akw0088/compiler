
main() returns void
begin
    prints("The solution for 4 disks is:")
    printLine()
    hanoi(1,3,2,4)
end

hanoi(x,y,z,n) returns void
    int x;
    int y;
    int z;
    int n 
begin
    if( n == 1 )
    begin
        prints("     Move disk from ")
	printi(x)
        prints(" to ")
	printi(y)
	printLine()
    end
    else
    begin
        hanoi(x,z,y,n-1)
        prints("     Move disk from ")
	printi(x)
        prints(" to ")
	printi(z)
	printLine()
        hanoi(z,y,x,n-1)
    end
end
