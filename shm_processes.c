#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  ClientProcess(int []);

int  main(int  argc, char *argv[])
{
    // Shared memory ID.
int ShmID;

// Pointer to shared memory.
int *ShmPTR;

// Process ID for child process.
pid_t pid;

// Status of child process.
int status;

// Check if the correct number of arguments were provided.
if (argc != 5) {
    printf("Use: %s #1 #2 #3 #4\n", argv[0]);
    exit(1);
}

// Create shared memory for four integers.
ShmID = shmget(IPC_PRIVATE, 4 * sizeof(int), IPC_CREAT | 0666);
if (ShmID < 0) {
    printf("*** shmget error (server) ***\n");
    exit(1);
}
printf("Server has received a shared memory of four integers...\n");

// Attach shared memory to the server.
ShmPTR = (int *) shmat(ShmID, NULL, 0);
if (*ShmPTR == -1) {
    printf("*** shmat error (server) ***\n");
    exit(1);
}
printf("Server has attached the shared memory...\n");


     // Convert the arguments passed to the program to integers
ShmPTR[0] = atoi(argv[1]);
ShmPTR[1] = atoi(argv[2]);
ShmPTR[2] = atoi(argv[3]);
ShmPTR[3] = atoi(argv[4]);

// Print the values that were stored in the shared memory
printf("Server has filled %d %d %d %d in shared memory...\n",
       ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3]);

// Print a message before creating a child process
printf("Server is about to fork a child process...\n");

// Create a child process
pid = fork();
if (pid < 0) {
     // Print an error message if the fork failed
     printf("*** fork error (server) ***\n");
     exit(1);
}
else if (pid == 0) {
     // Call the ClientProcess function in the child process
     ClientProcess(ShmPTR);
     exit(0);
}

// Wait for the child process to complete
wait(&status);

// Print a message when the child process has finished
printf("Server has detected the completion of its child...\n");

// Detach the shared memory
shmdt((void *) ShmPTR);
printf("Server has detached its shared memory...\n");

// Remove the shared memory
shmctl(ShmID, IPC_RMID, NULL);
printf("Server has removed its shared memory...\n");

// Print a message before exiting
printf("Server exits...\n");
exit(0);


// Function to run in the child process
void  ClientProcess(int  SharedMem[])
{
     // Print a message when the function starts
     printf("   Client process started\n");

     // Print the values that were stored in the shared memory
     printf("   Client found %d %d %d %d in shared memory\n",
                SharedMem[0], SharedMem[1], SharedMem[2], SharedMem[3]);

     // Print a message before exiting
     printf("   Client is about to exit\n");
}
