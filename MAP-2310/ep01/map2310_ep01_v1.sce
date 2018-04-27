clear
clf();
printf("\n==============================\n");

//
// A Solucao Geral para o problema de Cauchy:
// /
// | y'(x) = x^2 / y(x)^2
// | y(a) = k
// \
//
// E' dada por:
// y(x) = (x^3 + k^3 - a^3) ^ (1/3)
//

function y = CubicRoot(x)
  y = [];
  i = 1;
  for(s = x)
    if(s > 0)
      y(i) = s^(1/3);
    else
      y(i) = (-1)*(abs(s)^(1/3));
    end;
    i = i + 1;
  end;
endfunction;

function y = f(x, c)
  y = CubicRoot(x^3 + c);
endfunction;

N  = 20;
dx = 6/N;
x  = -3:1/100:3;

for(k  = [1 -1])
  for(a = -3:dx:3)
    c = k^3 - a^3;
    y = [];
    j = 1;
    for(s = x)
      y(j) = f(s, c);
      j = j + 1;
    end;
    plot(x,y);
  end;
end;

