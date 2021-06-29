#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct ImgBuf{
    unsigned int img_w, img_h;
    unsigned char* buf;
    unsigned short bpp;
};



struct ImgBuf* readBMP(const char* fn)
{
    FILE* fp = fopen(fn, "rb");
    printf("Try to open: %s\n",fn);

    if(fp == NULL){
        return NULL;
    }

    printf("Succesfully open the file\n");


    unsigned char info[54];
    fread(info, 1, 54, fp);

    unsigned short w = *(unsigned short*)&info[18];
    unsigned short h = *(unsigned short*)&info[22];
    unsigned short bpp = *(unsigned short*)&info[28];
    unsigned int compMethod = *(unsigned int*)&info[30];

    if(compMethod != 0){
        fclose(fp);
        return 0;
    }
    unsigned int imgSize = (bpp>>3)*w*h;
    struct ImgBuf* imgbuf = malloc(sizeof(struct ImgBuf));

    memset(imgbuf, 0, sizeof(struct ImgBuf));

    imgbuf->img_w = (unsigned int)w;
    imgbuf->img_h = (unsigned int)h;
    imgbuf->bpp = bpp;
    imgbuf->buf = malloc(imgSize);
    memset(imgbuf->buf, 0, imgSize);

    fread(imgbuf->buf, 1, imgSize, fp);

    fclose(fp);

    for(unsigned int i = 0; i < imgSize; i += 3){
        unsigned char tmp = imgbuf->buf[i];
        imgbuf->buf[i] = imgbuf->buf[i+2];
        imgbuf->buf[i+2] = tmp;
    }

    return imgbuf;
}


int main(int argc, char* argv[])
{
    printf("Printing BMP info\n");
    printf("=================\n");

    if(argc < 3){
        return 0;
    }

    struct ImgBuf* fontmap = readBMP(argv[2]);

    free(fontmap->buf);
    free(fontmap);

    return 0;
}