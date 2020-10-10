#include<stdlib.h>
#include<iostream>
#include<string>
#include<string.h>
#include<vector>
using namespace std;

void split_input(string str,vector<string> &tuple){
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	char *p = strtok(cstr, " ");
	while (p != 0){
		tuple.push_back(p);
		p = strtok(NULL, " ");
	}
}
int main(){
    string input;
    while(1){
        vector<string> s_input;        
        cout << "%";
        getline(cin,input);
        split_input(input,s_input);
        for(int i=0;i<s_input.size();i++)
            cout <<s_input[i]<< endl;
        //cout << input << endl;

    }
}