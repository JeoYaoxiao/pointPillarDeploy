#ifndef _XRP_KERNEL_DEFS_H
#define _XRP_KERNEL_DEFS_H

#define XRP_IOCTL_MAGIC 'r'
#define XRP_IOCTL_ALLOC _IO(XRP_IOCTL_MAGIC, 1)
#define XRP_IOCTL_FREE _IO(XRP_IOCTL_MAGIC, 2)
#define XRP_IOCTL_QUEUE _IO(XRP_IOCTL_MAGIC, 3)
#define XRP_IOCTL_WAIT _IO(XRP_IOCTL_MAGIC, 4)
#define XRP_IOCTL_SYNC _IO(XRP_IOCTL_MAGIC, 5)

#pragma pack(push) /* push current alignment to stack */
#pragma pack(4)    /* set alignment to 4 byte boundary */

struct xrp_ioctl_alloc {
    uint32_t size;
    uint32_t align;
    uint32_t addr;
    uint64_t ptr;
};

enum {
    XRP_FLAG_READ = 0x1,
    XRP_FLAG_WRITE = 0x2,
    XRP_FLAG_READ_WRITE = 0x3,
};

struct xrp_ioctl_buffer {
    uint32_t flags;
    uint32_t size;
    uint32_t addr;
};

struct xrp_ioctl_queue {
    uint32_t cookie;
    uint32_t in_data_size;
    uint32_t out_data_size;
    uint32_t buffer_size;
    uint64_t in_data_addr;
    uint64_t out_data_addr;
    uint64_t buffer_addr;
};

struct xrp_ioctl_wait {
    uint32_t cookie;
};

#pragma pack(pop) /* restore original alignment from stack */

#endif
