//Includes here

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
//Defining true here for while loop

#define True 1

//Headers

void type_prompt();
void parse_input(char* input, char* commands[], char* arguments[512][512], int* rows, int* pipecount, int* and);
void sig_handler(int unused);
//Main Program

int main(int argc,char **argv ){

struct sigaction zombie;

zombie.sa_handler = sig_handler;
zombie.sa_flags = SA_RESTART | SA_NOCLDSTOP;

sigemptyset(&zombie.sa_mask);


int j = 0;

//While loop

while(True){
if (argc == 2 && strcmp(argv[1],"-n")==0){

}else{
type_prompt();
}

//Variables
 
char* commands[512] = {};
char* arguments[512][512];
char input[512];
int rows;

// End of variables


if(fgets(input, 512,stdin) == NULL) {//SEG FAULT
printf("\n");
break;

}
input[strcspn(input, "\n")] = 0;

// THIS FINDS OUT HOW MANY MORE SPACES I WILL NEED FOR NEW ARRAY
        int inputsize = strlen(input);
        int i = 0;
        int count = 0;
        for (i=1; i<inputsize; i++){
        if (input[i] == '<' || input[i] ==  '>' || input[i] == '|'){
                if(input[i-1] != ' ' && input[i+1] == ' '){

        count++;
}
        else if(input[i-1] == ' ' && input[i+1] !=  ' '){

        count++;
}else if(input[i-1] != ' ' && input[i+1] !=  ' '){

        count += 2;
}


}

}

for (i=1; i<inputsize; i++){
        if (input[i] == '&'){
                if(input[i-1] != ' ' && input[i+1] == ' '){
        count++;
}

}

}







// END OF FINDING OUT HOW MANY MORE SPACES

// THIS MAKES THE NEW ARRAY WITH PROPERLY SPACED INPUT
//char newinput[inputsize+count];
char newinput[800];
i=0;
newinput[i] = input[i];
int inputi = 1;

        for (i=1; i<inputsize; i++){
        if (input[i] == '<' || input[i] ==  '>' || input[i] == '|' || input[i] == '&'){
                if(input[i-1] != ' ' && input[i+1] == ' '){

        newinput[inputi] = ' ';
        inputi++;
        newinput[inputi] = input[i];
        inputi++;
}
        else if(input[i-1] == ' ' && input[i+1] !=  ' '){

        newinput[inputi] = input[i];
        inputi++;
        newinput[inputi] = ' ';
        inputi++;
}else if(input[i-1] != ' ' && input[i+1] !=  ' '){

        newinput[inputi] = ' ';
        inputi++;
        newinput[inputi] = input[i];
        inputi++;
        newinput[inputi] = ' ';
        inputi++;
}else{

newinput[inputi] = input[i];
inputi++;

}


}
else{
newinput[inputi] = input[i];
inputi++;
}
}

int and = 0;
int pipecount;
// END OF MAKING NEW ARRAY
parse_input(newinput,commands, arguments, &rows, &pipecount, &and);

// NEED TO PUT IN THE EXAMPLE OF GREP O < README > TEST4.TXT


/*
for (i=0; i<rows; i++){

	printf("%s",commands[rows]);
	printf("%s", arguments[rows][0]);
}
*/
/*
for(i=0; i<inputsize; i++){
	input[i] = ' ' ;
}
for(i=0; i<inputsize+count; i++){
       newinput[i] = ' ';
}
i=0;
*/
/*
int arglen3;

for(j=0; j<rows; j++){
i = 0;
while(arguments[j][i] != NULL){
        arglen3++;
        i++;
}
//printf("%d\n",arglen);
for (i = 0; i<arglen3; i++){
        printf("%s ",arguments[j][i]);

}
printf("\n");
arglen3 = 0;
}
*/

//printf("%s", commands[0]);
int status;
//fflush(stdout);
int l = 0;
int commandcnt = 0;
int fd[pipecount][2];
int pipeind = 0;
pid_t child1;

// Handing multiple commands:
//printf("%d", rows);
for(i=0; i<rows; i++){
        if(i == rows-1 && i == 0){
		fflush(stdout);
                pid_t child = fork();
                if (child < 0){
                printf("Error: Fork Failed");
                break;
                }
                if (child == 0){
                        if(execvp(commands[commandcnt], arguments[commandcnt]) == -1){
                                printf("Error: Invalid command: %s\n", strerror(errno));
                                exit(errno);
}
                }else{
                        if(and!=1){
                        wait(&status);
                        }
//                        fflush(stdout);
                }
}else if(i == 0 && i != rows-1 && strcmp(commands[commandcnt+1], ">") != 0){



	if (strcmp(commands[commandcnt+1], "<") == 0 && (strcmp(commands[commandcnt+2], ">") == 0)){

	pipe(fd[pipeind]);

	if (access(arguments[commandcnt+2][1], F_OK) != 0) {

        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // File permissions

        int file = creat(arguments[commandcnt+2][1], mode);
        close(file);
        }

	int fdout = open(arguments[commandcnt+2][1],O_WRONLY);


        int fd1 = open(arguments[commandcnt+1][1], O_RDONLY);
//      fflush(stdout); 
       child1 = fork();
        if (child1 < 0){
                printf("Error: Fork Failed");
                break;
        }
        if (child1 == 0){
                //int fd1 = open(arguments[commandcnt+1][1], O_RDONLY);
                if (fd1 != -1) {
                   dup2(fd1, 0); // Redirect stdin
                   close(fd1);
                }
                 close(fd[pipeind][0]);
                dup2(fdout, 1);
                close(fdout);
                close(fd[pipeind][1]); // THIS MIGHT BE THE ISSUE
                if(execvp(commands[commandcnt], arguments[commandcnt]) == -1){
                                printf("Error: Invalid command: %s\n", strerror(errno));
                                exit(errno);
}


        }else{
                        if(and!=1){
                        wait(NULL);
                        }
                        
                      close(fd[pipeind][0]);
                        
                        close(fd[pipeind][1]);
                        commandcnt++;
                        close(fd1);
			close(fdout);
                        commandcnt++;
			i++;
                        i++;


                        }	



} else  if (strcmp(commands[commandcnt+1], "<") == 0){


        pipe(fd[pipeind]);
        int fd1 = open(arguments[commandcnt+1][1], O_RDONLY);
//	fflush(stdout); 
       child1 = fork();
        if (child1 < 0){
                printf("Error: Fork Failed");
                break;
        }
        if (child1 == 0){
                //int fd1 = open(arguments[commandcnt+1][1], O_RDONLY);
                if (fd1 != -1) {
                   dup2(fd1, 0); // Redirect stdin
                   close(fd1);
                }
                 close(fd[pipeind][0]);
                if (i+1 != rows-1){
                 close(1); // Closing stdout ONLY IN CHILD1s PROCESS
                 dup(fd[pipeind][1]);
//                 close(fd[pipeind][1]);
                }
                close(fd[pipeind][1]); // THIS MIGHT BE THE ISSUE
                if(execvp(commands[commandcnt], arguments[commandcnt]) == -1){
                                printf("Error: Invalid command: %s\n", strerror(errno));
                                exit(errno);
}


        }else{
                        if(and!=1){
                        wait(NULL);
                        }
                        if (i+1 == rows-1){
                      close(fd[pipeind][0]);
                        }
                        close(fd[pipeind][1]);
                        commandcnt++;
                        close(fd1);
			
                        commandcnt++;
			
                        i++;


                        }


}else{

        pipe(fd[pipeind]);
	fflush(stdout);
        child1 = fork();
        if (child1 < 0){
                printf("Error: Fork Failed");
                break;
        }
        if (child1 == 0){
                 close(fd[pipeind][0]);
                 close(1); // Closing stdout ONLY IN CHILD1s PROCESS
                 dup(fd[pipeind][1]);
                 close(fd[pipeind][1]);
                 if(execvp(commands[commandcnt], arguments[commandcnt]) == -1){
                                printf("Error: Invalid command: %s\n", strerror(errno));
                                exit(errno);
}


        }else{
			 if(and!=1){
                        wait(NULL);
                        }
//                      close(fd[pipeind][0]);
                        close(fd[pipeind][1]);
                        commandcnt++;


                        }


}
}else if((i != 0 && i!= rows-1) || (i == 0 && strcmp(commands[commandcnt+1], ">") == 0)){
        if(strcmp(commands[commandcnt+1], ">") == 0){
                if(i+1 != rows-1){
                        printf("Error: invalid input string!");
                        break;


}else{


        if (access(arguments[commandcnt+1][1], F_OK) != 0) {

        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // File permissions

        int file = creat(arguments[commandcnt+1][1], mode);
        close(file);
        }

        //printf("%s", commands[commandcnt]);
        pipeind++;
        pipe(fd[pipeind]);
        int fdout = open(arguments[commandcnt+1][1],O_WRONLY);
	fflush(stdout); 
       child1 = fork();
        if (child1 < 0){
                printf("Error: Fork Failed");
                break;
        }
        if (child1 == 0){
                 close(0);
                 dup(fd[pipeind-1][0]);
                 close(fd[pipeind-1][0]);
                //creating the next pipe


              //   close(fd[pipeind][0]);
               //  close(fd[pipeind-1][1]);//close(1); // Closing stdout ONLY IN CHILD1s PROCESS
                // dup2(fd[pipeind][1], 1);
                dup2(fdout, 1);
                close(fdout);
                //freopen(arguments[commandcnt+1][1], "w", stdout);
                 close(fd[pipeind][1]);
                close(fd[pipeind][0]);
                if(execvp(commands[commandcnt], arguments[commandcnt]) == -1){
                                printf("Error: Invalid command: %s\n", strerror(errno));
                                exit(errno);
}
 } else{
        
                        if(and!=1){
                        wait(NULL);
                        }
 //                       close(fd[pipeind][1]);
                        close(fd[pipeind][0]);
                        
                          commandcnt++;
                        close(fdout);
                        i++;
                        }
                        
        }               
}else{  
        pipeind++;
        pipe(fd[pipeind]);
	fflush(stdout);
        child1 = fork();
        if (child1 < 0){
                printf("Error: Fork Failed");
                break;
        }
        if (child1 == 0){
                 close(0);
                 dup(fd[pipeind-1][0]);
                 close(fd[pipeind-1][0]);
                //creating the next pipe


                 close(fd[pipeind][0]);
                 close(fd[pipeind-1][1]);//close(1); // Closing stdout ONLY IN CHILD1s PROCESS
                 dup2(fd[pipeind][1], 1);

                 close(fd[pipeind][1]);

                 if(execvp(commands[commandcnt], arguments[commandcnt]) == -1){
                                printf("Error: Invalid command: %s\n", strerror(errno));
                                exit(errno);
}


        } else{

  if(and!=1){
                        wait(NULL);
                        }
                        close(fd[pipeind][1]);
                         commandcnt++;
                        }

        }

}else if(i!= 0 && i == rows-1){
// In here write for the case that the next one is the last command
//              printf("%s", commands[commandcnt]);     
//              commandcnt++;   
		fflush(stdout); 
               child1 = fork();
                if (child1 < 0){
                printf("Error: Fork Failed");
                break;
               }
                if (child1 == 0){
                        close(0);
                        dup(fd[pipeind][0]);
                        close(fd[pipeind][0]);
                        if(execvp(commands[commandcnt], arguments[commandcnt]) == -1){
                                printf("Error: Invalid command: %s\n", strerror(errno));
                                exit(errno);
}
                }else{

                        if(and!=1){
                        wait(NULL);
                        }

                        close(fd[pipeind][0]);
 //                       close(fd[pipeind][1]);        

                }


}



}

// End of handing multipe commands

//printf("Gets out of loop");

int strlen1;
int k;
int arglen;
strlen1  = 0;

for(l=0; l<rows; l++){
	 strlen1 = strlen(commands[l]);
		for (j = 0; j< strlen1; j++){
                        *commands[l] = '\0';
                        commands[l]++;
}

}


j=0;
for(l=0; l<rows; l++){
arglen = 0;
k = 0;
while(arguments[l][k] != NULL){
        arglen++;
        k++;
}
        for(i = 0; i<arglen; i++){
        strlen1 = strlen(arguments[l][i]);
                for (j = 0; j< strlen1; j++){
                        *arguments[l][i] = '\0';
                        arguments[l][i]++;
}

}
}



for (i=0; i<inputsize; i++){
        input[i] = ' ' ;
}
for(i=0; i<inputsize+count; i++){
       newinput[i] = ' ';
}
i=0;





}

return 0;

}

// PROMPT FUNCTION

void type_prompt(){
        printf("my_shell$ ");
}

void sig_handler(int unused){
	pid_t child;
	while((child = waitpid(-1, NULL, WNOHANG)) > 0){
	}
}

// PARSING FUNCTION

void parse_input(char* input, char* commands[], char* arguments[512][512], int *rows, int* pipecount, int* and) {
        char* token;
        int arg_count = 0;
        int cmd_ind = 0;
        int arg_row = 0;
        int arg_col = 0;

// NOW, I TOKENIZE THE NEW INPUT ARRAY

        token = strtok(input, " \t\n");


    while (token != NULL) {

        if (strcmp(token, "|") == 0 || strcmp(token, "<") == 0 || strcmp(token, ">") == 0 || strcmp(token, "&") == 0 ){
                if(strcmp(token, "<") == 0){
                        arguments[arg_row][arg_col] = NULL;
                        arg_col = 0;
                        arg_row++;
//                      arguments[arg_row][arg_col] = "";
                        arguments[arg_row][arg_col] = token;
                        arg_col++;
//                      commands[cmd_ind] = "";
                        commands[cmd_ind] = token;
                        cmd_ind++;
                        arg_count = 0;
                }else if(strcmp(token, ">") == 0){
                        arguments[arg_row][arg_col] = NULL;
                        arg_col = 0;
                        arg_row++;
//                      arguments[arg_row][arg_col] = "";
                        arguments[arg_row][arg_col] = token;
                        arg_col++;
//                      commands[cmd_ind] = "";
                        commands[cmd_ind] = token;
                        cmd_ind++;
                        arg_count = 0;
                }else if (strcmp(token, "&") == 0){
                        // Wait on this
                        *and = 1;
                }else{ // this is the case of "|"
		arguments[arg_row][arg_col] = NULL; // Did this for execve, might need to rethink for Part2
                        pipecount++;
                        arg_row++;
                        arg_col = 0;
                        arg_count = -1;

                }
        }
        else {
            if (arg_count == 0) {

                // Need argument array to have both commands and arguments
  //                    commands[cmd_ind] = "";  
                commands[cmd_ind] = token;
        //      printf("%s", commands[0]);
//              arguments[arg_row][arg_col] = "";
                arguments[arg_row][arg_col] = token;
//                      printf("%s",arguments[arg_row][arg_col]);
                 arg_col++;
                cmd_ind++;
            } else {
                arguments[arg_row][arg_col] = "";
                arguments[arg_row][arg_col] = token;
//              printf("%s",arguments[arg_row][arg_col]);
                arg_col++;
            }
        }
        arg_count++;

        token = strtok(NULL, " ");
    }
        if(arg_row == 0){

        arguments[arg_row][arg_col] = NULL;
}
*rows = arg_row+1;
}
                                
