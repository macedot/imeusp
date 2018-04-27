u1 = 0.0;   # Variável aleatória com Distribuição Uniforme(0,1)
u2 = 0.0;	
md = 0.0;	# Média
sd = 0.0;	# Desvio Padrão
e1 = 0.0;	# Variável aleatória com Distribuição Normal(0,1)	
e2 = 0.0;	# Variável aleatória com Distribuição Normal(0,e1^2)
e3 = 0.0;	# Variável aleatória com Distribuição 
		# Normal((e1^2) + (e2^2),abs(e1^5)+(3*(e1^2)*(e2^4))+(abs(e2)^sqrt(2)))
g1 = 0.0;	# Variável aleatória com Distribuição Exponencial(0.5)
g2 = 0.0;	# Variável aleatória com Distribuição (2+Geo(g1/(1+g1))
x = 0;	# Variável aleatória com Distribuição Bernoulli(0.5)
z = 0.0;	# Variável aleatória com Distribuição de Cauchy;
		
a = 0.0;	# Coordenadas a, b e c do vetor aleatório com 
b = 0.0;	# Distribuição Uniforme(0,1)
c = 0.0;

r = 0.0;	# Raio da Curva de Nível, onde o vetor aleatório é gerado
w = 0.0;	# Ângulo entre Raio e o eixo das abcissas, onde o vetor aleatório é gerado

a1 = 0.0;	# Coeficientes usados na solução da Equação DIferencial
b1 = 0.0;	# pelo Método dos Coeficientes Indeterminados.
c1 = 0.0;

Y1 = 0.0;	# Solução particular da Equação Diferencial
r1 = 0.0 	# Raízes da Equação Característica
r2 = 0.0
c1 = 0.0 	# Constantes da Solução Geral da Equação Diferencial 
c2 = 0.0
y = 0.0;	# Solução da Equação Diferencial
t = 0.0;	# Tempo


# Bernoulli x #
u1 = runif(1);
if(u1 <= 0.5){ 
	x = 1; 
} else { 
	x = 0; 
}

# Duas Normais e1, e2 #
u1 = runif(1);
u2 = runif(1);
e1 = sqrt(-2*log(u1)) * cos(2*pi*u2);
e2 = sqrt(-2*log(u1)) * sin(2*pi*u2);
sd = e1^2;
e2 = md + e2*sd;

# Normal e3 #
u1 = runif(1);
u2 = runif(1);
e3 = sqrt(-2*log(u1)) * cos(2*pi*u2);
md = (e1^2) + (e2^2);
sd = abs(e1^5) + (3*(e1^2) * (e2^4)) + (abs(e2) ^ sqrt(2));
e3 = md + e3*sd;

# Exponencial g1 #
u1 = runif(1);
g1 = -0.5*log(1 - u1);

# Geométrica g2 #
u1 = runif(1);
g2 = 2 + (log(u1)/log(1-(g1/(1+g1))));

# Cauchy z #
u1 = runif(1);
z = tan(pi*(u1-0.5));

# Vetor alatório Uniforme no Parabolóide #
u1 = runif(1);
u2 = runif(1);
w = 2*pi*u1;
r = 0.5*sqrt(sqrt(((u2*(5*sqrt(5)-1))+1)^3)-1);
a = r*cos(w);
b = r*sin(w);
c = (a^2)+(b^2);

t = z;

# Solução Particular da Equação Diferencial #
a1 = e1/g1;
b1 = (e2 - 2*g1*a1)/g2;
c1 = ((a*cos(t) + b*sin(t) + c*exp(t)) - 2*a1 - g1*b1 + e3)/g2;
Y1 = a1*(t^2) + b1*t + c1;

# Soluções Gerais da Equação Diferencial #
if ( ((g1^2) - 4*g2) == 0 ){
	r1 = -g1/2;
	c1 = 1 - r1;
	c2 = 1;
	y = c1*t*exp(r1*t) + c2*exp(r1*t) + Y1;
} else {
	if( ((g1^2) - 4*g2) > 0 ){
		r1 = (-g1 + sqrt((g1^2) - 4*g2))/2;
		r2 = (-g1 - sqrt((g1^2) - 4*g2))/2;
		c2 = (1 - r1)/(r2-r1);
		c1 = 1 - c2;
		y = c1*exp(r1*t) + c2*exp(r2*t) + Y1;
	} else { 
		c1 = 1;
		c2 = (1 + (g1/2)) * (2/sqrt(abs((g1^2)- 4*g2)));
		y = c1*exp((-g1/2)*t)*cos( ( sqrt( abs((g1^2) - 4*g2) )/2 )*t ) + c2*exp((-g1/2)*t)*sin( ( sqrt( abs((g1^2) - 4*g2) )/2 )*t ) + Y1;
	}
}

A = atan((2*x - 1)*y);





# Testes #
for(i in 1:5000) {
	u1 = runif(1);
	if(u1 <= 0.5){ 
		x[i] = 1; 
	} else { 
		x[i] = 0; 
	}	

	u1 = runif(1);
	u2 = runif(1);
	e1[i] = sqrt(-2*log(u1)) * cos(2*pi*u2);
	e2[i] = sqrt(-2*log(u1)) * sin(2*pi*u2);
	sd[i] = e1[i]^2;
	e2[i] = e2[i]*sd[i];

	u1 = runif(1);
	u2 = runif(1);
	e3[i] = sqrt(-2*log(u1)) * cos(2*pi*u2);
	md[i] = (e1[i]^2) + (e2[i]^2);
	sd[i] = abs(e1[i]^5)+(3*(e1[i]^2)*(e2[i]^4))+(abs(e2[i])^sqrt(2));
	e3[i] = md[i] + e3[i]*sd[i];

	u1 = runif(1);
	g1[i] = -0.5*log(1 - u1);

	u1 = runif(1);
	g2[i] = 2 + (log(u1)/log(1-(g1[i]/(1+g1[i]))));

	u1 = runif(1);
	z[i] = tan(pi*(u1-0.5));

	u1 = runif(1);
	u2 = runif(1);
	w[i] = 2*pi*u1;
	r[i] = 0.5*sqrt(sqrt(((u2*(5*sqrt(5)-1))+1)^3)-1);
	a[i] = r[i]*cos(w[i]);
	b[i] = r[i]*sin(w[i]);
	c[i] = (a[i]^2)+(b[i]^2);

}