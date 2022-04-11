
#include <stdio.h>
#include <string.h>
int helperArray[81];
unsigned long inputArrayLen;
int checkIfAlphaNumeric(char inputArray[],int i, int j){ /* if inputArray[i] and inputArray[i+1] are alphanumeric return 1, else 0;*/ 
    char char1 = inputArray[i];
    char char2 = inputArray[j];
    if (( (48 <= char1 &&char1 <= 57) || (65<= char1&&char1 <=90) || (97<=char1 && char1 <=122)) 
         & ((48 <= char2 &&char2 <= 57) || (65<= char2 &&char2 <=90) || (97<=char2 && char2 <=122))) {
            return 1;
        }
    
    else 
    {
        return 0;
    }
}
void removeTwoSizedSeries() {
    int i;
         for (i = 0; i<inputArrayLen;i++) { /*removing 2-sized series*/
         if (helperArray[i] == 1 && helperArray[i+1] == -1) {
             helperArray[i] = 0;
             helperArray[i+1] = 0;
         }
     }
}
void fillDummyArray(char inputArray[]) { /*only alpha numeric characters!*/
     int i;
     if (inputArray[0] + 1== inputArray[1] && checkIfAlphaNumeric(inputArray,0,1)) { /* handling  inputArray[0] case*/
         helperArray[0] = 1;
     }
     for(i =1;i<inputArrayLen;i++) { /* handling i>0 inputArray[i] cases*/
        if (inputArray[i]+1 == inputArray[i+1] && inputArray[i] != inputArray[i-1] +1 &&
        checkIfAlphaNumeric(inputArray, i, i+1)){ /*begins a new series*/
            helperArray[i] = 1;
        } 
        if ( ((!checkIfAlphaNumeric(inputArray,i,i+1)) || inputArray[i] + 1 != inputArray[i+1] ) && (checkIfAlphaNumeric(inputArray,i,i-1) && inputArray[i] == inputArray[i-1] + 1))  {/* ends a series*/
            helperArray[i] = -1;
        }
     }
     }
void fillOutputArray(char inputArray[], char outputArray[]) {
         int i = -1, j =0, inASeries = 0, arryLen = inputArrayLen;
     /* i is s1 iterator, j is s2 iterator*/
     while (i< arryLen) {
         i++;
         if (inASeries == 1) { /* if currently in a series*/
             if (helperArray[i] ==-1) {
                 outputArray[j] = '-'; j++;
                 outputArray[j] = inputArray[i]; j++;
                 helperArray[i] = 0;
                 inASeries = 0;
             }
         } 
         else {
             if (helperArray[i] == 1) { /* if currently starting a new series*/
                 outputArray[j] = inputArray[i]; j++;
                 inASeries = 1;
             } 
             else {
                 outputArray[j] = inputArray[i]; j++; /* if not in a series and not starting a new one*/
             }
         }
     }
}
void contract(char inputArray[], char outputArray[]) {
    /* initialize*/
     inputArrayLen = strlen(inputArray);
    /* fill the dummyArray*/
     fillDummyArray(inputArray);

    /* rmeove the series like a-b, 1-2, aka two sized series*/
     removeTwoSizedSeries();
    /* fill the outputArray based on the dummyArray*/
     fillOutputArray(inputArray, outputArray);
}

int main()
{
    char inputArray[81]; 
    char outputArray[81];
    printf("Please write a string to be shortened \n");
    fgets(inputArray,81, stdin);
    contract(inputArray, outputArray);

    printf("The original input is: %sThe shortened input is: %s ", inputArray, outputArray);
    return 0;
}





