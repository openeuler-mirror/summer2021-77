#ifndef DAEMON_MODULES_API_CHECKPOINT_API_H
#define DAEMON_MODULES_API_CHECKPOINT_API_H

#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

struct checkpoint {
    char *dir;
    char *name;
};

struct checkpoints {
    struct checkpoint **ches;
    size_t ches_len;
};

typedef struct {
    char *dir;

    char *name;
}checkpoint_checkpoint;

char* checkpoint_create(char* container,char* dir);

char* checkpoint_restore(char* container,char* dir);

char* checkpoint_remove(char* container,char* dir);

struct checkpoints* checkpoint_list(char* dir);


#ifdef __cplusplus
}
#endif

#endif // DAEMON_MODULES_API_CHECKPOINT_API_H
