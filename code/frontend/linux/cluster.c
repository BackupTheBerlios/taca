/*
 * cluster.c
 *
 * Representacio d'un cluster
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include <backend.h>
#include <linux_debug.h>

#include "linux_common.h"
#include "node.h"


/*
 * Funcions
 */
static struct dentry *tacafs_i_lookup (struct inode *, struct dentry *);
static int tacafs_d_revalidate (struct dentry *, int);
static int tacafs_f_readdir (struct file *, void *, filldir_t);

/*
 * Estructures
 */

struct inode_operations tacafs_cluster_iops = {
    lookup: tacafs_i_lookup
};

struct dentry_operations tacafs_cluster_dops = {
    d_revalidate: tacafs_d_revalidate
};

struct file_operations tacafs_cluster_fops = {
    read   : generic_read_dir,
    readdir: tacafs_f_readdir
};


/*
 * Operacions del sistema de fitxers
 */


/*
 * Operacions sobre inodes
 */


/**
 * tacafs_i_lookup - Crea el inode del dentry qye es vol mirar
 **/
static struct dentry *tacafs_i_lookup (struct inode *parent_inode, struct dentry *dentry)
{
  struct dentry *res = ERR_PTR(-ENOENT);
  struct cluster_t *cluster = parent_inode->u.generic_ip;
  struct node_t node;
  
  dprint("dentry %s\n", dentry->d_name.name);

  if (!obtenir_node(cluster, dentry->d_name.name, &node))
      goto out;
  
  dprint2("match!\n");

  /*
   * Hem de generar el inode per mostrar un node (el dentry)
   * Per tant un directori
   */
  res = tacafs_fill_dentry(parent_inode->i_sb, dentry, INODE_NODE,
	  MODE_DIR, &node, sizeof(node), &tacafs_node_iops,
	  &tacafs_node_dops, &tacafs_node_fops);

out:
  dprint("inode per node %s %s\n", dentry->d_name.name,
	  res == ERR_PTR(-ENOENT) ? "inexistent" :
	  (res == NULL ? "generat" : "no generat per error"));
  return res;
}


/*
 * Operacions sobre dentrys
 */


/**
 * tacafs_d_revalidate - Permet verificar si un dentry encara es valid
 **/
static int tacafs_d_revalidate (struct dentry *dentry, int flags)
{
    struct cluster_t *cluster = dentry->d_inode->u.generic_ip;
    dprint("%s\n", cluster->nom);
    return cluster_valid(cluster);
}


/*
 * Operacions sobre files
 */


/**
 * tacafs_f_readdir - Genera la llista de continguts d'un directori
 **/
static int tacafs_f_readdir (struct file *file, void *dirent, filldir_t filldir)
{
    struct dentry *dentry = file->f_dentry;
    struct node_t *llista;
    struct cluster_t *cluster = dentry->d_parent->d_inode->u.generic_ip;
    int num;

    dprint2("==>\n");

    if(file->f_pos > 0 )
	return 1;

    if(filldir(dirent, ".", 1, file->f_pos++, dentry->d_inode->i_ino, DT_DIR) ||
	    filldir(dirent, "..", 2, file->f_pos++, dentry->d_parent->d_inode->i_ino, DT_DIR))
	return 0;

    /* Llistem tots els nodes del cluster */
    num = num_nodes(cluster);
    if (num > 0) {
	llista = (struct node_t *)kmalloc(sizeof(struct node_t)*num ,GFP_KERNEL);
	if (llistar_nodes(cluster,llista, num) > 0) {
	    dprint2("Creem els directoris pels nodes\n");
	    num = 0;
	    while (llista != NULL) {
		if(filldir(dirent, llista->nom, strlen(llista->nom), file->f_pos++, INO(++num,INODE_NODE), DT_DIR ))
		    return 0;
		llista = llista->next;
	    }
	    dprint2("Directoris per nodes creats\n");
	}
	kfree(llista);
    }
    else
	dprint2("No hi ha nodes a mostrar\n");
    return 1;
}

