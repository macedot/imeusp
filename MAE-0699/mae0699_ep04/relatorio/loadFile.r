#e=read.table("elem_1_10838828.txt");
#e=read.table("elem_2_10838828.txt");
e=read.table("elem_1_24318671.txt");
n1=0;
n2=0;
x1=0;
x2=0;
i=0;
for(i in 1:length(e$V1))
{
	if(e$V1[i] == 0)
	{
		x1[n1] = e$V2[i];
		n1 = n1 + 1;
	}
	else
	{
		x2[n2] = e$V2[i];
		n2 = n2 + 1;
	}
}
