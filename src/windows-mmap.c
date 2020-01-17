/* 
 *  IMS Open Corpus Workbench (CWB)
 *  Copyright (C) 1993-2006 by IMS, University of Stuttgart
 *  Copyright (C) 2007-     by the respective contributers (see file AUTHORS)
 * 
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2, or (at your option) any later
 *  version.
 * 
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *  Public License for more details (in the file "COPYING", or available via
 *  WWW at http://www.gnu.org/copyleft/gpl.html).
 */
/*
 * Windows/Unicode-compatibility extensions to CWB in this file
 *  Copyright (C) 2010      by ANR Textométrie, ENS de Lyon
 */

/**
 * @file
 *
 * This file provides an equivalent of the POSIX mmap() function for use in
 * Windows; the Windows API lacks this function.
 *
 * The code was adapted from the Git project (note that Git is GPL-licensed).
 *
 * Very important note: this file should only be compiled and linked when
 * targeting a windows environment.
 *
 */


#include "windows-mmap.h"

/**
 * Substitute POSIX mmap() for use in Windows.
 *
 * It wraps the Windows API functions CreateFileMapping() and
 * MapViewOfFileEx().
 *
 * The arguments, return, etc. are the same as POSIX mmap(), which is
 * documented much more thoroughly than we could hope to do here.
 */
void *
mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset)
{
  HANDLE hmap;
  void *temp;
  size_t len;
  struct stat st;
  uint64_t o = offset;
  uint32_t l = o & 0xFFFFFFFF;
  uint32_t h = (o >> 32) & 0xFFFFFFFF;
  unsigned long granularity;
  DWORD d;
  SYSTEM_INFO sysinfo;

  memset(&sysinfo,0,sizeof(SYSTEM_INFO));
  GetSystemInfo(&sysinfo);
  granularity=sysinfo.dwAllocationGranularity;

  if (!fstat(fd, &st))
    len = (size_t) st.st_size;
  else {
    fprintf(stderr,"mmap: could not determine filesize");
    exit(1);
  }

  if ((length + offset) > len)
    length = len - offset;

  /*
  if (!(flags & MAP_PRIVATE)) {
    fprintf(stderr,"Invalid usage of mmap when built with USE_WIN32_MMAP");
    exit(1);
  }
  */

  if(flags & MAP_PRIVATE)
  {
	  hmap = CreateFileMapping((HANDLE)_get_osfhandle(fd), 0, PAGE_WRITECOPY, 0, 0, 0);

	  if (!hmap)
		return MAP_FAILED;

	  if((start==0 || (granularity % (unsigned long)start) == 0) && (granularity % length) ==0)
	  {
		  temp = MapViewOfFileEx(hmap, FILE_MAP_COPY, h, l, length, start);
	  }
	  else
	  {
		  temp = MapViewOfFileEx(hmap, FILE_MAP_COPY, h, l, length, start);
	  }
  }
  else if(flags & MAP_SHARED)
  {
	  hmap = CreateFileMapping((HANDLE)_get_osfhandle(fd), 0, PAGE_READWRITE, 0, 0, 0);

	  if(!hmap)
		  return MAP_FAILED;

	  if((start==0 || (granularity % (unsigned long)start) == 0) && (granularity % length) == 0)
	  {
		  temp = MapViewOfFileEx(hmap, FILE_MAP_ALL_ACCESS, h, l, length, start);
	  }
	  else
	  {
		  temp = MapViewOfFileEx(hmap, FILE_MAP_ALL_ACCESS, h, l, length, start);
	  }
	  d=GetLastError();
  }

  if (!CloseHandle(hmap))
    fprintf(stderr,"unable to close file mapping handle\n");
  return temp ? temp : MAP_FAILED;
}



/**
 * Substitute POSIX munmap() for environments that lack it.
 *
 * It wraps the Windows API function UnmapViewOfFile().
 *
 * @see mmap
 */
int
munmap(void *start, size_t length)
{
  return !UnmapViewOfFile(start);
}

int
msync(void *addr, size_t length, int flags)
{
	if(!FlushViewOfFile(addr,length))
	{
		errno=GetLastError();
		return -1;
	}
	if((flags & MS_SYNC) == MS_SYNC)
	{
		//FlushFileBuffers(); -> FlushFileBuffers needs handle to file, not memory
	}
	return 0;
}
