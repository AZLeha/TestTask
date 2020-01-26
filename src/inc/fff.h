#ifndef INC_FFff_H_
#define INC_FFff_H_
#include "ff.h"


void ff_init(void);


FRESULT ff_open (FIL* fp, const TCHAR* path, BYTE mode);				/* Open or create a file */
FRESULT ff_close (FIL* fp);											/* Close an open file object */
FRESULT ff_read (FIL* fp, void* buff, UINT btr, UINT* br);			/* Read data from the file */
FRESULT ff_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	/* Write data to the file */
FRESULT ff_lseek (FIL* fp, FSIZE_t ofs);								/* Move file pointer of the file object */
FRESULT ff_truncate (FIL* fp);										/* Truncate the file */
FRESULT ff_sync (FIL* fp);											/* Flush cached data of the writing file */
FRESULT ff_opendir (DIR* dp, const TCHAR* path);						/* Open a directory */
FRESULT ff_closedir (DIR* dp);										/* Close an open directory */
FRESULT ff_readdir (DIR* dp, FILINFO* fno);							/* Read a directory item */
FRESULT ff_findfirst (DIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern);	/* Find first file */
FRESULT ff_findnext (DIR* dp, FILINFO* fno);							/* Find next file */
FRESULT ff_mkdir (const TCHAR* path);								/* Create a sub directory */
FRESULT ff_unlink (const TCHAR* path);								/* Delete an existing file or directory */
FRESULT ff_rename (const TCHAR* path_old, const TCHAR* path_new);	/* Rename/Move a file or directory */
FRESULT ff_stat (const TCHAR* path, FILINFO* fno);					/* Get file status */
FRESULT ff_chmod (const TCHAR* path, BYTE attr, BYTE mask);			/* Change attribute of a file/dir */
FRESULT ff_utime (const TCHAR* path, const FILINFO* fno);			/* Change timestamp of a file/dir */
FRESULT ff_chdir (const TCHAR* path);								/* Change current directory */
FRESULT ff_chdrive (const TCHAR* path);								/* Change current drive */
FRESULT ff_getcwd (TCHAR* buff, UINT len);							/* Get current directory */
FRESULT ff_getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs);	/* Get number of free clusters on the drive */
FRESULT ff_getlabel (const TCHAR* path, TCHAR* label, DWORD* vsn);	/* Get volume label */
FRESULT ff_setlabel (const TCHAR* label);							/* Set volume label */
FRESULT ff_forward (FIL* fp, UINT(*func)(const BYTE*,UINT), UINT btf, UINT* bf);	/* Forward data to the stream */
FRESULT ff_expand (FIL* fp, FSIZE_t fsz, BYTE opt);					/* Allocate a contiguous block to the file */
FRESULT ff_mount (FATFS* fs, const TCHAR* path, BYTE opt);			/* Mount/Unmount a logical drive */
FRESULT ff_mkfs (const TCHAR* path, const MKFS_PARM* opt, void* work, UINT len);	/* Create a FAT volume */
FRESULT ff_fdisk (BYTE pdrv, const LBA_t ptbl[], void* work);		/* Divide a physical drive into some partitions */
FRESULT ff_setcp (WORD cp);											/* Set current code page */
int ff_putc (TCHAR c, FIL* fp);										/* Put a character to the file */
int ff_puts (const TCHAR* str, FIL* cp);								/* Put a string to the file */
int ff_printf (FIL* fp, const TCHAR* str, ...);						/* Put a formatted string to the file */
TCHAR* ff_gets (TCHAR* buff, int len, FIL* fp);						/* Get a string from the file */

#define ff_eof(fp) ((int)((fp)->fptr == (fp)->obj.objsize))
#define ff_error(fp) ((fp)->err)
#define ff_tell(fp) ((fp)->fptr)
#define ff_size(fp) ((fp)->obj.objsize)
#define ff_rewind(fp) ff_lseek((fp), 0)
#define ff_rewinddir(dp) ff_readdir((dp), 0)
#define ff_rmdir(path) ff_unlink(path)
#define ff_unmount(path) ff_mount(0, path, 0)

#endif /* INC_FFff_H_ */
