/*
Name: Siddarth S
Date: 29/03/2026
Description: LSB Image Steganography using BMP format

For Encoding Command:
./stego -e test/beautiful.bmp test/secret.txt output/output.bmp

Sample Encoding Output:
INFO: Encoding mode selected
INFO: Starting encoding process...
INFO: ===== Encoding Started =====
INFO: Opening required files...
INFO: Files opened successfully
INFO: Checking image capacity...
INFO: Image width = <value>, height = <value>
INFO: Capacity check passed
INFO: Copying BMP header...
INFO: Encoding magic string...
INFO: Encoding secret file size...
INFO: Encoding secret file data...
INFO: Copying remaining image data...
SUCCESS: ===== Encoding Completed =====
SUCCESS: Encoding completed successfully

For the Decoding Command:
./stego -d output/output.bmp output/decoded.txt

Sample Decoding Output:
INFO: Decoding mode selected
INFO: Starting decoding process...
SUCCESS: Decoding completed successfully

Verification to check:
cat test/secret.txt
cat output/decoded.txt

Expected Output:
My password is SiddarthS ;)
My password is SiddarthS ;)

Key Concept:
In this project, I used the LSB (Least Significant Bit) technique to hide the secret data.
Instead of changing the full pixel value, I modify only the last bit of each pixel byte.
Since this change is very small scale (only ±1), it does not visibly affect the image quality overall in terms of visual perception.
This allows me to embed the secret message inside the image without making any noticeable difference.
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage:\n");
        printf("./stego -e <input.bmp> <secret.txt> [output.bmp]\n");
        printf("./stego -d <stego.bmp> [output.txt]\n");
        return 1;
    }
    if (strcmp(argv[1], "-e") == 0)
    {
        printf("INFO: Encoding mode selected\n");

        if (argc < 4)
        {
            printf("ERROR: Insufficient arguments for encoding\n");
            return 1;
        }
        EncodeInfo encInfo;
        encInfo.src_image_fname = argv[2];
        encInfo.secret_fname = argv[3];
        if (argv[4] != NULL)
            encInfo.stego_image_fname = argv[4];
        else
            encInfo.stego_image_fname = "output.bmp";
        printf("INFO: Starting encoding process...\n");
        if (do_encoding(&encInfo) == e_success)
        {
            printf("SUCCESS: Encoding completed successfully\n");
        }
        else
        {
            printf("ERROR: Encoding failed\n");
            return 1;
        }
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        printf("INFO: Decoding mode selected\n");
        DecodeInfo decInfo;
        decInfo.stego_image_fname = argv[2];
        if (argv[3] != NULL)
            decInfo.output_fname = argv[3];
        else
            decInfo.output_fname = "decoded.txt";
        printf("INFO: Starting decoding process...\n");
        if (do_decoding(&decInfo) == e_success)
        {
            printf("SUCCESS: Decoding completed successfully\n");
        }
        else
        {
            printf("ERROR: Decoding failed\n");
            return 1;
        }
    }
    else
    {
        printf("ERROR: Invalid option\n");
        printf("Use:\n");
        printf("./stego -e <input.bmp> <secret.txt> [output.bmp]\n");
        printf("./stego -d <stego.bmp> [output.txt]\n");
    }
    return 0;
}