for n in $(seq 1 9); do
	echo "Regiao $n"
	./ep02 -P1000000 -R$n -V
done
exit 0
