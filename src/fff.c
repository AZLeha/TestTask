#include "inc/fff.h"

#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t xMutexFATFS = NULL;




//в идеале проверять статусы мютекса, но лень


void ff_init(void)
{
	xMutexFATFS = xSemaphoreCreateMutex();
}


FRESULT ff_open (FIL* fp, const TCHAR* path, BYTE mode)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_open (fp, path, mode);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_close (FIL* fp)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_close (fp);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;

}
FRESULT ff_read (FIL* fp, void* buff, UINT btr, UINT* br)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_read (fp, buff, btr, br);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_write (FIL* fp, const void* buff, UINT btw, UINT* bw)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_write (fp, buff, btw, bw);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_lseek (FIL* fp, FSIZE_t ofs)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_lseek (fp, ofs);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_truncate (FIL* fp)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_truncate (fp);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_sync (FIL* fp)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_sync (fp);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;

}


FRESULT ff_opendir (DIR* dp, const TCHAR* path)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_opendir (dp, path);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_closedir (DIR* dp)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_closedir (dp);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_readdir (DIR* dp, FILINFO* fno)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_readdir (dp,fno);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_findfirst (DIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_findfirst (dp, fno, path, pattern);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_findnext (DIR* dp, FILINFO* fno)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_findnext (dp, fno);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_mkdir (const TCHAR* path)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_mkdir (path);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_unlink (const TCHAR* path)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_unlink (path);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_rename (const TCHAR* path_old, const TCHAR* path_new)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_rename (path_old, path_new);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_stat (const TCHAR* path, FILINFO* fno)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_stat (path, fno);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}
FRESULT ff_chmod (const TCHAR* path, BYTE attr, BYTE mask)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_chmod (path, attr, mask);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_utime (const TCHAR* path, const FILINFO* fno)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_utime (path, fno);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_chdir (const TCHAR* path)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_chdir (path);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_chdrive (const TCHAR* path)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_chdrive (path);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_getcwd (TCHAR* buff, UINT len)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_getcwd (buff, len);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_getfree (path, nclst, fatfs);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_getlabel (const TCHAR* path, TCHAR* label, DWORD* vsn)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_getlabel (path, label, vsn);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_setlabel (const TCHAR* label)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_setlabel (label);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_forward (FIL* fp, UINT(*func)(const BYTE*,UINT), UINT btf, UINT* bf)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_forward (fp, func, btf, bf);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_expand (FIL* fp, FSIZE_t fsz, BYTE opt)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_expand (fp, fsz, opt);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_mount (FATFS* fs, const TCHAR* path, BYTE opt)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_mount (fs, path, opt);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_mkfs (const TCHAR* path, const MKFS_PARM* opt, void* work, UINT len)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_mkfs (path, opt, work, len);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_fdisk (BYTE pdrv, const LBA_t ptbl[], void* work)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_fdisk (pdrv, ptbl, work);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


FRESULT ff_setcp (WORD cp)
{
	FRESULT lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus =f_setcp (cp);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


int ff_putc (TCHAR c, FIL* fp)
{
	int lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_putc (c, fp);
	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


int ff_puts (const TCHAR* str, FIL* cp)
{
	int lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_puts (str, cp);
	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}


TCHAR* ff_gets (TCHAR* buff, int len, FIL* fp)
{
	TCHAR *lcalStatus = FR_OK;

	xSemaphoreTake(xMutexFATFS,1000);

	lcalStatus = f_gets (buff, len, fp);

	xSemaphoreGive(xMutexFATFS);
	return lcalStatus;
}

