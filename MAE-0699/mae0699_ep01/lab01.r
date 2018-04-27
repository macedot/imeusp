geraA = function(X, Y)
{
	return(atan(Y * (2 * X - 1)));
}

geraBernoulli = function(p)
{
	u = runif(1);
	if(u < p) 
		return(1);
	return(0);
}

geraCauchy = function()
{
	repeat 
	{
		u = runif(1);
		if(u != 0.5) 
			break;
	}
	return(tan(pi * u));
}

geraExp = function(lambda)
{
	u = runif(1);
	return((-1.0 / lambda) * log(u));
}

geraGeo = function(p)
{
	u = runif(1);
	if(p >= 1)
		return(1)
	return(1 + round(log(u) / log(1 - p)));
}

GAUSSIAN_S  =  0.449871;
GAUSSIAN_T  = -0.386595;
GAUSSIAN_A  =  0.19600;
GAUSSIAN_B  =  0.25472;
GAUSSIAN_R1 =  0.27597;
GAUSSIAN_R2 =  0.27846;
GAUSSIAN_V  =  1.7156;

geraGauss = function()
{
	repeat 
	{
		u = 1 - runif(1);
		v = GAUSSIAN_V * (runif(1) - 0.5);
		x = u - GAUSSIAN_S;
		y = abs(v) - GAUSSIAN_T;
		Q = x * x + y * (GAUSSIAN_A * y - GAUSSIAN_B * x);
		if(Q < GAUSSIAN_R1) 
			break;
		if(Q <= GAUSSIAN_R2 && (v * v <= -4 * u * u * log(u))) 
			break;
	}
	return(v / u);
}

UNIF_SUPERF_PARABOLOIDE_C = (5 * sqrt(5) - 1);
UNIF_SUPERF_PARABOLOIDE_R = (2 / 3);

geraRaioUnifParaboloide = function()
{
	u = runif(1);
	r = 1 + (u * UNIF_SUPERF_PARABOLOIDE_C);
	r = (r ^ UNIF_SUPERF_PARABOLOIDE_R);
	return(0.5 * sqrt(r - 1));
}

SQRT_2  = sqrt(2);
DOIS_PI = 2 * pi;

geraY = function(t)
{
	# Resolver EDO para obter Y
	# y'' + L1 * y' + L2 * y = (E1 * t ^ 2) + (E2 * t) + E3 + Gamma(cos(t), sin(t), exp(t));
	# y'' + L1 * y' + L2 * y = (E1 * t ^ 2) + (E2 * t) + E3 + GammaX * cos(t) + GammaY * sin(t) + GammaZ * exp(t);

	L1 = geraExp(0.5);
	L2 = 2 + geraGeo(L1 / (1 + L1));
	E1 = geraGauss();
	E2 = E1 * geraGauss();
	E3_mean = (E1 ^ 2) + (E2 ^ 2);
	E3_sigma = sqrt((abs(E1) ^ 5) + (3 * (E1 ^ 2)) * (E2 ^ 4) + abs(E2) ^ SQRT_2);
	E3 = E3_mean + E3_sigma * geraGauss();
	theta = runif(1, 0, DOIS_PI);
	radius = geraRaioUnifParaboloide();
	GammaX = radius * cos(theta);
	GammaY = radius * sin(theta);
	GammaZ = radius ^ 2;

	# Condicoes Iniciais:
	# y (0) = 1;
	# y'(0) = 1;
	
	Yp = 0; # Solucao Particular;
	Yc = 0; # Solucao Complementar (caso homogeneo)

	delta = (L1 ^ 2) - (4 * L2);
	if(delta > 0)
	{
		# Yc  =	     c1 * exp(r1 * t) +      c2 * exp(r2 * t);
		# Yc' = r1 * c1 * exp(r1 * t) + r2 * c2 * exp(r2 * t);
		# Yc (0) =      c1 +      c2 = 1 => c1 = (1 - c2);
		# Yc'(0) = r1 * c1 + r2 * c2 = 1 => r1 * (1 - c2) + r2 * c2 = 1 => c2 = (1 - r1) / (r2 - r1);

		sqrtDelta = sqrt(delta);

		r1 = (-L1 + sqrtDelta) / 2;
		r2 = (-L1 - sqrtDelta) / 2;

		c2 = (1 - r1) / (r2 - r1);
		c1 = (1 - c2);

		Yc = c1 * exp(r1 * t) + c2 * exp(r2 * t);
	}
	else if(delta < 0)
	{
		# Yc  = exp(A * t) * (c1 * cos(B * t) + c2 * sin(B * t));
		# Y'c = exp(A * t) * ((A * c1 + B * c2) * cos(B * t) + (A * c2 - B * c1) * sin(B * t));
		# Yc (0) = c1          = 1;
		# Y'c(0) = A * c1 + B * c2 = 1 => c2 = (1 - A) / B;
		# Y'c    = A * c1 + B * c2 = 1;
		# A = -b / 2 * a = -b / 2;
		# B = sqrt(4 * L2 - L1^2) / 2 * a;

		A = -(L1 / 2);
		B = sqrt((4 * L2) - (L1 ^ 2)) / 2;

		# c1 = 1;
		c2 = (1 - A) / B;

		# Yc = exp(A * t) * (c1 * cos(B * t) + c2 * sin(B * t));
		Yc = exp(A * t) * (cos(B * t) + c2 * sin(B * t));
	}
	else
	{
		# Yc  =	    c1 * exp(r * t) +     c2 * t * exp(r * t);
		# Yc' =	r * c1 * exp(r * t) + r * c2 * t * exp(r * t);
		# Yc (0) =     c1 + c2  = 1 => c2 = (1 - c1);
		# Yc'(0) = r * c1       = 1 => c1 = (1 / r);

		r = (-L1 / 2);

		c1 = (1 / r);
		c2 = (1 - c1);

		Yc = c1 * exp(r * t) + c2 * t * exp(r * t);
	}

	# Solucao particular do caso nao homogeneo;
	YpA = E1 / L1;
	YpB = (E2 - 2 * L1 * YpA) / L2;
	YpC = ((GammaX * cos(t) + GammaY * sin(t) + GammaZ * exp(t)) - 2 * YpA - L1 * YpB + E3) / L2;
	Yp = YpA * (t ^ 2) + YpB * t + YpC;

	return(Yp + Yc);
}

geraFuncao = function(NumPts, NumCos, NumSen)
{
	seqX = seq(0, 1, length=NumPts);
	F = 0;
	i = 0;
	for(x in seqX)
	{
		X = geraBernoulli(0.5);
		Y = geraY(geraCauchy());
		F[i] = geraA(X, Y);
		for(k in 1:NumCos) 
		{
			a = geraA(X, Y);
			u = runif(1);
			F[i] = F[i] + (a * cos(2 * k * pi * (x + u)) / 2 ^ k);
		}
		for(k in 1:NumSen) 
		{
			a = geraA(X, Y);
			u = runif(1);
			F[i] = F[i] + (a * sin(2 * k * pi * (x^2 + u)) / 2 ^ (k + abs(a)));
		}
		i = i + 1;
	}
	return(F);
}


Lab01 = function(NumFunc, NumPts, NumCos, NumSen)
{
	seqX = seq(0, 1, length=NumPts);
	maxF = 0;
	F = 0;
	for(i in 1:NumFunc) 
	{
		F = geraFuncao(NumPts+1, NumCos, NumSen);
		maxF[i] = max(F);
		# plot(seqX, F);
	}
	plot(seqX, F);
	return(maxF);
}

NumPts  = 100;
NumCos  = 30;
NumSen  = 30;
NumFunc = 1;

#Lab01(NumFunc, NumPts, NumCos, NumSen);
#Lab01(1, 10, 10, 10);
#m = Lab01(10, NumPts, NumCos, NumSen);
#m = Lab01(1, 1000, 50, 50);
#m = Lab01(1, 10000, 50, 50);
#m = Lab01(1, 10000, 100, 100);
#m = Lab01(1, 100000, 10, 10);
