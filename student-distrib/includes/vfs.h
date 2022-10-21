#if !defined(_VFS_H)
#define _VFS_H
#include "types.h"

typedef file_ops_struct_t {
    int (*open)     (const uint8_t* fname);
    int (*close)    (void);
    int (*read)     (uint8_t* buf, uint32_t length);
    int (*write)    (uint8_t* buf, uint32_t length);
} file_ops;

#endif // MACRO
