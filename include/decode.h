#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"
typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char *output_fname;
    FILE *fptr_output;
    long size_secret_file;
} DecodeInfo;
Status open_decode_files(DecodeInfo *decInfo);
Status decode_magic_string(DecodeInfo *decInfo);
Status decode_secret_file_size(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image);
Status decode_byte_from_lsb(char *data, char *image_buffer);
Status do_decoding(DecodeInfo *decInfo);

#endif