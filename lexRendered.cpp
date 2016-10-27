#include<iostream>
#include<fstream>
#include<string>
#include<ctype.h>
using namespace std;
int linecount=1;
int currentstate=0;
int keywordslength=5;
int isOver=0;
int isGotOne=0;
string cachestring;
void handleToken(int k);
string tokens[]={"ID","Bracket","BasicOP","Comparison","Number","SEMI","Comment","ASSIGN","NEQ","SPACE"};
string keywords[]={"int","if","while","void","else"};
int main(){
ifstream input;
input.open(./test.txt);
int c=input.get();
while (!isOver) {isGotOne=0;
if (c==-1) isOver=1;
if (c=='\n') {linecount++;c=input.get();continue;}
switch (currentstate) {
case 0:
switch (c) {
case ' ':
currentstate=1; cachestring+=' ';  break;
case '!':
currentstate=2; cachestring+='!';  break;
case '+':
currentstate=3; cachestring+='+';  break;
case '-':
currentstate=4; cachestring+='-';  break;
case '/':
currentstate=5; cachestring+='/';  break;
case '0':
currentstate=6; cachestring+='0';  break;
case '1':
currentstate=6; cachestring+='1';  break;
case '2':
currentstate=6; cachestring+='2';  break;
case '3':
currentstate=6; cachestring+='3';  break;
case '4':
currentstate=6; cachestring+='4';  break;
case '5':
currentstate=6; cachestring+='5';  break;
case '6':
currentstate=6; cachestring+='6';  break;
case '7':
currentstate=6; cachestring+='7';  break;
case '8':
currentstate=6; cachestring+='8';  break;
case '9':
currentstate=6; cachestring+='9';  break;
case ';':
currentstate=16; cachestring+=';';  break;
case '<':
currentstate=17; cachestring+='<';  break;
case '=':
currentstate=18; cachestring+='=';  break;
case '>':
currentstate=19; cachestring+='>';  break;
case '[':
currentstate=20; cachestring+='[';  break;
case ']':
currentstate=21; cachestring+=']';  break;
case 'a':
currentstate=22; cachestring+='a';  break;
case 'b':
currentstate=22; cachestring+='b';  break;
case 'c':
currentstate=22; cachestring+='c';  break;
case 'd':
currentstate=22; cachestring+='d';  break;
case 'e':
currentstate=22; cachestring+='e';  break;
case 'f':
currentstate=22; cachestring+='f';  break;
case 'g':
currentstate=22; cachestring+='g';  break;
case 'h':
currentstate=22; cachestring+='h';  break;
case 'i':
currentstate=22; cachestring+='i';  break;
case 'j':
currentstate=22; cachestring+='j';  break;
case 'k':
currentstate=22; cachestring+='k';  break;
case 'l':
currentstate=22; cachestring+='l';  break;
case 'm':
currentstate=22; cachestring+='m';  break;
case 'n':
currentstate=22; cachestring+='n';  break;
case 'o':
currentstate=22; cachestring+='o';  break;
case 'p':
currentstate=22; cachestring+='p';  break;
case 'q':
currentstate=22; cachestring+='q';  break;
case 'r':
currentstate=22; cachestring+='r';  break;
case 's':
currentstate=22; cachestring+='s';  break;
case 't':
currentstate=22; cachestring+='t';  break;
case 'u':
currentstate=22; cachestring+='u';  break;
case 'v':
currentstate=22; cachestring+='v';  break;
case 'w':
currentstate=22; cachestring+='w';  break;
case 'x':
currentstate=22; cachestring+='x';  break;
case 'y':
currentstate=22; cachestring+='y';  break;
case 'z':
currentstate=22; cachestring+='z';  break;
case '{':
currentstate=48; cachestring+='{';  break;
case '}':
currentstate=49; cachestring+='}';  break;
default: cout<<"error at line:"<<linecount<<endl;}
break;
case 1:
switch (c) {
default: handleToken(9); isGotOne=1;}
break;
case 2:
switch (c) {
case '=':
currentstate=50; cachestring+='=';  break;
default: cout<<"error at line:"<<linecount<<endl;}
break;
case 3:
switch (c) {
default: handleToken(2); isGotOne=1;}
break;
case 4:
switch (c) {
default: handleToken(2); isGotOne=1;}
break;
case 5:
switch (c) {
case '/':
currentstate=51; cachestring+='/';  break;
default: cout<<"error at line:"<<linecount<<endl;}
break;
case 6:
switch (c) {
case '0':
currentstate=6; cachestring+='0';  break;
case '1':
currentstate=6; cachestring+='1';  break;
case '2':
currentstate=6; cachestring+='2';  break;
case '3':
currentstate=6; cachestring+='3';  break;
case '4':
currentstate=6; cachestring+='4';  break;
case '5':
currentstate=6; cachestring+='5';  break;
case '6':
currentstate=6; cachestring+='6';  break;
case '7':
currentstate=6; cachestring+='7';  break;
case '8':
currentstate=6; cachestring+='8';  break;
case '9':
currentstate=6; cachestring+='9';  break;
default: handleToken(4); isGotOne=1;}
break;
case 16:
switch (c) {
default: handleToken(5); isGotOne=1;}
break;
case 17:
switch (c) {
default: handleToken(3); isGotOne=1;}
break;
case 18:
switch (c) {
default: handleToken(7); isGotOne=1;}
break;
case 19:
switch (c) {
default: handleToken(3); isGotOne=1;}
break;
case 20:
switch (c) {
default: handleToken(1); isGotOne=1;}
break;
case 21:
switch (c) {
default: handleToken(1); isGotOne=1;}
break;
case 22:
switch (c) {
case '0':
currentstate=22; cachestring+='0';  break;
case '1':
currentstate=22; cachestring+='1';  break;
case '2':
currentstate=22; cachestring+='2';  break;
case '3':
currentstate=22; cachestring+='3';  break;
case '4':
currentstate=22; cachestring+='4';  break;
case '5':
currentstate=22; cachestring+='5';  break;
case '6':
currentstate=22; cachestring+='6';  break;
case '7':
currentstate=22; cachestring+='7';  break;
case '8':
currentstate=22; cachestring+='8';  break;
case '9':
currentstate=22; cachestring+='9';  break;
case 'a':
currentstate=22; cachestring+='a';  break;
case 'b':
currentstate=22; cachestring+='b';  break;
case 'c':
currentstate=22; cachestring+='c';  break;
case 'd':
currentstate=22; cachestring+='d';  break;
case 'e':
currentstate=22; cachestring+='e';  break;
case 'f':
currentstate=22; cachestring+='f';  break;
case 'g':
currentstate=22; cachestring+='g';  break;
case 'h':
currentstate=22; cachestring+='h';  break;
case 'i':
currentstate=22; cachestring+='i';  break;
case 'j':
currentstate=22; cachestring+='j';  break;
case 'k':
currentstate=22; cachestring+='k';  break;
case 'l':
currentstate=22; cachestring+='l';  break;
case 'm':
currentstate=22; cachestring+='m';  break;
case 'n':
currentstate=22; cachestring+='n';  break;
case 'o':
currentstate=22; cachestring+='o';  break;
case 'p':
currentstate=22; cachestring+='p';  break;
case 'q':
currentstate=22; cachestring+='q';  break;
case 'r':
currentstate=22; cachestring+='r';  break;
case 's':
currentstate=22; cachestring+='s';  break;
case 't':
currentstate=22; cachestring+='t';  break;
case 'u':
currentstate=22; cachestring+='u';  break;
case 'v':
currentstate=22; cachestring+='v';  break;
case 'w':
currentstate=22; cachestring+='w';  break;
case 'x':
currentstate=22; cachestring+='x';  break;
case 'y':
currentstate=22; cachestring+='y';  break;
case 'z':
currentstate=22; cachestring+='z';  break;
default: handleToken(0); isGotOne=1;}
break;
case 48:
switch (c) {
default: handleToken(1); isGotOne=1;}
break;
case 49:
switch (c) {
default: handleToken(1); isGotOne=1;}
break;
case 50:
switch (c) {
default: handleToken(8); isGotOne=1;}
break;
case 51:
switch (c) {
case ' ':
currentstate=51; cachestring+=' ';  break;
case '/':
currentstate=99; cachestring+='/';  break;
case '0':
currentstate=51; cachestring+='0';  break;
case '1':
currentstate=51; cachestring+='1';  break;
case '2':
currentstate=51; cachestring+='2';  break;
case '3':
currentstate=51; cachestring+='3';  break;
case '4':
currentstate=51; cachestring+='4';  break;
case '5':
currentstate=51; cachestring+='5';  break;
case '6':
currentstate=51; cachestring+='6';  break;
case '7':
currentstate=51; cachestring+='7';  break;
case '8':
currentstate=51; cachestring+='8';  break;
case '9':
currentstate=51; cachestring+='9';  break;
case 'a':
currentstate=51; cachestring+='a';  break;
case 'b':
currentstate=51; cachestring+='b';  break;
case 'c':
currentstate=51; cachestring+='c';  break;
case 'd':
currentstate=51; cachestring+='d';  break;
case 'e':
currentstate=51; cachestring+='e';  break;
case 'f':
currentstate=51; cachestring+='f';  break;
case 'g':
currentstate=51; cachestring+='g';  break;
case 'h':
currentstate=51; cachestring+='h';  break;
case 'i':
currentstate=51; cachestring+='i';  break;
case 'j':
currentstate=51; cachestring+='j';  break;
case 'k':
currentstate=51; cachestring+='k';  break;
case 'l':
currentstate=51; cachestring+='l';  break;
case 'm':
currentstate=51; cachestring+='m';  break;
case 'n':
currentstate=51; cachestring+='n';  break;
case 'o':
currentstate=51; cachestring+='o';  break;
case 'p':
currentstate=51; cachestring+='p';  break;
case 'q':
currentstate=51; cachestring+='q';  break;
case 'r':
currentstate=51; cachestring+='r';  break;
case 's':
currentstate=51; cachestring+='s';  break;
case 't':
currentstate=51; cachestring+='t';  break;
case 'u':
currentstate=51; cachestring+='u';  break;
case 'v':
currentstate=51; cachestring+='v';  break;
case 'w':
currentstate=51; cachestring+='w';  break;
case 'x':
currentstate=51; cachestring+='x';  break;
case 'y':
currentstate=51; cachestring+='y';  break;
case 'z':
currentstate=51; cachestring+='z';  break;
default: cout<<"error at line:"<<linecount<<endl;}
break;
case 99:
switch (c) {
case '/':
currentstate=136; cachestring+='/';  break;
default: cout<<"error at line:"<<linecount<<endl;}
break;
case 136:
switch (c) {
default: handleToken(6); isGotOne=1;}
break;
}if(!isOver)if(!isGotOne)c=input.get();}}
void handleToken(int k){
if(tokens[k]!="ID"){
cout<<"{"<<tokens[k]<<" ,"<<cachestring<<"}"<<endl;
}else{
for (int i=0;i<keywordslength;i++) {
if(cachestring==keywords[i]){
cout<<"{"<<keywords[i]<<"}"<<endl;
cachestring="";currentstate=0;
return;}}
cout<<"{"<<tokens[k]<<" ,"<<cachestring<<"}"<<endl;}
cachestring="";currentstate=0;}
