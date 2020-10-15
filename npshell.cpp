#include<unistd.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<wait.h>
#include<fcntl.h>

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

void write_to_file(char *filename){
    int fd;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    fd = creat(filename, mode);

    if (fd == -1){
        cerr << "create file error.\n";
    }
    dup2(fd,STDOUT_FILENO);
}

void close_pipe(int pipeline[][2],int pipe_count){
    close(pipeline[pipe_count-1][0]);
    close(pipeline[pipe_count-1][1]);
    close(pipeline[pipe_count][0]);
    close(pipeline[pipe_count][1]);
}

void excute_cmd(vector<string> s_input){
    int i = 0;
    vector<string> tmp_input;

    int pipeline[1000][2];//maximum command = 1000
    int pipe_count = -1;
    while(i<s_input.size()){
        if (s_input[i]=="|" || s_input[i]==">" || i == s_input.size()-1){

            if(i == s_input.size()-1)
                tmp_input.push_back(s_input[i]);                       
            pipe_count += 1 ;

            if(pipe(pipeline[pipe_count])<0)
                cerr << "create pipe error" << std::endl;         

            if(fork()==0){//child
                cerr << STDIN_FILENO << "\t" << STDOUT_FILENO << endl;

                cerr << pipeline[pipe_count][0] << "\t" << pipeline[pipe_count][1] << endl<< endl;
                if (pipe_count != 0)
                    dup2(pipeline[pipe_count-1][0],STDIN_FILENO); 
                if (i != s_input.size()-1)
                    dup2(pipeline[pipe_count][1],STDOUT_FILENO);
                if(s_input[i]==">"){
                    write_to_file((char*)(s_input[i+1].c_str()));    
                }                                      

                close_pipe(pipeline,pipe_count);

                char* arg[tmp_input.size()+1];
                for(int j=0;j<tmp_input.size();j++){
                    arg[j] = (char*)(tmp_input[j].c_str());
                }

                arg[tmp_input.size()] = NULL;
                execvp(arg[0],arg);
                cerr << arg[0] << " is BAKANONO" << endl; //unknow commend
                exit(1);
            }
            else{//paraent
                if(s_input[i]==">")
                    i++;
                int status;
                pid_t waitpid;
                //waitpid = wait(&status);
                
                if (pipe_count != 0){
                    close(pipeline[pipe_count-1][0]);
                    close(pipeline[pipe_count-1][1]);
                } 
                if (i == s_input.size()-1 && s_input[i][0] != '|'){
                    close(pipeline[pipe_count][0]);
                    close(pipeline[pipe_count][1]);
                }
            }
            tmp_input.clear();
        }
        else{
            tmp_input.push_back(s_input[i]);  
        } 
        i++;          
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
            cerr << getenv(s_input[1].c_str()) << endl;
            continue;
        }
        else if (s_input[0]=="setenv"){
            setenv(s_input[1].c_str(),s_input[2].c_str(),1);
            continue;
        }

        excute_cmd(s_input);
    }
}