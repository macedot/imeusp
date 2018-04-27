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
D:= PontoInterseção ( c0,r )
c1:= Circunferencia ( C,D )
E:= PontoInterseção ( ,c1 )
c2:= Circunferencia ( B,E )
E:= PontoInterseção ( c0,c2 )
c3:= Circunferencia ( E,A )
G:= PontoMedio ( A,E )
PontoInterseção ( ,c4 )
H:= PontoInterseção ( c3,s )
I:= PontoInterseção ( ,c4 )
D:= PontoInterseção ( c3,c5 )
s1:= SemiReta ( E,D )
s2:= SemiReta ( A,E )
s:= Perpendicular ( ,E )
c4:= Circunferencia ( G,H )
c5:= Circunferencia ( A,I )
c6:= Circunferencia ( D,E )
t:= Perpendicular ( ,D )
K:= PontoMedio ( E,D )
PontoInterseção ( c6,t )
PontoInterseção ( ,c7 )
L:= PontoInterseção ( c6,t )
c7:= Circunferencia ( K,L )
M:= PontoInterseção ( ,c7 )
c8:= Circunferencia ( E,M )
P:= PontoInterseção ( c6,c8 )
C:= PontoInterseção ( c6,c8 )
s3:= Segmento ( C,B )
s4:= Segmento ( D,C )
s5:= SemiReta ( D,E )
s6:= Segmento ( E,A )
s7:= Segmento ( A,D )
F:= PontoInterseção ( , )
Distância_0 := ângulo(A,D,E) = 35.999°
Distância_1 := ângulo(A,E,D) = 108.000°
Distância_2 := ângulo(E,A,D) = 35.999°
Distância_3 := ângulo(A,E,F) = 71.999°
Distância_4 := ângulo(E,F,A) = 36.000°
Distância_5 := ângulo(F,A,E) = 71.999°
Texto 13 := Seja ABCDE um pentágono regular.
Texto 13 := Sabe-se portanto que AED = 108°, e AEF = EAF = 72° (externos do pentágono) ;  
Texto 13 := Notamos então que AFE = 36°, assim como EAD e EDA (triânbulo DEA é isóceles); 
Texto 13 := d 
Texto 13 := t
Texto 13 := t
Texto 13 := Ve-se que os triângulos FAD e DEA são semelhantes, portanto, segue que: 
Texto 13 := AD / DF = AE / AD  =>  d / (d + t) = t / d  =>  d^2 = t(d+t)  =>  d^2 - td - t^2 = 0
Texto 13 := Por Bhaskara, teremos:
Texto 13 := d' = t *  (1 +  sqrt(5)) / 2    ou    d" = t *  (1 -  sqrt(5)) / 2 
Texto 13 := Como sqrt(5) > 1, d" não convém, donde se obtém:
Texto 13 := d = t *  (1 +  sqrt(5)) / 2  =>  d / t = (1 +  sqrt(5)) / 2 
Texto 13 := [c.q.d] 
