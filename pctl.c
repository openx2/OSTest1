/*
 *Filename	:pctl.c
 *copyright:2016 by chenxin
 *Function	:multiprocess concurrence
 */
#include "pctl.h"
int main(int argc,char *argv[])
{
    int i;
    int pspid;
    int lspid;
    int psstatus;
    int lsstatus;
    char *args1[] = {"/bin/ls","-a",NULL};
    char *args2[] = {"/bin/ps","-f",NULL};
    lspid = fork();
    signal(SIGCONT,(sighandler_t)sigcat);
    if(lspid<0)
    {
        printf("Create Process ls fail!\n");
        exit(EXIT_FAILURE);
    }
    if(lspid==0)
    {
        printf("I am Child process ls,id = %d.\nMy father is %d.\n",getpid(),getppid());
        pause();
        printf("%d child will Running:\n",getpid());
        for(i=0;args1[i]!=NULL;i++)
            printf("%s ",args1[i]);
        printf("\n");
        lsstatus=execve(args1[0],args1,NULL);
    }
    pspid = fork();
    if(pspid<0)
    {
        printf("Create Process ps fail!\n");
        exit(EXIT_FAILURE);
    }
    if(pspid==0)
    {
        printf("I am Child process ps,id = %d.\nMy father is %d.\n",getpid(),getppid());
        printf("%d child will Running:\n",getpid());
        for(i=0;args2[i]!=NULL;i++)
            printf("%s ",args2[i]);
        printf("\n");
        psstatus=execve(args2[0],args2,NULL);
    }
    else
    {
        waitpid(pspid,&psstatus,0);
        if(kill(lspid,SIGCONT)>=0)
            printf("%d Wakeup %d child.\n",getpid(),lspid);
    }
    return EXIT_SUCCESS;
}
