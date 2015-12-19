#!/bin/bash
for i in `seq 1 9`;
do
         convert -flatten $i.png Sudoku-build/chiffres/$i.png
done    
