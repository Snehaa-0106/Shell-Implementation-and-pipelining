#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <sys/wait.h>
//all header files that are useful for this program arfe listed above

#define MAX_TOKENS 100
#define MAX_STRING_LEN 100
size_t MAX_LINE_LEN = 10000;
#define EXIT_STR "exit"
#define EXIT_CMD 0
#define UNKNOWN_CMD 99

FILE *fp; // file struct for stdin
char **tokens;
char *line;
int c=1,j=0,pid,input,output,ret,status;
void initialize()
{// allocate space for the whole line
	assert( (line = malloc(sizeof(char) * MAX_STRING_LEN)) != NULL);
	// allocate space for individual tokens
	assert( (tokens = malloc(sizeof(char*)*MAX_TOKENS)) != NULL);
	// open stdin as a file pointer
	assert( (fp = fdopen(STDIN_FILENO, "r")) != NULL);
}
//the below function is used to tokenize the entered sting and pass them as tokens which then be useful for piping and redirection.
char** tokenize (char * string)
{
	int into_file=1,out_to_file=1,z1,z2,n=1,count=0;
  char samp[10][20], samp1[10][20];
	int token_count = 0;
	int size = MAX_TOKENS;
	char *this_token;
	pid_t pid;
	pid = fork();
	int q=0,w=0,indices;
	while ((this_token = strsep( &string, " \t\v\f\n\r")) != NULL)//separates the entered string into tokens
	{
		if (*this_token == '\0') continue;
		tokens[token_count] = this_token;
    token_count++;//everytime the flow enters this loop, the token_count would be incremented inorder to know the total count of tokens
		if(token_count >= size)//just in case if the size exceeds
		{
			size*=2;
			assert ( (tokens = realloc(tokens, sizeof(char*) * size)) != NULL);
		}
	}
	int i;
  for(i =0; i< token_count;i++)
  {
  	if(strcmp(tokens[i],"<")==0)//comparing if any of the tokens are matching with this for redirection
  	{
      z1 = i;
  		into_file--;//if true, then decrement this variable
  		for(i =0; i<z1;i++)
  		{
  			strcat(samp[i],tokens[i]);
  			n++;
  		}
  	}
  	if(strcmp(tokens[i],">")==0)//comparing if any of the tokens are matching with this for redirection
  	{
  		z2= i;
  		out_to_file--;//if true then decrement this variable
  	}
  }
	if(pid<0)
	{
		perror("fork failed:");
		exit(1);
	}
	if(pid==0)
	{
	 if(into_file==0)
	 {
			input = open(tokens[token_count-1],O_RDONLY);//if this variable that was set previously is true , then do the following read only operation
			close(0);
			dup2(input,0);
			close(input);
			tokens[token_count-2] = NULL;
			execvp(tokens[0],tokens);
			exit(0);
		}
		if (out_to_file==0)
		{
			output = creat(tokens[token_count-1],0644);//if this variable that was set previously is true , then do the following operation
			close(0);
			close(1);
			dup2(output,1);
			close(output);
			tokens[z2] = NULL;
			execvp(tokens[0],tokens);
			exit(0);
		}
		else
		{
			tokens[token_count] = NULL; //if nothing is true, then there is no token for redirection
			execvp(tokens[0],tokens);
			exit(0);
		}
	}
	if(pid>0)
	{
		ret = waitpid(pid,&status,0);
	}
	for(int i = 0; i<token_count;i++)
	{
	  if(strcmp(tokens[i],"|")==0)//check if any of the token has a pipe for piping .
	  {
	    indices = i;
	    w++;
	    q = 1;
	  }
	}
	if(w==1)
	{ int i,j=indices;
	  int y = token_count-1;
	  for(i=y;i>indices;i--)
	  {
	    count++;
	  }
	  for(i=0;i<=count;i++)
	  {
	    strcpy(samp1[i],tokens[j]);
	    j++;
	  }
	}
	if(w != 2)
	{
	  if(q==1)
	  {
	  int fd[2];
	  pipe(fd);
	  int pid1 = fork();
	  int pid2 = fork();
	  if(pid2==0)
	  {
	    int i,j = indices;
	    for(i = 0;i<=count;i++)
	    {
	      tokens[i] = tokens[j+1];
	      j++;
	    }
	    close(0);
	    dup2(fd[0],0);
	    close(fd[1]);
	    tokens[count+1] = NULL;
	    execvp(tokens[0],tokens);
	    exit(0);
	  }
	  if(pid1 == 0)
	  {
	    close(1);
	    dup2(fd[1],1);
	    close(fd[0]);
	    tokens[indices]= NULL;
	    execvp(tokens[0],tokens);
	    exit(0);
	  }
	  close(fd[0]);
	  close(fd[1]);
		int i;
	  for(i=0;i<2;i++)
	  {
	    pid = wait(&status);
	  }
	 }
 }return 0;
 }
void read_command()
{
	// getline will reallocate if input exceeds max length
	assert( getline(&line, &MAX_LINE_LEN, fp) > -1);
	tokenize(line);
}
int run_command() {
	if (strcmp( tokens[0], EXIT_STR ) == 0)
		return EXIT_CMD;
	return UNKNOWN_CMD;
}
int main()
{
  initialize();
  do {
    printf("mysh> ");
    read_command();
  } while( run_command() != EXIT_CMD );
  return 0;
}
