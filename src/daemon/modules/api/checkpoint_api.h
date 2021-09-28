
#ifndef DAEMON_MODULES_API_CHECKPOINT_API_H
#define DAEMON_MODULES_API_CHECKPOINT_API_H

#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif



char* checkpoint_create(char* container,char* dir);


char* checkpoint_remove(char* container,char* dir);

char* checkpoint_list(char* dir);


#ifdef __cplusplus
}
#endif

#endif // DAEMON_MODULES_API_CHECKPOINT_API_H
