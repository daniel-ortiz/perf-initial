#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */
#include <omp.h>
 
int main()
{
    pid_t childpid; /* variable to store the child's pid */
    int retval;     /* child process: user-provided return code */
    int status;     /* parent process: child's exit status */
    int err;

    /* only 1 int variable is needed because each process would have its
       own instance of the variable
       here, 2 int variables are used for clarity */
        
    /* now create new process */
    childpid = fork();
    char *numaan, pid[6];
    numaan=(char* )(100*sizeof(char));
    
    if (childpid >= 0) /* fork succeeded */
    {
		if (childpid == 0) /* fork() returns 0 to the child process */ 
		{
            printf("CHILD: I am the child process!\n");
            printf("CHILD: Here's my PID: %d\n", getpid());
            printf("CHILD: My parent's PID is: %d\n", getppid());
            printf("CHILD: The value of my copy of childpid is: %d\n", childpid);
            /**
             * */
             
             char* aff=getenv("OMP_NUM_THREADS");
			 char str[2];
			 int N=100000000;
		     int M=1000000000;

			double* A;
			A  = (double*)malloc(M *sizeof(double) );
			srand ( time(NULL) );
			printf("OMP2: Here's my PID: %d\n", getpid());
				#pragma omp master
				{	
				int cpu, node;
				cpu= sched_getcpu();
	
				//	#pragma omp for
				for (int i = 0; i < M; i++) {
				A[i] = 3.1416;
				}
				printf ("end of initialization  %d %s %d  \n",omp_get_num_threads(), aff, cpu );
			}	
			//scanf ("%79s",str);  
			printf ("all set\n");
  
			#pragma omp parallel 
			{
				if (omp_get_thread_num( )!=0){
				int cpu= sched_getcpu();
				printf ("modifying mem %d %s %d  \n",omp_get_num_threads(), aff, cpu );
				#pragma omp parallel  
				{
					for (int i = 0; i < N; i++) {
						int index= rand() % M ;
						A[index] += 1;
					}
				}
			}
			}
   
             /**
              
              */
              
            //printf("CHILD: Sleeping for 1 second...\n");
            //sleep(1); /* sleep for 1 second */
            //printf("CHILD: Enter an exit value (0 to 255): ");
            //scanf(" %d", &retval);
            printf("CHILD: Goodbye!\n");    
            exit(retval); /* child exits with user-provided return code */
        }
        else /* fork() returns new pid to the parent process */
        {
            printf("PARENT: I am the parent process!\n");
            printf("PARENT: Here's my PID: %d\n", getpid());
            printf("PARENT: The value of my copy of childpid is %d\n", childpid);
            printf("PARENT: I will now wait for my child to exit.\n");
            numaan=strdup("./perf numa-an ");
            sprintf(pid,"%d", childpid);
            err=strcat(numaan, pid);
            printf("PARENT: shell call %s.\n",numaan);
            system(numaan);
            wait(&status); /* wait for child to exit, and store its status */
            printf("PARENT: Child's exit code is: %d\n", WEXITSTATUS(status));
            printf("PARENT: Goodbye!\n");             
            exit(0);  /* parent exits */       
        }
    }
    else /* fork returns -1 on failure */
    {
        perror("fork"); /* display error message */
        exit(0); 
    }
}
