k=1; w=3;
deff("[xdot] = derpol(t,x)",..
        ["xd1 = x(2)";..
         "xd2 = -k*x(1) + w*(1 - x(1)**2)*x(2)";..
         "xdot = [ xd1 ; xd2 ]"])
xf=-1:0.1:1;
yf=-1:0.1:1;
clf(); 
fchamp(derpol,0,xf,yf);
//fchamp(derpol,0,xf,yf,1,[0,0,10,10],"011")

