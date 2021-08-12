//
// Created by root on 2021/8/12.
//

#include "checkpoint.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_arguments.h"
#include "isula_libutils/log.h"
#include "utils.h"
#include "isula_connect.h"
#include "console.h"
#include "error.h"
#include "connect.h"
#include "create.h"

#include "start.h"
#include "wait.h"

const char g_cmd_checkpoint_desc[] = "Run a command in a new container";
const char g_cmd_checkpoint_usage[] = "run [OPTIONS] ROOTFS|IMAGE [COMMAND] [ARG...]";

struct client_arguments g_cmd_checkpoint_args = {
        .runtime = "",
        .restart = "no",
        .pull = "missing"
};

int checkpoint(struct client_arguments *args);

int cmd_checkpoint_main(int argc, const char **argv)
{
    /*
     * isula checkpoint create --checkpoint-dir --leave-running
     * isula checkpoint ls --checkpoint-dir
     * isula checkpoint rm --checkpoint-dir
     */



    int ret = 0;
    command_t cmd = { 0 };
    struct isula_libutils_log_config lconf = { 0 };


    //args
    if (client_arguments_init(&g_cmd_run_args)) {
        COMMAND_ERROR("client arguments init failed");
        exit(ECOMMON);
    }
    g_cmd_run_args.custom_conf.attach_stdout = true;
    g_cmd_run_args.custom_conf.attach_stderr = true;
    g_cmd_run_args.resize_cb = do_resize_run_console;

    g_cmd_run_args.progname = argv[0];
    g_cmd_run_args.subcommand = argv[1];
    struct command_option options[] = { LOG_OPTIONS(lconf) COMMON_OPTIONS(g_cmd_run_args) CREATE_OPTIONS(g_cmd_run_args)
            CREATE_EXTEND_OPTIONS(g_cmd_run_args) RUN_OPTIONS(g_cmd_run_args)
    };
    isula_libutils_default_log_config(argv[0], &lconf);
    command_init(&cmd, options, sizeof(options) / sizeof(options[0]), argc, (const char **)argv, g_cmd_run_desc,
                 g_cmd_run_usage);
    if (command_parse_args(&cmd, &g_cmd_run_args.argc, &g_cmd_run_args.argv) || run_checker(&g_cmd_run_args)) {
        exit(EINVALIDARGS);
    }

    if (isula_libutils_log_enable(&lconf)) {
        COMMAND_ERROR("log init failed");
        exit(ECOMMON);
    }

    //do
    ret = checkpoint(&g_cmd_run_args);
    if (ret) {
        ERROR("Container \"%s\" checkpoint failed", g_cmd_run_args.name);
        exit(ret);
    }


    if (g_cmd_run_args.detach) {
        printf("%s\n", g_cmd_run_args.name);
    }
}

int checkpoint(struct client_arguments *args){
    printf("name:%s\n", args.name);
    printf("progname:%s\n", args.progname);
    printf("subcommand:%s\n", args.subcommand);
    printf("optionname:%s\n", args.option.name);
    printf("optionval:%s\n", args.option.val);
}