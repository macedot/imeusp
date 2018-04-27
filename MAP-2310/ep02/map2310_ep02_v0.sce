clear

function ret = FuncaoY(t)
  ret = exp(t_0 - t);
endfunction;

t_0 = 5;
t_m = 11;
y_0 = 1;
t = [];
y = [];

h = 1 / 2^5;
m = (t_m - t_0) / h;
disp(m, "m = ");
t = [t_0];
y = [y_0];
for(i = 1:m)
  y(i+1) = FuncaoY(t(i));
  t(i+1) = t(i) + h;
end;

clf();
plot(t,y);

