#include <stdio.h>
#include <string.h>
#include <ctype.h>
char inputLine[100]; /* the line recieved*/
void checkParenthesis();
int removeCommentsAndQuotes(int isInOverrideMode);
int checkParenthesesString();
int isCurvedBrackets();
void parenthesesOutput(int areParenthesesBalanced, int areCurvedBracketsClosed);

int main()
{
    checkParenthesis();
    return 0;

}
char DeleteParenthesesInChar(char x) { /*recieves a char. if the char is a parentheses return ' ' else return the char*/
    if (x == '{' || x == '}' || x == '[' || x == ']' || x== '(' || x==')') {
        return ' ';
    }
    else return x;
    
}
void checkParenthesis() { /** main app*/
    int areParenthesesBalanced = 1, areCurvedBracketsClosed =0, areCurvedBracketsBalancedGlobally =-1,isInOverrideMode =0;
    /*
    Exceptions are: 
    1. single '{' AND '}'
    2. Comments
    3. Stuff inside quotation marks
    
    */
    while(fgets(inputLine,100, stdin)) {
        printf("%s", inputLine);
        
        if (isInOverrideMode == 0) {areCurvedBracketsClosed = areCurvedBracketsClosed + isCurvedBrackets();} /*if the line is not preceded by a / * in previous line (aka a comment)*/
        
        if (areCurvedBracketsClosed < -1) {/** Check for cases like {}}, where for sure the parentheses arent balanced*/
            areCurvedBracketsBalancedGlobally = -99;
        }
        
        if (isCurvedBrackets() == 0) { /* if not the special case (curved brackets)*/
         
         isInOverrideMode = removeCommentsAndQuotes(isInOverrideMode); /* removes commments and quotes*/
         areParenthesesBalanced = checkParenthesesString();
         
         if (areParenthesesBalanced == 1) {
            printf("The parentheses in the line above are balanced. \n");
         } else {
            printf("The parentheses in the line above are NOT balanced. \n");
         }
         
        } else {
            printf("The parentheses in the line above are not balanced. (special case) \n");
        }
        
    }
    
    if (areCurvedBracketsBalancedGlobally != -99 && areCurvedBracketsClosed == 0) { /* if there werent cases like {}}, and balanced globally*/
    areCurvedBracketsBalancedGlobally = 1; 
    }
    parenthesesOutput(areParenthesesBalanced, areCurvedBracketsBalancedGlobally);
}

int removeCommentsAndQuotes(int isInOverrideMode) { /* recieves string. removes things inside quotation marks and comments*/
        int i, isInQuotationMarkOverrideMode =0;
        /* removes things inside quotation marks (and the qutation marks themselves)*/
        /*printf("%s",inputLine);*/
        for(i = 0; i<strlen(inputLine);i++) {
         if (isInQuotationMarkOverrideMode == 1) {
            if (inputLine[i] == '"') {
                 isInQuotationMarkOverrideMode = 0;
             }
             inputLine[i] = DeleteParenthesesInChar(inputLine[i]);;

         } else {
         if (inputLine[i] == '"') {
             isInQuotationMarkOverrideMode = 1;
             inputLine[i] = DeleteParenthesesInChar(inputLine[i]);;
            
            }
         } 
        }
        /* removes things inside comments (and the comments themselves)*/
        
        for(i = 0; i<strlen(inputLine);i++) {
         if (isInOverrideMode == 1) {
             if (inputLine[i] == '*' && inputLine[i+1] == '/' ) { /* ending of comment */
                inputLine[i] = DeleteParenthesesInChar(inputLine[i]);
                inputLine[i+1] = DeleteParenthesesInChar(inputLine[+1]);
                i++;
                isInOverrideMode = 0;
             } else { /* in the middle of a comment*/
                inputLine[i] = DeleteParenthesesInChar(inputLine[i]);
             }
             
         } else { /* not in Override mode*/
         if (inputLine[i+1] == '*' && inputLine[i] == '/' ) { /* beginning of a comment*/
             isInOverrideMode = 1;
             inputLine[i] = DeleteParenthesesInChar(inputLine[i]);
             inputLine[i+1] = DeleteParenthesesInChar(inputLine[i+1]);
             i++;
             isInOverrideMode =1;
            }
         } 
        }
            /*printf("%s",inputLine);*/
    return isInOverrideMode;
        
}
int checkParenthesesString() {
    int i;
    char current;
    int squarePara = 0, curvedPara = 0, normalPara = 0;
        for (i = 0; i<strlen(inputLine) + 1;i++) {
            current = inputLine[i];
            switch(current) 
            {
                case '(':
                    normalPara++;
                    break;
                case ')':
                    normalPara--;
                    if (normalPara < 0) return 0;
                    break;
                case '[':
                    squarePara++;
                    break;
                case ']':
                    squarePara--;
                    if (squarePara < 0) return 0;
                    break;
                case '{':
                    curvedPara++;
                    break;
                case '}':
                    curvedPara--;
                    if (curvedPara < 0) return 0;
                    break;

            }
        }
        if (normalPara == 0 && curvedPara == 0 && squarePara == 0) {
            return 1;
        } 
        return 0;
}


int isCurvedBrackets() { /* return 1 if the line is "{", -1 if "}" and 0 if none (not including spaces)*/
    int i, amount = 0, openBracket = 0, closingBracket = 0;
    for (i = 0; i<strlen(inputLine) + 1;i++) {/* iterate over the string*/

        if (!isspace(inputLine[i]) && inputLine[i] != 0) {
            amount++;
            if (inputLine[i] == '{' || inputLine[i] == '}') {
                if (inputLine[i] == '{') {openBracket = 1;}
                else {closingBracket = 1;}
            }
            
            else {return 0;} /* if found char not "/" or "\" */
        }
    }
    if (amount == 1 && openBracket == 1) {
        return 1;
    }
    if (amount == 1 && closingBracket == 1) {
        return -1;
    }
    return 0;
}

void parenthesesOutput(int areParenthesesBalanced, int areCurvedBracketsClosed) { /* handles the output aka printf*/
    if ((areParenthesesBalanced == 1) && (areCurvedBracketsClosed == 1)) { /* if the brackets are balanced*/
        printf("All the parenthesis in the input are balanced! \n");
    }
    else {
        printf("The parenthesis in the input are not balanced! :( \n" );
    }
}






































 









