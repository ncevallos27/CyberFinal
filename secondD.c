#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "r+");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(fp);
        return 1;
    }
    long file_size = ftell(fp);
    if (file_size < 0) {
        perror("ftell");
        fclose(fp);
        return 1;
    }

    uint8_t key = file_size % 256;

    rewind(fp);

    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (fseek(fp, -1, SEEK_CUR) != 0) {
            perror("fseek");
            fclose(fp);
            return 1;
        }

        uint8_t decrypted = (uint8_t)c - key;
        if (fputc(decrypted, fp) == EOF) {
            perror("fputc");
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    printf("Decryption complete. Key = %u\n", key);
    return 0;
}
