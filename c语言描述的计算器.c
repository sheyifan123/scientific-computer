#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "windows.h"
#include "math.h"
#include "conio.h"

double stringToDouble(char a[1000])
{
	int i=0;
	char *p=(char*)malloc(1000*sizeof(char));
	int top=0;
	double sum=0;
	double mul=1;
	while(a[i]!='\0'&&a[i]!='.')
	{
		p[top]=a[i];
		top++;
		i++;
	}
	while(top!=0)
	{
		top--;
		sum=sum+(p[top]-48)*mul;
		mul=mul*10;
	}
	if(a[i]=='.')
	{
		i++;
		mul=0.1;
		while(a[i]!='\0')
		{
			sum=sum+(a[i]-48)*mul;
			mul=mul/10;
			i++;
		}
	}
	return sum;
}
struct expre
{
	double num;
	char sign;
	int ifNum;//标志位，1表示数字，0表示符号
	int end;//标志位，标志结构体数据末尾
}*expre1,*expre2;
struct numstack
{
	double *base;
}numstack1;//计算后缀表达式所需的数据栈
struct signstack
{
	char *base;
}signstack1;//符号栈
void structExpreRead(struct expre expre1[1000])
{
	int i=0;
	while(expre1[i].end!=1)
	{
		if(expre1[i].ifNum==1)
		{
			printf("%f ",expre1[i].num);
			i++;
		}
		else
		{
			printf("%c ",expre1[i].sign);
			i++;
		}
	}
}
//四则运算
double compute(double a,double b,char sign)
{
	if(sign=='+')
		return a+b;
	if(sign=='-')
		return b-a;
	if(sign=='*')
		return a*b;
	if(sign=='/')
		return b/a;
}
//符号优先级
int signnum(char x)
{
	if(x=='(')
		return 1;
	if(x=='+'||x=='-')
		return 2;
	if(x=='*'||x=='/')
		return 3;
}
void stringCopy(char *a,char *b,int start,int length)
{
	int i;
	int j=0;
	for(i=start;i<start+length;i++)
	{
		a[j]=b[i];
		j++;
	}
	a[j]='\0';
}
double com(char exp[1000])
{
	int strRead=0,strWrite=0;
	int strStart;
	int length=0;
	double strCuttenFloat;
	char strCutten[1000];
	int i=0;//表达式遍历索引
	int j=0;//后缀表达式索引
	int signtop=-1;//符号栈顶指针
	int signtop1=-1;//数据栈顶指针
	int s=0;
	int k=0;//后缀表达式遍历
	double a,b;//四则运算参数
	expre1=(struct expre*)malloc(strlen(exp)*sizeof(struct expre));
	while(exp[strRead]!='\0')
	{
		//如果是数字，存进结构体中的num
		if(exp[strRead]>='0'&&exp[strRead]<='9'||exp[strRead]=='.')
		{
			strStart=strRead;
			while(exp[strRead]>='0'&&exp[strRead]<='9'||exp[strRead]=='.')
			{
				strRead++;
				length++;
			}
			//截取数字字符串，并转换成数字
			strncpy(strCutten,exp+strStart,length);
			strCutten[length]='\0';
			strCuttenFloat=stringToDouble(strCutten);
			expre1[strWrite].num=strCuttenFloat;
			expre1[strWrite].ifNum=1;
			expre1[strWrite].end=0;
			expre1[strWrite].sign='o';
			strWrite++;
			length=0;
		}
		//如果是符号，存进结构体中的sign
		else
		{
			expre1[strWrite].sign=exp[strRead];
			expre1[strWrite].ifNum=0;
			expre1[strWrite].end=0;
			expre1[strWrite].num=0;
			strRead++;
			strWrite++;
		}
	}
	expre1[strWrite].end=1;
	expre1[strWrite].ifNum=1;
	expre1[strWrite].sign='o';
	expre1[strWrite].num=0;
	/*下面将表达式转换成后缀表达式*/
	//符号栈初始化
	signstack1.base=(char*)malloc(strlen(exp)*sizeof(char));
	//后缀表达式初始化
	expre2=(struct expre*)malloc(strlen(exp)*sizeof(struct expre));
	while(expre1[i].end!=1)
	{
		if(expre1[i].ifNum==1)
		{
			expre2[j].num=expre1[i].num;
			expre2[j].ifNum=1;
			j++;
			i++;
		}
		else if(expre1[i].sign=='(')
		{
			signtop++;
			signstack1.base[signtop]=expre1[i].sign;
			i++;
		}
		else if(signnum(expre1[i].sign)==2)
		{
			if(signtop==-1)
			{
				signtop++;
				signstack1.base[signtop]=expre1[i].sign;
				i++;
			}
			else
			{
				while(signnum(signstack1.base[signtop])==2||signnum(signstack1.base[signtop])==3)
				{
					expre2[j].sign=signstack1.base[signtop];
					expre2[j].ifNum=0;
					j++;
					signtop--;
					if(signtop==-1)
						break;
				}
				signtop++;
				signstack1.base[signtop]=expre1[i].sign;
				i++;
			}
		}
		else if(expre1[i].sign=='*'||expre1[i].sign=='/')
		{
			if(signtop==-1)
			{
				signtop++;
				signstack1.base[signtop]=expre1[i].sign;
				i++;
			}
			else
			{
				while(signnum(signstack1.base[signtop]==3))
				{
					expre2[j].sign=signstack1.base[signtop];
					expre2[j].ifNum=0;
					j++;
					signtop--;
					if(signtop==-1)
						break;
				}
				signtop++;
				signstack1.base[signtop]=expre1[i].sign;
				i++;
			}
		}
		else
		{
			if(expre1[i].sign==')')
			{
				while(signstack1.base[signtop]!='(')
				{
					expre2[j].sign=signstack1.base[signtop];
					expre2[j].ifNum=0;
					j++;
					signtop--;
				}
				i++;
				signtop--;
			}
			else if(expre1[i].sign=='s')
			{

			}
			else
			{
				printf("输入表达式格式不规范!\n");
				Sleep(2000);
				exit(1);
			}
		}
	}
	while(signtop!=-1)
	{
		expre2[j].sign=signstack1.base[signtop];
		expre2[j].ifNum=0;
		j++;
		signtop--;
	}
	expre2[j].end=1;
	numstack1.base=(double*)malloc(100*sizeof(double));
	if(numstack1.base==NULL)
	{
		printf("\n动态内存分配失败\n");
		Sleep(2000);
		exit(1);
	}
	while(expre2[k].end!=1)
	{
		if(expre2[k].ifNum==1)/*数字直接入栈*/
		{
			signtop1++;
			numstack1.base[signtop1]=expre2[k].num;
			k++;
		}
		else/*如果是符号*/
		{	
			
			a=numstack1.base[signtop1];
			signtop1--;
			b=numstack1.base[signtop1];		
			numstack1.base[signtop1]=compute(a,b,expre2[k].sign);
			k++;
		}
	}
	return numstack1.base[signtop1];
}
//结构体用来分别存储表达式中的数字和符号
//表达式转换成标准的中缀表达式
//操作符分为单目和双目
/*
*s:sin;x:arcsin
*c:cos;d:arccos
*t:tan;g:arctan
*q:平方根
*l:lg
*e:ln
*/
//检测表达式是否需要化简
int ifNeedSimplify(char *expre)
{
	int i=0;
	while(expre[i]!='\0')
	{
		if(expre[i]!='+'&&expre[i]!='-'&&expre[i]!='*'&&expre[i]!='/'&&expre[i]!='('&&expre[i]!=')')
			return 1;
		i++;
	}
	return 0;
}
void HideCursor() /*隐藏光标*/
{ 
 CONSOLE_CURSOR_INFO cursor_info = {1, 0};  
 SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); 
}
void gotoxy(int x,int y)
{
	COORD c;
	c.X=x-1;
	c.Y=y-1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
void setxy(int x, int y)/*列，行*/
{   COORD coord = {x, y};   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void Horizontalline(int x1,int y1,int length)
{
	int i;
	setxy(x1,y1);
	for(i=0;i<length*2;i++)
		putchar('_');
}
void Verticalline(int x1,int y1,int length)
{
	int i;
	setxy(x1,y1);
	for(i=0;i<length;i++)
	{
		putchar('|');
		y1++;
		setxy(x1,y1);
	}
}
void initialSet()
{
	Horizontalline(0,4,18);
	Horizontalline(0,8,18);
	Horizontalline(0,12,18);
	Horizontalline(0,16,18);
	Verticalline(0,4,14);
	Verticalline(8,4,14);
	Verticalline(16,4,14);
	Verticalline(24,4,14);
	Verticalline(32,4,14);

	setxy(3,6);
	printf("sin:s");
	setxy(1,10);
	printf("arcsin:x");
	setxy(3,14);
	printf("sqrt:q");
	setxy(11,6);
	printf("cos:c");
	setxy(10,10);
	printf("arccos:d");
	setxy(11,14);
	printf("+/-");
	setxy(19,6);
	printf("tan:t");
	setxy(19,10);
	printf("atan:g");
	setxy(19,14);
	printf("*/");
	setxy(27,6);
	printf("lg:l");
	setxy(27,10);
	printf("ln:e");
	setxy(27,14);
	printf("( )");
	setxy(0,0);
}
void clearIn()
{
	setxy(0,1);
	printf("                                                         ");
	setxy(2,0);
	printf("                                                         ");
	setxy(0,0);
}
void clearOut()
{
	setxy(0,20);
	printf("                                                     ");
}
void main()
{
	while(1){
	char expre[1000];
	int i=0,j=1;
	int startI;
	char str1[1000],numStr[1000],str2[1000];//将expre分成三个部分
	int bracketsNum=1;
	double sinValue,cosValue,tanValue,qValue,logValue,lnValue,arcsinValue,arccosValue,arctanValue;
	char numChar[1000];
	int s=0;
	char pause;
	initialSet();
	printf("请输入表达式:\n");
	gets(expre);
	while(expre[i]!='\0')//缺陷：sin里面不能实现函数的嵌套。sin函数必须带括号.
	{
		if(expre[i]=='s')
		{
			startI=i;
			//截取expre的第一个片段
			stringCopy(str1,expre,0,startI);
			//读取sin函数中的表达式
			i=i+2;
			while(bracketsNum!=0)
			{
				if(expre[i]=='(')
					bracketsNum++;
				if(expre[i]==')')
					bracketsNum--;
				i++;
			}
			bracketsNum=1;
			//截取expre的第二个片段
			stringCopy(numStr,expre,startI+2,i-startI-3);
			//数字片段进行计算
			sinValue=sin(com(numStr));
			gcvt(sinValue, i-startI-2, numChar);
			//截取expre的第三个片段
			stringCopy(str2,expre,i,strlen(expre)-3);
			//三个片段重新拼接成expre
			strcat(str1,numChar);
			strcat(str1,str2);
			strcpy(expre,str1);
		}
		else if(expre[i]=='c')
		{
			startI=i;
			//截取expre的第一个片段
			stringCopy(str1,expre,0,startI);
			//读取sin函数中的表达式
			i=i+2;
			while(bracketsNum!=0)
			{
				if(expre[i]=='(')
					bracketsNum++;
				if(expre[i]==')')
					bracketsNum--;
				i++;
			}
			bracketsNum=1;
			//截取expre的第二个片段
			stringCopy(numStr,expre,startI+2,i-startI-3);
			//数字片段进行计算
			cosValue=cos(com(numStr));
			gcvt(cosValue, i-startI-2, numChar);
			//截取expre的第三个片段
			stringCopy(str2,expre,i,strlen(expre)-3);
			//三个片段重新拼接成expre
			strcat(str1,numChar);
			strcat(str1,str2);
			strcpy(expre,str1);
		}
		else if(expre[i]=='t')
		{
			startI=i;
			//截取expre的第一个片段
			stringCopy(str1,expre,0,startI);
			//读取sin函数中的表达式
			i=i+2;
			while(bracketsNum!=0)
			{
				if(expre[i]=='(')
					bracketsNum++;
				if(expre[i]==')')
					bracketsNum--;
				i++;
			}
			bracketsNum=1;
			//截取expre的第二个片段
			stringCopy(numStr,expre,startI+2,i-startI-3);
			//数字片段进行计算
			tanValue=tan(com(numStr));
			gcvt(tanValue, i-startI-2, numChar);
			//截取expre的第三个片段
			stringCopy(str2,expre,i,strlen(expre)-3);
			//三个片段重新拼接成expre
			strcat(str1,numChar);
			strcat(str1,str2);
			strcpy(expre,str1);
		}
		else if(expre[i]=='q')
		{
			startI=i;
			//截取expre的第一个片段
			stringCopy(str1,expre,0,startI);
			//读取sin函数中的表达式
			i=i+2;
			while(bracketsNum!=0)
			{
				if(expre[i]=='(')
					bracketsNum++;
				if(expre[i]==')')
					bracketsNum--;
				i++;
			}
			bracketsNum=1;
			//截取expre的第二个片段
			stringCopy(numStr,expre,startI+2,i-startI-3);
			//数字片段进行计算
			qValue=sqrt(com(numStr));
			gcvt(qValue, i-startI-2, numChar);
			//截取expre的第三个片段
			stringCopy(str2,expre,i,strlen(expre)-3);
			//三个片段重新拼接成expre
			strcat(str1,numChar);
			strcat(str1,str2);
			strcpy(expre,str1);
		}
		else if(expre[i]=='l')
		{
			startI=i;
			//截取expre的第一个片段
			stringCopy(str1,expre,0,startI);
			//读取sin函数中的表达式
			i=i+2;
			while(bracketsNum!=0)
			{
				if(expre[i]=='(')
					bracketsNum++;
				if(expre[i]==')')
					bracketsNum--;
				i++;
			}
			bracketsNum=1;
			//截取expre的第二个片段
			stringCopy(numStr,expre,startI+2,i-startI-3);
			//数字片段进行计算
			logValue=log10(com(numStr));
			gcvt(logValue, i-startI-2, numChar);
			//截取expre的第三个片段
			stringCopy(str2,expre,i,strlen(expre)-3);
			//三个片段重新拼接成expre
			strcat(str1,numChar);
			strcat(str1,str2);
			strcpy(expre,str1);
		}
		else if(expre[i]=='e')
		{
			startI=i;
			//截取expre的第一个片段
			stringCopy(str1,expre,0,startI);
			//读取sin函数中的表达式
			i=i+2;
			while(bracketsNum!=0)
			{
				if(expre[i]=='(')
					bracketsNum++;
				if(expre[i]==')')
					bracketsNum--;
				i++;
			}
			bracketsNum=1;
			//截取expre的第二个片段
			stringCopy(numStr,expre,startI+2,i-startI-3);
			//数字片段进行计算
			lnValue=log(com(numStr));
			gcvt(lnValue, i-startI-2, numChar);
			//截取expre的第三个片段
			stringCopy(str2,expre,i,strlen(expre)-3);
			//三个片段重新拼接成expre
			strcat(str1,numChar);
			strcat(str1,str2);
			strcpy(expre,str1);
		}
		else if(expre[i]=='x')
		{
			startI=i;
			//截取expre的第一个片段
			stringCopy(str1,expre,0,startI);
			//读取sin函数中的表达式
			i=i+2;
			while(bracketsNum!=0)
			{
				if(expre[i]=='(')
					bracketsNum++;
				if(expre[i]==')')
					bracketsNum--;
				i++;
			}
			bracketsNum=1;
			//截取expre的第二个片段
			stringCopy(numStr,expre,startI+2,i-startI-3);
			//数字片段进行计算
			arcsinValue=asin(com(numStr));
			gcvt(arcsinValue, i-startI-2, numChar);
			//截取expre的第三个片段
			stringCopy(str2,expre,i,strlen(expre)-3);
			//三个片段重新拼接成expre
			strcat(str1,numChar);
			strcat(str1,str2);
			strcpy(expre,str1);
		}
		else if(expre[i]=='d')
		{
			startI=i;
			//截取expre的第一个片段
			stringCopy(str1,expre,0,startI);
			//读取sin函数中的表达式
			i=i+2;
			while(bracketsNum!=0)
			{
				if(expre[i]=='(')
					bracketsNum++;
				if(expre[i]==')')
					bracketsNum--;
				i++;
			}
			bracketsNum=1;
			//截取expre的第二个片段
			stringCopy(numStr,expre,startI+2,i-startI-3);
			//数字片段进行计算
			arccosValue=acos(com(numStr));
			gcvt(arccosValue, i-startI-2, numChar);
			//截取expre的第三个片段
			stringCopy(str2,expre,i,strlen(expre)-3);
			//三个片段重新拼接成expre
			strcat(str1,numChar);
			strcat(str1,str2);
			strcpy(expre,str1);
		}
		else if(expre[i]=='g')
		{
			startI=i;
			//截取expre的第一个片段
			stringCopy(str1,expre,0,startI);
			//读取sin函数中的表达式
			i=i+2;
			while(bracketsNum!=0)
			{
				if(expre[i]=='(')
					bracketsNum++;
				if(expre[i]==')')
					bracketsNum--;
				i++;
			}
			bracketsNum=1;
			//截取expre的第二个片段
			stringCopy(numStr,expre,startI+2,i-startI-3);
			//数字片段进行计算
			arctanValue=atan(com(numStr));
			gcvt(arctanValue, i-startI-2, numChar);
			//截取expre的第三个片段
			stringCopy(str2,expre,i,strlen(expre)-3);
			//三个片段重新拼接成expre
			strcat(str1,numChar);
			strcat(str1,str2);
			strcpy(expre,str1);
		}
		else
		{
			i++;
		}
	}
	setxy(0,20);
	printf("%f\n",com(expre));
	pause=getchar();
	clearOut();
	setxy(0,0);
	clearIn();
	}
	system("pause");
}