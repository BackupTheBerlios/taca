/*
 * linux_debug.h
 *
 * Funcions de debug per a Linux
 */

#ifndef _linux_debug_h_
#define _linux_debug_h_

#define DEBUG_FLAG KERN_DEBUG

#ifdef DEBUG
#define dprint(msg, args...)			\
	do { printk(DEBUG_FLAG "%s.%d:%s: \n\t"msg,__FILE_,__LINE__,__FUNCTION__,##args); } while (0)
#else
#define dprint(msg, args...)			\
	do { ; } while (0)
#endif

#endif

