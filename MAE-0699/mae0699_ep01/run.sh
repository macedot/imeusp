#!/bin/bash
gcc -Ofast -msse2 -fno-strict-aliasing \
	-DHAVE_SSE2=1 -DSFMT_MEXP=19937 \
	-I./SFMT ./SFMT/SFMT.c \
	./mae0699_ep01.c -lm -o ep02.exe
rm F*MAX*
printf './ep02.exe -F1000000\n%.0s' {1..8} > run.txt
$HOME/threader < run.txt
cat F*MAX* >> MAX.TXT
rm F*MAX*
exit 0
