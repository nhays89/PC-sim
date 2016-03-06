#include <stdio.h>
#include <sys/stat.h>

char *int2bin(int a, char *buffer, int buf_size) 
{
    buffer += (buf_size - 1);
    int i = 0;

    for (i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
        }
        //annoying. added this
        buffer[33] = '\0';
        return buffer;
}

int fsize(const char *filename) 
{
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

void createTestFile()
{
    FILE *outFile;
    int i = 0;

    outFile = fopen("test.bin", "wb");
    if (!outFile)
        return;
    for (i =0; i <= 25; i++)
    {
        fwrite(&i, sizeof(int), 1, outFile);
    }
    fclose(outFile);
}

void readTestFile()
{

    FILE *outFile;
    int i = 0;
    int out = 0;
    char binString[33];

    outFile = fopen("ok.done", "rb");
    if (!outFile)
        return;
    for (i =0; i <= (fsize("ok.done")/sizeof(int) -1); i++)
    {
        fread(&out, sizeof(int), 1, outFile);
        int2bin(out, binString, 32);

        printf("%s\n", binString);
    }
    printf("file size: %d\n", fsize("ok.done")/sizeof(int));

    fclose(outFile);
}

int main()
{
    //createTestFile();
    readTestFile();
    return 0;
}
