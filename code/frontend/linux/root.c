/*
 * root.c
 *
 * Arrel del nostre sistema de fitxers
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include <backend.h>
#include <linux_debug.h>

#include "linux_common.h"
#include "cluster.h"

#define TACAFS_MAGIC 0x20040617


/*
 * Funcions
 */
static struct super_block *tacafs_read_super (struct super_block *, void *, int);
static void tacafs_s_clear_inode (struct inode *);
static void tacafs_s_read_inode (struct inode *);
static int tacafs_s_statfs (struct super_block *, struct statfs *);
static struct dentry *tacafs_i_lookup (struct inode *, struct dentry *);
static int tacafs_f_readdir (struct file *, void *, filldir_t);

/*
 * Estructures
 */

/* declaracio del file_system_type */
DECLARE_FSTYPE (tacafs_type, "tacafs", tacafs_read_super, 0);
static struct inode *tacafs_root_inode;

#include "dummy.c"

static struct super_operations tacafs_sops = {
put_inode: dummy_put_inode,
delete_inode: dummy_delete_inode,
    clear_inode: tacafs_s_clear_inode,
    read_inode: tacafs_s_read_inode,
    statfs: tacafs_s_statfs
};

static struct inode_operations tacafs_root_iops = {
    lookup: tacafs_i_lookup
};

static struct file_operations tacafs_root_fops = {
    read   : generic_read_dir,
    readdir: tacafs_f_readdir
};

kmem_cache_t *tacafs_inode_cachep;


/*
 * Operacions del sistema de fitxers
 */

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

    tacafs_root_inode = tacafs_make_inode(sb, INODE_ROOT, MODE_DIR,
	    &tacafs_root_iops, &tacafs_root_fops);
	    //&simple_dir_inode_operations, &dcache_dir_ops);
    if (!tacafs_root_inode)
	return ERR_PTR(-ENOMEM);

    if(!(sb->s_root = d_alloc_root(tacafs_root_inode))) { // allocata un dentry
        iput(tacafs_root_inode);
	return ERR_PTR(-ENOMEM);
    }

    dprint2("s'ha retornat un super_block valid\n");
    return sb;
}


/*
 * Operacions del super_block
 */

/**
 * tacafs_s_clear_inode - Elimina les dades especifiques del SF del inode
 **/
static void tacafs_s_clear_inode (struct inode *inode)
{
    /* totes les estructures tenen el nom a la mateixa posicio */
    if (inode != tacafs_root_inode) {
	dprint("%s\n",((struct cluster_t*)inode->u.generic_ip)->nom);
	kmem_cache_free(tacafs_inode_cachep, inode);
    }
}


/**
 * tacafs_s_readinode - Cridat per llegir un inode del SF muntat
 **/
static void tacafs_s_read_inode (struct inode *inode)
{
    inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
    dprint2("<==>\n");
}


/**
 * tacafs_s_statfs - Cridat per obtenir informacio del SF com tamany, etc.
 **/
static int tacafs_s_statfs (struct super_block *sb, struct statfs *buf)
{
    buf->f_type = TACAFS_MAGIC;
    buf->f_bsize = PAGE_SIZE/sizeof(long);
    buf->f_bfree = 0;
    buf->f_bavail = 0;
    buf->f_ffree = 0;
    buf->f_namelen = NAME_MAX;
    //dprint2("<==>\n");
    return 0;
}


/*
 * Operacions sobre inodes
 */

/**
 * tacafs_i_lookup - Crea el inode del dentry qye es vol mirar
 **/
static struct dentry *tacafs_i_lookup (struct inode *parent_inode, struct dentry *dentry)
{
    struct dentry *res = ERR_PTR(-ENOENT);
    struct cluster_t cluster;

    dprint("dentry %s\n", dentry->d_name.name);

    if (!obtenir_cluster(dentry->d_name.name, &cluster))
	goto out;

    dprint2("match!\n");

    /*
     * Hem de generar el inode per mostrar un cluster (el dentry)
     * Per tant un directori
     */
    res = tacafs_fill_dentry(parent_inode->i_sb, dentry, INODE_CLUSTER,
	    MODE_DIR, &cluster, sizeof(cluster), &tacafs_cluster_iops,
	    &tacafs_cluster_dops, &tacafs_cluster_fops);

out:
    dprint("inode per cluster %s %s\n", dentry->d_name.name,
	    res == ERR_PTR(-ENOENT) ? "inexistent" :
	    (res == NULL ? "generat" : "no generat per error"));
    return res;
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
    struct cluster_t *llista;
    int num;

    dprint2("==>\n");

    if(file->f_pos > 0 )
	return 1;

    if(filldir(dirent, ".", 1, file->f_pos++, dentry->d_inode->i_ino, DT_DIR) ||
	    filldir(dirent, "..", 2, file->f_pos++, dentry->d_parent->d_inode->i_ino, DT_DIR))
	return 0;

    num = num_clusters();
    if (num > 0) {
	llista = (struct cluster_t *)kmalloc(sizeof(struct cluster_t)*num ,GFP_KERNEL);	
	if (llistar_clusters(llista, num) > 0) {
	    dprint2("Creant clusters\n");
	    num = 0;
	    while (llista != NULL) {
		if(filldir(dirent, llista->nom, strlen(llista->nom), file->f_pos++, INO(++num,INODE_CLUSTER), DT_DIR ))
		    return 0;
		llista = llista->next;
	    } 
	    dprint2("Directoris per clusters creats\n");
	}
	kfree(llista);
    }
    else
	dprint2("No hi ha clusters a mostrar\n");
    return 1;
}


/*
 * Informacio/codi especific dels moduls de linux
 */

#define CACHE_NAMELEN   20 
#include <asm/hardirq.h>
#define BYTES_PER_WORD          sizeof(void *)
#define MAX_OBJ_ORDER   5
static int __init tacafs_init(void)
{
    dprint2("Enregistrant sistema de fitxers\n");
    tacafs_inode_cachep = kmem_cache_create("tacafs_inode",
	    sizeof(struct tacafs_inode_info), 0, 0, NULL, NULL);
    if (!tacafs_inode_cachep)
	return -ENOMEM;
    return register_filesystem(&tacafs_type);
}

static void __exit tacafs_exit(void)
{
    dprint2("Desenregistrant sistema de fitxers\n");
    unregister_filesystem(&tacafs_type);
    kmem_cache_destroy(tacafs_inode_cachep);
}

module_init(tacafs_init);
module_exit(tacafs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("tacafs crew");
MODULE_DESCRIPTION("taca file system module");
//MODULE_INFO(tag, info);
//MODULE_PARM_DESC(_parm, desc);

