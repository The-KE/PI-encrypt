#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define PI_STR "3141592653589793"
#define PI_LENGTH 16

int StrungPi[PI_LENGTH];
int InputNums[3];
char inputFSname[512];
char outputFSname[512];
unsigned char RandKey[16];
char hexKey[32];
bool ktofile = false;
size_t fileSize;

void arrayifyPi() {
    const char *piTemp = PI_STR;
    for (int i = 0; i < PI_LENGTH; i++) {
        StrungPi[i] = piTemp[i] - '0';
    }
}
unsigned char *createOutputFile() {
    int desc = open(inputFSname, O_RDONLY);
    int ndesc = open(outputFSname, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (ndesc == -1) {
        printf("Failed to create output file.");
        exit(0);
    }
    if (desc == -1) {
        printf("Failed to read input file.");
        exit(0);
    }
    struct stat bytes;
    fstat(desc, &bytes);
    if (bytes.st_size < 32000000) {
        return malloc(bytes.st_size + 1);
    }
    ftruncate(ndesc, bytes.st_size + 1);
    unsigned char *fuf = mmap(NULL, bytes.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, ndesc, 0);
    if (fuf == MAP_FAILED) {
        printf("Memory Map of output file failed. Either my mediocre coding or an invalid file :/\n");
        munmap(fuf, bytes.st_size);
        close(desc);
        exit(0);
    }
    close(desc);
    close(ndesc);
    return fuf;
}
unsigned char *ReadFileAndKeyStuff(char fring[]) {
    int desc = open(inputFSname, O_RDONLY);
    if (desc == -1) {
        printf("Failed to read input file.");
        exit(0);
    }
    struct stat bytes;
    fstat(desc, &bytes);
    if (bytes.st_size < 32000000) {
        FILE *EncryptionVictim = fopen(fring, "r+");
        if (EncryptionVictim == NULL) {
            printf("Unable to open input file (<32MB: used fopen)\n");
            fclose(EncryptionVictim);
            exit(0);
        }
        fseek(EncryptionVictim, 0, SEEK_END);
        long fize = ftell(EncryptionVictim);
        fileSize = fize;
        rewind(EncryptionVictim);
        unsigned char *fuf = malloc(fize+1);
        fread(fuf, 1, fize, EncryptionVictim);
        fclose(EncryptionVictim);
        close(desc);
        return fuf;
    }
    unsigned char *fuf = mmap(NULL, bytes.st_size, PROT_READ, MAP_PRIVATE, desc, 0);
    if (fuf == MAP_FAILED) {
        printf("Memory Map of input file failed. Either my mediocre coding or an invalid file (>32MB: used mmap)\n");
        munmap(fuf, bytes.st_size);
        close(desc);
        exit(0);
    }
    close(desc);
    madvise(fuf, bytes.st_size, MADV_SEQUENTIAL);
    fileSize = bytes.st_size;
    return fuf;
}
void arrayifyArgs(int argc, char *argv[]) {
  for (int i = 0; i < 3; i++) {
    InputNums[i] = atoi(argv[i+1]);
    if (InputNums[i] == 0) {
        printf("Invalid Ints: either used zero or inputted non integer for int fields. Int %d was invalid.\n", i+1);
        exit(0);
    }
  }
  strncpy(inputFSname, argv[4], sizeof(inputFSname)-1);
  inputFSname[sizeof(inputFSname)-1] = '\0';
  strncpy(outputFSname, argv[5], sizeof(outputFSname)-1);
  outputFSname[sizeof(outputFSname)-1] = '\0';
  strncpy(hexKey, argv[6], sizeof(hexKey)-1);
  if (argc >= 7 && !strcmp(argv[6], "-k"))
      ktofile = true;
}
void deHexKey(const char *hex) {
    int idx = 0;
    for (int i = 0; i < 32; i += 2) {
        unsigned int byte;
        char pair[3] = {hex[i], hex[i+1], '\0'};
        sscanf(pair, "%x", &byte);
        RandKey[idx++] = (char)byte;
    }    
}
void rotaterow(unsigned char matrix[4][4], int row, int n) {
    n = n % 4;
    if (n == 0) return;
    char temp[4];
    memcpy(temp, matrix[row], 4);
    for (int j = 0; j < 4; j++)
        matrix[row][(j + n) % 4] = temp[j];
}

void rotatecol(unsigned char matrix[4][4], int col, int n) {
    n = n % 4;
    if (n == 0) return;
    char temp[4];
    for (int i = 0; i < 4; i++) temp[i] = matrix[i][col];
    for (int i = 0; i < 4; i++)
        matrix[(i - n + 4) % 4][col] = temp[i];
}
void EnCrYpT(unsigned char *buf, unsigned char *out) {
    unsigned char sectMat[4][4];
    div_t chunk16remainder = div(fileSize, 16);
    for (int i = 0; i < chunk16remainder.quot; i++) {
        memset(sectMat, 0, sizeof(sectMat));
        for (int j = 0; j < 16; j++) {
            int rows = j/4;
            int cols = j%4;
            sectMat[rows][cols] = (unsigned char)buf[i*16+j];
        }
        rotaterow(sectMat, 2, 4-((RandKey[InputNums[1]%16])%4));
        rotaterow(sectMat, 0, 4-((InputNums[(((int)RandKey[8]*StrungPi[12]) ^ InputNums[2])%3]*42)%4));
        rotatecol(sectMat, 2, 4-((InputNums[(int)RandKey[15]%3])%4));
        rotatecol(sectMat, 0, 4-((InputNums[2]*InputNums[0])%4));
        rotaterow(sectMat, 3, 4-((RandKey[InputNums[1]%16])%4));
        rotaterow(sectMat, 1, 4-((InputNums[(((int)RandKey[8]*StrungPi[12]) ^ InputNums[2])%3]*42)%4));
        rotatecol(sectMat, 3, 4-((InputNums[(int)RandKey[15]%3])%4));
        rotatecol(sectMat, 1, 4-((InputNums[2]*InputNums[0])%4));
        for (int j = 0; j < 16; j++) {
            int rows = j/4;
            int cols = j%4;
            int charascii = sectMat[rows][cols];
            int randAscii = RandKey[j];
            int piDigit = StrungPi[(randAscii+(i*j))%16];
            int xored = piDigit ^ charascii;
            sectMat[rows][cols] = (unsigned char)xored;
        }
        for (int k = 0; k < 4; k++) {
            for (int l = 0; l < 4; l++) {
                out[(i*16)+(k*4+l)] = sectMat[k][l];
            }
        }
    }
    for (int i = 0; i < chunk16remainder.rem; i++)
        out[(chunk16remainder.quot*16) + i] = buf[(chunk16remainder.quot*16) + i];
    if (fileSize < 32000000) {
        FILE *outFile = fopen(outputFSname, "r+");
        if (outFile == NULL) {
            printf("Unable to open output file for final writing (<32MB: used fopen)\n");
        }
        fwrite(out, 1, fileSize, outFile);
        fclose(outFile);
    }
}

int main(int argc, char *argv[]) {
    // note to self - FIND A LOCK CHARACTER THAT ISNT AN EMOJI!!!
    printf("π Pi Decrypt\n");
    printf("By UnknownKE/The KE\n");
    printf("Made for Instructables spring 2026 All Things Pi contest\n\n");
    if (argc == 1) {
      printf("Usage: int int int fileToDecrypt outputFileName -k key\n");
      printf("make sure the 3 ints are far apart, abide by the 32 bit int limit, and are hard to guess as they are used for encryption.\n");
      return 0;
    }
    printf("Doing unimportant stuff you prob dont care about...\n");
    arrayifyPi();
    arrayifyArgs(argc, argv);
    printf("converting key from hex...\n");
    if (ktofile == true) {
         FILE *kout = fopen("piEncKey.txt", "r");
         if (kout == NULL) {
             printf("Failed to open file for key reading.");
             exit(0);
         }
         fread(&hexKey, 1, 32, kout);
         fclose(kout);
    }
    deHexKey(hexKey);
    printf("Just a reminder that it is impossible to use 0 as in input integer.\n");
    printf("Putting a filename larger than 511 chars will cause a buffer overflow so just dont!\n");
    printf("Input ints: %d, %d, %d\n\n", InputNums[0], InputNums[1], InputNums[2]);
    printf("reading file...\n");
    unsigned char *fileData = ReadFileAndKeyStuff(inputFSname);
    unsigned char *outPtr = createOutputFile();
    printf("Make sure to save it and your three input integers!\n\n");
    printf("Beginning encryption...\n");
    EnCrYpT(fileData, outPtr);
    munmap(fileData, fileSize);
    munmap(outPtr, fileSize);
    return 0;
}
