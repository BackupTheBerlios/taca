/*
 * node.c
 *
 * Representacio d'un node
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include <backend.h>
#include <linux_debug.h>

#include "linux_common.h"
#include "proc.h"


/*
 * Funcions
 */
static struct dentry *tacafs_i_lookup (struct inode *, struct dentry *);
static int tacafs_d_revalidate (struct dentry *, int);
static int tacafs_f_readdir (struct file *, void *, filldir_t);

/*
 * Estructures
 */

struct inode_operations tacafs_node_iops = {
    lookup: tacafs_i_lookup
};

struct dentry_operations tacafs_node_dops = {
    d_revalidate: tacafs_d_revalidate
};

struct file_operations tacafs_node_fops = {
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
  struct dentry *res = NULL;
  struct proces_t proces;
  struct node_t *node = (struct node_t*)parent_inode->u.generic_ip;
  
  dprint("dentry %s\n", dentry->d_name.name);

  if (!obtenir_proc(node, dentry->d_name.name, &proces))
      goto out;

  dprint2("match!\n");

  /*
   * Hem de generar el inode per mostrar un proces (el dentry)
   * Per tant un fitxer
   */
  res = tacafs_fill_dentry(parent_inode->i_sb, dentry, INODE_PROC,
	  MODE_FILE, &proces, sizeof(proces), NULL,
	  &tacafs_proc_dops, &tacafs_proc_fops);

out:
  dprint("inode per proces %s %s\n", dentry->d_name.name,
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
    struct node_t *node = (struct node_t*)dentry->d_inode->u.generic_ip;
    dprint("%s/%s\n", node->parent->nom, node->nom);
    return node_valid(node);
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
    struct proces_t *llista;
    struct node_t *node = (struct node_t*)dentry->d_parent->d_inode->u.generic_ip;
    int num;

    dprint2("==>\n");

    if(file->f_pos > 0 )
        return 1;
    
    if(filldir(dirent, ".", 1, file->f_pos++, dentry->d_inode->i_ino, DT_DIR) ||
       filldir(dirent, "..", 2, file->f_pos++, dentry->d_parent->d_inode->i_ino, DT_DIR))
	return 0;

    /* LListem els processos del node */
    num = num_procs(node);
    if (num > 0) {
	llista = (struct proces_t *)kmalloc(sizeof(struct proces_t)*num ,GFP_KERNEL);
	if (llistar_procs(node, llista, num) > 0) {
	    dprint2("Creant processos\n");
	    num = 0;
	    while (llista != NULL) {
		if(filldir(dirent, llista->nom, strlen(llista->nom), file->f_pos++, INO(++num, INODE_PROC), DT_REG ))
		    return 0;
		llista = llista->next;
	    }
	    dprint2("Fitxers per processos creats\n");
	}
	kfree (llista);
    }
    else
	dprint2("No hi ha processos a mostrar\n");
    return 1;
}

