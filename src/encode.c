#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "common.h"

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, fptr_image);
    fread(&height, sizeof(int), 1, fptr_image);
    printf("INFO: Image width = %u, height = %u\n", width, height);
    return width * height * 3;
}
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO: Opening required files...\n");
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    if (!encInfo->fptr_src_image)
    {
        printf("ERROR: Unable to open source image\n");
        return e_failure;
    }
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    if (!encInfo->fptr_secret)
    {
        printf("ERROR: Unable to open secret file\n");
        return e_failure;
    }
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    if (!encInfo->fptr_stego_image)
    {
        printf("ERROR: Unable to create output file\n");
        return e_failure;
    }
    printf("INFO: Files opened successfully\n");
    return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
    printf("INFO: Checking image capacity...\n");
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    fseek(encInfo->fptr_secret, 0, SEEK_END);
    encInfo->size_secret_file = ftell(encInfo->fptr_secret);
    rewind(encInfo->fptr_secret);
    long required = (strlen(MAGIC_STRING) + sizeof(long) + encInfo->size_secret_file) * 8;
    if (encInfo->image_capacity >= required)
    {
        printf("INFO: Capacity check passed\n");
        return e_success;
    }
    else
    {
        printf("ERROR: Insufficient image capacity\n");
        return e_failure;
    }
}
Status copy_bmp_header(FILE *src, FILE *dest)
{
    printf("INFO: Copying BMP header...\n");
    char buffer[54];
    rewind(src);
    fread(buffer, 54, 1, src);
    fwrite(buffer, 54, 1, dest);
    printf("INFO: Header copied successfully\n");
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
    }
    return e_success;
}
Status encode_data_to_image(char *data, int size, FILE *src, FILE *dest)
{
    char buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(buffer, 8, 1, src);
        encode_byte_to_lsb(data[i], buffer);
        fwrite(buffer, 8, 1, dest);
    }
    return e_success;
}
Status encode_magic_string(const char *magic, EncodeInfo *encInfo)
{
    printf("INFO: Encoding magic string...\n");

    encode_data_to_image((char *)magic, strlen(magic),
                         encInfo->fptr_src_image,
                         encInfo->fptr_stego_image);

    printf("INFO: Magic string encoded\n");
    return e_success;
}
Status encode_secret_file_size(long size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret file size...\n");
    encode_data_to_image((char *)&size, sizeof(size),
                         encInfo->fptr_src_image,
                         encInfo->fptr_stego_image);
    printf("INFO: File size encoded\n");
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret file data...\n");
    char ch;
    while (fread(&ch, 1, 1, encInfo->fptr_secret))
    {
        encode_data_to_image(&ch, 1,
                             encInfo->fptr_src_image,
                             encInfo->fptr_stego_image);
    }
    printf("INFO: Secret data encoded\n");
    return e_success;
}
Status copy_remaining_img_data(FILE *src, FILE *dest)
{
    printf("INFO: Copying remaining image data...\n");
    char ch;
    while (fread(&ch, 1, 1, src))
    {
        fwrite(&ch, 1, 1, dest);
    }
    printf("INFO: Remaining data copied\n");
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    printf("INFO: ===== Encoding Started =====\n");
    if (open_files(encInfo) == e_failure)
        return e_failure;
    if (check_capacity(encInfo) == e_failure)
        return e_failure;
    copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);
    encode_magic_string(MAGIC_STRING, encInfo);
    encode_secret_file_size(encInfo->size_secret_file, encInfo);
    encode_secret_file_data(encInfo);
    copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);
    printf("SUCCESS: ===== Encoding Completed =====\n");
    return e_success;
}