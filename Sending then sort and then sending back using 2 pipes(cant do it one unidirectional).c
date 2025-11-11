#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>


int main(int argc, char* argv[])
{
//first argument is our own program ./out
int num[argc-1];

int j=0;
for(int i=1; i<argc; i++)
{
num[j]=atoi(argv[i]);
j++;
}

//size of num arr would be one less

int size=argc-1;




//For sending array to child to sort it 
int fd[2];

if(pipe(fd)==-1){
printf("Error in pipe creation. \n");
return 2;
}

//for sending sorted from child to parent
int fdb[2];


if(pipe(fdb)==-1){
printf("Error in pipe creation. \n");
return 4;
}


pid_t pid=fork();
if(pid<0)
{
printf("Error in fork creation. \n");
return 3;
}


if(pid>0){
close(fd[0]);
close(fdb[1]);


printf("Sending the array to get sorted: \n");

write(fd[1],num,size*sizeof(int));
close(fd[1]);

int sorted[size];

read(fdb[0],sorted,sizeof(int)*size);

printf("Sorted Array in Parent: \n");
for(int i=0; i<size; i++){
printf("%d, ", sorted[i]);
}

wait(NULL);

}
else if(pid==0)
{
close(fd[1]);
close(fdb[0]);

int tobesort[size];
printf("Recieved unsorted array in child! \n");
read(fd[0],tobesort,size*sizeof(int));
close(fd[0]);

for(int i=0; i<size-1; i++){
for(int j=0; j<size-1-i; j++){

if(tobesort[j]>tobesort[j+1]){
int temp=tobesort[j];
tobesort[j]=tobesort[j+1];
tobesort[j+1]=temp;
}

}
}

write(fdb[1],tobesort,sizeof(int)*size);
close(fdb[1]);

}

return 0;
}
