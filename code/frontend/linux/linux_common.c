/*
 * linux_common.c
 *
 * Operacions comunes a tothom en el VFS de linux
 */

#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/dcache.h>
#include <linux/slab.h>

#include <linux_debug.h>

#include "linux_common.h"


/*
 * Operacions generiques
 */

/**
 * tacafs_make_inode - Crea un inode
 *
 * Crea un inode o retorna NULL en cas de no poder.
 **/
struct inode *tacafs_make_inode (struct super_block *sb, int ino, int mode,
	struct inode_operations *iops, struct file_operations *fops)
{
#if 0
    struct inode *ret = iget(sb, ino);
#else
    /* qualsevol numero d'inode va be */
    struct inode *ret = new_inode(sb);
#endif

    if (ret) {
	ret->i_mode = mode;
	ret->i_uid = ret->i_gid = 0;
	ret->i_blksize = PAGE_CACHE_SIZE;
	ret->i_blocks = 0;
	ret->i_atime = ret->i_mtime = ret->i_ctime = CURRENT_TIME;
	//if (iops)
	    ret->i_op = iops;
	//if (fops)
	    ret->i_fop = fops;
    }
    return ret;
}

/**
 * tacafs_write_data - Escriu les dades especifiques a un dentry
 *
 * Escriu les dades especifiques i li associa les operacions corresponents a
 * un dentry.
 * Retorna 0 si no hi ha prou memoria, 1 altrament.
 **/
static int tacafs_write_data (struct inode *inode, void *data, int size)
{
    void **idata = &(inode->u.generic_ip);

    *idata = kmem_cache_alloc(tacafs_inode_cachep, GFP_USER);
    if (!*idata)
	return 0;
    memcpy(*idata, data, size);
    return 1;
}

/**
 * atoi16 - Transforma un text a un numero de 16 bits
 **/
static int atoi16(const char *num, int len)
{
    int n = 0;
    char c;

    while(len-- > 0)
    {
	c = *num++;
	if(c < '0' || c > '9')
	    /* saltem les lletres */
	    continue;
	n = n * 10 + c - '0';
	if(n & 0xffff0000)
	    /* no mes de 16 bits */
	    return(0);
    }
    /* petit arranjament per evitar error en els que comencen per 0 */
    return(n+1);
}

/**
 * tacafs_fill_dentry - Inicialitza un dentry previament creat
 *
 * Inicialitza un dentry previament creat i li crea el inode associat.
 * Retorna NULL si tot va be o l'error corresponent.
 **/
struct dentry *tacafs_fill_dentry (struct super_block *sb,
	struct dentry *dentry, int inobase, int mode, void *data, int datasize,
	struct inode_operations *iops, struct dentry_operations *dops,
	struct file_operations *fops)
{
    struct inode *inode;
    int name2ino = atoi16(dentry->d_name.name, dentry->d_name.len);

    if (!name2ino) {
	dprint("fallada de atoi16 amb %s\n", dentry->d_name.name);
	return ERR_PTR(-ENOENT);
    }

    inode = tacafs_make_inode(sb, INO(name2ino,inobase), mode,
	    iops, fops);
    if (!inode)
	return ERR_PTR(-ENOMEM);

    dentry->d_op = dops;

    if (!tacafs_write_data(inode, data, datasize)) {
	iput(inode);
	return ERR_PTR(-ENOMEM);
    }

    d_add(dentry, inode);

    return NULL;
}

/**
 * tacafs_create_dentry - Crea una entrada al sistema de fitxers
 *
 * Crea el dentry i inode corresponents i els posa a la dentry cache
 * Retorna el dentry generat o l'error corresponent (IS_ERR, PTR_ERR).
 **/
static inline struct dentry *tacafs_create_dentry (struct super_block *sb,
	struct dentry *dir, int inobase, const char *name, int mode, void *data,
	int datasize, struct inode_operations *iops,
	struct dentry_operations *dops, struct file_operations *fops)
{
    struct dentry *dentry, *res;
    struct qstr qname;

    /* Versio amb hash del nom per anar a la dentry cache */
    qname.name = name;
    qname.len = strlen (name);
    qname.hash = full_name_hash(name, qname.len);

    /* Creem el dentry */
    dentry = d_alloc(dir, &qname);
    if (!dentry)
	return ERR_PTR(-ENOMEM);

    /* Creem i omplim l'inode que hi associem */
    res = tacafs_fill_dentry (sb, dentry, inobase, mode, data, datasize,
	    iops, dops, fops);
    if (!res)
	res = dentry;
    else
	dput(dentry);

    return res;
}


/**
 * tacafs_create_file - Crea un fitxer
 *
 * Crea el dentry i inode corresponents a un fitxer en un directori i els posa
 * a la dentry cache.
 * Retorna el dentry generat o l'error corresponent (IS_ERR, PTR_ERR).
 **/
struct dentry *tacafs_create_file (struct super_block *sb, struct dentry *dir,
	int inobase, const char *name, void *data, int datasize,
	struct inode_operations *iops, struct dentry_operations *dops,
	struct file_operations *fops)
{
    return tacafs_create_dentry(sb, dir, inobase, name, MODE_FILE, data, datasize,
	    iops, dops, fops);
}

/*
 * Buscar les dades. Cas trivial, si el dentry no existia ja a la dentry
 * cache, es un dentry negatiu.
 */
static struct dentry *simple_lookup (struct inode *dir, struct dentry *dentry)
{
    if (dentry->d_name.len > NAME_MAX)
	return ERR_PTR(-ENAMETOOLONG);
    d_add(dentry, NULL);
    return NULL;
}

static struct inode_operations simple_dir_inode_operations = {
lookup: simple_lookup
};

/**
 * tacafs_create_dir - Crea un directori
 * @iops: si es NULL, utilitza les funcions per defecte que miren a la dentry
 *        cache
 * @fops: si es NULL, utilitza les funcions per defecte que miren a la dentry
 *        cache
 * 
 * Igual que amb un fitxer, crea el dentry, inode i ho posa a la dentry cache.
 * Retorna el dentry generat o l'error corresponent (IS_ERR, PTR_ERR).
 **/
struct dentry *tacafs_create_dir (struct super_block *sb, struct dentry *dir,
	int inobase, const char *name, void *data, int datasize,
	struct inode_operations *iops, struct dentry_operations *dops,
	struct file_operations *fops)
{
    return tacafs_create_dentry(sb, dir, inobase, name, MODE_DIR, data, datasize,
	    iops == NULL ? &simple_dir_inode_operations : iops,
	    dops,
	    fops == NULL ? &dcache_dir_ops : fops);
}

