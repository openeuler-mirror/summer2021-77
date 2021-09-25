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
 * Description: provide volume functions
 *********************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <isula_libutils/defs.h>
#include <isula_libutils/json_common.h>
#include "isula_libutils/volume_list_volume_request.h"
#include "isula_libutils/volume_list_volume_response.h"
#include "isula_libutils/volume_remove_volume_request.h"
#include "isula_libutils/volume_remove_volume_response.h"
#include "isula_libutils/volume_prune_volume_request.h"
#include "isula_libutils/volume_prune_volume_response.h"
#include "isula_libutils/volume_volume.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "checkpoint_cb.h"
#include "utils.h"
#include "error.h"
#include "err_msg.h"
#include "isula_libutils/log.h"
#include "volume_api.h"


/* checkpoint create cb */
static int volume_create_cb(const volume_prune_volume_request *request, volume_prune_volume_response **response)
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

    if (volume_prune(request->container,request->checkpoint,request->dir) != 0) {
        cc = ISULAD_ERR_EXEC;
        goto out;
    }

    (*response)->volumes = pruned->names;
    pruned->names = NULL;
    (*response)->volumes_len = pruned->names_len;
    pruned->names_len = 0;

    EVENT("Volume Event: {Object: prune volumes, Type: Pruned");

out:
    if (*response != NULL) {
        (*response)->cc = cc;
        if (g_isulad_errmsg != NULL) {
            (*response)->errmsg = util_strdup_s(g_isulad_errmsg);
            DAEMON_CLEAR_ERRMSG();
        }
    }
    free_volume_names(pruned);

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
}
