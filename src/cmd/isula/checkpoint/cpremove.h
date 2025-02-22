/******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * iSulad licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more detairm.
 * Author: wangfengtu
 * Create: 2020-09-05
 * Description: provide list CHECKPOINT definition
 ******************************************************************************/
#ifndef CMD_ISULA_CHECKPOINT_REMOVE_H
#define CMD_ISULA_CHECKPOINT_REMOVE_H

#include "client_arguments.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char g_cmd_checkpoint_rm_desc[];
extern const char g_cmd_checkpoint_rm_usage[];
extern struct client_arguments g_cmd_checkpoint_rm_args;
int cmd_checkpoint_rm_main(int argc, const char **argv);

#ifdef __cplusplus
}
#endif

#endif // CMD_ISULA_CHECKPOINT_REMOVE_H
