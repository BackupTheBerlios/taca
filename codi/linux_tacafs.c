#define MODULE
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static struct super_block *tacafs_read_super(struct super_block *, void *, int);
static int tacafs_fill_super (struct super_block *, void *, int);


static DECLARE_FSTYPE(tacafs_fs_type, "tacafs", tacafs_read_super);




struct file_system_type tacafs_fs_type = {
	.owner = THIS_MODULE,
	.name = "tacafs",
	.read_super = tacafs_read_super,
};

static struct super_operations tacafs_s_ops = {
	.statfs         = simple_statfs,	//depen de libfs
	.drop_inode     = generic_delete_inode, //depen de libfs
};





struct inode *tacafs_get_inode(struct super_block *sb, int mode, int dev)
{
	struct inode * inode = new_inode(sb);

	if (inode) {
		inode->i_mode = mode;
		inode->i_uid = current->fsuid;
		inode->i_gid = current->fsgid;
		inode->i_blksize = PAGE_CACHE_SIZE;
		inode->i_blocks = 0;
		inode->i_rdev = NODEV;
		inode->i_mapping->a_ops = &tacafs_s_aops;
		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
		switch (mode & S_IFMT) {
			default:
				init_special_inode(inode, mode, dev);
				break;
			case S_IFREG:
				inode->i_fop = &tacafs_file_operations;
				break;
			case S_IFDIR:
				inode->i_op = &tacafs_dir_inode_operations;
				inode->i_fop = &dcache_dir_ops;
				break;
			case S_IFLNK:
				inode->i_op = &page_symlink_inode_operations;
				break;
		}
	}
	return inode;
}













/*
 * Operacio per llegir el superblock
 */
static struct super_block *tacafs_get_super(struct file_system_type *fst,
				int flags, const char *devname, void *data)
{
		return get_sb_single(fst, flags, data, tacafs_fill_super);
}

/* 
 * Operacio per omplir camps del superblock
 */
static int tacafs_fill_super (struct super_block *sb, void *data, int silent) {
	
tatic struct super_block *tacafs_read_super(struct super_block * sb, void * data, int silent)
{
	struct inode * inode;
	struct dentry * root;

	sb->s_blocksize = PAGE_CACHE_SIZE;
	sb->s_blocksize_bits = PAGE_CACHE_SHIFT;
	sb->s_magic = TACAFS_MAGIC;
	sb->s_op = &tacafs_s_ops;
	inode = tacafs_get_inode(sb, S_IFDIR | 0755, 0);
	if (!inode)
		return NULL;

	root = d_alloc_root(inode);
	if (!root) {
		iput(inode);
		return NULL;
	}
	sb->s_root = root;
	return sb;
}


}

static int __init init_tacafs_fs(void)
{
	        return register_filesystem(&tacafs_fs_type);
}

static void __exit exit_tacafs_fs(void)
{
	        unregister_filesystem(&tacafs_fs_type);
}

module_init(init_tacafs_fs);
module_exit(exit_tacafs_fs);

	









int init_module(void)      
{ 
	//printk(KERN_INFO "Hello, world\n"); 
	return register_filesystem(&tacafs_type);
}


struct file_system_type lfs_type = {
	.owner 	= THIS_MODULE,
	.name	= "lwnfs",
	.get_sb	= lfs_get_super,
	.kill_sb	= kill_litter_super,
};


