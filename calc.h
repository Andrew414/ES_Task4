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
