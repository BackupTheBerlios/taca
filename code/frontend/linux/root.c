/*
 * root.c
 *
 * Arrel del nostre sistema de fitxers
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>

#include <linux_debug.h>

#include "linux_common.h"

#define TACAFS_MAGIC 0x20040617


/*
 * Funcions
 */
static struct super_block *tacafs_read_super (struct super_block *, void *, int);
void tacafs_s_readinode (struct inode *);
int tacafs_s_statfs (struct super_block *, struct statfs *);
struct dentry *tacafs_i_lookup (struct inode *, struct dentry *);
int tacafs_f_readdir (struct file *, void *, filldir_t);

/*
 * Estructures
 */

/* declaracio del file_system_type */
DECLARE_FSTYPE (tacafs_type, "tacafs", tacafs_read_super, 0);
struct inode *tacafs_root_inode;

/* numeros d'inode */
enum {
    TACAFS_ROOT_INO = 1
};

static struct super_operations tacafs_sops = {
    read_inode: tacafs_s_readinode,
    statfs: tacafs_s_statfs
    //  put_inode: tacafs_s_putinode, // inode fora de la cache
};

static struct inode_operations tacafs_root_iops = {
    lookup: tacafs_i_lookup
};

static struct file_operations tacafs_root_fops = {
    read   : generic_read_dir,
    readdir: tacafs_f_readdir
};



/*
 * Operacions del sistema de fitxers
 */

void crea_arbre_estatic (struct super_block *sb)
{
    dprint("creant arbre estatic...\n");
    tacafs_create_dir(sb, S_IRWXUGO, NULL, NULL, sb->s_root, "c1");
}

/**
 * tacafs_read_super - Omple les dades del super bloc
 *
 * Cridat en quan el kernel prova de muntar el SF, ens passa el super block en
 * contruccio per a que l'acabem d'omplir.
 **/
static struct super_block *tacafs_read_super (struct super_block *sb, void *buf, int size)
{
    sb->s_blocksize = 1024;
    sb->s_blocksize_bits = 10;
    sb->s_magic = TACAFS_MAGIC;
    sb->s_op = &tacafs_sops; // operacions del super block
    sb->s_type = &tacafs_type; // file_system_type

    tacafs_root_inode = tacafs_make_inode(sb, S_IFDIR|S_IRWXU,
	    &simple_dir_inode_operations, &dcache_dir_ops);
	    //&tacafs_root_iops, &tacafs_root_fops);
    if (!tacafs_root_inode)
	return ERR_PTR(-ENOMEM);

    if(!(sb->s_root = d_alloc_root(tacafs_root_inode))) { // allocata un dentry
        iput(tacafs_root_inode);
	return ERR_PTR(-ENOMEM);
    }

    crea_arbre_estatic(sb);

    dprint("s'ha retornat un super_block valid\n");
    return sb;
}


/*
 * Operacions del super_block
 */


/**
 * tacafs_s_readinode - Cridat per llegir un inode del SF muntat
 **/
void tacafs_s_readinode (struct inode *inode)
{
    inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
    dprint("\n");
}


/**
 * tacafs_s_statfs - Cridat per obtenir informacio del SF com tamany, etc.
 **/
int tacafs_s_statfs (struct super_block *sb, struct statfs *buf) {
    buf->f_type = TACAFS_MAGIC;
    buf->f_bsize = PAGE_SIZE/sizeof(long);
    buf->f_bfree = 0;
    buf->f_bavail = 0;
    buf->f_ffree = 0;
    buf->f_namelen = NAME_MAX;
    dprint("\n");
    return 0;
}


/*
 * Operacions sobre inodes
 */


/**
 * tacafs_i_lookup - 
 * Aquesta funcio genera el inode pel dentry que es vol mirar 
 * El inode que es vol generar és "fill" del parent_inode
 *
 *
 * TODO: comprovar que el directori on es vol entrar és correcte
 */

struct dentry *tacafs_i_lookup (struct inode *parent_inode, struct dentry *dentry)
{
  struct inode *file_inode;
  
  dprint("dentry %s\n", dentry->d_name.name);
  
  
  /* Hem de generar el inode per mostrar un clusters (el dentry) 
   * Per tant un directori
   */
  // allocate an inode object
 // if(!(file_inode = iget( parent_inode->i_sb, DIR_INODE_NUMBER)))
      return ERR_PTR(-ENOMEM);
  //file_inode->i_size = file_size;
  file_inode->i_mode = S_IFDIR|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
//  file_inode->i_fop = &tacafs_dir_fops;
//  file_inode->i_op = &tacafs_iops;
  //  add the inode to the dentry object
  d_add(dentry, file_inode);

  dprint(" lookup: inode per cluster %s generat\n", dentry->d_name.name);
  return NULL;
}

//  readdir: tacafs_f_readdir

/*
 * Informacio/codi especific dels moduls de linux
 */

static int __init tacafs_init(void)
{
    	return register_filesystem(&tacafs_type);
}

static void __exit tacafs_exit(void)
{
    	unregister_filesystem(&tacafs_type);
}

module_init(tacafs_init);
module_exit(tacafs_exit);

//MODULE_INFO(tag, info);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("tacafs crew");
MODULE_DESCRIPTION("taca file system module");
//MODULE_PARM_DESC(_parm, desc);

