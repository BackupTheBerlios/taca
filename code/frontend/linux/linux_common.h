/*
 * linux_common.h
 *
 * Operacions comunes a tothom en el VFS de linux
 */

#ifndef _linux_common_h_
#define _linux_common_h_

#include "backend.h"

/* Tipus d'inode per als objectes del SF */
enum
{
    INODE_ROOT = 1,
    INODE_CLUSTER,
    INODE_NODE,
    INODE_PROC
};

/* Combina el tipus d'inode amb el seu identificador per crear-ne el numero */
#define INO(id, type) \
	((id << 16) | type)

/* Modes per als diferents objectes del SF */
#define MODE_GEN  S_IRUSR|S_IWUSR|S_IRGRP
#define MODE_DIR  S_IFDIR|S_IXUSR|S_IXGRP|MODE_GEN
#define MODE_FILE S_IFREG|MODE_GEN

#define TRUE 1
#define FALSE 0

extern struct inode *tacafs_make_inode (struct super_block *, int, int,
	struct inode_operations *, struct file_operations *);
extern struct dentry *tacafs_fill_dentry (struct super_block *,
	struct dentry *dentry, int, int, void *, int, struct inode_operations *,
	struct dentry_operations *, struct file_operations *);
extern struct dentry *tacafs_create_file (struct super_block *, struct dentry *,
	int, const char *, void *, int,  struct inode_operations *,
	struct dentry_operations *dops, struct file_operations *);
extern struct dentry *tacafs_create_dir (struct super_block *, struct dentry *,
	int, const char *, void *, int, struct inode_operations *,
	struct dentry_operations *dops, struct file_operations *);

/* tots els objectes van a la mateixa cache */
struct tacafs_inode_info
{
    union {
	struct cluster_t cluster;
	struct node_t    node;
	struct proces_t  proces;
    } u;
};

extern kmem_cache_t *tacafs_inode_cachep;

#endif

