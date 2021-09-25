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
 * Create: 2020-09-07
 * Description: provide isula volume functions
 ******************************************************************************/
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

int checkpoint_remove(char *name)
{
    int ret = 0;
    volume_driver *driver = NULL;
    struct volume_names *vns = NULL;

    if (name == NULL) {
        ERROR("invalid null param for volume remove");
        return -1;
    }

    mutex_lock(&g_vs.mutex);
    driver = lookup_driver_by_volume_name(name);
    if (driver == NULL) {
        ret = -1;
        goto out;
    }

    vns = get_name_refs(g_vs.name_refs, name);
    if (vns != NULL && vns->names_len > 0) {
        ERROR("remove volume %s failed: volume is used by container %s", name, vns->names[0]);
        isulad_try_set_error_message("remove volume %s failed: volume is used by container %s", name, vns->names[0]);
        ret = -1;
        goto out;
    }

    ret = driver->remove(name);
    if (ret != 0) {
        goto out;
    }

out:
    mutex_unlock(&g_vs.mutex);
    free_volume_names(vns);

    return ret;
}

int checkpoint_create(char* container,char* checkpoint,char* dir)
{
    dir="/var/lib/isulad/engines/lcr/"
    struct lxc_container *c;
    c=lxc_container_new(container,dir);
    if (!c) {
		ERROR("System error loading %s\n", container);
		return 0;
	}
    if (!c->is_defined(c)) {
		ERROR("Error response from daemon: No such container:%s\n",container);
		return 0;
	}
    if (!c->is_running(c)) {
		ERROR("%s not running, not checkpointing\n", container);
		return 0;
	}
   
    char checkpoint_dir[1000]="/tmp/isula-criu/";
    strcat(checkpoint_dir,c->name);
    bool res;
    if(args->checkpoint_dir){
        res = c->checkpoint(c,args->checkpoint_dir,true,false);
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

int checkpoint_list(){

}


