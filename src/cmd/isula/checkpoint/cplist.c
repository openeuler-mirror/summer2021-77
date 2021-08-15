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
 * Create: 2020-09-04
 * Description: provide checkpoint remove functions
 ******************************************************************************/
#include "cplist.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lxc/lxccontainer.h>
#include <dirent.h>
#include "utils.h"
#include "client_arguments.h"
#include "isula_connect.h"
#include "isula_libutils/log.h"
#include "connect.h"
#include "protocol_type.h"

const char g_cmd_checkpoint_ls_desc[] =
    "List checkpoints for a container";
const char g_cmd_checkpoint_ls_usage[] = "ls [OPTIONS]";

struct client_arguments g_cmd_checkpoint_ls_args;

#define LS_OPTIONS(cmdargs) \
    { CMD_OPT_TYPE_STRING, false, "checkpoint-dir", 0, &(cmdargs).checkpoint_dir, "Use a custom checkpoint storage directory", NULL },



static int client_checkpoint_ls(const struct client_arguments *args, char ***volumes, size_t *volumes_len){
    
    //printf("%s\n",);
    DIR *dirp;
    struct dirent *dp;
    if(args->checkpoint_dir){
        //printf("%s\n",args->checkpoint_dir);
        dirp=opendir(args->checkpoint_dir);
        //dirp=opendir("/tmp/isula/");

        //printf("error\n");
    }else{
        dirp=opendir("/tmp/isula-criu/");
    }
    
    while((dp=readdir(dirp))!=NULL){
        if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
            continue;
        }

        printf("%s\n",dp->d_name);
    }
    (void)closedir(dirp);
    return 0;
}

int cmd_checkpoint_ls_main(int argc, const char **argv)
{
    //int i = 0;
    struct isula_libutils_log_config lconf = { 0 };
    int exit_code = 1;
    command_t cmd;
    
    char **checkpoints = NULL;
    size_t checkpoints_len = 0;
   // char ch = 'n';
   
    struct command_option options[] = { 
        LS_OPTIONS(g_cmd_checkpoint_ls_args)
    };
    //printf("%s",options[0])
    

    if (client_arguments_init(&g_cmd_checkpoint_ls_args)) {
        COMMAND_ERROR("client arguments init failed");
        exit(ECOMMON);
    }
    //printf("%d\n",cmd.option_count);
    g_cmd_checkpoint_ls_args.progname = util_string_join(" ", argv, 2);
    isula_libutils_default_log_config(argv[0], &lconf);
    subcommand_init(&cmd, options, sizeof(options) / sizeof(options[0]), argc, (const char **)argv, g_cmd_checkpoint_ls_desc,
                    g_cmd_checkpoint_ls_usage);
    //here failed
    //printf("%d\n",cmd.option_count);
    if (command_parse_args(&cmd, &g_cmd_checkpoint_ls_args.argc, &g_cmd_checkpoint_ls_args.argv)) {
        exit(exit_code);
    }
    //printf("%s\n",cmd.options->large);
    //printf("%s\n",g_cmd_checkpoint_ls_args.checkpoint_dir);
    
    if (isula_libutils_log_enable(&lconf)) {
        COMMAND_ERROR("checkpoint ls: log init failed");
        exit(exit_code);
    }


    //printf("%s\n",g_cmd_checkpoint_ls_args.argv[0]);
    if (g_cmd_checkpoint_ls_args.argc != 0) {
        
        COMMAND_ERROR("%s: \"checkpoint ls\" requires exactly 0 arguments.", g_cmd_checkpoint_ls_args.progname);
        exit(exit_code);
    }

    //printf("%s\n","name");
    //g_cmd_checkpoint_ls_args.name=g_cmd_checkpoint_ls_args.argv[0];
    //printf("%s\n",g_cmd_checkpoint_ls_args.name);
    if (client_checkpoint_ls(&g_cmd_checkpoint_ls_args, &checkpoints, &checkpoints_len) != 0) {
        ERROR("Create checkpoints failed");
        exit(exit_code);
    }


    exit(EXIT_SUCCESS);
    
}
