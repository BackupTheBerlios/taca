/*
 * linux_common.c
 *
 * Operacions comunes a tothom en el VFS de linux
 */

#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/dcache.h>


/**
 * tacafs_make_inode - Crea un inode
 *
 * Crea un inode o retorna NULL en cas de no poder.
 **/
static struct inode *tacafs_make_inode (struct super_block *sb, int mode,
	struct inode_operations *iops, struct file_operations *fops)
{
    struct inode *ret = new_inode(sb);

    if (ret) {
	ret->i_mode = mode;
	ret->i_uid = ret->i_gid = 0;
	ret->i_blksize = PAGE_CACHE_SIZE;
	ret->i_blocks = 0;
	ret->i_atime = ret->i_mtime = ret->i_ctime = CURRENT_TIME;
	ret->i_op = iops;
	ret->i_fop = fops;
    }
    return ret;
}


/**
 * tacafs_create_file - Crea un fitxer i el posa a la dentry cache
 *
 * Crea el dentry i inode corresponents a un fitxer en un directori i els posa
 * a la dentry cache per a un acces mes rapid.
 **/
static struct dentry *tacafs_create_file (struct super_block *sb, int mode,
	struct inode_operations *iops, struct file_operations *fops,
	struct dentry *dir, const char *name)
{
    struct dentry *dentry;
    struct inode *inode;
    struct qstr qname;

    /*
     * Versio amb hash del nom per anar a la dentry cache
     */
    qname.name = name;
    qname.len = strlen (name);
    qname.hash = full_name_hash(name, qname.len);
    /*
     * Creem el dentry i l'inode que hi associem
     */
    dentry = d_alloc(dir, &qname);
    if (!dentry)
	goto out;
    inode = tacafs_make_inode(sb, S_IFREG | mode, iops, fops);
    if (!inode)
	goto out_dput;
    //inode->u.generic_ip = dades;
    /*
     * Ho posem tot a la dentry cache
     */
    d_add(dentry, inode);
    return dentry;
    /*
     *  Alguna cosa no ha anat be
     */
out_dput:
    dput(dentry);
out:
    return 0;
}


/*
 * Buscar les dades. Cas trivial, si el dentry no existia ja a la dentry
 * cache, es un dentry negatiu.
 */
struct dentry *simple_lookup (struct inode *dir, struct dentry *dentry)
{
    if (dentry->d_name.len > NAME_MAX)
	return ERR_PTR(-ENAMETOOLONG);
    d_add(dentry, NULL);
    return NULL;
}

struct inode_operations simple_dir_inode_operations = {
    lookup: simple_lookup
};


/**
 * tacafs_create_dir - Crea un directori i el posa a la dentry cache
 * @iops: si es NULL, utilitza les funcions per defecte que miren a la dentry
 *        cache
 * @fops: si es NULL, utilitza les funcions per defecte que miren a la dentry
 *        cache
 * 
 * Igual que amb un fitxer, crea el dentry, inode i ho posa a la dentry cache.
 **/
static struct dentry *tacafs_create_dir (struct super_block *sb, int mode,
	struct inode_operations *iops, struct file_operations *fops,
	struct dentry *parent, const char *name)
{
    struct dentry *dentry;
    struct inode *inode;
    struct qstr qname;

    qname.name = name;
    qname.len = strlen (name);
    qname.hash = full_name_hash(name, qname.len);
    dentry = d_alloc(parent, &qname);
    if (! dentry)
	goto out;

    inode = tacafs_make_inode(sb, S_IFDIR | 0644,
	    iops == NULL ? &simple_dir_inode_operations : iops,
	    fops == NULL ? &dcache_dir_ops : fops);
    if (! inode)
	goto out_dput;
    d_add(dentry, inode);
    return dentry;

out_dput:
    dput(dentry);
out:
    return 0;
}

