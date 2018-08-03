#include "file.h"
#include <string.h>

uint8_t read_bin_file(struct file_ops *fops,char *filename)
{
    //TODO assert file name and size

    if(filename == NULL)
    {
        printf("no image file.\n");
        return -1;
    }

    fops->fp = fopen(filename,"r");
    if(fops->fp<0)
    {
        printf("open the image file failed...\n");
        return -1;
    }

    //get file size
    fseek(fops->fp,0,SEEK_END);
    fops->fileSize = ftell(fops->fp);
    fseek(fops->fp,0,SEEK_SET);

    //malloc memory
    fops->buf = malloc(fops->fileSize);

    if(fops->buf == NULL)
    {
        printf("malloc buffer failed...\n");
        return -1;
    }

    if(fread(fops->buf,1,fops->fileSize,fops->fp) != fops->fileSize)
    {
        printf("read file failed...\n");
        return -1;
    }
    printf("read file SUCCESS...\n");

    return 0;

}

uint16_t file_create(struct file_ops *fops,frame_file_t *file_frame)
{
    uint16_t len=MAX_FILE_LEN;
	

    if( fops->fileIndex*MAX_FILE_LEN > fops->fileSize)
        return 0;

    if( (fops->fileIndex+1)*MAX_FILE_LEN > fops->fileSize)
        len = fops->fileSize - fops->fileIndex*MAX_FILE_LEN;
	fops->fileIndex++;
    file_frame->Sequence = fops->fileIndex;
    file_frame->length = len;

    memcpy(file_frame->buf,fops->buf+fops->fileIndex*MAX_FILE_LEN,len);

    
    return sizeof(frame_file_t)+len-MAX_FILE_LEN;

}

void file_close(struct file_ops *fops)
{
    free(fops->buf);
    fops->buf=NULL;
    fclose(fops->fp);

}

file_ops __file_ops=
{
NULL,
NULL,
0,
0,
read_bin_file,
file_create,
file_close
};
