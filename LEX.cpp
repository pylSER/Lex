//
//  LEX.cpp
//  Lex
//
//  Created by 裴玉林 on 2016/10/24.
//  Copyright ? 2016年 裴玉林. All rights reserved.
//
#include <iostream>
#include <vector>
#include <string>
#include <ctype.h>
#include <stack>
#include<set>
#include <fstream>
#include <stdlib.h>

using namespace std;

string lfilepath="./test.l";
string targetfilepath="./lexRendered.cpp";
string testfilepath="./test.txt";

struct edge {
    int from;
    int to;
    char symbol;
};

struct State{
    int id;
    set<int> kernel;
    vector<int> other;
};

vector<State> cacheKernel;  //optimize nfa->dfa algo
vector<int> newvertexList;
vector<string> keywords;
vector<string> tokens;
vector<string> regulars;
vector<int> tokentable;


class NFA{
public:

    NFA(){
        startnum=0;
        tokennum=0;
    }
    vector<int> vertexList;
    vector<edge> edges;
    vector<int> finalStates;
    int startnum;
    int tokennum;

    vector<int> tokentable;




    void addVerticesNum(int num){
        for (int i=0; i<num; i++) {
            vertexList.push_back(i);
        }
    }

    int getVerticesNum(){
        return vertexList.size();
    }

    void addAVertex(){
        vertexList.push_back(vertexList.size());
    }



    void addAnEdge(int from,int to,char symbol){
        edge e;
        e.from=from;
        e.to=to;
        e.symbol=symbol;
        edges.push_back(e);
    }


    int getStartNum(){//获得起始状态的标号
        return startnum;
    }


    void setStartNum(int num){
         startnum=num;
    }

    vector<int> getFinalNum(){//获得终止状态的标号
        return finalStates;
    }
    void setFinalNum(vector<int> a){
        finalStates=a;
    }

    void showNFA(){
        edge eachedge;
        for (int i=0; i<edges.size(); i++) {
            eachedge=edges.at(i);
            cout<<"State"<<eachedge.from<<"-----"<<eachedge.symbol<<"------>"<<"State"<<eachedge.to<<endl;
        }


        cout<<"State"<<getStartNum()<<" is Start Node."<<endl;

        cout<<"State";
        for (int i=0; i<finalStates.size(); i++) {
            int x=finalStates.at(i);
            cout<<x<<"   ";
        }

        cout<<" are final States"<<endl;
    }

};


NFA finaldfa;
int isNotOperator(char c);
string addDots(string re);
string infixToPrefix(string redots);
int getPushValue(char c);
int isOperator(char c);
NFA connectNFA(NFA firstnfa,NFA secondnfa);
NFA closure(NFA nfa);
NFA branch(vector<NFA> nfaList);
NFA readREandBuildNFA(string re,int tokennum);
NFA buildASimpleNFA(char symbol);
NFA convertNFA2DFA(NFA nfa);
vector<int> findEpsilonClosure(NFA nfa, int startState);

void showVector(vector<int>);

set<char> getAllSymbols(NFA nfa);

set<int> getStateKernel(NFA nfa, State startstate,char symbol);

int isInVector(int x,vector<int> v);

int isInSet(int x,set<int> s);

int isInKernelCache(set<int> s);
void showSet(set<int> s);
NFA optimizeDFA(NFA dfa,set<char> symbols);
void showDFA(vector<State> states,set<char> symbols);


void renderCFile(NFA mdfa);

void readLFile();

NFA addNewNFA(vector<NFA> nfaList);


int main(){

    readLFile();
    vector<NFA> nfalist;
    for (int i=0; i<regulars.size(); i++) {
        string re=regulars[i];
        string realre=infixToPrefix(addDots(re));
        NFA n=readREandBuildNFA(realre,i);
        nfalist.push_back(n);
    }

    NFA nfax=addNewNFA(nfalist);

    cout<<"Here is NFA (all regular expressions integrated)"<<endl;
    nfax.showNFA();

//    showVector(nfax.tokentable);
//
    NFA mdfa=convertNFA2DFA(nfax);
//
    renderCFile(mdfa);

    return 0;
}

NFA connectNFA(NFA firstnfa,NFA secondnfa){

    int firstsize=firstnfa.vertexList.size();
    int secondsize=secondnfa.vertexList.size();

    NFA res=firstnfa;
    res.addVerticesNum(secondsize);

    res.addAnEdge(firstnfa.getFinalNum()[0], secondnfa.getStartNum()+firstsize, '@');
    for (int i=0; i<secondnfa.edges.size(); i++) {
        res.addAnEdge(secondnfa.edges[i].from+firstsize, secondnfa.edges[i].to+firstsize, secondnfa.edges[i].symbol);
    }


    vector<int> temp;
    temp.push_back(secondnfa.getFinalNum()[0]+firstsize);

    res.setFinalNum(temp);

    return res;
}



NFA closure(NFA nfa){

    NFA a;
    a.addAVertex();
    vector<int> final1;
    final1.push_back(0);
    a.setFinalNum(final1);

    nfa.addAnEdge(nfa.getFinalNum()[0],nfa.getStartNum(),'@');

    NFA temp1=connectNFA(a, nfa);

    NFA temp2=connectNFA(temp1, a);

    temp2.setStartNum(a.getStartNum());
    vector<int> final2;
    final2.push_back(temp2.getVerticesNum()-1);
    temp2.setFinalNum(final2);

    temp2.addAnEdge(temp2.getStartNum(), temp2.getFinalNum()[0], '@');

    return temp2;
}



NFA branch(vector<NFA> nfaList){ //第一行加1

    NFA res;
    int totalsize=0;
    for (int i=0; i<nfaList.size(); i++) {
        totalsize+=nfaList[i].getVerticesNum();
    }

    res.addVerticesNum(totalsize+2);

    vector<int> final2;
    final2.push_back(totalsize+1);
    res.setFinalNum(final2);


   int firstnfasize=nfaList[0].getVerticesNum();

    for (int i=0;i<nfaList.size(); i++) {
        NFA currnfa=nfaList[i];
        if (i==0) {
            for (int j=0; j<currnfa.edges.size(); j++) {
                res.addAnEdge(currnfa.edges[j].from+1, currnfa.edges[j].to+1, currnfa.edges[j].symbol);
            }
            res.addAnEdge(res.getStartNum(), currnfa.getStartNum()+1, '@');
            res.addAnEdge(currnfa.getFinalNum()[0]+1, res.getFinalNum()[0], '@');
        }else{
            for (int j=0; j<currnfa.edges.size(); j++) {
                res.addAnEdge(currnfa.edges[j].from+firstnfasize*i+1, currnfa.edges[j].to+firstnfasize*i+1, currnfa.edges[j].symbol);
            }
            res.addAnEdge(res.getStartNum(), currnfa.getStartNum()+firstnfasize*i+1, '@');
            res.addAnEdge(currnfa.getFinalNum()[0]+firstnfasize*i+1, res.getFinalNum()[0], '@');
        }
    }

    return res;
}



int isNotOperator(char c){
    if (isdigit(c)||isalpha(c)) {
        return 1;
    }else{
        return 0;
    }
}

int isOperator(char c){
    if (c=='.'||c=='*'||c=='|') {
        return 1;
    }
    return 0;
}

string infixToPrefix(string redots){
    stack<char> s;
    string res="";
    for (int i=0; i<redots.length(); i++) {
        char x=redots[i];
        if (isOperator(x)==1||x=='(') {
            if (s.empty()) {
                s.push(x);
                continue;
            }
            char top=s.top();
            if (x=='(') {
                s.push(x);
                continue;
            }
            while (getPushValue(top)>=getPushValue(x)) {
                if (top!=0) {
                    res+=top;
                }

                if (s.empty()) {
                    break;
                }
                s.pop();
                if (s.empty()) {
                    break;
                }
                top=s.top();
            }

            s.push(x);


        }else if (x==')'){
            char top=s.top();
            s.pop();
            while (top!='(') {
                if (top!=0) {
                    res+=top;
                }


                top=s.top();
                s.pop();
                if (s.empty()) {
                    break;
                }

            }
        }
        else{
            if (x!=0) {
                res+=x;
            }
        }
    }
    while (!s.empty()) {
        if (s.top()!=0) {
            res+=s.top();
        }
        s.pop();
    }
    return res;
}



string addDots(string re){
    char dot='.';

    for (int i=0; i<re.length(); i++) {
        char first=re[i];
        char second=re[i+1];
        if((first=='(')&&(isOperator(second)==0)){

        }else if ((second==')')&&(isOperator(first)==0)){

        }else if ( (isOperator(first)==0)&&(second=='|') ){

        }else if ( (isOperator(second)==0)&&(first=='|')  ){

        }else if (second=='*'){

        }else if (first=='*'&&second==')'){

        }else if (first=='('&&second=='('){

        }else if (first==')'&&second==')'){

        }else if (first=='*'&&second=='|'){

        }
        else{
            re.insert(i+1,1,dot);
            i++;
        }
    }

    re[re.length()-1]=0;
//    cout<<re<<endl;
    return re;
}


int getPushValue(char c){
       // int value;
        switch (c) {
            case '|': return 1;
            case '.': return 2;
            case '*': return 3;
            case ')': return 4;
            case '(' :return 0;
        }
        return 0;
}


NFA readREandBuildNFA(string re,int tokennum){

    stack<NFA> postfixstack;

    for (int i=0; i<re.length(); i++) {
        if (isOperator(re[i])==0) {
            postfixstack.push(buildASimpleNFA(re[i]));

        }else{
            if (re[i]=='.') {
                NFA second=postfixstack.top();
                postfixstack.pop();



                NFA first=postfixstack.top();
                postfixstack.pop();

                postfixstack.push(connectNFA(first, second));


            }else if (re[i]=='|'){
                NFA second=postfixstack.top();
                postfixstack.pop();
                NFA first=postfixstack.top();
                postfixstack.pop();
                vector<NFA> list;
                list.push_back(first);
                list.push_back(second);

                postfixstack.push(branch(list));

            }else if(re[i]=='*'){
                NFA first=postfixstack.top();
                postfixstack.pop();

                postfixstack.push(closure(first));
            }
        }
    }

    postfixstack.top().tokennum=tokennum;
    return postfixstack.top();
}

NFA buildASimpleNFA(char symbol){
    NFA nfa;
    nfa.addAVertex();
    nfa.addAVertex();
    nfa.addAnEdge(0, 1, symbol);
    vector<int> final;
    final.push_back(1);
    nfa.setFinalNum(final);
    return nfa;
}


NFA convertNFA2DFA(NFA nfa){

    vector<State> states;

    int currentmax=0;
    int currentrow=0;



    State start;
    start.id=0;
    start.kernel.insert(nfa.getStartNum());
    start.other=findEpsilonClosure(nfa, nfa.getStartNum());

    states.push_back(start);
    cacheKernel.push_back(start);

    set<char> symbols=getAllSymbols(nfa);

    set<char>::iterator it; //定义前向迭代器
    //中序遍历集合中的所有元素


    while (currentmax*symbols.size()+1>=states.size()||currentmax==0) {


    for(it=symbols.begin();it!=symbols.end();it++){

        char sy=(*it);

//        cout<<"getting kernel for State"<<cacheKernel[currentrow].id<<" "<<sy<<endl;
        set<int> kernelset= getStateKernel(nfa, cacheKernel[currentrow],sy);

//        cout<<"ksets";
//        showSet(kernelset);


        if (kernelset.size()==0) {
            State emptystate;
            emptystate.id=-1;
            states.push_back(emptystate);
            continue;
        }


        int num=isInKernelCache(kernelset);
        if (num>=0) {
            states.push_back(cacheKernel[num]);
        }else{
            currentmax++;
            State newstate;
            newstate.id=currentmax;
            newstate.kernel=kernelset;

            vector<int> other;

            set<int>::iterator iter;
            for (iter=kernelset.begin(); iter!=kernelset.end(); iter++) {
                vector<int> v=findEpsilonClosure(nfa, (*iter));
                other.insert(other.end(),v.begin(),v.end());
            }
            newstate.other=other;

            states.push_back(newstate);
            cacheKernel.push_back(newstate);

        }

    }
        currentrow++;
    }


    showDFA(states,symbols);

    //now build dfa
    NFA dfa;
    dfa.addVerticesNum((states.size()-1)/(symbols.size()));


    int statenum=(states.size()-1)/symbols.size();
    for (int i=0; i<statenum; i++) {

        set<char>::iterator it;
        int j=0;

        for (it=symbols.begin();it!=symbols.end();it++) {

            if (states[i*symbols.size()+j+1].id!=-1) {
                dfa.addAnEdge(i, states[i*symbols.size()+j+1].id, (*it));
            }
            j++;
        }
    }



     vector<int> finalstates;
    for (int k=0; k<nfa.getFinalNum().size(); k++) {


    int final=nfa.getFinalNum()[k];



    for (int i=0; i<states.size(); i++) {
        if ( (isInVector(final, states[i].other)==1)||
            (isInSet(final ,states[i].kernel)==1)  ) {


            if (!isInVector(states[i].id, finalstates)) {
                finalstates.push_back(states[i].id);
                dfa.tokentable.push_back(k);
            }

        }
    }
    }

    dfa.setFinalNum(finalstates);

    dfa.showNFA();
//    showVector(dfa.tokentable);

    optimizeDFA(dfa,symbols);


    return dfa;
}


vector<int> findEpsilonClosure(NFA nfa, int startState){//return vector contains startState
    vector<int> res;
    res.push_back(startState);

    int pointer=0;

    while (pointer<res.size()) {


    for (int i=0; i<nfa.edges.size(); i++) {
        if (nfa.edges[i].from==res[pointer]&&nfa.edges[i].symbol=='@') {
            res.push_back(nfa.edges[i].to);
        }
    }
        pointer++;
    }


    return res;
}


void showVector(vector<int> v){
    for (int i=0; i<v.size(); i++) {
        cout<<v[i]<<", ";
    }
    cout<<endl;
}

void showSet(set<int> s){
    set<int>::iterator iter;
    for(iter=s.begin();iter!=s.end();iter++){
        cout<<(*iter)<<", ";
    }
    cout<<endl;

}


set<char> getAllSymbols(NFA nfa){
    set<char> res;
    for (int i=0; i<nfa.edges.size(); i++) {
        if (nfa.edges[i].symbol!='@') {
            res.insert(nfa.edges[i].symbol);
        }
    }

    return res;
}


set<int> getStateKernel(NFA nfa, State startstate,char symbol){
    set<int> res;

    for (int i=0; i<nfa.edges.size(); i++) {
        if ( (isInVector(nfa.edges[i].from, startstate.other)==1)||
            (isInSet(nfa.edges[i].from, startstate.kernel)==1) ) {

            if (symbol==nfa.edges[i].symbol) {
                res.insert(nfa.edges[i].to);
            }
        }
    }
    return res;
}


int isInVector(int x,vector<int> v){
    for (int i=0; i<v.size(); i++) {
        if (x==v[i]) {
            return 1;
        }
    }
    return 0;
}


int isInSet(int x,set<int> s){
    set<int>::iterator it;
    //中序遍历集合中的所有元素
    for(it=s.begin();it!=s.end();it++){
        if (x==(*it)) {
            return 1;
        }
    }
    return 0;

}


int isInKernelCache(set<int> s){
    for (int i=0; i<cacheKernel.size(); i++) {
        if (s==cacheKernel[i].kernel) {
            return i;
        }
    }
    return -1;
}



NFA optimizeDFA(NFA dfa,set<char> symbols){
    vector<int> end;
    vector<int> nonend;

    vector<edge> endedge;
    vector<edge> nonendedge;

    vector<int> remove;

    for (int i=0; i<cacheKernel.size(); i++) {

        if (isInVector(cacheKernel[i].id, dfa.getFinalNum())) {
            end.push_back(cacheKernel[i].id);
        }else{
            nonend.push_back(cacheKernel[i].id);
        }
    }





    for (int i=0; i<dfa.edges.size(); i++) {
        if (isInVector(dfa.edges[i].from, dfa.getFinalNum())) {
            endedge.push_back(dfa.edges[i]);
        }else{
            nonendedge.push_back(dfa.edges[i]);
        }
    }


    for (int i=0; i<endedge.size(); i++) {
        int dest=endedge[i].to;
        char s=endedge[i].symbol;


        for (int j=i+1; j<endedge.size(); j++) {

            if (endedge[j].to==dest&&endedge[j].symbol==s) {
                int removenode=endedge[j].from;
                vector<edge>::iterator it;
                int k=0;
                for (it=dfa.edges.begin(); it!=dfa.edges.end();it++) {
                    if (k>=dfa.edges.size()) {
                        break;
                    }
                    if ((*it).from==removenode) {
                        dfa.edges.erase(it);

                        if (dfa.edges.size()<=1) {
                            break;
                        }
                    }else if((*it).to==removenode){
                        (*it).to=endedge[i].from;
                    }
                    k++;
                }

            }
        }
    }


    for (int i=0; i<nonendedge.size(); i++) {
        int dest=nonendedge[i].to;
        char s=nonendedge[i].symbol;


        for (int j=i+1; j<nonendedge.size(); j++) {
            if (nonendedge[j].to==dest&&nonendedge[j].symbol==s) {
                int removenode=nonendedge[j].from;
                vector<edge>::iterator it;

                int k=0;
                for (it=dfa.edges.begin(); it!=dfa.edges.end();it++) {
                    if (k>=dfa.edges.size()) {
                        break;
                    }
                    if ((*it).from==removenode) {
                        dfa.edges.erase(it);
                    }else if((*it).to==removenode){
                        (*it).to=nonendedge[i].from;
                    }
                    k++;
                }

            }
        }
    }



    for (int i=0; i<dfa.edges.size(); i++) {
        if (isInVector(dfa.edges[i].from, newvertexList)==0) {
            newvertexList.push_back(dfa.edges[i].from);
        }
        if (isInVector(dfa.edges[i].to, newvertexList)==0) {

            newvertexList.push_back(dfa.edges[i].to);
        }
    }


    cout<<"Here is optimized DFA"<<endl;
    dfa.showNFA();

    finaldfa=dfa;

    return dfa;
}


void showDFA(vector<State> states,set<char> symbols){

    cout<<"HERE is DFA:"<<endl;

    //SHOW in DETAIL

//    for (int i=0; i<states.size(); i++) {
//        cout<<"I"<<states[i].id<<endl;
//        cout<<"kernel:";
//        showSet(states[i].kernel);
//        cout<<"other:";
//        showVector(states[i].other);
//    }

    //SHOW in GRAPH
    int statenum=(states.size()-1)/symbols.size();
    for (int i=0; i<statenum; i++) {

        set<char>::iterator it;
        int j=0;

        for (it=symbols.begin();it!=symbols.end();it++) {

            if (states[i*symbols.size()+j+1].id!=-1) {
                cout<<"I"<<i<<"-----";
                cout<<(*it);
                cout<<"----->I"<<states[i*symbols.size()+j+1].id<<endl;
            }
            j++;
        }
    }
}


void renderCFile(NFA mdfa){


    ofstream fout;
    fout.open(targetfilepath.c_str());
//
////
    fout<<"#include<iostream>"<<endl;
    fout<<"#include<fstream>"<<endl;
    fout<<"#include<string>"<<endl;
    fout<<"#include<ctype.h>"<<endl;
    fout<<"using namespace std;"<<endl;
    fout<<"int linecount=1;"<<endl;
    fout<<"int currentstate=0;"<<endl;
    fout<<"int keywordslength="<<keywords.size()<<";"<<endl;
    fout<<"int isOver=0;"<<endl;
    fout<<"int isGotOne=0;"<<endl;
    fout<<"string cachestring;"<<endl;
    fout<<"void handleToken(int k);"<<endl;

    fout<<"string tokens[]={";
    for (int i=0; i<tokens.size(); i++) {
        if (i==tokens.size()-1) {
            fout<<"\""<<tokens[i]<<"\"};"<<endl;
            break;
        }
        fout<<"\""<<tokens[i]<<"\",";
    }

    fout<<"string keywords[]={";
    for (int i=0; i<keywords.size(); i++) {
        if (i==keywords.size()-1) {
            fout<<"\""<<keywords[i]<<"\"};"<<endl;
            break;
        }
        fout<<"\""<<keywords[i]<<"\",";
    }


    fout<<"int main(){"<<endl;
    fout<<"ifstream input;"<<endl;
    fout<<"input.open("<<testfilepath<<");"<<endl;
    fout<<"int c=input.get();"<<endl;
    fout<<"while (!isOver) {isGotOne=0;"<<endl;
    fout<<"if (c==-1) isOver=1;"<<endl;
    fout<<"if (c=='\\n') {linecount++;c=input.get();continue;}"<<endl;
    fout<<"switch (currentstate) {"<<endl;



    mdfa=finaldfa;


    for (int i=0; i<newvertexList.size(); i++) {
        fout<<"case "<<newvertexList[i]<<":"<<endl;
        fout<<"switch (c) {"<<endl;
        for (int j=0; j<mdfa.edges.size(); j++) {
            if (newvertexList[i]==mdfa.edges[j].from) {
                fout<<"case '"<<mdfa.edges[j].symbol<<"':"<<endl;
                fout<<"currentstate="<<mdfa.edges[j].to<<"; cachestring+='"<<mdfa.edges[j].symbol<<"';  break;"<<endl;
            }
        }
        if (isInVector(newvertexList[i], mdfa.getFinalNum())) {
            int q;
            for (q=0; q<mdfa.getFinalNum().size(); q++) {
                if (newvertexList[i]==mdfa.getFinalNum()[q]) {
                    break;
                }
            }


            fout<<"default: handleToken("<<mdfa.tokentable[q]<<"); isGotOne=1;}"<<endl;
        }else{
            fout<<"default: cout<<\"error at line:\"<<linecount<<endl;}"<<endl;
        }
        fout<<"break;"<<endl;

    }
    fout<<"}if(!isOver)if(!isGotOne)c=input.get();}}"<<endl;



    fout<<"void handleToken(int k){"<<endl;
    fout<<"if(tokens[k]!=\"ID\"){"<<endl;
    fout<<"cout<<\"{\"<<tokens[k]<<\" ,\"<<cachestring<<\"}\"<<endl;"<<endl;

    fout<<"}else{"<<endl;

    fout<<"for (int i=0;i<keywordslength;i++) {"<<endl;
    fout<<"if(cachestring==keywords[i]){"<<endl;
    fout<<"cout<<\"{\"<<keywords[i]<<\"}\"<<endl;"<<endl;
    fout<<"cachestring=\"\";currentstate=0;"<<endl;

    fout<<"return;}}"<<endl;
     fout<<"cout<<\"{\"<<tokens[k]<<\" ,\"<<cachestring<<\"}\"<<endl;}"<<endl;
    fout<<"cachestring=\"\";currentstate=0;}"<<endl;


    fout.close();

}


void readLFile(){
    string line;
    ifstream input;
    input.open(lfilepath.c_str());
    int isNextToken=0;
    while (getline(input,line)) {
//        cout<<line<<endl;
        if (isNextToken&&line[0]!='!') {
            cout<<"Wrong format of .l file!"<<endl;
            exit(0);
        }


        if(line[0]=='#'){
            keywords.push_back(line.substr(1));
        }else if(line[0]=='%'){
            regulars.push_back(line.substr(1));
            isNextToken=1;
        }else if (line[0]=='!'){
            tokens.push_back(line.substr(1));
            isNextToken=0;
        }


    }

    for (int i=0; i<keywords.size(); i++) {
        cout<<keywords[i]<<endl;
    }

    for (int i=0; i<regulars.size(); i++) {
        cout<<regulars[i]<<endl;
    }
    for (int i=0; i<tokens.size(); i++) {
        cout<<tokens[i]<<endl;
    }

}



NFA addNewNFA(vector<NFA> nfaList){
    NFA res;
    int totalsize=0;
    vector<int> final2;
    for (int i=0; i<nfaList.size(); i++) {
        totalsize+=nfaList[i].getVerticesNum();
    }

    res.addVerticesNum(totalsize+1);


    int firstnfasize=nfaList[0].getVerticesNum();

    for (int i=0;i<nfaList.size(); i++) {
        NFA currnfa=nfaList[i];
        if (i==0) {
            for (int j=0; j<currnfa.edges.size(); j++) {
                res.addAnEdge(currnfa.edges[j].from+1, currnfa.edges[j].to+1, currnfa.edges[j].symbol);
            }
            res.addAnEdge(res.getStartNum(), currnfa.getStartNum()+1, '@');

            for (int p=0; p<currnfa.getFinalNum().size(); p++) {
                res.finalStates.push_back(currnfa.getFinalNum()[p]+1);
                res.tokentable.push_back(currnfa.tokennum);
            }



        }else{
            for (int j=0; j<currnfa.edges.size(); j++) {
                res.addAnEdge(currnfa.edges[j].from+firstnfasize*i+1, currnfa.edges[j].to+firstnfasize*i+1, currnfa.edges[j].symbol);
            }
            res.addAnEdge(res.getStartNum(), currnfa.getStartNum()+firstnfasize*i+1, '@');

            for (int p=0; p<currnfa.getFinalNum().size(); p++) {
                res.finalStates.push_back(currnfa.getFinalNum()[p]+firstnfasize*i+1);
                res.tokentable.push_back(currnfa.tokennum);
            }
        }

    }

    return res;

}
