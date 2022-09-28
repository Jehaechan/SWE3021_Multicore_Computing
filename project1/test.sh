#!/bin/sh

FILE="$1.txt"

if [ $# -lt 1 ]; then
	echo "Usage: ./test.sh <filename>"
	exit 1
fi

if [ -e $FILE ]; then
	echo "File Already Exists."
	exit 1
fi

make

touch $FILE
input="input1.txt"

nl='\n'
str="${nl}<${input}>${nl}"
echo $str >> $FILE

str="Thread num: 1"
echo $str >> $FILE
./a.out $input 1 >> $FILE

str="Thread num: 2"
echo $str >> $FILE
./a.out $input 2 >> $FILE

str="Thread num: 4"
echo $str >> $FILE
./a.out $input 4 >> $FILE

str="Thread num: 8"
echo $str >> $FILE
./a.out $input 8 >> $FILE

str="Thread num: 16"
echo $str >> $FILE
./a.out $input 16 >> $FILE

str="Thread num: 32"
echo $str >> $FILE
./a.out $input 32 >> $FILE

str="Thread num: 40"
echo $str >> $FILE
./a.out $input 40 >> $FILE


input="input2.txt"

nl='\n'
str="${nl}<${input}>${nl}"
echo $str >> $FILE

str="Thread num: 1"
echo $str >> $FILE
./a.out $input 1 >> $FILE

str="Thread num: 2"
echo $str >> $FILE
./a.out $input 2 >> $FILE

str="Thread num: 4"
echo $str >> $FILE
./a.out $input 4 >> $FILE

str="Thread num: 8"
echo $str >> $FILE
./a.out $input 8 >> $FILE

str="Thread num: 16"
echo $str >> $FILE
./a.out $input 16 >> $FILE

str="Thread num: 32"
echo $str >> $FILE
./a.out $input 32 >> $FILE

str="Thread num: 40"
echo $str >> $FILE
./a.out $input 40 >> $FILE


input="input3.txt"

nl='\n'
str="${nl}<${input}>${nl}"
echo $str >> $FILE

str="Thread num: 1"
echo $str >> $FILE
./a.out $input 1 >> $FILE

str="Thread num: 2"
echo $str >> $FILE
./a.out $input 2 >> $FILE

str="Thread num: 4"
echo $str >> $FILE
./a.out $input 4 >> $FILE

str="Thread num: 8"
echo $str >> $FILE
./a.out $input 8 >> $FILE

str="Thread num: 16"
echo $str >> $FILE
./a.out $input 16 >> $FILE

str="Thread num: 32"
echo $str >> $FILE
./a.out $input 32 >> $FILE

str="Thread num: 40"
echo $str >> $FILE
./a.out $input 40 >> $FILE


make clean
