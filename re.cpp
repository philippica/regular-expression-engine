#include<iostream>
#include<cstdio>
#include<cstring>
#define maxn 1000
using namespace std;
int valid[maxn],p[maxn],h=0,p_num = 0,visit[maxn];
int head[maxn],next[maxn],now,point[maxn];
char value[maxn];
struct Expression
{
	int error;
	int start;
	int end;
	Expression()
	{
		error = start = end = 0;
	}
};


void add(int x,int y,char v)
{
	next[++now] = head[x];
	head[x] = now;
	point[now] = y;
	value[now] = v;
}

void dfs(int start,int k)
{
	visit[k] = 1;
	for(int i = head[k]; i!=0; i=next[i])
	{
		int u = point[i];
		if(visit[u])continue;
		if (!valid[u])
		{
			dfs(start,u);
		}
		else if (k != start)
		{
			add(start,u,value[i]);
		}
	}
}

void errase(int start)
{
	valid[start] = 1;
	p[++h] = start;
	for(int i = 1; i <= h; i++)
	{
		memset(visit,0,sizeof(visit));
		dfs(p[i], p[i]);
	}
	for(int i = 1; i <= h; i++)
	{
		for(int j = head[p[i]]; j; j=next[j])
		{
			if (!valid[point[j]] || value[j] == '@')
			{
				point[j] = point[next[j]];
				value[j] = value[next[j]];
				next[j] = next[next[j]];
			}
		}
	}
}

bool Is(char*& Stream ,const char* str)
{
	char* Read = Stream;
	int str_len = strlen(str);
	while(*Read == ' ')Read++;
	if(!strncmp(Read,str,str_len))
	{
		Stream = Read;
		return true;
	}
	return false;
}
bool IsAlg(const char ch)
{
	if('a' <= ch && ch <= 'z')return true;
	if('A' <= ch && ch <= 'Z')return true;
	if('0' <= ch && ch <= '9')return true;
	return false;
}
Expression GetTerm(char*& Stream)
{
	char* Read = Stream;
	Expression ret;
	while(*Read == ' ')Read++;
	if(IsAlg(*Read))
	{
		ret.start = ++p_num;
		ret.end = ++p_num;
		valid[ret.end] = 1;
		p[++h] = ret.end;
		add(ret.start,ret.end,*Read);
		Read++;
		Stream = Read;
		return ret;
	}
	else
	{
		ret.error = 1;
		return ret;
	}
}
Expression GetFactor(char*& Stream)
{
	char* Read = Stream;
	Expression ret = GetTerm(Read);
	if(ret.error)
	{
		ret.error = 1;
		return ret;
	}
	while(1)
	{
		Expression term = GetTerm(Read);
		if(!term.error)
		{
			add(ret.end,term.start,'@');
			ret.end = term.end;
		}
		else
		{
			break;
		}
	}
	Stream = Read;
	errase(ret.start);
	return ret;
}
int en[maxn];
bool match(Expression Re,char* Text)
{
    char* Read = Text;
    int p = Re.start;
    en[Re.end] = 1;
    while(true)
    {
        int flag = 0;
        for(int i = head[p]; i; i = next[i])
        {
            int u = point[i];
            if (value[i] == *Read)
            {
                flag = 1;
                p = u;
                Read++;
            }
        }
        if (flag == 0)break;
    }
    if(en[p])return true;else return false;
}

int main()
{
	char ch[100],text[100];
	gets(ch);
	gets(text);
	char *pp = ch;
	if(match(GetFactor(pp),text))puts("match");else puts("error");
	return 0;
}

