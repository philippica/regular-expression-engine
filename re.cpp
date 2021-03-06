#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include <map>
#include <queue>
#define maxn 10000
using namespace std;
int valid[maxn],p[maxn],h=0,p_num = 0,visit[maxn],new_p=0;
int head[maxn],next[maxn],now,point[maxn],en[maxn];
char value[maxn];
int head2[maxn],next2[maxn],now2,point2[maxn],en2[maxn];
char value2[maxn];
char gang[10];
struct Expression
{
	int error;
	int start;
	int end;
	Expression();
};
Expression::Expression():
error(0),
start(0),
end(0)
{
}

Expression GetTemp(char*& Stream);

void add(int x,int y,char v)
{
	next[++now] = head[x];
	head[x] = now;
	point[now] = y;
	value[now] = v;
}
void add2(int x,int y,char v)
{
	next2[++now2] = head2[x];
	head2[x] = now2;
	point2[now2] = y;
	value2[now2] = v;
}


void dfs(int start,int k)
{
	for(int i = head[k]; i!=0; i=next[i])
	{
		int u = point[i];
		if(visit[u])continue;
		if (value[i] == '@')
		{
            if(en[u])en[start] = 1;
			dfs(start,u);
		}
		else if (start != k)
		{
			add(start,u,value[i]);
		}
	}
    visit[k] = 1;
}


void del(int x,int k,int pre)
{
    if(k==pre)
    {
        head[x] = next[k];
    }
    else
    {
        next[pre] = next[k];
    }
}

void errase(int start,int end)
{
	valid[start] = 1;
	p[++h] = start;
	en[end] = 1;
	for(int i = 1; i <= h; i++)
	{
        if(i==37)
        {
            puts("12");
        }
		memset(visit,0,sizeof(visit));
		dfs(p[i], p[i]);
	}
	for(int i = 1; i <= h; i++)
	{
		for(int j = head[p[i]], pre = head[p[i]]; j; pre = j,j=next[j])
		{
			if (!valid[point[j]] || value[j] == '@')
			{
				del(p[i],j,pre);
				j = pre;
			}
		}
	}
}

void Nfa_To_Dfa(int start)
{
    queue< set<int> > q;
    map<set<int>,int> set_set;
    set<int> fir;
    fir.insert(start);
    set_set[fir] = ++new_p;
    q.push(fir);
    int f[200] = {0};
    while(!q.empty())
    {
        set <int> fron = q.front();
        q.pop();
        set <int> ::iterator it;
        set <int> alg[200];
        for(it = fron.begin(); it != fron.end(); it++)
        {
            int v = *it;
            for(int i = head[v]; i; i = next[i])
            {
                int u = point[i];
                if(en[u])
                {
                    f[(int)value[i]] = 1;
                }
                alg[(int)value[i]].insert(u);
            }
        }
        int temp = set_set[fron];
        for(int i = 1; i <= 130; i++) if (!alg[i].empty())
        {
            if(set_set.find(alg[i]) == set_set.end())
            {
                set_set[alg[i]] = ++new_p;
                if(f[i])en2[new_p] = 1;
                q.push(alg[i]);
            }
            add2(temp,set_set[alg[i]],(char)i);
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
		Stream = Read + str_len;
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


Expression getExtern(char*& Stream)
{
	const char* gg = gang;
	char* Read = Stream ;
	Expression ret;
	if (Is(Read,"["))
	{
		char ww[] = "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)";
		char WW[] = "(A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)";
		char num[] = "(0|1|2|3|4|5|6|7|8|9)";
		if(Is(Read,"a") && Is(Read,"-") && Is(Read,"z") && Is(Read, "]"))
		{
			char* ip = ww;
			ret = GetTemp(ip);
		}
		else if(Is(Read,"A") && Is(Read,"-") && Is(Read,"Z") && Is(Read, "]"))
		{
			char* ip = WW;
			ret = GetTemp(ip);
		}
		else if(Is(Read,"0") && Is(Read,"-") && Is(Read,"9") && Is(Read,"]"))
		{
			char* ip = num;
			ret = GetTemp(ip);
		}
		return ret;
	}
	else if(Is(Read, gg))
	{
		//puts("in");
		char ww[] = "([0-9]|[a-z]|[A-Z])";
		char* ip = ww;
		ret = GetTemp(ip);
		return ret;
	}
	else
	{
		ret.error = 1;
		return ret;
	}
}



Expression GetTerm(char*& Stream)
{
	char* Read = Stream ;
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
	}
	else if(Is(Read , "("))
	{
        Expression factor = GetTemp(Read);
        if(factor.error)
        {
            ret.error =1;
        }
        else
        {
            if (Is(Read,")"))
            {
                ret = factor;
            }
            else ret.error = 1;
        }
	}
	else
	{
		Expression factor = getExtern(Read);
        if(factor.error)
        {
            ret.error =1;
        }
	}
    if(Is(Read,"*"))
    {
        Expression ret2;
        ret2.start = ++p_num;
        ret2.end = ++p_num;
        add(ret2.end,ret.start,'@');
        add(ret2.start,ret2.end,'@');
        add(ret.end,ret2.end,'@');
        ret = ret2;
    }
	Stream = Read;
	return ret;
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
	return ret;
}

Expression GetTemp(char*& Stream)
{
	char* Read = Stream;
	Expression term = GetFactor(Read), ret;
	ret.start = ++p_num;
	ret.end = ++p_num;
	add(ret.start, term.start,'@');
	add(term.end, ret.end, '@');
	if(Is(Read,"|"))
	{
		while(1)
		{
			Expression temp = GetTemp(Read);
			if(!term.error)
			{
				add(ret.start, temp.start, '@');
				add(temp.end, ret.end, '@');
			}
			if(!Is(Read, "|"))break;
		}
	}
	Stream = Read;
	return ret;
}

Expression GetExp(char*& Stream)
{
	gang[0] = '\\';
	gang[1] = '\0';
	memset(head,0,sizeof(head));
	memset(head2,0,sizeof(head));
	memset(en,0,sizeof(en));
	memset(en2,0,sizeof(en2));
	memset(valid,0,sizeof(valid));
	new_p=0;
    h=0,p_num = 0,now=now2=0;
    Expression ret = GetTemp(Stream);
    errase(ret.start,ret.end);
    Nfa_To_Dfa(ret.start);
    ret.start = 1;
    memcpy(head,head2,sizeof(head));
    memcpy(point,point2,sizeof(point));
    memcpy(value,value2,sizeof(value));
    memcpy(next,next2,sizeof(next));
    memcpy(en,en2,sizeof(en));
    now = now2;
    return ret;
}


bool match(Expression Re,char* Text)
{
    char* Read = Text;
    int p = Re.start;
	printf("%d\n",p);
    en[Re.end] = 1;
    for(;;)
    {
        int flag = 0;
        for(int i = head[p]; i; i = next[i])
        {
            int u = point[i];
            if (value[i] == *Read)
            {
                flag = 1;
                p = u;
				//printf("%d\n",p);
                Read++;
                break;
            }
        }
        if(!(*Read))break;
        if (flag == 0)return false;
    }
    if(en[p])return true;else return false;
}


int main()
{
	char ch[100],text[100];
	gets(ch);
	while(1)
	{
        gets(text);
        char *pp = ch;
        if(match(GetExp(pp),text))puts("match");else puts("error");
        break;
	}
	return 0;
}
