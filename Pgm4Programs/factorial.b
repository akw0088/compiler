
main() returns void
begin
    int I
    do 100 I = 1, 10
    	printi(I)
        prints(" Factorial = ")
    	printi(factorial(I))
        printLine()
100 continue
end

factorial(n) returns int
    int n 
begin
    if( n == 1 )
    begin
	factorial = 1
    end
    else
    begin
	factorial = n * factorial(n-1)
    end
end
