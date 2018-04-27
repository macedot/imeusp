function menor = menor2(x,y),
  menor = x ;
  if (x>y) then,
    menor = y ;
  end ;
endfunction ;
 
function menorTodos = meuMenordeTodos(x,y,z,w),
  mxy = menor2(x,y) ;
  mzw = menor2(z,w) ;
  menorTodos = menor2(mxy,mzw) ;
endfunction ;

// Outra solucao

function menor = meuMenordeTodos4(x,y,z,w),
   if ((x<=y)&(x<=z)&(x<=w)) then,
     menor = x ;
   else,
     if ((y<=x)&(y<=z)&(y<=w)) then,
       menor = y ;
     else, 
       if ((z<=x)&(z<=y)&(z<=w)) then,
         menor = z ;
       else, 
         menor = w ;
       end ;
     end ;
   end ;
endfunction ;

\\ Programa principal

a = scanf(``%d'') ;
printf(``%d\n'',a) ;
b = scanf(``%d'') ;
printf(``%d\n'',b) ;
c = scanf(``%d'') ;
printf(``%d\n'',c) ;
d = scanf(``%d'') ;
printf(``%d\n'',d) ;
 
menor4 = meuMenordeTodos(a,b,c,d) ;
printf(``%d\n'',menor4) ;