/*************************************************************************

This file is part of 'Mercenaries of Astonia v2'
Copyright (c) 1997-2001 Daniel Brockhaus (joker@astonia.com)
All rights reserved.

**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <direct.h>
#include <io.h>
#endif
#include "cgilib/cgi-lib.h" /* include the cgi-lib.h header file */
#include "cgilib/html-lib.h" /* include the html-lib.h header file */

int main(int argc, char *args[])
{
        int spr=0;
        char *tmp;
		char buf[256];
		char fname[256];
		char ch;
		//unsigned char uch;
		unsigned char r,g,b;
		int offset=0;
		int nextoffset=0;
		int temp=0;
		int w=0;
		int h=0;
		int xx,yy;
		int linelength=0;
		short tempshort=0;
		unsigned short tempushort;
		FILE *fd;
        LIST *head;
        head = is_form_empty() ? NULL : cgi_input_parse();

		// set stdout to binary mode
#ifndef _WIN32
		setmode(fileno(stdout), O_BINARY);
#else
		_setmode(_fileno(stdout), _O_BINARY);
#endif

       	if (head) {
                tmp=find_val(head,"spr");
                if (tmp) spr=atoi(tmp);
        }
		
		memset(buf,0,256);
		fd=fopen("moav2datadir.txt","r");
		if(fd) {
			memset(buf,0,256);
			if(fgets(buf,255,fd)) {
				chdir(buf);
			}
			fclose(fd);
		}


		memset(fname,0,256);
		sprintf(fname,"%s\\GFX\\%05d.png",buf,spr);
		if(fd=fopen(fname,"rb")) {
			printf("Content-Type: image/png\n\n");
			fflush(stdout);
			while(!feof(fd)) {
				//ch=fgetc(fd);
				fread(&ch,1,1,fd);
				if(!feof(fd)) {
					//fputc(ch,stdout);
					fwrite(&ch,1,1,stdout);
				}
			}
			fclose(fd);
			return 0;
		}

		memset(fname,0,256);
		sprintf(fname,"%s\\pnglib.idx",buf);
		if(fd=fopen(fname,"rb")) {
			
			if(fseek(fd,spr*4,SEEK_SET)==0) {
				//printf("Seeked to %d<br />\n",spr*4);
				fread(&offset,4,1,fd);
				nextoffset=INT_MAX;

				if(offset>0) {
					// offsets aren't always in order - have to read entire file to find next larger offset
					fseek(fd,0,SEEK_SET);
					while(!feof(fd))
					{
						if(fread(&temp,4,1,fd)==1) {
							if(temp!=0 && temp>offset && temp<nextoffset) {
								nextoffset=temp;
							}
						}
					}
				}
			}
			fclose(fd);

			if(offset>0 && nextoffset>0) {
				memset(fname,0,256);
				sprintf(fname,"%s\\pnglib.dat",buf);
				if(fd=fopen(fname,"rb")) {
					printf("Content-Type: image/png\n\n");
					fflush(stdout);
					if(fseek(fd,offset,SEEK_SET)==0) {
						temp=0;
						while(!feof(fd) && temp<(nextoffset-offset)) {
							fread(&ch,1,1,fd);
							fwrite(&ch,1,1,stdout);
							temp++;
						}
					}
					fclose(fd);
					return 0;
				}
			}
		}

		memset(fname,0,256);
		sprintf(fname,"%s\\GFX\\%05d.bmp",buf,spr);
		if(fd=fopen(fname,"rb")) {
			printf("Content-Type: image/bmp\n\n");
			fflush(stdout);
			while(!feof(fd)) {
				fread(&ch,1,1,fd);
				if(!feof(fd)) {
					//printf("%c",ch);
					fwrite(&ch,1,1,stdout);
				}
			}
			return 0;
		}

		// read from gx00.idx and gx00.dat
		memset(fname,0,256);
		sprintf(fname,"%s\\gx00.idx",buf);
		if(fd=fopen(fname,"rb")) {
			if(fseek(fd,spr*12,SEEK_SET)==0) {
				//printf("Seeked to %d<br />\n",spr*4);
				fread(&w,4,1,fd);
				fread(&h,4,1,fd);
				fread(&offset,4,1,fd);
			}
			fclose(fd);

			//printf("Content-Type: text/html\n\n");
			//printf("w=%d h=%d o=%d<br />\n",w,h,offset);
			//return 0;

			if(offset>=0 && w>0 && h>0) {
				memset(fname,0,256);
				sprintf(fname,"%s\\gx00.dat",buf);
				if(fd=fopen(fname,"rb")) {
					printf("Content-Type: image/bmp\n\n");
					fflush(stdout);

					linelength=w*3;
					while(linelength%4!=0) {
						linelength++;
					}

					// bitmap header - https://en.wikipedia.org/wiki/BMP_file_format
					printf("BM");
					temp=(54+(linelength*h));	// total file size
					fwrite(&temp,4,1,stdout);
					temp=0;
					fwrite(&temp,4,1,stdout);	// unused
					temp=54;				// offset to start of pixel data
					fwrite(&temp,4,1,stdout);
					// dib header
					temp=40;				// number of bytes in this dib header
					fwrite(&temp,4,1,stdout);
					temp=w;					// width in pixels
					fwrite(&temp,4,1,stdout);
					temp=-h;				// height in pixels	// negative for top to bottom, default is bottom to top
					fwrite(&temp,4,1,stdout);
					tempshort=1;			// color planes (always one)
					fwrite(&tempshort,2,1,stdout);
					tempshort=24;			// bits per pixel
					fwrite(&tempshort,2,1,stdout);
					temp=0;					// rgb no compression
					fwrite(&temp,4,1,stdout);
					temp=(linelength*h);	// size of data (including padding)
					fwrite(&temp,4,1,stdout);
					temp=2835;				// print resolution (72 dpi) x,y
					fwrite(&temp,4,1,stdout);
					fwrite(&temp,4,1,stdout);
					temp=0;					// colors in palette
					fwrite(&temp,4,1,stdout);
					fwrite(&temp,4,1,stdout);	// 0=all colors important

					// pixel data
					if(fseek(fd,offset,SEEK_SET)==0) {
						for(yy=0; yy<h; yy++) {
							for(xx=0; xx<w; xx++) {
								if(fread(&tempushort,2,1,fd)==1) {

									// blue
									b=((tempushort & 0xf800) >> 8) & 0xff;
									if((b >> 3) & 1 == 1)
									{
										b|=7;
									}

									// green
									g=((tempushort & 0x7e0) >> 3) & 0xff;
									if((g >> 2) & 1 == 1)
									{
										g|=3;
									}

									// red
									r=((tempushort & 0x1f) << 3) & 0xff;
									if((r >> 3) & 1 == 1)
									{
										r|=7;
									}

									fwrite(&r,1,1,stdout);
									fwrite(&g,1,1,stdout);
									fwrite(&b,1,1,stdout);

								}
							}

							// write line padding
							temp=(linelength-(w*3));
							while(temp>0) {
								r=0;
								fwrite(&r,1,1,stdout);
							}
						}
					}

					fclose(fd);
					return 0;
				}
			}
		}

		printf("Status: 404 Not Found\n");
		printf("Content-Type: text/html\n\n");
		printf("<h1>404 File not found!</h1>");

        return 0;
}
