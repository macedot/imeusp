function intermediario = meuInter(x,y,z),

  n1 = x*y ;
  n2 = x*z ;
  n3 = y*z ;

  if (n1>n2) then,
    tmp = n1 ;
    n1 = n2 ; 
    n2 = tmp ;
  end ;
  
  if (n1>n3) then,
    tmp = n3 ;
    n3 = n1 ;
    n1 = tmp ;
  end ;

  if (n2>n3) then,
    tmp = n2 ;
    n2 = n3 ;
    n3 = tmp ;
  end ;

  intermediario = n2 ;

endfunction ;


function intermediario = meuInter2(x,y,z),

  n1 = x*y ;
  n2 = x*z ;
  n3 = y*z ;

  if (n1<=n2) then,
    if (n2<=n3)then,
      intermediario = n2 ;
    else,
      if (n1<=n3) then,
	intermediario = n3 ;
      else,
        intermediario = n1 ;
      end ;
    end ;
  else,
    if (n1<=n3) then,
      intermediario = n1 ;
    else,
      if (n2<=n3) then,
        intermediario = n3 ;
      else,
        intermediario = n2 ;
      end ;
    end ;
  end ;
    
endfunction ;