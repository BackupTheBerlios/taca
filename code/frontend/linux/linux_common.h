/*
 * linux_common.h
 *
 * Operacions comunes a tothom en el VFS de linux
 */

#ifndef _linux_common_h_
#define _linux_common_h_

struct inode *tacafs_make_inode (struct super_block *, int,
	struct inode_operations *, struct file_operations *);
static struct dentry *tacafs_create_file (struct super_block *, int,
	struct inode_operations *, struct file_operations *, struct dentry *,
	const char *);
static struct dentry *tacafs_create_dir (struct super_block *, int,
	struct inode_operations *, struct file_operations *, struct dentry *,
	const char *);

extern struct inode_operations simple_dir_inode_operations;

#endif

