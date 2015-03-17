#include <stdio.h>
#include <stdlib.h>

void save_file(char *text, long fsize, const char *filename)
{
    char filepath[100];
    sprintf(filepath, "files/%s", filename);
    FILE *file = fopen(filepath, "w");
    fwrite(text, fsize, 1, file);
    fclose(file);
}

int main()
{
    char text[5] = {'q', 'w', 'e', 'r', 't'};
    save_file(text, 5, "output.txt");
    return 0;
}
