#include "calc.h"

/* Hooks when reading from the module proc file. */
static int proc_read(char* buffer, char** buffer_location, off_t offset,
  	int buffer_length, int* eof, void* data)
{
	int ret, number;
	long a, b, result = 0;
	char op, *end;

	number = *((int*) data);
	printk(KERN_INFO "Reading /proc/%s", names[number - 1]);

	if (offset > 0) {
		ret = 0;
	} else {
		if (number == 4) {
			a = simple_strtol(procfs_buffer[0], &end, 10);
			if (a == 0 && (*end == procfs_buffer[0][0])) {
				return sprintf(buffer, "\n%s\n\n", "First operand is not integer.");
			}
	
			b = simple_strtol(procfs_buffer[1], &end, 10);
			if (b == 0 && (*end == procfs_buffer[1][0])) {
				return sprintf(buffer, "\n%s\n\n", "Second operand is not integer.");
			}
			op = procfs_buffer[2][0];
			switch (op) {
				case '+': result = a + b; break;
				case '-': result = a - b; break;
				case '*': result = a * b; break;
				case '/':
					if (b == 0) {
						return sprintf(buffer, "\n%s\n\n",
							"Division by zero!");
					}
					result = a / b;
					break;
				default: return sprintf(buffer, "\nUnknown operand: %c\n\n", op);
			}
			ret = sprintf(buffer, "\n%ld %c %ld = %ld\n\n", a, op, b, result);
		} else {
			ret = sprintf(buffer, "\n%s\n\n", procfs_buffer[number - 1]);
		}
	}

	return ret;
}

/* Hooks when writing the data into the module proc file. */
static int proc_write(struct file* file, const char* buffer,
		unsigned long count, void* data)
{
	int procfs_buffer_size, index;

	if (count >= PROCFS_MAX_SIZE) {
		procfs_buffer_size = PROCFS_MAX_SIZE - 1;
	} else {
		procfs_buffer_size = count;
	}
	index = *((int*) data);
	if (index != 4) {
		if (copy_from_user(procfs_buffer[index - 1], buffer, procfs_buffer_size)) {
			printk(KERN_ERR "Failed to write to /proc/%s\n", names[index - 1]);
			return -EFAULT;
		}
		procfs_buffer[index - 1][procfs_buffer_size - 1] = '\0';
	}

	printk(KERN_INFO "Writing to /proc/%s\n", names[index - 1]);
	return procfs_buffer_size;
}


