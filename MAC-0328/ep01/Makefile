PROG  = debug/5124272_ep01
ENTRA = input.txt
SAI   = output.txt

CFLAGS = -Wall -ansi -pedantic -g -O2 -I.

LDLIBS = -lm
# para o caso de usar sqrt, sin, cos, etc.


default :
	@echo ""
	gcc $(CFLAGS) $(PROG).c $(LDLIBS) -o $(PROG)
	@echo ""
	@echo "-----------------------------------"
	@echo "Teste:"
	-$(PROG) <$(ENTRA) >$(SAI)
	cat $(SAI)
