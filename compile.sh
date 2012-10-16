make
echo "Compiling one.b"
./compiler < Pgm4Programs/one.b > Pgm4Programs/one.asm
cat startAssembler Pgm4Programs/one.asm > Pgm4Programs/one.s
gcc Pgm4Programs/one.s Pgm4Programs/accessories.o -g -o Pgm4Programs/one

echo "Compiling two.b"
./compiler < Pgm4Programs/two.b > Pgm4Programs/two.asm
cat startAssembler Pgm4Programs/two.asm > Pgm4Programs/two.s
gcc Pgm4Programs/two.s Pgm4Programs/accessories.o -g -o Pgm4Programs/two

echo "Compiling three.b"
./compiler < Pgm4Programs/three.b > Pgm4Programs/three.asm
cat startAssembler Pgm4Programs/three.asm > Pgm4Programs/three.s
gcc Pgm4Programs/three.s Pgm4Programs/accessories.o -g -o Pgm4Programs/three

echo "Compiling four.b"
./compiler < Pgm4Programs/four.b > Pgm4Programs/four.asm
cat startAssembler Pgm4Programs/four.asm > Pgm4Programs/four.s
gcc Pgm4Programs/four.s Pgm4Programs/accessories.o -g -o Pgm4Programs/four

echo "Compiling five.b"
./compiler < Pgm4Programs/five.b > Pgm4Programs/five.asm
cat startAssembler Pgm4Programs/five.asm > Pgm4Programs/five.s
gcc Pgm4Programs/five.s Pgm4Programs/accessories.o -g -o Pgm4Programs/five

echo "Compiling six.b"
./compiler < Pgm4Programs/six.b > Pgm4Programs/six.asm
cat startAssembler Pgm4Programs/six.asm > Pgm4Programs/six.s
gcc Pgm4Programs/six.s Pgm4Programs/accessories.o -g -o Pgm4Programs/six

echo "Compiling seven.b"
./compiler < Pgm4Programs/seven.b > Pgm4Programs/seven.asm
cat startAssembler Pgm4Programs/seven.asm > Pgm4Programs/seven.s
gcc Pgm4Programs/seven.s Pgm4Programs/accessories.o -g -o Pgm4Programs/seven

echo "Compiling eight.b"
./compiler < Pgm4Programs/eight.b > Pgm4Programs/eight.asm
cat startAssembler Pgm4Programs/eight.asm > Pgm4Programs/eight.s
gcc Pgm4Programs/eight.s Pgm4Programs/accessories.o -g -o Pgm4Programs/eight

echo "Compiling nine.b"
./compiler < Pgm4Programs/nine.b > Pgm4Programs/nine.asm
cat startAssembler Pgm4Programs/nine.asm > Pgm4Programs/nine.s
gcc Pgm4Programs/nine.s Pgm4Programs/accessories.o -g -o Pgm4Programs/nine

echo "Compiling ten.b"
./compiler < Pgm4Programs/ten.b > Pgm4Programs/ten.asm
cat startAssembler Pgm4Programs/ten.asm > Pgm4Programs/ten.s
gcc Pgm4Programs/ten.s Pgm4Programs/accessories.o -g -o Pgm4Programs/ten

echo "Compiling 8q.b"
./compiler < Pgm4Programs/8q.b > Pgm4Programs/8q.asm
cat startAssembler Pgm4Programs/8q.asm > Pgm4Programs/8q.s
gcc Pgm4Programs/8q.s Pgm4Programs/accessories.o -g -o Pgm4Programs/8q

echo "Compiling bubble.b"
./compiler < Pgm4Programs/bubble.b > Pgm4Programs/bubble.asm
cat startAssembler Pgm4Programs/bubble.asm > Pgm4Programs/bubble.s
gcc Pgm4Programs/bubble.s Pgm4Programs/accessories.o -g -o Pgm4Programs/bubble

echo "Compiling hanoi.b"
./compiler < Pgm4Programs/hanoi.b > Pgm4Programs/hanoi.asm
cat startAssembler Pgm4Programs/hanoi.asm > Pgm4Programs/hanoi.s
gcc Pgm4Programs/hanoi.s Pgm4Programs/accessories.o -g -o Pgm4Programs/hanoi

echo "Compiling queens.b"
./compiler < Pgm4Programs/queens.b > Pgm4Programs/queens.asm
cat startAssembler Pgm4Programs/queens.asm > Pgm4Programs/queens.s
gcc Pgm4Programs/queens.s Pgm4Programs/accessories.o -g -o Pgm4Programs/queens

echo "Compiling mod.b"
./compiler < Pgm4Programs/mod.b > Pgm4Programs/mod.asm
cat startAssembler Pgm4Programs/mod.asm > Pgm4Programs/mod.s
gcc Pgm4Programs/mod.s Pgm4Programs/accessories.o -g -o Pgm4Programs/mod


echo "Compiling multiDimension.b"
./compiler < Pgm4Programs/multiDimension.b > Pgm4Programs/multiDimension.asm
cat startAssembler Pgm4Programs/multiDimension.asm > Pgm4Programs/multiDimension.s
gcc Pgm4Programs/multiDimension.s Pgm4Programs/accessories.o -g -o Pgm4Programs/multiDimension

echo "Compiling factorial.b"
./compiler < Pgm4Programs/factorial.b > Pgm4Programs/factorial.asm
cat startAssembler Pgm4Programs/factorial.asm > Pgm4Programs/factorial.s
gcc Pgm4Programs/factorial.s Pgm4Programs/accessories.o -g -o Pgm4Programs/factorial

echo "Compiling sum.b"
./compiler < Pgm4Programs/sum.b > Pgm4Programs/sum.asm
cat startAssembler Pgm4Programs/sum.asm > Pgm4Programs/sum.s
gcc Pgm4Programs/sum.s Pgm4Programs/accessories.o -g -o Pgm4Programs/sum

echo "Compiling scope.b"
./compiler < Pgm4Programs/scope.b > Pgm4Programs/scope.asm
cat startAssembler Pgm4Programs/scope.asm > Pgm4Programs/scope.s
gcc Pgm4Programs/scope.s Pgm4Programs/accessories.o -g -o Pgm4Programs/scope
