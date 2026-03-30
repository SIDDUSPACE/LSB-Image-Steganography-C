#ifndef ENCODE_H
#define ENCODE_H
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

#include <stdio.h>
#include "types.h"
typedef struct _EncodeInfo
{
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    char *secret_fname;
    FILE *fptr_secret;
    long size_secret_file;
    char *stego_image_fname;
    FILE *fptr_stego_image;
} EncodeInfo;
Status open_files(EncodeInfo *encInfo);
uint get_image_size_for_bmp(FILE *fptr_image);
Status check_capacity(EncodeInfo *encInfo);
Status copy_bmp_header(FILE *src, FILE *dest);
Status encode_magic_string(const char *magic, EncodeInfo *encInfo);
Status encode_secret_file_size(long size, EncodeInfo *encInfo);
Status encode_secret_file_data(EncodeInfo *encInfo);
Status encode_data_to_image(char *data, int size, FILE *src, FILE *dest);
Status encode_byte_to_lsb(char data, char *image_buffer);
Status copy_remaining_img_data(FILE *src, FILE *dest);
Status do_encoding(EncodeInfo *encInfo);

#endif