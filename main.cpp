#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

//单词结构二元组
struct WordType
{
    int code;
    string pro;
};

// 关键字表和对应编码
string codestring[11] = { "System","Version","Code","Data","UTF-8","</","<!","!>","//","/@",":="};
int codebook[11] = {1,2,3,4,5,8,9,10,13,17,25};

//全局变量
char ch;
int flag = 0;

//函数声明
WordType get_w();
void GetChar();
void GetBC();
bool IsLetter(char c);
bool IsDigit(char c);
void Retract();
int Reserve(string str);
string Concat(string str);

int main()
{
    char choose;
    do {
        system("cls");
        WordType word;
        cout << "********************************" << endl;
        cout << "*			  词法分析器  	    *" << endl;
        cout << "********************************" << endl;

        cout << "请输入源程序序列（以~结束）:" << endl;
        word = get_w();
        while (word.pro != "~") //结束标识符
        {
            cout << word.code << '\t' << word.pro << endl;
            word = get_w();
        };
        cout << endl << endl << "继续输入新的词法分析器按y，其他字符退出：";
        cin.clear();
        cin.sync();
        cin >> choose;
    } while (choose == 'y');
    return 0;
}

WordType get_w()
{
    string strToken = "";
    string sz = ""; //临时储存
    int code;
    WordType wordtmp;
    GetChar();
    GetBC();
    if (IsLetter(ch)) {  //先判断是否为关键字 UTF-8
        if (ch == 'U') {
            strToken = Concat(strToken);
            GetChar();
            if (ch == 'T') {
                strToken = Concat(strToken);
                GetChar();
                if (ch == 'F') {
                    strToken = Concat(strToken);
                    GetChar();
                    if (ch == '-') {
                        strToken = Concat(strToken);
                        GetChar();
                        if (ch == '8') {
                            strToken = Concat(strToken);
                            wordtmp.code = 5;
                            wordtmp.pro = strToken;
                            GetChar();
                        }
                        else { strToken.pop_back(); ch = '-';}
                    }
                    else { strToken.pop_back(); ch = 'F';}
                }
                else { strToken.pop_back(); ch = 'T';}
            }
            else Retract();
        }
        //  判断正常字符串
        while (IsLetter(ch) || IsDigit(ch)) {
            strToken = Concat(strToken);
            GetChar();
        }
        Retract();
        code = Reserve(strToken);
        wordtmp.code = code;
        wordtmp.pro = strToken;
    }
    else if (IsDigit(ch)) {
        int count = 0;
        while (IsDigit(ch) && count <=1 ) {
            strToken = Concat(strToken);
            GetChar();
            if (ch == '.' && count <1 ) {
                count += 1;
                sz = strToken;
                strToken = Concat(strToken);
                GetChar();
                if (!IsDigit(ch)) {
                    strToken = sz;
                    ch = '.';
            }
                }
        }
        Retract();
        wordtmp.code = 16;
        wordtmp.pro = strToken;
    }
    else if (ch == '<') {
        strToken = Concat(strToken);
        GetChar();
        if (ch == '/' || ch == '!') {
            strToken = Concat(strToken);
            code = Reserve(strToken);
            wordtmp.code = code;
            wordtmp.pro = strToken;
        }
        else {
            Retract();
            wordtmp.code = 6;
            wordtmp.pro = '<';
        }
    }
    else if (ch == ':') {
        strToken = Concat(strToken);
        GetChar();
        if (ch == ':' || ch =='=') {
            strToken = Concat(strToken);
            code = Reserve(strToken);
            wordtmp.code = code;
            wordtmp.pro = strToken;
        }
        else {
            Retract();
            wordtmp.code = 20;
            wordtmp.pro = ':';
        }
    }
    else if (ch == '!') {
        strToken = Concat(strToken);
        GetChar();
        if (ch == '>') {
            strToken = Concat(strToken);
            wordtmp.code = 10;
            wordtmp.pro = strToken;
        }
        else {
            Retract();
            wordtmp.code = 0;
            wordtmp.pro = '!';
        }
    }
    else if (ch == '/') {
        strToken = Concat(strToken);
        GetChar();
        if (ch == '/'|| ch == '@') {
            strToken = Concat(strToken);
            code = Reserve(strToken);
            wordtmp.code = code;
            wordtmp.pro = strToken;
        }
        else {
            Retract();
            wordtmp.code = 0;
            wordtmp.pro = '/';
        }
    }


    else if (ch == '>') { wordtmp.code = 7; wordtmp.pro = '>'; }
    else if (ch == '@') { wordtmp.code = 11; wordtmp.pro = '@'; }
    else if (ch == '#') { wordtmp.code = 12; wordtmp.pro = '#'; }
    else if (ch == '=') { wordtmp.code = 15; wordtmp.pro = '='; }
    else if (ch == '.') { wordtmp.code = 16; wordtmp.pro = '.'; }
    else if (ch == '%') { wordtmp.code = 18; wordtmp.pro = '%'; }
    else if (ch == '$') { wordtmp.code = 19; wordtmp.pro = '$'; }
    else if (ch == ':') { wordtmp.code = 20; wordtmp.pro = ':'; }
    else if (ch == '*') { wordtmp.code = 21; wordtmp.pro = '*'; }
    else if (ch == '\'') { wordtmp.code = 22; wordtmp.pro = '\''; }
    else if (ch == '-') { wordtmp.code = 23; wordtmp.pro = '-'; }
    else if (ch == ',') { wordtmp.code = 24; wordtmp.pro = ','; }
    else				{ wordtmp.code = 0; wordtmp.pro = ch; }

    return wordtmp;
}

void GetChar() {
    if (flag == 0)  //没有回退继续读取下一个字符
        ch = getchar();
    else
        flag = 0;  //有回退字符，用回退字符，并重置
}

void GetBC() {
    if (ch == ' ' || ch == '\n' || ch == '\t')
        ch = getchar();
}

bool IsLetter(char c) {
    if (c >= 'a' && c <= 'z')
        return true;
    if (c >= 'A' && c <= 'Z')
        return true;
    return false;
}

bool IsDigit(char c) {
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

string Concat(string str){
    string c;
    c.push_back(ch);
    str += c;
    return str;
}

void Retract() {
    flag = 1;
}

int Reserve(string str) {
    for (int i = 0; i < 11; i++)
    {
        if (!str.compare(codestring[i]))
            return codebook[i];
    }
    return 0;
}
