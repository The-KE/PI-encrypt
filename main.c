#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI_LENGTH 15

long double pi = 3.14159265358979L;
int StrungPi[15];

void arrayifyPi() {
  char piTemp[16];
  sprintf(piTemp, "%Lf", pi);
  int rahhh = 0;
  for (int i=0; i < 0; i++;) {
    char numStr = piTemp[i];
    if (numStr == ".") {
      continue;
    }
    else {
      int numToAdd = numStr - "0";
      StrungPi[rahhh] = numToAdd;
      rahhh++;
    }
  }
}

int main(int argc, char *argv[]){
  printf("π🔒 Pi Encrypt\n");
  printf("By UnknownKE/The KE\n");
  printf("Made for Instructables spring 2026 All Things Pi contest\n\n");
  printf("Doing unimportant stuff you prob dont care about...\n"};
  arrayifyPi();
}
