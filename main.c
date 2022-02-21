#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _picture {
    unsigned int size;
    char *name;
    char *Bytes_array;
    int header;
} picture;

int main(int argc, char* argv[]) {
    //crypting
    if (argc == 3) {
        crypting(argv[1], argv[2]);
    }
    //decrypting
    else if (argc == 2) {
        decrypting(argv[1]);
    }
    else {
        printf("Wrong parametres, try again!");
        return 1;
    }
    return 0;
}

void charToBin(unsigned char c) {
    int dec = (int)c;
    int bin_out[8] = {0};
    printf("%d(d): ", dec);

    int i = 0;
    while((dec) != 0) {
        bin_out[i++] = dec % 2 + '0';
        dec /= 2;
    }
    for (i--; i >= 0; i--) {
        printf("%c", bin_out[i]);
    }
    printf("(b)\t");
}

int getSize(char *string) {
    char size[4];
    unsigned int d = 0;
    int j = 0;
    for (int i = 5; i > 1; i--) {
        size[j] = string[i];
        j++;
    }
    d = (unsigned char)size[0] * 16777216 + (unsigned char)size[1] * 65536 + (unsigned char)size[2] * 256 + (unsigned char)size[3]; 
    return d;
}

void imageInfo(picture *bmp) {

    FILE *file;
    int fileLen = 0;

    file = fopen(bmp->name, "rb");
    if (!file) {
        printf("Unable to open the file");
        return;
    }
    fseek(file, 0, SEEK_END);
    fileLen = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    bmp->Bytes_array = malloc(sizeof(char) * fileLen);
    if (!bmp->Bytes_array) {
        printf("Error1");
        return;
    }
    fread(bmp->Bytes_array, sizeof(char), fileLen, file);
    fclose(file);
    
    bmp->size = getSize(bmp->Bytes_array);
    bmp->header = (int)bmp->Bytes_array[10];
}

void crypting(char *name, char *new_name) {
    picture *figure = (picture *)malloc(sizeof(picture));
    if (!figure) {
        printf("Error");
        return;
    }
    figure->name = name;
    imageInfo(figure);

    picture *message = (picture *)malloc(sizeof(picture));
    if (!message) {
        printf("Error");
        return;
    }
    message->name = new_name;
    imageInfo(message);

    //counter = sum of all chars of secret message 
    //count_byte = current position in figure->Bytes_array
    unsigned int counter;
    int count_byte = figure->header, round = 0;
    char *textfile = message->Bytes_array;
    unsigned char hexa0x0001 = 0x00000001;
    unsigned char hexa0x00FE = 0x000000FE;
    unsigned char token = 0x00000001;

    for (counter = 0; counter < message->size; counter++){
        char pismeno = textfile[counter];
        // one char of message
        for (int j = 0; j < 8; j++) {
            if (pismeno & 0x00000001) {
                //LSB to 1
                figure->Bytes_array[count_byte] |= hexa0x0001;
                count_byte++;
            }
            else {
                //LSB to 0
                figure->Bytes_array[count_byte] &= hexa0x00FE;
                count_byte++;
            }
            pismeno >>= 1;  
        }
        if ((unsigned int)count_byte + 8 > figure->size) {
            token <<= round;
            hexa0x00FE -= token;
            hexa0x0001 <<= 1;
            count_byte = figure->header;
            round++;
            token = 0x00000001;
        }
    }
    FILE *fp = fopen("merged_image.bmp", "wb");
    if (!fp) {
        printf("Error");
        return;
    }
    for (unsigned int i = 0; i < figure->size; i++) {
        fputc(figure->Bytes_array[i], fp);
    }
    free(figure->Bytes_array);
    free(figure);
    free(message->Bytes_array);
    free(message);
    fclose(fp);
}

void decrypting(char *name) {
    picture *figure = (picture *)malloc(sizeof(picture));
    figure->name = name;
    imageInfo(figure);

    FILE *hiding = fopen("obr2_separated.bmp", "wb");
    if (!hiding) {
        printf("Error");
        return;
    }
    for (unsigned int i = 0; i < figure->size; i++) {
        fputc(figure->Bytes_array[i], hiding);
    }
    fclose(hiding);

    picture *message = (picture *)malloc(sizeof(picture));
    message->name = "obr1_separated.bmp";
    message->size = 10;

    //amount of all finded chars from message
    unsigned int counter;
    //count_bytes = current position in figure->Bytes->array
    int count_bytes = figure->header, round = 0;
    char template = 0x00000000, znak;
    char *textfile = (char *)malloc(sizeof(char) * figure->size);
    if (!textfile) {
        printf("Error");
        return;
    }

    for (counter = 0; counter < message->size; counter++) {
        for (int j = 0; j < 8; j++) {
            znak = figure->Bytes_array[count_bytes+7-j];
            if ((znak >> round) & 0x00000001) {
                //LSB to 1
                template |= 0x00000001;
            }
            else {
                //LSB to 0
                template &= 0x000000FE;
            }
            if (j != 7) template <<= 1;
        }
        if (counter == 6) {
            message->size = getSize(textfile);
            realloc(textfile, sizeof(char) * message->size);
        }
        textfile[counter] = template;
        template = 0x00000000;
        if ((unsigned int)count_bytes + 8 > figure->size) {
            count_bytes = figure->header; 
            round++;
        }
        count_bytes += 8;
    }

    message->Bytes_array = textfile;
    FILE *fp = fopen(message->name, "wb");
    if (!fp) {
        printf("Error");
        return;
    }
    for (unsigned int i = 0; i < message->size; i++) {
        fputc(message->Bytes_array[i], fp);
    }
    free(figure->Bytes_array);
    free(figure);
    free(message->Bytes_array);
    free(message);
    fclose(fp);
}