/**
 * Notes:
 * Implementing a small filesystem having one file
 *
 * -> What happens when we mount a file system?
 * -> What do we need to provide to the kernel so that we are mountable?
 * -> What inode, dentry and file operations do we have to support?
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
/*
  #if CONFIG_MODVERSIONS==1
  #define MODVERSIONS
  #include <linux/modversions.h>
  #endif
*/
#include <linux/fs.h>
#include <linux/sched.h>

#include <linux/mm.h>

#include "generic.h"

#define RKFS_MAGIC 0xabcd
#define FILE_INODE_NUMBER 2
#define DIR_INODE_NUMBER 3

static struct super_block *rkfs_read_super(struct super_block *, void *, int);
void rkfs_s_readinode( struct inode *inode );
int rkfs_s_statfs( struct super_block *sb, struct statfs *buf );
struct dentry *rkfs_i_lookup( struct inode *parent_inode, struct dentry *dentry );
ssize_t rkfs_f_read( struct file *file, char *buf, size_t max, loff_t *len );
int rkfs_f_readdir( struct file *file, void *dirent, filldir_t filldir );
ssize_t rkfs_f_write (struct file *, const char *, size_t, loff_t *);
int rkfs_f_release (struct inode *, struct file *);

/*
 * Data declarations
 */

static struct super_operations rkfs_sops = {
    read_inode: rkfs_s_readinode,
    statfs: rkfs_s_statfs
    //  put_inode: rkfs_s_putinode,
    //  delete_inode:    rkfs_s_deleteinode,
    //  statfs: rkfs_s_statfs
};

struct inode_operations rkfs_iops = {
    lookup: rkfs_i_lookup
};

struct file_operations rkfs_dir_fops = {
    read   : generic_read_dir,
    readdir: &rkfs_f_readdir
};

struct file_operations rkfs_file_fops = {
    read : &rkfs_f_read,
    write: &rkfs_f_write
    //    release: &rkfs_f_release
};

// use this macro to declare the filesystem structure
DECLARE_FSTYPE( rkfs, "rkfs", rkfs_read_super, 0 );
struct inode *rkfs_root_inode;

char file_buf[1024] = "Hello World\n";
int file_size = 12;

/*
 * File-System Operations
 */
/*
 * This will be called when the kernel is attempting to mount something. It creates a
 * super_block structure and calls our callback/function to fill it.
 */
static struct super_block *rkfs_read_super( struct super_block *sb, void *buf, int size ) {
    sb->s_blocksize = 1024;
    sb->s_blocksize_bits = 10;
    sb->s_magic = RKFS_MAGIC;
    sb->s_op = &rkfs_sops; // super block operations
    sb->s_type = &rkfs; // file_system_type

    rkfs_root_inode = iget( sb, 1 ); // allocate an inode
    rkfs_root_inode->i_op = &rkfs_iops; // set the inode ops
    rkfs_root_inode->i_mode = S_IFDIR|S_IRWXU;
    rkfs_root_inode->i_fop = &rkfs_dir_fops;

    if(!(sb->s_root = d_alloc_root(rkfs_root_inode))) {
        iput(rkfs_root_inode);
        return NULL;
    }

    printk( "rkfs: read_super returning a valid super_block\n" );
    return sb;
}

/*
 * Super-Block Operations
 */

void rkfs_s_readinode( struct inode *inode ) {
    inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
    printk( "rkfs: super_operations.readinode called\n" );
}

/*
 * This will be called to get the filesystem information like size etc.
 */
int rkfs_s_statfs( struct super_block *sb, struct statfs *buf ) {
    buf->f_type = RKFS_MAGIC;
    buf->f_bsize = PAGE_SIZE/sizeof(long);
    buf->f_bfree = 0;
    buf->f_bavail = 0;
    buf->f_ffree = 0;
    buf->f_namelen = NAME_MAX;
    printk( "rkfs: super_operations.statfs called\n" );
    return 0;
}

/*
 * Inode Operations
 */

/* Aquesta funcio genera el inode pel dentry que es vol mirar 
 * El inode que es vol generar és "fill" del parent_inode
 *
 *
 * TODO: comprovar que el directori on es vol entrar és correcte
 */

struct dentry *rkfs_i_lookup( struct inode *parent_inode, struct dentry *dentry ) {
  struct inode *file_inode;
  
  printk( "rkfs: inode_operations.lookup called with dentry %s. size = %d\n", dentry->d_name.name, file_size );
  
  /* Hem de generar el inode per mostrar un clusters (el dentry) 
   * Per tant un directori
   * */
  if( parent_inode->i_ino == rkfs_root_inode->i_ino) {
	  
	  // allocate an inode object
	  if(!(file_inode = iget( parent_inode->i_sb, DIR_INODE_NUMBER)))
		  return ERR_PTR(-ENOMEM);
	  //file_inode->i_size = file_size;
	  file_inode->i_mode = S_IFDIR|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
	  file_inode->i_fop = &rkfs_dir_fops;
	  file_inode->i_op = &rkfs_iops;
	  //  add the inode to the dentry object
	  d_add(dentry, file_inode);
	  
	  printk(" lookup: inode per cluster %s generat\n", dentry->d_name.name);
  }

  /* hem de generar el inode per mostrar el node */	
  else if( dentry->d_parent->d_parent->d_inode->i_ino == rkfs_root_inode->i_ino) {


	  //allocate an inode object
	  if(!(file_inode = iget( parent_inode->i_sb, DIR_INODE_NUMBER)))
		  return ERR_PTR(-ENOMEM);
	  file_inode->i_mode = S_IFDIR|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
	  file_inode->i_fop = &rkfs_dir_fops;
	  file_inode->i_op = &rkfs_iops;
	  //add the inode to the dentry object
	  d_add(dentry, file_inode);

	  printk(" lookup: inode per node %s generat\n", dentry->d_name.name);


  }
	  
  // Hem de generar l'inode per mostrar el proces
  else if( dentry->d_parent->d_parent->d_parent->d_inode->i_ino == rkfs_root_inode->i_ino) {

	  //allocate an inode object
	  if(!(file_inode = iget( parent_inode->i_sb, FILE_INODE_NUMBER)))
			  return ERR_PTR(-ENOMEM);
	  file_inode->i_size = file_size;
	  file_inode->i_mode = S_IFREG|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
	  file_inode->i_fop = &rkfs_file_fops;
	  //add the inode to the dentry object
	  d_add(dentry, file_inode);

	  printk(" lookup: inode per process %s generat\n", dentry->d_name.name);


  }
  
  else {
      return ERR_PTR(-ENOENT);
  }
  
  return NULL;
}

/*
 * File Operations
 */

ssize_t rkfs_f_read( struct file *file, char *buf, size_t max, loff_t *offset ) {
    int i;
    int buflen;
    if(*offset > 0)
        return 0;
    printk( "rkfs: file_operations.read called %d %d\n", max, *offset );
    buflen = file_size > max ? max : file_size;
    __generic_copy_to_user(buf, file_buf, buflen);
    //           copy_to_user(buf, file_buf, buflen);
    *offset += buflen; // advance the offset
    return buflen;
}

ssize_t rkfs_f_write (struct file *file, const char *buf, size_t maxlen, loff_t *offset) {
    int count;
    if(*offset > 0) {
        printk("Positive offset %d\n", *offset);
        return 0;
    }
    count = maxlen > sizeof(file_buf) ? sizeof(file_buf) : maxlen;
    __generic_copy_from_user(file_buf, buf, count);
        //    copy_from_user(file_buf, buf, maxlen);
    printk( "file_operations.write called with maxlen=%d, off=%d\n", maxlen, *offset );
    *offset += count;
    if(*offset > file_size)
        file_size = *offset;
    return count;
}

/*
int rkfs_f_release (struct inode *ino, struct file *file) {
    printk( "rkfs: file_operations.release called\n" );
    return 0;
}
*/

int rkfs_f_readdir( struct file *file, void *dirent, filldir_t filldir ) {
    int err;
    struct dentry *de = file->f_dentry;

    printk( "rkfs: file_operations.readdir called\n" );
    if(file->f_pos > 0 )
        return 1;
    
    if(filldir(dirent, ".", 1, file->f_pos++, de->d_inode->i_ino, DT_DIR)||
       (filldir(dirent, "..", 2, file->f_pos++, de->d_parent->d_inode->i_ino, DT_DIR)))
        return 0;
    
    if(de->d_inode->i_ino == rkfs_root_inode->i_ino) // Estem a l'arrel 
    {
	struct cluster_t *llista;
	int num;
	
	num = num_clusters();
	if (num > 0) {
		llista = (struct cluster_t *)kmalloc(sizeof(struct cluster_t)*num ,GFP_KERNEL);	
		llistar_clusters(llista);

    		printk( " readdir: Creant clusters\n" );
		while (llista != NULL) {	
			if(filldir(dirent, llista->nom, strlen(llista->nom), file->f_pos++, DIR_INODE_NUMBER, DT_DIR ))
	        	 	return 0;
			llista = llista->next;
		} 
		printk(" readdir: Directoris per clusters creats\n");
		kfree(llista);
	}
	else printk(" readdir: No hi ha clusters a mostrar\n");
    
		
    }
    
    
    else if(de->d_parent->d_inode->i_ino == rkfs_root_inode->i_ino) // Estem a un cluster 
    {
	struct cluster_t *llista;
	struct node_t *llista1;
	int num;
	int i=0;
	
	printk(" readdir: Llistant nodes del cluster %s\n", de->d_name.name);

	
	// Obtenim tots els clusters i busquem en quin estem
	num = num_clusters();
	if (num>0) {
		llista = (struct cluster_t *)kmalloc(sizeof(struct cluster_t)*num ,GFP_KERNEL);	
		llistar_clusters(llista);
	
		while (llista != NULL && i==0) {	
			
			printk(" readdir: Busquem el cluster\n");
		
			if (!strcmp(de->d_name.name, llista->nom)) i=1;
			else llista = llista->next;
		} 

	
		// Llistem tots els nodes del cluster
		num = num_nodes(llista);	
		if (num>0) {
			llista1 = (struct node_t *)kmalloc(sizeof(struct node_t)*num ,GFP_KERNEL);
			llistar_nodes(llista,llista1);

			while (llista1 != NULL) {
		
				printk(" readdir: Creem els directoris pels nodes\n");
		
				if(filldir(dirent, llista1->nom, strlen(llista1->nom), file->f_pos++, DIR_INODE_NUMBER, DT_DIR ))
					return 0;
				llista1 = llista1->next;
			}
    	
			printk(" readdir: Directoris per nodes creats\n");
    			kfree(llista);
			kfree(llista1);
		}
		else {
			printk (" readdir: No hi ha nodes a mostrar\n");
			kfree(llista);
		}
	}
	else printk (" readdir: No hi ha clusters amb nodes a mostrar\n");
		
    }
    
    else if(de->d_parent->d_parent->d_inode->i_ino == rkfs_root_inode->i_ino) // Estem a un node 
    {
	struct cluster_t *llista;
	struct node_t *llista1;
	struct proces_t *llista2;
	int num;
	int i=0;
	
	printk(" readdir: Llistant procs del node %s\n", de->d_name.name);

	
	// Obtenim tots els clusters i busquem en quin estem
	num = num_clusters();
	if (num>0) {
		llista = (struct cluster_t *)kmalloc(sizeof(struct cluster_t)*num ,GFP_KERNEL);	
		llistar_clusters(llista);

		while (llista != NULL && i==0) {	

			printk(" readdir: Busquem el cluster\n");

			if (!strcmp(de->d_parent->d_name.name, llista->nom)) i=1;
			else llista = llista->next;
		} 

		// Obtenim  tots els nodes del cluster i busquem en quin estem
		i = 0; 
		num = num_nodes(llista);	
		if (num>0) {
			llista1 = (struct node_t *)kmalloc(sizeof(struct node_t)*num ,GFP_KERNEL);
			llistar_nodes(llista,llista1);

			while (llista1 != NULL && i==0) {

				printk(" readdir: Busquem el node\n");

				if (!strcmp(de->d_name.name, llista1->nom)) i=1;
				else llista1 = llista1->next;
			}


			// LListem els processos del cluster
			num = num_procs(llista,llista1);	
			if (num>0) {
				llista2 = (struct proces_t *)kmalloc(sizeof(struct proces_t)*num ,GFP_KERNEL);
				llistar_procs(llista,llista1,llista2);

				while (llista2 != NULL) {
					if(filldir(dirent, llista2->nom, strlen(llista2->nom), file->f_pos++, FILE_INODE_NUMBER, DT_REG ))
					return 0;
					llista2 = llista2->next;
				}
				kfree (llista);
				kfree (llista1);
				kfree (llista2);
				printk(" readdir: Fitxers per processos creats\n");
			}
			else {
				printk(" readdir: No hi ha processos a mostrar\n");
				kfree(llista);
				kfree(llista1);
			}
		}
		else {
			printk(" readdir: No hi ha nodes amb processos a mostrar\n");
			kfree(llista);
		}
    	}
	else {
		printk(" readdir: No hi ha clusters amb nodes amb processos a mostrar\n");
	}
    }
    
    return 1;
}

int init_module(void) {
    int err;
    err = register_filesystem( &rkfs );
    return err;
}

void cleanup_module(void) {
    unregister_filesystem( &rkfs );
}

MODULE_LICENSE("GPL");
