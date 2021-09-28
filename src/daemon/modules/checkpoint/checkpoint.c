#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include "isula_libutils/log.h"
#include "volume_api.h"
#include "utils.h"
#include "map.h"
#include "local.h"
#include "err_msg.h"
#include "utils_file.h"
#include <lxc/lxccontainer.h>

/*
 * 根据容器Id，checkpoint Id，以及checkpoint路径去创建一个checkpoint
 */

//判断是否为目录
bool is_dir(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)//lstat返回文件的信息，文件信息存放在stat结构中
    {
        return S_ISDIR(statbuf.st_mode) != 0;//S_ISDIR宏，判断文件类型是否为目录
    }
    return false;
}

//判断是否为常规文件
bool is_file(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)
        return S_ISREG(statbuf.st_mode) != 0;//判断文件是否为常规文件
    return false;
}

//判断是否是特殊目录
bool is_special_dir(const char *path)
{
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

//生成完整的文件路径
void get_file_path(const char *path, const char *file_name,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, file_name);
}

void delete_file(const char *path)
{
    DIR *dir;
    struct dirent *dir_info;
    char file_path[PATH_MAX];
    if(is_file(path))
    {
        remove(path);
        return;
    }
    if(is_dir(path))
    {
        //printf("is_dir\n");
        if((dir = opendir(path)) == NULL)
            
            return;
        while((dir_info = readdir(dir)) != NULL)
        {
            get_file_path(path, dir_info->d_name, file_path);
            if(is_special_dir(dir_info->d_name))
                continue;
            delete_file(file_path);
            rmdir(file_path);
        }
        rmdir(path);
    }
}
char* checkpoint_remove(char* container,char* dir)
{

    printf("modules\n");
    if(!dir){
        dir="/tmp/isula-criu/";
    }
    strcat(dir,container);
    
    delete_file(dir);
    printf("dir:%s\n",dir);
    printf("modules\n");
    return container;
    
}



char* checkpoint_create(char* container,char* dir)
{
    struct lxc_container *c;
    c=lxc_container_new(container,"/var/lib/isulad/engines/lcr/");
    //无法获取容器
    if (!c) {
		ERROR("System error loading %s\n", container);
		return 0;
	}
    //容器未定义
    if (!c->is_defined(c)) {
		ERROR("(deamon modules)Error response from daemon: No such container:%s\n",container);
		return 0;
	}
    //容器未运行
    if (!c->is_running(c)) {
		ERROR("%s not running, not checkpointing\n", container);
		return 0;
	}

    char checkpoint_dir[1000]="/tmp/isula-criu/";
    strcat(checkpoint_dir,c->name);
    bool res;
    //如果自定义了路径就使用自定义路径，否则使用默认路径
    if(dir){
        res = c->checkpoint(c,dir,true,false);
    }else{
        res =  c->checkpoint(c,checkpoint_dir,true,false);
    }
    if(!res){
        ERROR("Checkpointing %s failed",container);
    }else{
        printf("%s\n",container);
    }
    return container;
}

char* checkpoint_list(char* dir){
    DIR *dirp;
    struct dirent *dp;
    if(dir){
        dirp=opendir(dir);
    }else{
        dirp=opendir("/tmp/isula-criu/");
    }
    
    int sum=0;
    while((dp=readdir(dirp))!=NULL){
        if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
            continue;
        }
        sum++;
        printf("%s\n",dp->d_name);
    }
  
    if(dir){
        dirp=opendir(dir);
    }else{
        dirp=opendir("/tmp/isula-criu/");
    }
    
    char* checkpoints=(char*)malloc(sum*70);
    while((dp=readdir(dirp))!=NULL){
        if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
            continue;
        }
        sum++;
        strcat(checkpoints,dp->d_name);
        strcat(checkpoints,"\n");
    }
    (void)closedir(dirp);
    return checkpoints;
}



