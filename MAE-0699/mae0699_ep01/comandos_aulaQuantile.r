# Analisando Temp e Wind do banco "airquality"

data(airquality)
attach(airquality)

##22.5 oC

hist(Temp,prob=T)
x <- seq(min(Temp), max(Temp), length=1000)
lines(x, dnorm(x, mean(Temp), sd(Temp)))
dnorm(72.5, mean(Temp), sd(Temp))
abline(v=dnorm(72.5, mean(Temp),  sd(Temp)))
m <- mean(Temp)
v <- var(Temp)


# densidade normal(m,v) no ponto 72.5
(1/sqrt(2*pi*v))*exp(-0.5*(72.5-m)^2/v)
# idem usando a funcao dnorm do R
dnorm(72.5,m,sqrt(v))


# Calculando probabilidade acumulada
pnorm(72.5,m,sqrt(v))
pnorm(70,m,sqrt(v))
pnorm(71,m,sqrt(v))


# quantis da amostra atraves da funcao quantile
quantile(Temp,0.25)
quantile(Temp,0.5)
quantile(Temp,0.75)
abline(v=72,col=2)
abline(v=79,col=2)
abline(v=85,col=2)




qnorm(0.25,m,sqrt(v))
qnorm(0.5,m,sqrt(v))
qnorm(0.75,m,sqrt(v))
abline(v=qnorm(0.25,m,sqrt(v)),col=3)
abline(v=qnorm(0.5,m,sqrt(v)),col=3)
abline(v=qnorm(0.75,m,sqrt(v)),col=3)

# medindo quantis mais extremos 
quantile(Wind,0.05)
quantile(Wind,0.10)

quantile(Temp,0.05)
quantile(Temp,0.10)

qnorm(0.05,m,sqrt(v))
qnorm(0.10,m,sqrt(v))

quantile(Temp,0.90)
quantile(Temp,0.95)

qnorm(0.90,m,sqrt(v))
qnorm(0.95,m,sqrt(v))

quantile(Temp,seq(0.01,0.99,0.01))

quantisamostrais <- quantile(Temp,seq(0.01,0.99,0.01))

quantispopulacionais <- qnorm(seq(0.01,0.99,0.01),m,sqrt(v))

plot(quantispopulacionais,quantisamostrais)

abline(0,1)

sort(Temp)

hist(Temp)

summary(Temp)



# Discretizando a variável Temp
sum(Temp<=60)
sum((Temp>60)&(Temp<=65))
contagem <- function(a,b){sum((Temp>a)&(Temp<=b))}
contagem(55,60)
contagem(60,65)
contagem(65,70)
contagem(70,75)
contagem(75,80)
contagem(80,85)
contagem(85,90)
contagem(90,95)
contagem(95,100)

obs <- c(8,10,15,19,33,34,20,12,2)

####################################################

pnorm(seq(55,100,5),m,sqrt(v))

diff(pnorm(seq(55,100,5),m,sqrt(v))*153)

# os valores acima representam o numero de observacoes esperadas
# em cada intervalo
# Agora, podemos utilizar o teste chi-quadrado que foi introduzido
# para tabelas de contingencias


esp <- c(diff(pnorm(seq(55,100,5),m,sqrt(v))*153))
esp




##################################################



cbind(obs,esp)
(obs-esp)^2/esp
(qui <- sum((obs-esp)^2/esp))


pchisq(qui, 8, lower.tail = FALSE )



par(mfrow=c(1,2))
hist(Temp,prob=T)
qqnorm(Temp)
qqline(Temp)


shapiro.test(Temp)


