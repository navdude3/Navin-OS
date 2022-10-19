#if !defined(_VFS_H)
#define _VFS_H
#include "types.h"

typedef file_ops_struct_t {
    int (*open)     (void);
    int (*close)    (void);
    int (*read)     (void* buf, int32_t nbytes);
    int (*write)    (void* buf, int32_t nbytes);
} file_ops;

#endif // MACRO
