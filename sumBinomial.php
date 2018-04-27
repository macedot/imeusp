<?php
function Factorial($n) { 
  $fact = 1;
  for($i=2; $i<=$n; $i++) $fact *= $i;
  return($fact); 
}

function Combination1($n,$p) { 
  $comb = Factorial($n) / (Factorial($p) * Factorial($n-$p));
  return($comb); 
}

function Combination2($n,$p) { 
  $fN_P         = $n-$p;
  $fFatorialN   = Factorial($n);
  $fFatorialP   = Factorial($p);
  $fFatorialN_P = Factorial($fN_P);
  $comb = $fFatorialN / ($fFatorialP * $fFatorialN_P);
  return($comb); 
}

function Binomial($n,$p,$x) {
  $binomial = Combination1($n,$x) * pow($p,$x) * pow((1-$p),($n-$x));
  return($binomial);
}

function SomaBinomial($n,$inicio,$fim,$prob) { 
  $soma = 0.0;
  $fOneLessProb = (1 - $prob);
  for($x = $inicio; $x<=$fim; $x++) {
    $soma +=  Binomial($n,$prob,$x);
  }
  return($soma); 
}

/*
$n = floatval($_REQUEST['n']);
$p =  floatval($_REQUEST['p']);
$fIni = floatval($_REQUEST['ini']);
$fFim = floatval($_REQUEST['fim']);
*/
/*
$n = 100;
$p =  0.9082;
$fIni = 0;
$fFim = 19;

$fTime = time();
for($i=0; $i<100; $i++) $fSomaBinomial = SomaBinomial($n,$fIni,$fFim,$p);
$fTime = time()-$fTime;
echo("($fTime) SomaBinomial($n,$fIni,$fFim,$p) = \n");
var_dump($fSomaBinomial);
unset($fSomaBinomial);
*/

$n=10;
$p=0.5;
$x=3;

$binomial = Combination1($n,$x) * pow($prob,$x) * pow((1-$p),($n-$x));
echo("X ~ b(n=$n,p=$p)\n");
echo("P(X=$x) = "); var_dump($binomial);

/*
$arBinomial = array();
$arBinomial[0] = SomaBinomial(100,20,100,0.9082);
$arBinomial[1] = SomaBinomial(100, 0, 19,0.9082);
var_dump($fSomaBinomial);
unset($arBinomial);
*/
?>