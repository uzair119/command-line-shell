#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include<string.h>
#include <time.h>

typedef struct
{
    char* name;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char status[10];
    time_t starttime;
    time_t endtime;

}Process;

void printProcess(Process p);
int endProcess(Process p);
static char* path;
int findpath(char* str,STARTUPINFO startup, PROCESS_INFORMATION pinfo);


int main()
{
    path = getenv("PATH");
    int procnum = 0;
    while(1)
    {
    char str[150]; //initiialize string for input
    char* arr[50]; //create an array of strings
    Process proclist[100]; //list of processes
    printf(">");
    fgets(str,150,stdin);
    if(!strcmp(str,"exit\n") || !strcmp(str,"EXIT\n"))//check for exit command
        return 0;
    char* token = strtok(str," ");
    int i = 0;
    while(token != NULL)
    {
        arr[i] = token;
        token = strtok(NULL," ");
        i++;
    }

     //i is the length of my 'useful' array. Everything at i and after i is garbage
     //addition logic
    if(!strcmp(arr[0],"ADD") || !strcmp(arr[0],"add"))
    {
        if(i < 3)
        {
            printf("Enter atleast 2 arguments.");
        }
        else
        {
            double sum = 0;
            for(int j = 1; j < i; j++)
            {
                sum += atof(arr[j]);
            }
            printf("%.2f\n",sum);
        }
    }
    //subtraction logic
    else if(!strcmp(arr[0],"SUB") || !strcmp(arr[0],"sub"))
    {
        if(i < 3)
        {
            printf("Enter atleast 2 arguments.");
        }
        else
        {
            double diff = atoi(arr[1]);
            for(int j = 2; j < i; j++)
            {
                diff -= atoi(arr[j]);
            }
            printf("%.2f\n",diff);
        }
    }
    //multiplication logic
    else if(!strcmp(arr[0],"MUL") || !strcmp(arr[0],"mul"))
    {
        if(i < 3)
        {
            printf("Enter atleast 2 arguments.\n");
        }
        else
        {
            double mult = 1;
            for(int j = 1; j < i; j++)
            {
                //omit spaces
                if((int)atof(arr[j]) == 0)
                {
                    if(!strcmp(arr[j],"0") || !strcmp(arr[j],"0\n"))
                    {
                        mult = 0;
                        break;
                    }
                }
                else
                    mult *= atof(arr[j]);
            }
            printf("%.2f\n",mult);
        }
    }
    //division logic
    else if(!strcmp(arr[0],"DIV") || !strcmp(arr[0],"div"))
    {
        if(i < 3)
        {
            printf("Enter atleast 2 arguments.\n");
        }
        else
        {
            double div = atof(arr[1]);
            int flag = 1;
            for(int j = 2; j < i; j++)
            {
                //omit spaces
                if(atof(arr[j]) == 0)
                {
                    if(!strcmp(arr[j],"0") || !strcmp(arr[j],"0\n"))
                    {
                        printf("Cannot divide by 0.\n");
                        flag = 0;
                        break;
                    }
                }
                else
                    div /= atof(arr[j]);
            }
            if(flag)
                printf("%.2f\n",div);
        }
    }
    //run command logic
    else if (!strcmp(arr[0],"RUN") || !strcmp(arr[0],"run"))
    {
        STARTUPINFO startup={sizeof(startup)};
        PROCESS_INFORMATION pinfo;
        //char* str1 = arr[1];
        char buff[500]; //buffer string for concatenation
        strcpy(buff,arr[1]);
//        char* path = getenv(arr[1]);
//      For concatentating the arguments of the RUN command which have been separated due to the space delimiter.
        for(int j = 2; j < i; j++)
        {
            strcat(buff," ");
            strcat(buff,arr[j]);
        }
        //printf("%s\n",buff);
        char* ptr;
        if( (ptr = strchr(buff, '\n')) != NULL)  //remove newline character at the end
            *ptr = '\0';
        int created = 0;
        //char* app = getenv("PATH");
        //printf(app);
        //strcat(app,arr[1]);
        //FIRST, LOOK FOR THE EXECUTABLE IN THE EVIRONMENT VARIABLE PATH USING THE SECOND PARAMETER
        if(!CreateProcess(NULL,             // Name of program to execute
                            buff,             // Command line
                            NULL,           // Process handle not inheritable
                            NULL,                       // Thread handle not inheritable
                            FALSE,                     // Set handle inheritance to FALSE
                            0,                         // No creation flags
                            NULL,                      // Use parent's environment block
                            NULL,                      // Use parent's starting directory
                            &startup,                       // Pointer to STARTUPINFO structure
                            &pinfo))                        // Pointer to PROCESS_INFORMATION structure
        {
            //IF THERE'S AN ABSOLUTE PATH ENTERED, THIS WILL RUN IT
                if(!CreateProcess(buff,             // Name of program to execute
                            NULL,             // Command line
                            NULL,           // Process handle not inheritable
                            NULL,                       // Thread handle not inheritable
                            FALSE,                     // Set handle inheritance to FALSE
                            0,                         // No creation flags
                            NULL,                      // Use parent's environment block
                            NULL,                      // Use parent's starting directory
                            &startup,                       // Pointer to STARTUPINFO structure
                            &pinfo                      // Pointer to PROCESS_INFORMATION structure
                            ))
                            {
                                //SOMETHING I TRIED
                                //if(!findpath(str1,startup,pinfo))
                                    printf("Run failed. Try entering the full path. For example:\nRUN C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe\n");
                            }
                            else
                            {
                                created = 1;
                            }
        }
        else
            created = 1;
        if(created)
        {
            proclist[procnum].name = strcpy(malloc(strlen(buff)),buff);
            proclist[procnum].si = startup;
            proclist[procnum].pi = pinfo;
            strcpy(proclist[procnum].status,"Active");
            /*
            time_t curtime;
            struct tm *loc_time;
            //Getting current time of system
            curtime = time (NULL);
            // Converting current time to local time
            loc_time = asctime(localtime (&curtime));*/
            proclist[procnum].starttime = time(NULL);
            procnum++;
        }
    }
    //help command
    else if(!strcmp(arr[0],"help\n") || !strcmp(arr[0],"HELP\n"))
    {
        char* temp = "List of Commands:\nADD <arguments>\nSUB <arguments>\nDIV <arguments>\nMUL <arguments>\nRUN <program name>\nKILL <process name OR pid>\nLIST\nEXIT\n";
        printf(temp);
    }
    else if(!strcmp(arr[0],"kill") || !strcmp(arr[0],"KILL"))
    {
        char buff[500]; //buffer string for concatenation
        char idbuff[10];
        strcpy(buff,arr[1]);
//        char* path = getenv(arr[1]);
//      For concatentating the arguments of the RUN command which have been separated due to the space delimiter.
        for(int j = 2; j < i; j++)
        {
            strcat(buff," ");
            strcat(buff,arr[j]);
        }
        //printf("%s\n",buff);
        char* ptr;
        if( (ptr = strchr(buff, '\n')) != NULL)  //remove newline character at the end
            *ptr = '\0';
        int end = 0;
        for(int y = 0; y < procnum; y++)
        {
            //printf(buff);
            ltoa(proclist[y].pi.dwProcessId,idbuff,10);
            if(!strcmp(proclist[y].name,buff) || !strcmp(ltoa(proclist[y].pi.dwProcessId,idbuff,10),buff))
            {
                if(endProcess(proclist[y]))
                {
                    proclist[y].endtime = time(NULL);
                    strcpy(proclist[y].status,"Inactive");
                    break;
                }
            }
        }
    }
    else if(!strcmp(arr[0],"list\n") || !strcmp(arr[0],"LIST\n"))
    {
        printf("PROCESS NAME\tPID\tSTATUS\t\tSTART TIME\t\t\tELAPSED TIME\tEND TIME\t\n");
        for(int y = 0; y < procnum; y++)
        {
            printProcess(proclist[y]);
        }
    }
    } //end of program's while loop
    return 0;
}


void printProcess(Process p)
{
    char* starttime = strcpy(malloc(75),asctime(localtime(&p.starttime)));
    char* endtime;
    char* ptr;
    double elapsed;
    //printf("1");
    if( (ptr = strchr(starttime, '\n')) != NULL)  //remove newline character at the end
            *ptr = '\0';

    if(!strcmp(p.status,"Active"))
    {
        endtime = "---------\n";
        elapsed = difftime(time(NULL),p.starttime);
        printf("%s\t\t%u\t%s\t\t%s\t%.0fs\t\t%s",p.name,p.pi.dwProcessId,p.status,starttime,elapsed,endtime);
    }
    else
    {
        //printf(starttime);
        endtime =  asctime(localtime(&p.endtime));
        //printf(starttime);
        elapsed = difftime(p.endtime,p.starttime);
        printf("%s\t\t%u\t%s\t%s\t%.0fs\t\t%s",p.name,p.pi.dwProcessId,p.status,starttime,elapsed,endtime);
    }


}

int endProcess(Process p)
{
    if(TerminateProcess(p.pi.hProcess,0))
    {
        return 1;
    }
    return 0;
}

//UNUSED METHOD AS OF NOW.
//ENVIRONMENT VARIABLE 'PATH' TOKENIZER. I USED THIS FOR CHECKING MY EXECUTABLE ON EVERY PATH IN THE ENVIRONMENT VARIABLE. IT WAS
//A SUCCESS, BUT TURNED OUT TO BE USELESS LATER ON, AS CREATEPROCESS AUTOMATICALLY CHECKS THOSE PATHS ITSELF. NOT GONNA REMOVE THIS THOUGH, I WORKED ON IT.
int findpath(char* str,STARTUPINFO startup, PROCESS_INFORMATION pinfo)
{
    //printf("finding");
    char* arr[50];
    char cmd[150];

    char paths[strlen(path)];
    strcpy(cmd,str);
    strcpy(paths,path);
    printf(paths);
    char* token = strtok(paths,";");
    int i = 0;
    while(token != NULL)
    {
        arr[i] = token;
        token = strtok(NULL,";");
        i++;
    }
    for(int j = 0; j < i; j++)
    {
        char temp[100];
        strcpy(temp,arr[j]);
        strcat(temp,"\\");
        strcat(temp,cmd);
        printf("%s\n",temp);
                if(CreateProcess(temp,             // Name of program to execute
                            NULL,             // Command line
                            NULL,           // Process handle not inheritable
                            NULL,                       // Thread handle not inheritable
                            FALSE,                     // Set handle inheritance to FALSE
                            0,                         // No creation flags
                            NULL,                      // Use parent's environment block
                            NULL,                      // Use parent's starting directory
                            &startup,                       // Pointer to STARTUPINFO structure
                            &pinfo))
                            {
                                    printf("%s\n",temp);
                                    return 1;
                            }



    }
    return 0;
}
