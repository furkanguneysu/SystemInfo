#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
int main(int argc,char *argv[])
{
long int amma;

if(argc == 1){
amma = syscall(335,1,"1");
printf("system call returned %ld\n",amma);
}

else if(argc == 2 && strcmp(argv[1],"-all")==0){
amma = syscall(335,2,"1");
printf("system call returned %ld\n",amma);
}

else if(argc == 3 && strcmp(argv[1],"-p")==0){
amma = syscall(335,3,argv[2]);
printf("system call returned %ld\n");
}

else if(argc == 4 && strcmp(argv[1],"-all")== 0 && strcmp(argv[2],"-p")==0){
amma = syscall(335,4,argv[3]);
printf("system call returned %ld\n");
}

else{
printf("Invalid or missing command\n");
}

return 0;

}
