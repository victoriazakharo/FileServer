#include <stdio.h>
#include <stdlib.h>

long read_file(char **file_contents, const char *filename)
{
    char filepath[100];
    sprintf(filepath, "files/%s", filename);
    FILE *file = fopen(filepath, "r");

    if (file == NULL)
        return -1;

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    *file_contents = malloc(fsize);
    fread(*file_contents, fsize, 1, file);
    fclose(file);
    return fsize;
}

int main()
{
    char *text;
    long len = read_file(&text, "horse.txt");
    printf("%Ld", len);
    if (len < 0)
        return -1;
    FILE *f = fopen("tmp.txt", "w");
    fwrite(text, sizeof(char), len, f);
    fclose(f);
    return 0;
}

