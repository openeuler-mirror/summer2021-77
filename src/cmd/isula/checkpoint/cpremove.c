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
const char g_cmd_checkpoint_rm_usage[] = "rm [OPTIONS] CONTAINER";

struct client_arguments g_cmd_checkpoint_rm_args;

#define REMOVE_OPTIONS(cmdargs) \
    { CMD_OPT_TYPE_STRING, false, "checkpoint-dir", 0, &(cmdargs).checkpoint_dir, "Use a custom checkpoint storage directory", NULL },






static int client_checkpoint_rm(const struct client_arguments *args, char ***volumes, size_t *volumes_len){
   
    isula_connect_ops *ops =NULL;
    struct isula_remove_checkpoint_request request ={0};
    struct isula_remove_checkpoint_response *response =NULL;
    client_connect_config_t config ={0};
    int ret = 0;

    response = util_common_calloc_s(sizeof(struct isula_remove_checkpoint_response));
    if (response==NULL){
        ERROR("Out of memory");
        return -1;
    }

    request.container = args->name;
    char checkpoint_dir[1000]="/tmp/isula-criu/";
    strcat(checkpoint_dir,args->name);
    if(args->checkpoint_dir){
         strcat(args->checkpoint_dir,args->name);
         request.dir=args->checkpoint_dir;
    }else{
        request.dir=checkpoint_dir;
    }
    
    

    ops = get_connect_client_ops();
    
    if(ops==NULL || ops->checkpoint.remove==NULL){
        ERROR("Unimplemented ops");
        ret=-1;
        goto out;
    }
    //把参数放到了config里
    config = get_connect_config(args);
 
    ret=ops->checkpoint.remove(&request,response,&config);
    if(ret!=0){
        client_print_error(response->cc,response->server_errono,response->errmsg);
        if(response->server_errono){
            ret=ESERVERERROR;
        }
        goto out;
    }

out:
    //isula_create_checkpoint_response_free(response);
    return ret;
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
