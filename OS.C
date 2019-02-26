#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
FILE *fp1,*fp2;
struct pcb
{
	int JID;
	int TTL;//total no of instr.
	int TLL;//total no of PD instr.
}PCB;

char M[300][4],IR[4],R[4],opcode[2],input[20],output[20];
int IC,SI,C,PTR,XPTR,RA,VA,operand,PI,TI,TTC,LLC,arr[30];

void INIT();
int ALLOCATE();
int ADDRESSMAP(int);
void LOAD();
void STARTEXECUTION();
void EXECUTEUSERPROGRAM();
void MOS();
void READ();
void WRITE();
void TERMINATE(int);

void main()
{
	clrscr();
	printf("\nEnter Input File Name:");
	scanf("%s",input);
	printf("\nEnter Output File Name:");
	scanf("%s",output);
	fp1=fopen(input,"r");
	fp2=fopen(output,"w");
	if(fp1==NULL||fp2==NULL)
	{
		printf("\nFile Opening Error...!");
		getch();
		exit(0);
	}
	LOAD();
}

void INIT()
{
	int i,j;
	for(i=0;i<=299;i++)
	{
		for(j=0;j<4;j++)
		{
			M[i][j]='\0';
		}
	}
	for(i=0;i<4;i++)
	{
		IR[i]='\0';
	}
	for(i=0;i<4;i++)
	{
		R[i]='\0';
	}
	IC=0;
	SI=0;
	PI=0;
	TI=0;
	C=0;
	PTR=0;
	XPTR=0;
	RA=0;
	VA=0;
	TTC=0;
	LLC=0;
	PCB.JID=0;
	PCB.TTL=0;
	PCB.TLL=0;
	for(i=0;i<30;i++)
		arr[i]=0;
}

int ALLOCATE()
{
	int i;
	i=rand()%30;
	while(arr[i]!=0)
	{
		i=rand()%30;
	}
	arr[i]=1;
	return(i);
}
void LOAD()
{
	int i,j,f=0;
	char line[40],buff[4],*l;
	FILE *temp;
	while(temp=(fgets(line,42,fp1))!=NULL)
	{
		l=line;
		for(i=0;i<=3;i++)
		{
			buff[i]=*l;
			l++;
		}
		if(buff[0]=='$'&&buff[1]=='A'&&buff[2]=='M'&&buff[3]=='J')
		{
			INIT();
			for(i=0;i<3;i++)
			{
				for(j=0;j<=3;j++)
				{
					buff[j]=*l;
					l++;
				}
				switch(i)
				{
					case 0:
						PCB.JID=((buff[3]-48)+(buff[2]-48)*10+(buff[1]-48)*100+(buff[0]-48)*1000);
						break;
					case 1:
						PCB.TTL=((buff[3]-48)+(buff[2]-48)*10+(buff[1]-48)*100+(buff[0]-48)*1000);
						break;
					case 2:
						PCB.TLL=((buff[3]-48)+(buff[2]-48)*10+(buff[1]-48)*100+(buff[0]-48)*1000);
				}
			}
			PTR=ALLOCATE()*10;
			for(i=PTR;i<(PTR+9);i++)
			{
				for(j=0;j<=3;j++)
				{
					M[i][j]='*';
				}
			}
			XPTR=PTR;
		}
		else if(buff[0]=='$'&&buff[1]=='D'&&buff[2]=='T'&&buff[3]=='A')
		{
			STARTEXECUTION();
		}
		else if(buff[0]=='$'&&buff[1]=='E'&&buff[2]=='N'&&buff[3]=='D')
		{
			continue;
		}
		else
		{
			RA=ALLOCATE();
			M[XPTR][3]=(RA%10);
			M[XPTR][2]=(RA/10);
			M[XPTR][0]='1';
			RA=RA*10;
			l=line;
			for(i=RA;i<=RA+9;i++)
			{
				for(j=0;j<=3;j++)
				{
					M[i][j]=*l;
					if(*l=='\0')
					{
						f=1;
						break;
					}
					l++;
				}
				if(f==1)
				{
					f=0;
					break;
				}
			}
			XPTR=XPTR+1;
		}
	}
	if(temp==NULL)
	{
		printf("\nProgram Executed SUCCESSFULLY CHECK FILE  ' %s '",output);
		fclose(fp1);
		fclose(fp2);
		free(fp1);
		free(fp2);
		getch();
		exit(0);
	}
}
void STARTEXECUTION()
{
	IC=0;
	EXECUTEUSERPROGRAM();
}
void EXECUTEUSERPROGRAM()
{
	int i;
	while(1)
	{
		SI=0;
		TI=0;
		PI=0;
		RA=ADDRESSMAP(IC);
		IC++;
		for(i=0;i<4;i++)
		{
			IR[i]=M[RA][i];
		}

		for(i=0;i<=1;i++)
		{
			opcode[i]=IR[i];
		}
		if((IR[2]>=48&&IR[2]<=57)&&IR[3]>=48&&IR[3]<=57)
			operand=(IR[2]-48)*10+(IR[3]-48);
		else
			operand=9999;
		if(opcode[0]!='H')
			RA=ADDRESSMAP(operand);
		TTC++;
		if(TTC>PCB.TTL)
		{
			TI=2;
		}

		if(PI!=0)
		{
			MOS();
			PI=0;
		}
		if(opcode[0]=='L'&&opcode[1]=='R')
		{
			strncpy(R,M[RA],4);
		}
		else if(opcode[0]=='S'&&opcode[1]=='R')
		{
			strncpy(M[RA],R,4);
		}
		else if(opcode[0]=='C'&&opcode[1]=='R')
		{
			if(R[0]==M[RA][0]&&R[1]==M[RA][1]&&R[2]==M[RA][2]&&R[3]==M[RA][3])
				C=1;
			else
				C=0;
		 }
		else if(opcode[0]=='B'&&opcode[1]=='T')
		{
			if(C==1)
			{
				IC=operand;
				C=0;
			}
		}
		else if(opcode[0]=='G'&&opcode[1]=='D')
		{
			SI=1;
		}
		else if(opcode[0]=='P'&&opcode[1]=='D')
		{
			SI=2;
		}
		else if(opcode[0]=='H')
		{
			SI=3;
		}
		else
		{
			PI=1;
		}
		if(TI!=0||SI!=0||PI!=0)
			MOS();
	}
}

void MOS()
{
	switch(TI)
	{
		case 0:
			switch(PI)
			{
				case 1:
					TERMINATE(4);
					break;
				case 2:
					TERMINATE(5);
					break;
				case 3:
					if((opcode[0]=='G'&&opcode[1]=='D')||(opcode[0]=='S'&&opcode[1]=='R'))
					{
						RA=ALLOCATE();
						M[PTR+operand/10][2]=(RA/10);
						M[PTR+operand/10][3]=(RA%10);
						M[PTR+operand/10][0]='1';
						RA=RA*10;
					}
					else
					{
						TERMINATE(6);
					}
					break;
			}
			switch(SI)
			{
				case 1:
					READ();
					break;
				case 2:
					WRITE();
					break;
				case 3:
					TERMINATE(0);
					break;
			}
			break;
		case 2:
			switch(PI)
			{
				case 1:
					TERMINATE(7);
					break;
				case 2:
					TERMINATE(8);
					break;
				case 3:
					TERMINATE(3);
					break;
			}
			switch(SI)
			{
				case 1:
					TERMINATE(3);
					break;
				case 2:
					WRITE();
					TERMINATE(3);
					break;
				case 3:
					TERMINATE(0);
					break;
			}
			break;
	}
}
int ADDRESSMAP(int VA)
{
	if(VA==9999) //VA=9999 represents it is not integer.
	{
		PI=2;
		return(0);
	}
	if(M[PTR+VA/10][0]=='1')
	{
		RA=((M[PTR+VA/10][2]*100+M[PTR+VA/10][3]*10)+(VA%10));
		return(RA);
	}
	else
	{
		PI=3;
		return(0);
	}
}
void READ()
{
	int i,j,f=0;
	char line[40],*l,buff[4];
	fgets(line,40,fp1);
	l=line;

	if(l[0]=='$'&&l[1]=='E'&&l[2]=='N'&&l[3]=='D')
		TERMINATE(1);

	for(i=RA;i<=RA+9;i++)
	{
		for(j=0;j<=3;j++)
		{
			M[i][j]=*l;
			l++;
			if(*l=='\0')
			{
				f=1;
				break;
			}
		}
		if(f==1)
			break;
	}
}
void WRITE()
{
	int i,j,k=0,f=0,operand;
	LLC++;
	if(LLC>PCB.TLL)
	{
		TERMINATE(2);
		return;
	}
	operand=(IR[2]-48)*10+(IR[3]-48);
	operand=operand-(operand%10);

	for(i=RA;i<=RA+9;i++)
	{
		for(j=0;j<=3;j++)
		{
			if(M[i][j]=='\n')
				continue;
			if(M[i][j]=='\0')
			{
				f=1;
				break;
			}
			fprintf(fp2,"%c",M[i][j]);
			k++;
		}
		if(f==1)
			break;
	}
	fprintf(fp2,"\n");
}
void TERMINATE(int EM)
{
	//fprintf(fp2,"\n\n");
	switch(EM)
	{
		case 0:
			fprintf(fp2,"%d\tNo Error",PCB.JID);
			break;
		case 1:
			fprintf(fp2,"%d\tOut of Data",PCB.JID);
			break;
		case 2:
			fprintf(fp2,"%d\tLine Limit Exceeded",PCB.JID);
			break;
		case 3:
			fprintf(fp2,"%d\tTime Limit Exceeded",PCB.JID);
			break;
		case 4:
			fprintf(fp2,"%d\tOperation Code Error",PCB.JID);
			break;
		case 5:
			fprintf(fp2,"%d\tOperand Error",PCB.JID);
			break;
		case 6:
			fprintf(fp2,"%d\tInvalid Page Fault",PCB.JID);
			break;
		case 7:
			fprintf(fp2,"%d\tTime Limit Exceeded with Operation Code Error",PCB.JID);
			break;
		case 8:
			fprintf(fp2,"%d\tTime Limit Exceeded with Operand Error",PCB.JID);
			break;
	}

	if(opcode[0]!='H')
		fprintf(fp2,"\n%d\t%c%c%c%c\t%d\t%d\n\n\n",IC,IR[0],IR[1],IR[2],IR[3],TTC,LLC);
	else
		fprintf(fp2,"\n%d\t%c\t%d\t%d\n\n\n",IC,IR[0],TTC,LLC);
	LOAD();
}


