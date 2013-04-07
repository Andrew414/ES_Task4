#include "calc.h"

static char names[][PROCFS_MAX_SIZE] = {PROCFS_FIRST, PROCFS_SECOND, PROCFS_OPERAND, PROCFS_RESULT};
static int indices[FILE_COUNT];
static char** procfs_buffer;
static struct proc_dir_entry** child_procs;

int __init module_load(void)
{
  int i;

	printk(KERN_INFO "Calculator module is loaded.\n");

	procfs_buffer = (char**) kmalloc(sizeof(char*) * FILE_COUNT, GFP_KERNEL);
	for (i = 0; i < FILE_COUNT; i++) {
		procfs_buffer[i] = (char*) kmalloc(sizeof(char) * PROCFS_MAX_SIZE, GFP_KERNEL);
		procfs_buffer[i][0] = '\0';
	}

	child_procs = (struct proc_dir_entry**) kmalloc(
		sizeof(struct proc_dir_entry *) * 4, GFP_KERNEL);

	for (i = 0; i < FILE_COUNT; i++) {
		child_procs[i] = create_proc_entry(names[i], 0644, NULL);
		if (!child_procs[i]) {
			remove_proc_entry(names[i], NULL);
			printk(KERN_ERR "Failed to create /proc/%s", names[i]);
			return -ENOMEM;
		}
		indices[i] = i + 1;
		child_procs[i]->read_proc  = proc_read;
		child_procs[i]->write_proc = proc_write;
		child_procs[i]->data	   = (void*) &indices[i];
	}

	printk(KERN_INFO "Calculator processes were created.\n");
	return 0;
}

void __exit module_unload(void)
{	
	int i;

	for (i = 0; i < FILE_COUNT; i++) {
		remove_proc_entry(names[i], NULL);
		kfree(procfs_buffer[i]);
	}

	kfree(child_procs);
	kfree(procfs_buffer);

	printk(KERN_INFO "[CALC PROC] proc entries were removed.\n");
	printk(KERN_INFO "[CALC PROC] module is unloaded.\nBye-bye!");
}

module_init(module_load);
module_exit(module_unload);
