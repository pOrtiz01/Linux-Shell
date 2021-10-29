//+
// File:	shell.c
//
// Purpose:	This program implements a simple shell program. It does not start
//		processes at this point in time. However, it will change directory
//		and list the contents of the current directory.
//
//		The commands are:
//		   cd name -> change to directory name, print an error if the directory doesn't exist.
//		              If there is no parameter, then change to the home directory.
//		   ls -> list the entries in the current directory.
//			      If no arguments, then ignores entries starting with .
//			      If -a then all entries
//		   pwd -> print the current directory.
//		   exit -> exit the shell (default exit value 0)
//
//		if the command is not recognized an error is printed.
//-

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctype.h>

#define CMD_BUFFSIZE 1024
#define MAXARGS 10

int splitCommandLine(char * commandBuffer, char* args[], int maxargs);
void doCommand(char * args[], int nargs);
char * skipChar(char * charPtr, char skip);

int main() {

    char commandBuffer[CMD_BUFFSIZE];
    char *args[MAXARGS];


    // print prompt.. fflush is needed because
    // stdout is line buffered, and won't
    // write to terminal until newline
    printf("%%> ");
    fflush(stdout);

    while(fgets(commandBuffer,CMD_BUFFSIZE,stdin) != NULL){

        // Remove newline at end of buffer
        // TODO Step 2: remove newline from end of buffer
        commandBuffer[strlen(commandBuffer)-1] = ' ';
        // Split command line into words.
        // TODO Step 2: call splitCommandLine with the right parameters

        //Debugging for step 2
        int nargs=splitCommandLine(commandBuffer,args,MAXARGS);
        // printf("%d\n", nargs);
        // int i;
        // for (i = 0; i < nargs; i++){
        // printf("%d: %s\n",i,args[i]);
        // }

        // Execute the command
        // Remember to check if there is a command (i.e. value of nargs)
        // TODO: Step 3 call doCommand with the right arguments
        doCommand(args, nargs);

        // print prompt
        printf("%%> ");
        fflush(stdout);
    }
}

////////////////////////////// String Handling ///////////////////////////////////

//+
// Function:	skipChar
//
// Purpose:	This function skips over a given char in a string
//		For security, the function will not skip null chars.
//
// Parameters:
//    charPtr	Pointer to string
//    skip	character to skip
//
// Returns:	Pointer to first character after skipped chars.
//		Identity function if the string doesn't start with skip,
//		or skip is the null character
//-

char * skipChar(char * charPtr, char skip){
 // TODO Step 2: skip over instances of the char skip
 //    return input value of charPtr if *char is null char
    while(*charPtr!='\0' && *charPtr ==skip){
        charPtr++;
    }
    return charPtr;
}

//+
// Function:	splitCommandLine
//
// Purpose:	This splits a string into an array of strings.
//		The array is passed as an argument. The string
//		is modified by replacing some of the space characters
//		with null characters to terminate each of the strings.
//
// Parameters:
//	commandBuffer	The string to split
//	args		An array of char pointers
//	maxargs		Size of array args (max number of arguments)
//
// Returns:	Number of arguments (< maxargs).
//
//-

int splitCommandLine(char * commandBuffer, char* args[], int maxargs){
   // TODO Step 2 split the command into words using only
   // the functions skipChar and strchr. You may use fprintf to report
   // errors that are encountered to stderr.
   int x=0;
   do{
       args[x]=skipChar(commandBuffer,' ');
       commandBuffer=args[x];
       x++;
       commandBuffer = strchr(commandBuffer,' ');
       if(!commandBuffer){
           break;
       }
       *commandBuffer='\0';
       commandBuffer++;
   }while(*commandBuffer!='\0' && x<maxargs);
   return x;
}


////////////////////////////// Command Handling ///////////////////////////////////

// typedef for pointer to command handling functions

// TODO STEP 3a write the typedef for the function handler pointer to
// use in the structure immediately below.
// See the description of the function prototypes at the bottom of
// the file in the comments.
typedef void(*handlerPoint)(char* args[], int nargs);


// cmdStruct type:
// Associates a command name with a command handling function

// TODO STEP 3b use the typedef above (Step 3a) to make a two element
// struct. The first is a char * for the name of the command
// the other is a function pointer for the command handling function
 typedef struct {
     char* commandName;
     handlerPoint handlPtr;
 } handlStruct;


// prototypes for command handling functions
// TODO STEP 4b,6: add a function prototype
// for each command handling function
void exitFunc(char* args[], int nargs);
void pwd(char* args[], int nargs); 
void cd(char* args[], int nargs); 
void ls(char* args[], int nargs);
int filterHidden(const struct dirent *d);

// Array that provides the list commands and functions
// must be terminated by {NULL, NULL} 
// in a real shell, this would be a hashtable.

// TODO Step 4a: add a global array of 
// the type above that contains initializers
// of strings and command handling funciton names
handlStruct functionArray[] = {
    {"exit", exitFunc},
    {"pwd", pwd},
    {"cd", cd},
    {"ls", ls},
    {NULL, NULL}
};

//+
// Function:	doCommand
//
// Purpose:	This command calls a command handling funciton from
//		the commands array based on the first argument passed
//		in the args array.
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

void doCommand(char * args[], int nargs){
   // TODO Step 5 this function is small
   //  this is the command search loop
    int i = 0;
    for (i = 0; i < 4; i++) {
        if (strcmp(functionArray[i].commandName, args[0]) == 0) {
            functionArray[i].handlPtr(args, nargs);
            return;
        }
    }
    fprintf(stderr, "Unrecognized input\n");
}

//////////////////////////////////////////////////
//            command Handling Functions        //
//////////////////////////////////////////////////
// all command handling functions have the same
// parameter types and return values.
//////////////////////////////////////////////////

//+
// Function:	all handling functions
//
// Purpose:	this command performs the funcdtion
//		associated with the commands.
//		The name of the comomand is in first argument.
//	 	Since it is called by doCommand, it is known
//		that arg is at least one element long
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

// TODO step 4b put command handling function for exit here
// e.g. void exitFunc(char * args[], int nargs){
// }

//+
// Function: exitFunc
// 
// Purpose: exits the program with exit code 0
// 
// Parameters:
//  args    command and parameters, an array of pointers to strings
//  nargs   number of entries in the args array
//
//  Return  nothing (void)
//-
void exitFunc(char* args[], int nargs) {
    exit(0);
}

// TODO step 6 put rest of command handling functions here

//+
// Function: pwd
// 
// Purpose: prints the current working directory
// 
// Parameters:
//   args  command and parameters, an array of pointers to strings
//   nargs numbers of entries in the args array
// 
// Output: Lists the current working directory with getcwd
// 
// Return  nothing (void)
//-
void pwd(char* args[], int nargs) {
    char* cwd = getcwd(NULL, 0);
    printf("%s\n", cwd);
    free(cwd);
}

//+
Function: cd

// Purpose : changes the current directory to the specified directory or the
//          home directory if no directory is specified
// 
// Parameters:
//   args   command and parameters, an array of pointers to strings
//   nargs  numbers of entries in the args array
// 
// Output: Appropriate error message if there are unrecognized parameters
//
// Return  nothing (void)
//-
void cd(char* args[], int nargs) {
    struct passwd *pw = getpwuid(getuid());
    if (pw->pw_dir == NULL) {
        fprintf(stderr, "NULL directory\n");
    }
    if (nargs == 2) {
        if (chdir(args[1]) != 0) fprintf(stderr, "Directory does not exist\n");
        return;
    }

    if (chdir(pw->pw_dir) != 0) {
        fprintf(stderr, "Error\n");
    }
}
//+
// Function: ls
// 
// Purpose: lists all the entries in the current directory excluding the
//          hidden files unless the parameter "-a" is passed
// 
// Parameters:
//   args   command and parameters, an array of pointers to strings
//   nargs  numbers of entries in the args array
// 
// Output: Appropriate error message if there are unrecognized parameters 
//         Otherwise, outputs each of the entries in the current directory
// 
// Return  nothing (void)
//-
void ls(char* args[], int nargs){
    struct dirent **namelist;
    int (*filVar)(const struct dirent *d);
    if(nargs==1){
        filVar=filterHidden;
    }
    else if(strcmp(args[1],"-a")==0){
        filVar=NULL;
    }
    else{
        fprintf(stderr, "Incorrect argument for ls function\n");
        return;
    }
    int numEnts=scandir(".",&namelist,filVar,NULL);
    int i=0;
    for(i=0;i<numEnts;i++){
        printf("%s\n",namelist[i]->d_name);
    }
}
//+
// Function: filterHidden
// 
// Purpose: Determines if the given entity should be included in the list
//          of entries to outputs
// 
// Parameters:
//    d     the entries in the current directory
// 
// Return   boolean
//-
int filterHidden(const struct dirent *d){
    if(dir->d_name[0]=='.') return 0;
    return 1;
}
