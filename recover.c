#include <stdio.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    FILE *outptr = NULL;
    
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    int jpgcount = 0;
    int jpgopen = 0;
    
    BYTE buffer[512];
    char filename[8];
    
    while(fread(buffer, 1, 512, inptr) > 0)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpgopen == 1)
            {
                fclose(outptr);
            }
            
            sprintf(filename, "%03i.jpg", jpgcount);
            outptr = fopen(filename, "w");
            
            if (outptr == NULL)
            {
                fprintf(stderr, "Could not open %s.\n", filename);
                return 3;
            }
            else
            {
                jpgcount++;
                jpgopen = 1;
                fwrite(buffer, 1, 512, outptr);
            }
        }
        else if (jpgopen == 1)
        {
            fwrite(buffer, 1, 512, outptr);
        }
    }
    fclose(outptr);
    return 0;
}