#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
struct Expression
{
	int result;
	int error;
	Expression ()
	{
		result = 0;
		error = 0;
	}
};
Expression GetExp(char*& Stream);
Expression GetNum(char*& Stream)
{
	Expression ret;
	int negtive = 1;
	char* Read = Stream; 
	while(*Read == ' ')Read++;
	bool flag = false;
	if(*Read == '+')Read++;
	else if(*Read =='-')
	{
		negtive = -1;
		Read++;
	}
	while(1)
	{
		char ch = *Read;
		if('0'<=ch && ch <='9')
		{
			flag = true;
			ret.result= ret.result * 10 + (ch - '0');
			Read++;
		}
		else break;
	}
	ret.result *= negtive;
	if(flag)Stream = Read;else ret.error =1;
	return ret;
}
bool Is(char*& Stream,const char* str)
{
	int str_len = strlen(str);
	char* Read = Stream;
	while(*Read == ' ')Read++;
	if(!strncmp(Read,str,str_len))
	{
		Stream = Read + str_len;
		return true;
	}
	else return false;
}
Expression GetTerm(char*& Stream)
{
	Expression ret = GetNum(Stream);
	if(!ret.error)return ret;
	char* Read = Stream;
	if(Is(Read,"("))
	{
		Expression ret = GetExp(Read);
		if(!ret.error)
		{
			if(Is(Read , ")"))
			{
				Stream = Read;
				return ret;
			}
		}
		return ret;
	}
}
Expression GetFactor(char*& Stream)
{
	char* Read = Stream;
	Expression ret = GetTerm(Read);
	if(ret.error)return ret;
	while(1)
	{
		if(Is(Read,"*"))
		{
			Expression term = GetTerm(Read);
			if(!term.error)
			{
				ret.result *= term.result;
			}
		}
		else if(Is(Read,"/"))
		{
			Expression term = GetTerm(Read);
			if(!term.error)
			{
				ret.result /= term.result;
			}
		}
		else break;
	}
	Stream = Read;
	return ret;
}
Expression GetExp(char*& Stream)
{
	char* Read = Stream;
	Expression ret = GetFactor(Read);
	if(ret.error)return ret;
	while(1)
	{
		if(Is(Read,"+"))
		{
			Expression factor = GetFactor(Read);
			if(!factor.error)
			{
				ret.result += factor.result;
			}
		}
		if(Is(Read,"-"))
		{
			Expression factor = GetFactor(Read);
			if(!factor.error)
			{
				ret.result -= factor.result;
			}
		}
		else break;
	}
	Stream = Read;
	return ret;
}
int main()
{
	char ch[1000];
	gets(ch);
	char *p = ch;
	Expression u = GetExp(p);
	printf("%d\n",u.result);
}
