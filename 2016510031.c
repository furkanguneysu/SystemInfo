#include <linux/module.h>  // Needed by all modules
#include <linux/kernel.h>  // Needed for KERN_INFO
#include <linux/fs.h>      // Needed by filp
#include <asm/uaccess.h>   // Needed by segment descriptors
#include <linux/string.h>  // String operations
#include <linux/syscalls.h>


int read_cpuinfo(void){
    struct file *f;
    char buf[200];
    char *filename = "/proc/cpuinfo";
    int size=-1;
    mm_segment_t fs;
    int i;
    for(i=0;i<200;i++){
        buf[i] = 0;
    }
    f = filp_open(filename, O_RDONLY, 0);
    if(f == NULL){
        printk(KERN_ALERT "filp_open error!!.\n");
    }
    else{
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f, buf,200, &f->f_pos);
        set_fs(fs);
	for(i=0;i<sizeof(buf);i++){
	if(buf[i]=='s' && buf[i+1]=='t' && buf[i+2]=='e' && buf[i+3]=='p'){
	size = i;
	break;
	}
	}
	char str[size];
	strncpy(str,buf,sizeof(str));
	str[sizeof(str)] = '\0';
	printk(KERN_INFO "CPU Information\n%s",str);
    }
    filp_close(f,NULL);
}

int read_activetask(void){
    struct file *f;
    int size=500;
    char buf[size];
    char *filename = "/proc/loadavg";
    mm_segment_t fs;
    int i;
    int z=0;
    int m=0;
    int start;
    int flag=0;
    int t=0;
    for(i=0;i<sizeof(buf);i++){
        buf[i] = 0;
    }
    f = filp_open(filename, O_RDONLY, 0);
    if(f == NULL){
        printk(KERN_ALERT "filp_open error!!.\n");
    }
    else{
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f, buf, sizeof(buf), &f->f_pos);
        set_fs(fs);
	for(i=0;i<sizeof(buf);i++){
	if(buf[i]==' '){
	z=z+1;
	}
	if(z==3 && flag==0){
	start = i;
	flag=-1;
	}
	if(z>=3 && z<4){
	t=t+1;
	}
	}
	char str[t];
	strncpy(str,buf+start,sizeof(str));
	str[sizeof(str)]='\0';//Active tasks
	char str2[3];
	for(i=0;i<sizeof(str);i++){
	m=m+1;
	if(str[i] == '/'){
	break;
	}
	}
	strncpy(str2,str+m,3);
	str2[sizeof(str2)]='\0';
        printk(KERN_INFO "The number of active tasks:%s \nThe total number of processes: %s ",str,str2);
    }
    filp_close(f,NULL);
}

int read_uptime(void){
    struct file *f;
    int size=500;
    char buf[size];
    char *filename = "/proc/uptime";
    mm_segment_t fs;
    int i,z;
    int t=0;
    for(i=0;i<sizeof(buf);i++){
        buf[i] = 0;
    }
    f = filp_open(filename, O_RDONLY, 0);
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f,buf,sizeof(buf), &f->f_pos);
        set_fs(fs);
	buf[sizeof(buf)] = '\0';
	while(!(buf[t]==' ')){
	t=t+1;
	}
	char str1[t];
	char str2[t];
	for(z=0;z<sizeof(str1);z++){
	str1[z] = 0;
	str2[z] = 0;
	}
	for(z=0;z<t;z++){
	str1[z] = buf[z];
	}
	str1[sizeof(str1)] = '\0';
	for(z=0;z<t;z++){
	str2[z] = buf[z+t];
	}
	str2[sizeof(str2)] = '\0';
	printk(KERN_INFO "System Statistics\nSystem was booted since:%s\nSystem has been idle since %s",str1,str2);
    }
    filp_close(f,NULL);
}

int read_process(char *filename){
    char filename2[100] = "/proc/";
    strcat(filename2,filename);
    strcat(filename2,"/stat");
    struct file *f;
    int size=500;
    char buf[size];
    int datasizes[] = {0,0,0,0};
    mm_segment_t fs;
    int i;
    int t=0;
    int z=0;
    for(i=0;i<sizeof(buf);i++){
        buf[i] = 0;
    }
    
    f = filp_open(filename2, O_RDONLY, 0);
    if(f == NULL){
        printk(KERN_ALERT "filp_open error!!.\n");
    }
    else{
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f, buf, sizeof(buf), &f->f_pos);
        set_fs(fs);
	buf[sizeof(buf)] = '\0';
	for(i=0;i<sizeof(buf);i++){
	if(z==4){
	break;
	}
	if(buf[i] == ' '){
	datasizes[z] = t;
	z=z+1;
	}
	t=t+1;
	}
	buf[t] = '\0';
	char pid[datasizes[0]];
	char name[datasizes[1]-datasizes[0]];
	char state[datasizes[2]-datasizes[1]];
	char ppid[datasizes[3]-datasizes[2]];
	for(i=0;i<sizeof(pid);i++){
	pid[i] = 0;
	}
	strncpy(pid,buf,sizeof(pid));
	pid[sizeof(pid)] = '\0';
	strncpy(name,buf+sizeof(pid),sizeof(name));
	name[sizeof(name)] = '\0';
	strncpy(state,buf+sizeof(name)+sizeof(pid),sizeof(state));
	state[sizeof(state)]='\0';
	strncpy(ppid,buf+sizeof(name)+sizeof(pid)+sizeof(state),sizeof(ppid));
	ppid[sizeof(ppid)] = '\0';
	printk(KERN_INFO "Process Infomation:\nName:%s\nState:%s\nPid: %s\nPpid:%s",name,state,pid,ppid);

   }

}

int read_uid(char *filename){
    char filename2[100] = "/proc/";
    strcat(filename2,filename);
    strcat(filename2,"/status");
    int size=500;
    struct file *f;
    char buf[size];
    mm_segment_t fs;
    int i;
    int start;
    int end;
    for(i=0;i<sizeof(buf);i++){
        buf[i] = 0;
    }
    f = filp_open(filename2, O_RDONLY, 0);
    if(f == NULL){
        printk(KERN_ALERT "filp_open error!!.\n");
    }
    else{
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f, buf, sizeof(buf), &f->f_pos);
        set_fs(fs);
        buf[sizeof(buf)] = '\0';
	for(i=0;i<sizeof(buf);i++){
	if(buf[i]=='U' && buf[i+1]=='i' && buf[i+2]=='d'){
	start = i;
	}
	if(buf[i]=='G' && buf[i+1]=='i' && buf[i+2]=='d'){
	end = i;
	break;
	}
	}
	char uid[end-start];
	strncpy(uid,buf+start,sizeof(uid));
	uid[sizeof(uid)] = '\0';
	printk(KERN_INFO "%s",uid);

   }

}

SYSCALL_DEFINE2(my_systeminfo, int, a, char *,filename){

if(a == 1){
read_cpuinfo();
}
else if(a == 2){
read_cpuinfo();
read_uptime();
read_activetask();
}
else if(a == 3){
read_cpuinfo();
read_process(filename);
read_uid(filename);
}
else if(a == 4){
read_cpuinfo();
read_uptime();
read_activetask();
read_process(filename);
read_uid(filename);
}
else{
printk("Invalid or missing Command\n");
}

return 0;
}


