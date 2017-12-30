make
echo "Compiling one.b"
./compiler < progs/one.b > progs/one.asm
cat startAssembler progs/one.asm > progs/one.s
gcc -m32 progs/one.s progs/accessories.o -g -o progs/one

echo "Compiling two.b"
./compiler < progs/two.b > progs/two.asm
cat startAssembler progs/two.asm > progs/two.s
gcc -m32 progs/two.s progs/accessories.o -g -o progs/two

echo "Compiling three.b"
./compiler < progs/three.b > progs/three.asm
cat startAssembler progs/three.asm > progs/three.s
gcc -m32 progs/three.s progs/accessories.o -g -o progs/three

echo "Compiling four.b"
./compiler < progs/four.b > progs/four.asm
cat startAssembler progs/four.asm > progs/four.s
gcc -m32 progs/four.s progs/accessories.o -g -o progs/four

echo "Compiling five.b"
./compiler < progs/five.b > progs/five.asm
cat startAssembler progs/five.asm > progs/five.s
gcc -m32 progs/five.s progs/accessories.o -g -o progs/five

echo "Compiling six.b"
./compiler < progs/six.b > progs/six.asm
cat startAssembler progs/six.asm > progs/six.s
gcc -m32 progs/six.s progs/accessories.o -g -o progs/six

echo "Compiling seven.b"
./compiler < progs/seven.b > progs/seven.asm
cat startAssembler progs/seven.asm > progs/seven.s
gcc -m32 progs/seven.s progs/accessories.o -g -o progs/seven

echo "Compiling eight.b"
./compiler < progs/eight.b > progs/eight.asm
cat startAssembler progs/eight.asm > progs/eight.s
gcc -m32 progs/eight.s progs/accessories.o -g -o progs/eight

echo "Compiling nine.b"
./compiler < progs/nine.b > progs/nine.asm
cat startAssembler progs/nine.asm > progs/nine.s
gcc -m32 progs/nine.s progs/accessories.o -g -o progs/nine

echo "Compiling ten.b"
./compiler < progs/ten.b > progs/ten.asm
cat startAssembler progs/ten.asm > progs/ten.s
gcc -m32 progs/ten.s progs/accessories.o -g -o progs/ten

echo "Compiling 8q.b"
./compiler < progs/8q.b > progs/8q.asm
cat startAssembler progs/8q.asm > progs/8q.s
gcc -m32 progs/8q.s progs/accessories.o -g -o progs/8q

echo "Compiling bubble.b"
./compiler < progs/bubble.b > progs/bubble.asm
cat startAssembler progs/bubble.asm > progs/bubble.s
gcc -m32 progs/bubble.s progs/accessories.o -g -o progs/bubble

echo "Compiling hanoi.b"
./compiler < progs/hanoi.b > progs/hanoi.asm
cat startAssembler progs/hanoi.asm > progs/hanoi.s
gcc -m32 progs/hanoi.s progs/accessories.o -g -o progs/hanoi

echo "Compiling queens.b"
./compiler < progs/queens.b > progs/queens.asm
cat startAssembler progs/queens.asm > progs/queens.s
gcc -m32 progs/queens.s progs/accessories.o -g -o progs/queens

echo "Compiling mod.b"
./compiler < progs/mod.b > progs/mod.asm
cat startAssembler progs/mod.asm > progs/mod.s
gcc -m32 progs/mod.s progs/accessories.o -g -o progs/mod


echo "Compiling multiDimension.b"
./compiler < progs/multiDimension.b > progs/multiDimension.asm
cat startAssembler progs/multiDimension.asm > progs/multiDimension.s
gcc -m32 progs/multiDimension.s progs/accessories.o -g -o progs/multiDimension

echo "Compiling factorial.b"
./compiler < progs/factorial.b > progs/factorial.asm
cat startAssembler progs/factorial.asm > progs/factorial.s
gcc -m32 progs/factorial.s progs/accessories.o -g -o progs/factorial

echo "Compiling sum.b"
./compiler < progs/sum.b > progs/sum.asm
cat startAssembler progs/sum.asm > progs/sum.s
gcc -m32 progs/sum.s progs/accessories.o -g -o progs/sum

echo "Compiling scope.b"
./compiler < progs/scope.b > progs/scope.asm
cat startAssembler progs/scope.asm > progs/scope.s
gcc -m32 progs/scope.s progs/accessories.o -g -o progs/scope
