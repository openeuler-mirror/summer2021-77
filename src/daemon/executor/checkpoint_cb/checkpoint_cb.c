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
 * Create: 2020-09-03
 * Description: provide checkpoint functions
 *********************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <isula_libutils/defs.h>
#include <isula_libutils/json_common.h>

#include "callback.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "checkpoint_cb.h"
#include "utils.h"
#include "error.h"
#include "err_msg.h"
#include "isula_libutils/log.h"
#include "checkpoint_api.h"


/* checkpoint create cb */
static int checkpoint_create_cb(const checkpoint_create_checkpoint_request *request, checkpoint_create_checkpoint_response **response)
{
    uint32_t cc = ISULAD_SUCCESS;


    DAEMON_CLEAR_ERRMSG();

    if (request == NULL || request->container==NULL || response == NULL) {
        ERROR("Invalid input arguments");
        return EINVALIDARGS;
    }

    *response = util_common_calloc_s(sizeof(checkpoint_create_checkpoint_response));
    if (*response == NULL) {
        ERROR("Out of memory");
        cc = ISULAD_ERR_MEMOUT;
        goto out;
    }

    EVENT("Checkpoint Event: {Object: create checkpoint, Type: Create}");

    char* container=checkpoint_create(request->container,request->dir);
    if (container==NULL) {
        cc = ISULAD_ERR_EXEC;
        goto out;
    }
    (*response)->container = container;

    

    EVENT("Checkpoint Event: {Object: create checkpoints, Type: Created");

out:
    if (*response != NULL) {
        (*response)->cc = cc;
        if (g_isulad_errmsg != NULL) {
            (*response)->errmsg = util_strdup_s(g_isulad_errmsg);
            DAEMON_CLEAR_ERRMSG();
        }
    }
    //free_checkpoint_names(created);

    return (cc != ISULAD_SUCCESS) ? ECOMMON : 0;
}

static int checkpoint_remove_cb(const checkpoint_remove_checkpoint_request *request, checkpoint_remove_checkpoint_response **response)
{
    uint32_t cc = ISULAD_SUCCESS;


    DAEMON_CLEAR_ERRMSG();

    if (request == NULL || request->container==NULL || response == NULL) {
        ERROR("Invalid input arguments");
        return EINVALIDARGS;
    }

    *response = util_common_calloc_s(sizeof(checkpoint_remove_checkpoint_response));
    if (*response == NULL) {
        ERROR("Out of memory");
        cc = ISULAD_ERR_MEMOUT;
        goto out;
    }

    EVENT("Checkpoint Event: {Object: create checkpoint, Type: Create}");

    char* container = checkpoint_remove(request->container,request->dir);
    if (container == NULL) {
        cc = ISULAD_ERR_EXEC;
        goto out;
    }
    (*response)->container=container;


    

    EVENT("Checkpoint Event: {Object: create checkpoints, Type: Created");

out:
    if (*response != NULL) {
        (*response)->cc = cc;
        
        if (g_isulad_errmsg != NULL) {
            (*response)->errmsg = util_strdup_s(g_isulad_errmsg);
            DAEMON_CLEAR_ERRMSG();
        }
    }
    //free_checkpoint_names(created);

    return (cc != ISULAD_SUCCESS) ? ECOMMON : 0;
}

static int checkpoint_list_cb(const checkpoint_list_checkpoint_request *request, checkpoint_list_checkpoint_response **response)
{
    uint32_t cc = ISULAD_SUCCESS;


    DAEMON_CLEAR_ERRMSG();

    if (request == NULL || response == NULL) {
        ERROR("Invalid input arguments");
        return EINVALIDARGS;
    }

    *response = util_common_calloc_s(sizeof(checkpoint_list_checkpoint_response));
    if (*response == NULL) {
        ERROR("Out of memory");
        cc = ISULAD_ERR_MEMOUT;
        goto out;
    }

    EVENT("Checkpoint Event: {Object: create checkpoint, Type: Create}");

    if (checkpoint_list(request->dir) == NULL) {
        cc = ISULAD_ERR_EXEC;
        goto out;
    }
    

    

    EVENT("Checkpoint Event: {Object: create checkpoints, Type: Created");

out:
    if (*response != NULL) {
        (*response)->cc = cc;
        (*response)->checkpoints=checkpoint_list(request->dir);
        if (g_isulad_errmsg != NULL) {
            (*response)->errmsg = util_strdup_s(g_isulad_errmsg);
            DAEMON_CLEAR_ERRMSG();
        }
    }
    //free_checkpoint_names(created);

    return (cc != ISULAD_SUCCESS) ? ECOMMON : 0;
}

/* checkpoint callback init */
void checkpoint_callback_init(service_checkpoint_callback_t *cb)
{
    if (cb == NULL) {
        ERROR("Invalid input arguments");
        return;
    }

    cb->create = checkpoint_create_cb;
    cb->remove = checkpoint_remove_cb;
    cb->list   = checkpoint_list_cb;
}
