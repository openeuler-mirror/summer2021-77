/******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * iSulad licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Author: wangfengtu
 * rm: 2020-09-04
 * Description: provide checkpoint remove functions
 ******************************************************************************/
#include "cpremove.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lxc/lxccontainer.h>

#include <sys/stat.h>
#include <unistd.h>

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include "utils.h"
#include "client_arguments.h"
#include "isula_connect.h"
#include "isula_libutils/log.h"
#include "connect.h"
#include "protocol_type.h"

const char g_cmd_checkpoint_rm_desc[] =
    "Remove a checkpoint";
const char g_cmd_checkpoint_rm_usage[] = "isula checkpoint rm [OPTIONS] CONTAINER";

struct client_arguments g_cmd_checkpoint_rm_args;

#define REMOVE_OPTIONS(cmdargs) \
    { CMD_OPT_TYPE_STRING, false, "checkpoint-dir", 0, &(cmdargs).checkpoint_dir, "Use a custom checkpoint storage directory", NULL },



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


static int client_checkpoint_rm(const struct client_arguments *args, char ***volumes, size_t *volumes_len){
   
    char checkpoint_dir[1000]="/tmp/isula-criu/";
    strcat(checkpoint_dir,args->name);
    if(args->checkpoint_dir){
         strcat(args->checkpoint_dir,args->name);
        delete_file(args->checkpoint_dir);
    }else{
        delete_file(checkpoint_dir);
    }
    
    return 1;
}

int cmd_checkpoint_rm_main(int argc, const char **argv)
{
    //int i = 0;
    struct isula_libutils_log_config lconf = { 0 };
    int exit_code = 1;
    command_t cmd;
    char **checkpoints = NULL;
    size_t checkpoints_len = 0;
   // char ch = 'n';
    struct command_option options[] = { LOG_OPTIONS(lconf) COMMON_OPTIONS(g_cmd_checkpoint_rm_args)
        REMOVE_OPTIONS(g_cmd_checkpoint_rm_args)
    };
    //printf("%s",options[0])

    if (client_arguments_init(&g_cmd_checkpoint_rm_args)) {
        COMMAND_ERROR("client arguments init failed");
        exit(ECOMMON);
    }
    g_cmd_checkpoint_rm_args.progname = util_string_join(" ", argv, 2);
    isula_libutils_default_log_config(argv[0], &lconf);
    subcommand_init(&cmd, options, sizeof(options) / sizeof(options[0]), argc, (const char **)argv, g_cmd_checkpoint_rm_desc,
                    g_cmd_checkpoint_rm_usage);

    if (command_parse_args(&cmd, &g_cmd_checkpoint_rm_args.argc, &g_cmd_checkpoint_rm_args.argv)) {
        exit(exit_code);
    }
    if (isula_libutils_log_enable(&lconf)) {
        COMMAND_ERROR("checkpoint rm: log init failed");
        exit(exit_code);
    }



    if (g_cmd_checkpoint_rm_args.argc != 1) {
        
        COMMAND_ERROR("%s: \"checkpoint rm\" requires exactly 1 arguments.", g_cmd_checkpoint_rm_args.progname);
        exit(exit_code);
    }

    //printf("%s\n","name");
    g_cmd_checkpoint_rm_args.name=g_cmd_checkpoint_rm_args.argv[0];
    //printf("%s\n",g_cmd_checkpoint_rm_args.name);
    if (client_checkpoint_rm(&g_cmd_checkpoint_rm_args, &checkpoints, &checkpoints_len) != 0) {
        ERROR("rm checkpoints failed");
        exit(exit_code);
    }


    exit(EXIT_SUCCESS);
    
}
