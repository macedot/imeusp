-----------------------------
iGeom - msgBSupIGeom - 3.2.11
http://www.matematica.br/igeom
msgCrtAlgCabecalho
-----------------------------
B:= Ponto ( 49.0,-358.0 )
A:= Ponto ( 167.0,-358.0 )
s0:= SemiReta ( B,A )
c0:= Circunferencia ( A,B )
r:= Perpendicular ( ,A )
C:= PontoMedio ( A,B )
D:= PontoInterse��o ( c0,r )
c1:= Circunferencia ( C,D )
E:= PontoInterse��o ( ,c1 )
c2:= Circunferencia ( B,E )
E:= PontoInterse��o ( c0,c2 )
c3:= Circunferencia ( E,A )
G:= PontoMedio ( A,E )
PontoInterse��o ( ,c4 )
H:= PontoInterse��o ( c3,s )
I:= PontoInterse��o ( ,c4 )
D:= PontoInterse��o ( c3,c5 )
s1:= SemiReta ( E,D )
s2:= SemiReta ( A,E )
s:= Perpendicular ( ,E )
c4:= Circunferencia ( G,H )
c5:= Circunferencia ( A,I )
c6:= Circunferencia ( D,E )
t:= Perpendicular ( ,D )
K:= PontoMedio ( E,D )
PontoInterse��o ( c6,t )
PontoInterse��o ( ,c7 )
L:= PontoInterse��o ( c6,t )
c7:= Circunferencia ( K,L )
M:= PontoInterse��o ( ,c7 )
c8:= Circunferencia ( E,M )
P:= PontoInterse��o ( c6,c8 )
C:= PontoInterse��o ( c6,c8 )
s3:= Segmento ( C,B )
s4:= Segmento ( D,C )
s5:= SemiReta ( D,E )
s6:= Segmento ( E,A )
s7:= Segmento ( A,D )
F:= PontoInterse��o ( , )
Dist�ncia_0 := �ngulo(A,D,E) = 35.999�
Dist�ncia_1 := �ngulo(A,E,D) = 108.000�
Dist�ncia_2 := �ngulo(E,A,D) = 35.999�
Dist�ncia_3 := �ngulo(A,E,F) = 71.999�
Dist�ncia_4 := �ngulo(E,F,A) = 36.000�
Dist�ncia_5 := �ngulo(F,A,E) = 71.999�
Texto 13 := Seja ABCDE um pent�gono regular.
Texto 13 := Sabe-se portanto que AED = 108�, e AEF = EAF = 72� (externos do pent�gono) ;  
Texto 13 := Notamos ent�o que AFE = 36�, assim como EAD e EDA (tri�nbulo DEA � is�celes); 
Texto 13 := d 
Texto 13 := t
Texto 13 := t
Texto 13 := Ve-se que os tri�ngulos FAD e DEA s�o semelhantes, portanto, segue que: 
Texto 13 := AD / DF = AE / AD  =>  d / (d + t) = t / d  =>  d^2 = t(d+t)  =>  d^2 - td - t^2 = 0
Texto 13 := Por Bhaskara, teremos:
Texto 13 := d' = t *  (1 +  sqrt(5)) / 2    ou    d" = t *  (1 -  sqrt(5)) / 2 
Texto 13 := Como sqrt(5) > 1, d" n�o conv�m, donde se obt�m:
Texto 13 := d = t *  (1 +  sqrt(5)) / 2  =>  d / t = (1 +  sqrt(5)) / 2 
Texto 13 := [c.q.d] 
