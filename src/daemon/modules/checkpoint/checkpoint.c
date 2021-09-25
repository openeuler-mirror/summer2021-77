#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "isula_libutils/log.h"
#include "volume_api.h"
#include "utils.h"
#include "map.h"
#include "local.h"
#include "err_msg.h"
#include "utils_file.h"

/*
 * 根据容器Id，checkpoint Id，以及checkpoint路径去创建一个checkpoint
 */
int checkpoint_create(char* container,char* checkpoint,char* dir)
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
        ERROR("Checkpointing %s failed",args->name);
    }else{
        printf("%s\n",args->name);
    }
    return res;
}



