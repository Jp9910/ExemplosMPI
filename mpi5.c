/**
 * Exemplo com recepção de mensagem e envio de ack
 */
 
 #include <stdio.h>
 #include <string.h>
 #include <mpi.h>
 
 const int MAX_STRING = 100;
 
 int main(){
     char message[MAX_STRING];
     int comm_size;
     int my_rank;
     
     MPI_Init(NULL,NULL);
     MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
     MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
     
     if(my_rank != 0){
         
         sprintf(message, "Msg do processo %d de %d!",my_rank,comm_size);
         MPI_Send(message,strlen(message)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
         
         char ack[4];
         MPI_Status status;
         MPI_Recv(ack, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         printf("Mensagem recebida pelo processo %d vinda do processo %d: %s\n",my_rank,status.MPI_SOURCE,ack);
         
     } else {
         printf("Processo centralizador em execução: %d of %d!\n", my_rank, comm_size);
         
         MPI_Status status;
         
         for (int q = 1; q < comm_size; q++) {
             MPI_Recv(message, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
             
             printf("O processo %d recebeu a seguinte mensagem: %s (status.MPI_SOURCE: %d status.MPI_TAG: %d)\n", my_rank, message, status.MPI_SOURCE, status.MPI_TAG);
             
             char ack[] = "ack";
             MPI_Send(ack,strlen(ack)+1,MPI_CHAR,status.MPI_SOURCE,status.MPI_TAG,MPI_COMM_WORLD);
        } 
    }
    
    MPI_Finalize();
    
    return 0;
 }