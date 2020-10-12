#include<unistd.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<wait.h>
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
    //setenv("PATH","./,bin/",1);
    setenv("PATH","bin",1);
    //cout << "PATH=" << getenv("PATH")<<endl;

    while(1){
        vector<string> s_input;        
        cout << "%";
        getline(cin,input);
        split_input(input,s_input);
        
        if (s_input.size()<1)
            continue;
        if (s_input[0]=="printenv"){
            cout << getenv(s_input[1].c_str()) << endl;
            continue;
        }
        else if (s_input[0]=="setenv"){
            setenv(s_input[1].c_str(),s_input[2].c_str(),1);
            continue;
        }

        int i = 0;
        vector<string> tmp_input;

        int pipeline[1000][2];//maximum command = 1000
        int pipe_count = -1;
        while(i<s_input.size()){

            if (s_input[i]=="|" || i == s_input.size()-1){
                
                if(i == s_input.size()-1)
                    tmp_input.push_back(s_input[i]);
           
/*
                for (int j=0;j<tmp_input.size();j++){
                    cerr << tmp_input[j]<<'\t';
                }
*/                               
                pipe_count += 1 ;

                if(pipe(pipeline[pipe_count])<0)
                    cerr << "create pipe error" << std::endl;
                
/*
                cerr << pipeline[pipe_count][0] << '\t';
                cerr << pipeline[pipe_count][1] << endl;
*/            

                if(fork()==0){//child
                    if (pipe_count != 0)
                        dup2(pipeline[pipe_count-1][0],STDIN_FILENO); 
                    if (i != s_input.size()-1)
                        dup2(pipeline[pipe_count][1],STDOUT_FILENO);
/*
                    cerr << pipeline[pipe_count-1][0] << '\t';
                    cerr << pipeline[pipe_count-1][1] << '\t';
                    cerr << pipeline[pipe_count][0] << '\t';
                    cerr << pipeline[pipe_count][1] << endl;
*/                   
                    
                    close(pipeline[pipe_count-1][0]);
                    close(pipeline[pipe_count-1][1]);
                    close(pipeline[pipe_count][0]);
                    close(pipeline[pipe_count][1]);

                    char* arg[tmp_input.size()+1];
                    for(int j=0;j<tmp_input.size();j++){
                        arg[j] = (char*)(tmp_input[j].c_str());
                    }
                    arg[tmp_input.size()] = NULL;
                    execvp(arg[0],arg);
                }
                else{//paraent
                    
                }
                tmp_input.clear();
            }
            else{
                tmp_input.push_back(s_input[i]);  
            } 
            i++;          
        }
        int status;
        pid_t waitpid;
        waitpid = wait(&status);
    }


}