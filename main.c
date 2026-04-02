#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PI_STR "314159265358979"
#define PI_LENGTH 15

int StrungPi[PI_LENGTH];
int InputNums[3];
char inputFSname[512];

void arrayifyPi() {
    const char *piTemp = PI_STR;
    for (int i = 0; i < PI_LENGTH; i++) {
        StrungPi[i] = piTemp[i] - '0';
    }
}
void arrayifyArgs(int argc, char *argv[]) {
  for (int i = 0; i < argc-3; i++) {
    InputNums[i] = argv[i+1];
  }
}

int main(int argc, char *argv[]) {
    // note to self - FIND A LOCK CHARACTER THAT ISNT AN EMOJI!!!
    printf("π Pi Encrypt\n");
    printf("By UnknownKE/The KE\n");
    printf("Made for Instructables spring 2026 All Things Pi contest\n\n");
    if (argc == 1) {
      printf("Usage: int int int fileToEncrypt outputFileName\n")
      printf("make sure the 3 ints are far apart, abide by the 32 bit int limit, and are hard to guess as they are used for encryption.\n")
      return 0;
    }
    printf("Doing unimportant stuff you prob dont care about...\n");
    arrayifyPi();
    printf("Remember to NOT use 0 as an input integer as it will make predictable encryted things...\n");
    printf("Putting a filename larger than 511 chars will cause a buffer overflow so just dont!\n");
    return 0;
}
