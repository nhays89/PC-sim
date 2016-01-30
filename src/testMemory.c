#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

void writeFileTest()
{
    int test[5] = {12345,33,444,333,777};
    FILE *out = fopen("test.bin", "wb");
    fwrite(&test, sizeof(int), 5, out);
    fclose(out);
}

void readFileTest()
{
    int test = 0;
    int size = 0;
    FILE *in = fopen("test.bin", "rb");
    while(!feof(in))
    {
        fread(&test, sizeof(int),1,in);
        printf("%d\n", test);
        size++;
    }
    printf("number of ints in binary file: %d", size);
    fclose(in);
}

void willItWork();

int main ()
{
    Memory *test;
    
    test = createMemory(8);

    test->write(test, 0, 3);
    printf("%d\n",test->read(test,0));
    test->destroyMemory(test);
    writeFileTest();
    readFileTest();
    willItWork();
    return 0;
}

void willItWork()
{
    printf("it worked");
}
