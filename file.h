#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "frame_phase.h"


typedef struct file_ops{
FILE* fp;
char* buf;
uint32_t fileSize;
uint16_t fileIndex;

uint8_t  (*open)(struct file_ops *fops,char *filename);
uint16_t (*create)(struct file_ops *fops,frame_file_t *fileframe);
void     (*close)(struct file_ops *fops);

}file_ops;

extern file_ops __file_ops;
#endif // FILE_H_INCLUDED
