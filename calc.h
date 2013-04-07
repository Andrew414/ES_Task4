#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>

/* Module proc file name. */
#define CALC_PROC "calc"

/* Module proc files names. */
#define PROCFS_RESULT  "calc_result"
#define PROCFS_FIRST   "calc_first"
#define PROCFS_SECOND  "calc_second"
#define  PROCFS_OPERAND "calc_operator"

/* Procfs maximum buffer size. */
#define PROCFS_MAX_SIZE 16


static int proc_read(char*, char**, off_t, int, int*, void*);

static int proc_write(struct file*, const char*, unsigned long, void*);

static int __init module_load(void);

static void __exit module_upload(void);
