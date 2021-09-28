
#ifndef DAEMON_MODULES_API_CHECKPOINT_API_H
#define DAEMON_MODULES_API_CHECKPOINT_API_H

#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif



int checkpoint_create(char* container,char* dir);


int checkpoint_remove(char* container,char* dir);

int checkpoint_list();


#ifdef __cplusplus
}
#endif

#endif // DAEMON_MODULES_API_CHECKPOINT_API_H
