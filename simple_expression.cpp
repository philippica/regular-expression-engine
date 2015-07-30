#include "simple_expression.h"


Expression::Expression():
    error(0),
    start(0),
    end(0)
{
}



Expression Exp::GetTemp(char*& Stream)
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

Expression Exp::GetFactor(char*& Stream)
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


Expression Exp::GetExp(char*& Stream)
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

Expression Exp::getExtern(char*& Stream)
{
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
		puts("in");
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



Expression Exp::GetTerm(char*& Stream)
{
	char* Read = Stream ;
	const char* gg = gang;
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



bool Exp::Is(char*& Stream ,const char* str)
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

