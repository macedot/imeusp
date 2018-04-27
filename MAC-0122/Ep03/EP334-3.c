#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct elem{
    int digito;
    struct elem *prox;
}Elem;

Elem *criaElem(int digArq)/* Cria elementos de lista ligada*/
{
  Elem *p;
  if((p=(Elem*)malloc(sizeof (Elem)))==NULL)
  {
    printf("O cria Elem nao foi alocado\n");
    /*system("pause");*/
    return (NULL);
  }
  
  p->digito=digArq;
  p->prox=NULL;
  return(p);
}

Elem *leArqLL(char *nomeArq)
{
  FILE *fp;
  Elem *inicio,*atual;

  int digitoi = 0;
  int digitoc = 0;

  if(( fp = fopen(nomeArq,"r") ) == NULL)
  {
    printf("O arquivo nao pode ser aberto\n");
    /*system("pause");*/
    return (NULL);
  }
  while ( !feof(fp))
  {
    digitoc=fgetc(fp);
    if (isdigit(digitoc))
    {
      break; 
    }
  }
  if (feof(fp))
  {
    fclose(fp);
    inicio=criaElem(0);
    return inicio;
  }
  
  digitoi=digitoc - '0';
  inicio=criaElem(digitoi);       
  atual=inicio;
  
  while (!feof(fp))
  {
    digitoc=fgetc(fp);
    if (isdigit(digitoc))
    {
      digitoi=digitoc - '0';
      atual->prox=criaElem(digitoi);
      atual=atual->prox;
    }
  }
  return inicio;
}


Elem *invertelista(Elem *pinicio)
{
  Elem *atual,*prox1,*ant;

  if(pinicio==NULL)
  {
    return NULL;
  }

  if(pinicio->prox==NULL)
  {
    return (pinicio);
  }

  atual=pinicio->prox;
  prox1=pinicio;
  ant=atual->prox;
  atual->prox=prox1;
  prox1->prox=NULL;

  while(ant!=NULL)
  {
    prox1=atual;
    atual=ant;
    ant=ant->prox;
    atual->prox=prox1;
  }
  return(atual);
}

Elem *copiaLista(Elem *lista)
{ 
  Elem *listaAtu = lista;
  Elem *res;
  Elem *resAtu;
    
  listaAtu = lista;
  res = resAtu = NULL;
  if((res=criaElem(listaAtu->digito))!=NULL)
  {
    listaAtu=listaAtu->prox;
    resAtu=res;
    while(listaAtu!=NULL)
    {
      resAtu->prox = criaElem(listaAtu->digito);
      listaAtu = listaAtu->prox;
      resAtu = resAtu->prox;
    }
  }
  return res;
} 

void freeLista(Elem *lista)
{
  Elem *atual,*aux;

  atual=lista;
  while(atual!=NULL)
  {
    aux=atual->prox;
    free(atual);
    atual=aux;
  }
}

Elem *soma(Elem *n1,Elem *n2)
{
  Elem *res,*resAtu,*atualn1,*atualn2;
  int soma = 0;
  int vaiUm = 0;
  int aux = 0;
  int aux1 = 0;

  atualn1=n1;
  atualn2=n2;
  vaiUm=0;
  soma=0;
  aux=0;
  aux1=0;
  if(atualn1!=NULL && atualn2!=NULL)/* Ambos sao diferentes de NULL*/
  {
    soma = atualn1->digito + atualn2->digito;
    if(soma>9)
    {  
      soma=soma%10;  
      vaiUm=1;
    } 
    res=criaElem(soma);
    resAtu=res;
    atualn1=atualn1->prox;
    atualn2=atualn2->prox;
    while(atualn1!=NULL && atualn2!=NULL)
    {         
      soma = atualn1->digito + atualn2->digito + vaiUm;
      if(soma>9)
      {  
        soma=soma%10;  
        vaiUm=1;
      } 
      else
      {
        vaiUm = 0;
      }
      resAtu->prox=criaElem(soma);
      resAtu=resAtu->prox;
      atualn1=atualn1->prox;
      atualn2=atualn2->prox;  
    }
    if (vaiUm!=0)
    {
      resAtu->prox=criaElem(vaiUm);
      resAtu=resAtu->prox;
    }
    if(atualn1!=NULL && vaiUm!=0 && atualn2==NULL)/* Quando o segundo num tem menos elementos q o primeiro*/
    {
      while(atualn1!=NULL || vaiUm==1)
      {
        soma = vaiUm  + atualn1->digito;             
        if (soma>9)
        {
          resAtu->digito=soma%10;
          resAtu->prox=criaElem(1);
          resAtu=resAtu->prox;
          if((atualn1=atualn1->prox)==NULL) return res;
        }
        else
        {
          resAtu->digito=soma;
          if((atualn1=atualn1->prox)==NULL) return res;
          else
          {
            resAtu->prox=criaElem(atualn1->digito);
            resAtu=resAtu->prox;
          }
          vaiUm=0;
        }
      }
      if(atualn1==NULL) return res;
      else
      {
        if(atualn1!=NULL && vaiUm ==0)
        {
          while(atualn1!=NULL);
          {
            resAtu->prox=criaElem(atualn1->digito);
            resAtu=resAtu->prox;
            atualn1=atualn1->prox;
          }
        }
      }
      if(atualn1==NULL) return res;    
    }
    else
    {
      if(atualn1!=NULL && vaiUm ==0 && atualn2==NULL)
      {
        while(atualn1!=NULL)
        {
          resAtu->prox=criaElem(atualn1->digito);
          resAtu=resAtu->prox;
          if ((atualn1=atualn1->prox)==NULL) return res;
        }
      } 
    }    
    if(atualn2!=NULL && vaiUm!=0 && atualn1==NULL)/*quando o primeiro num tem monos elementos q o segundo*/ 
    {
      while(atualn2!=NULL || vaiUm==1)
      {
        soma = vaiUm + atualn2->digito;
        if (soma>9)
        {
          resAtu->digito=soma%10;
          resAtu->prox=criaElem(1);
          resAtu=resAtu->prox;
          if((atualn2=atualn2->prox)==NULL) return res; 
        }
        else
        { 
          resAtu->digito=soma;
          if((atualn2=atualn2->prox)==NULL) return res;
          else
          {
            resAtu->prox=criaElem(atualn2->digito);
            resAtu=resAtu->prox;
          }
          vaiUm=0;
        }
      }
    }
    else
    { 
      if(atualn2!=NULL && vaiUm ==0 && atualn1==NULL)
      { 
        while(atualn2!=NULL)
        { 
          resAtu->prox=criaElem(atualn2->digito);
          resAtu=resAtu->prox;
          if ((atualn2->prox)==NULL) return res;
          else atualn2=atualn2->prox;
        }
      }
    }
  }
  else
  {   
    if(atualn1==NULL && atualn2!=NULL)
    {         
      res=copiaLista(n2);
    }
    else
    {
      if(atualn1!=NULL && atualn2==NULL)
      {  
        res=copiaLista(n1);
      }
      else
      {
        printf("Algo errado com os arq nao tem nada neles\n");
        return NULL;
      }
    }        
  }      
  return res;               
}

Elem *mult(Elem *n1, Elem *n2 )
{
  Elem *atualn1;
  Elem *atualn2;
  Elem *res;
  Elem *resAtu;
  Elem *resAux;
  Elem *resAtuini;
  Elem *auxn1;
  int mult = 0;
  int sobe = 0;
  int c = 0;
  int i = 0;
  
  atualn1=n1;
  auxn1=n1;
  atualn2=n2;
  res=NULL;
  if(atualn1!=NULL && atualn2!=NULL)
  {  
    mult=(atualn1->digito * atualn2->digito);
    if(mult>9)
    { 
      sobe=mult/10;       
      resAtuini=criaElem(mult%10);
    }
    else
    {
      sobe=0;
      resAtuini=criaElem(mult);
    }
    resAtu=resAtuini;
    atualn1=atualn1->prox;
    while(atualn2!=NULL)
    { 
      while(atualn1!=NULL)
      { 
        mult=(atualn1->digito * atualn2->digito) + sobe ;
        if(mult>9)
        { 
          sobe=mult/10;       
          resAtu->prox=criaElem(mult%10);
        }
        else
        {
          sobe=0;
          resAtu->prox=criaElem(mult);
        }
        resAtu=resAtu->prox;
        atualn1=atualn1->prox;
      }
      if(sobe!=0)
      { 
        resAtu->prox=criaElem(sobe);
        resAtu=resAtu->prox;
        sobe=0;
      }
      atualn1=auxn1;
      atualn2=atualn2->prox;
      c++;
      resAux=res;
      res=soma(res,resAtuini);
      freeLista(resAtuini);
      freeLista(resAux);

      resAtu=resAtuini=criaElem(0);
      for(i=1; i<c; i++)
      {
        resAtu->prox=criaElem(0);
        resAtu=resAtu->prox;
      }
    } 
  } 
  return res;
}         

void salvanoArq( Elem *res, char *nomeArq)/*funcao q grava a lista em um arquivo*/
{
  FILE *fp;
  Elem *aux;

  if((fp=fopen(nomeArq,"w")) == NULL)
  {
    printf("O arquivo nao pode ser aberto\n");
    /*system("pause");*/
    return;
  }
  aux=res;   
  while (aux!=NULL)
  {
    fprintf(fp,"%d",aux->digito);
    aux=aux->prox;
  }
  fclose(fp);
}
    
int main(int argc, char* argv[])
{
  char num1[61],num2[61],saida[61];
  Elem *Ln1,*Ln2,*resp,*respo;
  int met=0,i=0;

  if (argc != 9) 
  { /* Verificacao simples, a chamada deve ter 9 argumentos */
    printf("\n\nUse o programa como no exemplo abaixo:\n") ;
    printf("\n\ncalculadora -m <Operacao> -a <arquivoA> -b <arquivoB> -o <resultado.txt>\n") ;
    exit(1) ;
  }
  /* Trata os parametros */
  for (i=0; i<9; i++) {
    if(argv[i][0] == '-') {
      switch(argv[i][1]) {
        case 'm': met= atoi (argv[i+1]);   break ;
        case 'a': strcpy(num1,argv[i+1]);  break ;
        case 'b': strcpy(num2,argv[i+1]);  break ;
        case 'o': strcpy(saida,argv[i+1]); break ;
        default :
          printf("\nOpcao invalida.\n");
          printf("\nUse o programa como no exemplo abaixo:\n") ;
          printf("\n\ncalculadora -m <Operacao> -a <arquivoA> -b <arquivoB> -o <resultado.txt>\n") ;
          /*system("pause");*/
          exit(21);
      }
    }
  }
  Ln1=leArqLL(num1);
  Ln2=leArqLL(num2);
  if ((Ln1=invertelista(Ln1))==NULL)
  {
    printf("Nao foi possivel inverter 1\n");
    return 0;
  }
  if ((Ln2=invertelista(Ln2))==NULL)
  {
    printf("Nao foi possivel inverter 2\n");
    return 0;
  } 
  if (met==1)
  { 
    respo=soma(Ln1,Ln2);
  }
  else
  { 
    respo=mult(Ln1,Ln2);
  }
  resp=invertelista(respo);
  salvanoArq(resp,saida);
  while(resp!=NULL)
  {
    printf("%d",resp->digito);
    resp=resp->prox;
  }
  freeLista(respo);
  /*system("pause");*/
  return 0;
}
    
      
                              
    
    
             
             
            
            
    
    
    
    
