
main() returns void
{
    prints("The solution for 4 disks is:")
    printLine()
    hanoi(1,3,2,4)
}

hanoi(x,y,z,n) returns void
    int x;
    int y;
    int z;
    int n 
{
    if( n == 1 )
    {
        prints("     Move disk from ")
	printi(x)
        prints(" to ")
	printi(y)
	printLine()
    }
    else
    {
        hanoi(x,z,y,n-1)
        prints("     Move disk from ")
	printi(x)
        prints(" to ")
	printi(z)
	printLine()
        hanoi(z,y,x,n-1)
    }
}
