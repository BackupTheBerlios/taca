/*
 * Dummy FS operations for tracing purposes
 */


/* super_operations */
struct inode *dummy_alloc_inode (struct super_block * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
    return 0;
}

void dummy_destroy_inode (struct inode * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_read_inode (struct inode * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_read_inode2 (struct inode * x1, void * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_dirty_inode (struct inode * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_write_inode (struct inode * x1, int x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_put_inode (struct inode * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_drop_inode (struct inode * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_delete_inode (struct inode * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_put_super (struct super_block * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_write_super (struct super_block * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

int dummy_sync_fs (struct super_block * x1) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
    return 0;
}

void dummy_write_super_lockfs (struct super_block * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_unlockfs (struct super_block * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

int dummy_statfs (struct super_block * x1, struct statfs * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_remount_fs (struct super_block * x1, int * x2, char * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
    return 0;
}

void dummy_clear_inode (struct inode * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

void dummy_umount_begin (struct super_block * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
}

struct dentry * dummy_fh_to_dentry (struct super_block *sb, __u32 *fh, int len, int fhtype, int parent) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_dentry_to_fh (struct dentry * x1, __u32 * x2, int * x3, int x4) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_show_options (struct seq_file * x1, struct vfsmount * x0) {
    printk(KERN_INFO "!!!! super_operations.%s\n",__FUNCTION__);
    return 0;
}


/* file_operations */
loff_t dummy_llseek (struct file * x1, loff_t x2, int x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

ssize_t dummy_read (struct file * x1, char * x2, size_t x3, loff_t * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

ssize_t dummy_write (struct file * x1, const char * x2, size_t x3, loff_t * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_readdir (struct file * x1, void * x2, filldir_t x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

unsigned int dummy_poll (struct file * x1, struct poll_table_struct * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_ioctl (struct inode * x1, struct file * x2, unsigned int x3, unsigned long x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_mmap (struct file * x1, struct vm_area_struct * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_open (struct inode * x1, struct file * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_flush (struct file * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_f_release (struct inode * x1, struct file * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_fsync (struct file * x1, struct dentry * x2, int x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_fasync (int x1, struct file * x2, int x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_lock (struct file * x1, int x2, struct file_lock * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

ssize_t dummy_readv (struct file * x1, const struct iovec * x2, unsigned long x3, loff_t * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

ssize_t dummy_writev (struct file * x1, const struct iovec * x2, unsigned long x3, loff_t * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

ssize_t dummy_sendfile (struct file * x1, loff_t * x2, size_t x3, read_actor_t x4, void * x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

ssize_t dummy_sendpage (struct file * x1, struct page * x2, int x3, size_t x4, loff_t * x5, int x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}

unsigned long dummy_get_unmapped_area (struct file * x1, unsigned long x2, unsigned long x3, unsigned long x4, unsigned long x0) {
    printk(KERN_INFO "!!!! file_operations.%s\n",__FUNCTION__);
    return 0;
}


/* dentry_operations */
int dummy_d_revalidate (struct dentry * x1, struct nameidata * x0) {
    printk(KERN_INFO "!!!! dentry_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_hash (struct dentry * x1, struct qstr * x0) {
    printk(KERN_INFO "!!!! dentry_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_compare (struct dentry * x1, struct qstr * x2, struct qstr * x0) {
    printk(KERN_INFO "!!!! dentry_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_delete (struct dentry * x0) {
    printk(KERN_INFO "!!!! dentry_operations.%s\n",__FUNCTION__);
    return 0;
}

void dummy_d_release (struct dentry * x0) {
    printk(KERN_INFO "!!!! dentry_operations.%s\n",__FUNCTION__);
}

void dummy_iput (struct dentry * x1, struct inode * x0) {
    printk(KERN_INFO "!!!! dentry_operations.%s\n",__FUNCTION__);
}


/* inode_operations */
int dummy_create (struct inode * x1,struct dentry * x2, int x3) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

struct dentry * dummy_lookup (struct inode * x1,struct dentry * x2, struct nameidata * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_link (struct dentry * x1,struct inode * x2,struct dentry * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_unlink (struct inode * x1,struct dentry * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_symlink (struct inode * x1,struct dentry * x2,const char * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_mkdir (struct inode * x1,struct dentry * x2,int x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_rmdir (struct inode * x1,struct dentry * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_mknod (struct inode * x1,struct dentry * x2,int x3,int x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_rename (struct inode * x1, struct dentry * x2, struct inode * x3, struct dentry * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_readlink (struct dentry * x1, char * x2,int x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_follow_link (struct dentry * x1, struct nameidata * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

void dummy_truncate (struct inode * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
}

int dummy_permission (struct inode * x1, int x2) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_i_revalidate (struct dentry * x1) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_setattr (struct dentry * x1, struct iattr * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_getattr (struct dentry * x1, struct iattr * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_setxattr (struct dentry * x1, const char * x2, void * x3,size_t x4,int x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

ssize_t dummy_getxattr (struct dentry * x1, const char * x2, void * x3, size_t x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

ssize_t dummy_listxattr (struct dentry * x1, char * x2, size_t x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

int dummy_removexattr (struct dentry * x1, const char * x0) {
    printk(KERN_INFO "!!!! inode_operations.%s\n",__FUNCTION__);
    return 0;
}

