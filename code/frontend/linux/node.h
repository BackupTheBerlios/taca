/*
 * node.h
 *
 * Representacio d'un node
 */


#ifndef _node_h_
#define _node_h_

extern struct inode_operations tacafs_node_iops;
extern struct dentry_operations tacafs_node_dops;
extern struct file_operations tacafs_node_fops;

#endif
