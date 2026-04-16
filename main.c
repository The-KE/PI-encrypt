#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <b64/cencode.h>

#define PI_STR "3141592653589793"
#define PI_LENGTH 16

int StrungPi[PI_LENGTH];
int InputNums[3];
char inputFSname[512];
char RandKey[16];
size_t fileSize;

void arrayifyPi() {
    const char *piTemp = PI_STR;
    for (int i = 0; i < PI_LENGTH; i++) {
        StrungPi[i] = piTemp[i] - '0';
    }
}
unsigned char *ReadFileAndKeyStuff(char fring[]) {
    int desc = open(inputFSname, O_RDONLY);
    struct stat bytes;
    fstat(desc, &bytes);
    if (bytes.st_size < 32000000) {
        FILE *EncryptionVictim = fopen(fring, "rb");
        fseek(EncryptionVictim, 0, SEEK_END);
        long fize = ftell(EncryptionVictim);
        fileSize = fize;
        rewind(EncryptionVictim);
        unsigned char *fuf = malloc(fize+1);
        fread(fuf, 1, fize, EncryptionVictim);
        fclose(EncryptionVictim);
        FILE *urand = fopen("/dev/urandom", "r");
        fread(&RandKey, 1, sizeof(RandKey), urand);
        fclose(urand);
        return fuf;
    }
    unsigned char *fuf = mmap(NULL, bytes.st_size, PROT_READ, MAP_PRIVATE, desc, 0);
    if (fuf == MAP_FAILED) {
        printf("Memory Map failed. Either my mediocre coding or an invalid file :/\n");
        munmap(fuf, bytes.st_size);
        close(desc);
        exit(0);
    }
    madvise(fuf, bytes.st_size, MADV_SEQUENTIAL);
    FILE *urand = fopen("/dev/urandom", "r");
    fread(&RandKey, 1, sizeof(RandKey), urand);
    fclose(urand);
    fileSize = bytes.st_size;
    return fuf;
}
void arrayifyArgs(int argc, char *argv[]) {
  for (int i = 0; i < argc-3; i++) {
    InputNums[i] = atoi(argv[i+1]);
    if (InputNums[i] == 0) {
        printf("Invalid Ints: either used zero or inputted non integer for int fields. Int %d was invalid.\n", i+1);
        exit(0);
    }
  }
  strncpy(inputFSname, argv[4], sizeof(inputFSname)-1);
  inputFSname[sizeof(inputFSname)-1] = '\0';
}
void printKey(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        printf("%02x", (unsigned char)str[i]);
    }
    printf("\n");
}
void EnCrYpT(unsigned char *buf) {
    char sectMat[4][4];
    div_t chunk16remainder = div(fileSize, 16);
    for (int i = 0; i < chunk16remainder.quot; i++) {
        for (int j = 1; j < 17; j++) {
            div_t tempGridPos = div(4, j);
            int charascii = buf[(i*16)+j-1];
            
        }
    }
}

int main(int argc, char *argv[]) {
    // note to self - FIND A LOCK CHARACTER THAT ISNT AN EMOJI!!!
    printf("π Pi Encrypt\n");
    printf("By UnknownKE/The KE\n");
    printf("Made for Instructables spring 2026 All Things Pi contest\n\n");
    if (argc == 1) {
      printf("Usage: int int int fileToEncrypt outputFileName\n");
      printf("make sure the 3 ints are far apart, abide by the 32 bit int limit, and are hard to guess as they are used for encryption.\n");
      return 0;
    }
    printf("Doing unimportant stuff you prob dont care about...\n");
    arrayifyPi();
    printf("Remember to NOT use 0 as an input integer as it will make predictable encryted things...\n");
    printf("Putting a filename larger than 511 chars will cause a buffer overflow so just dont!\n");
    arrayifyArgs(argc, argv);
    printf("Input ints: %d, %d, %d\n\n", InputNums[0], InputNums[1], InputNums[2]);
    printf("reading file...\n");
    unsigned char *fileData = ReadFileAndKeyStuff(inputFSname);
    printf("Your're key is ");
    printKey(RandKey);
    printf("Make sure to save it and your three input integers!\n\n");
    printf("Beginning encryption...\n");
    EnCrYpT(fileData);
    munmap(fileData, fileSize);
    return 0;
}
