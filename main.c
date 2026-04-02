#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PI_STR "314159265358979"
#define PI_LENGTH 15

int StrungPi[PI_LENGTH];

void arrayifyPi() {
    const char *piTemp = PI_STR;
    for (int i = 0; i < PI_LENGTH; i++) {
        StrungPi[i] = piTemp[i] - '0';
    }
}

int main(int argc, char *argv[]) {
    printf("π🔒 Pi Encrypt\n");
    printf("By UnknownKE/The KE\n");
    printf("Made for Instructables spring 2026 All Things Pi contest\n\n");
    printf("Doing unimportant stuff you prob dont care about...\n");
    arrayifyPi();
    return 0;
}
