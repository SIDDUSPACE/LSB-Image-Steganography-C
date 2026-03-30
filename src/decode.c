#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (!decInfo->fptr_stego_image)
        return e_failure;
    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    if (!decInfo->fptr_output)
        return e_failure;
    return e_success;
}
Status decode_byte_from_lsb(char *data, char *buffer)
{
    *data = 0;
    for (int i = 0; i < 8; i++)
    {
        *data = (*data << 1) | (buffer[i] & 1);
    }
    return e_success;
}
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
    char buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(buffer, 8, 1, fptr_stego_image);
        decode_byte_from_lsb(&data[i], buffer);
    }
    return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic[3] = {0};
    decode_data_from_image(magic, strlen(MAGIC_STRING), decInfo->fptr_stego_image);
    if (strcmp(magic, MAGIC_STRING) != 0)
    {
        printf("ERROR: Magic string mismatch\n");
        return e_failure;
    }
    return e_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_data_from_image((char *)&decInfo->size_secret_file,
                           sizeof(decInfo->size_secret_file),
                           decInfo->fptr_stego_image);
    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        decode_data_from_image(&ch, 1, decInfo->fptr_stego_image);
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }
    return e_success;
}
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_failure)
    {
        printf("Error opening files\n");
        return e_failure;
    }
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    if (decode_magic_string(decInfo) == e_failure)
        return e_failure;
    decode_secret_file_size(decInfo);
    decode_secret_file_data(decInfo);
    printf("Decoding Successful\n");
    return e_success;
}