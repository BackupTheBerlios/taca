/*
 * cluster.c
 *
 * Representacio d'un cluster
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include <backend.h>
#include <linux_debug.h>

#include "linux_common.h"
#include "node.h"


/*
 * Funcions
 */
static int tacafs_d_revalidate (struct dentry *, int);
static ssize_t tacafs_f_read (struct file *, char *, size_t, loff_t *);
static ssize_t tacafs_f_write (struct file *, const char *, size_t, loff_t *);

/*
 * Estructures
 */

struct dentry_operations tacafs_proc_dops = {
    d_revalidate: tacafs_d_revalidate
};

struct file_operations tacafs_proc_fops = {
    read : &tacafs_f_read,
    write: &tacafs_f_write
};


/*
 * Operacions del sistema de fitxers
 */


/*
 * Operacions sobre dentrys
 */


/**
 * tacafs_d_revalidate - Permet verificar si un dentry encara es valid
 **/
static int tacafs_d_revalidate (struct dentry *dentry, int flags)
{
    struct proces_t *proces = (struct proces_t*)dentry->d_inode->u.generic_ip;
    dprint("%s/%s/%s\n", proces->parent->parent->nom, proces->parent->nom, proces->nom);
    return proc_valid(proces);
}


/*
 * Operacions sobre files
 */

static char file_buf[1024] = "Hello World\n";
static int file_size = 12;

/**
 * tacafs_f_read - Realitza una lectura sobre un fitxer
 **/
static ssize_t tacafs_f_read (struct file *file, char *buf, size_t max, loff_t *offset)
{
    int buflen;

    if(*offset > 0)
	return 0;

    dprint("%d %d\n", max, (int)*offset);
    buflen = file_size > max ? max : file_size;
    __generic_copy_to_user(buf, file_buf, buflen);
    //copy_to_user(buf, file_buf, buflen);
    /* avancem l'offset */
    *offset += buflen;
    return buflen;
}


/**
 * tacafs_f_write - Realitza una escriptura sobre un fitxer
 **/
static ssize_t tacafs_f_write (struct file *file, const char *buf, size_t maxlen, loff_t *offset)
{
    int count;

    if(*offset > 0) {
	printk("offset positiu %d\n", (int)*offset);
	return 0;
    }

    count = maxlen > sizeof(file_buf) ? sizeof(file_buf) : maxlen;
    __generic_copy_from_user(file_buf, buf, count);
    //copy_from_user(file_buf, buf, maxlen);
    dprint("maxlen=%d, off=%d\n", maxlen, (int)*offset);
    *offset += count;
    if(*offset > file_size)
	file_size = *offset;
    return count;
}

