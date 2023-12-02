// Name: Rachel Chan
// UID: 306280535

#include "grid.h"
#include <iostream>
#include <cctype>
#include <cassert>

using namespace std;

// method declarations
void plotHorizontalLine(int r, int c, int distance, char ch);
void plotVerticalLine(int r, int c, int distance, char ch);
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);

int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
bool entireStringHasValidSyntax(string commandString, int& badPos);
int findLastIndexOfCurrentCommand(string commandString, int startIndex);
bool commandHasValidSyntax(int startIndex, int endIndex, string commandString, int& badPos);

// global constants for plotLine() function
const int HORIZ = 0;
const int VERT = 1;
const int FG = 0;
const int BG = 1;

int main() {
}

//required function implementation
int performCommands(string commandString, char& plotChar, int& mode, int& badPos){
    // if plotChar is a non-printable character, return 2
    if(!isprint(plotChar)){
        return 2;
    }
    // if mode is not FG or BG, return 2
    if(mode != FG && mode != BG){
        return 2;
    }
    // if the commandString is empty, execute nothing successfully
    if(commandString == ""){
        return 0;
    }
    // check the syntax validity of the entire string; if not valid, then update badPos in the entireCommandStringIsValid() function and return 1
    if(!entireStringHasValidSyntax(commandString, badPos)){
        return 1;
    }
    // resets row and col number to 1
    int currentR = 1;
    int currentC = 1;
    // execute the command string COMMAND BY COMMAND, and use the startIndex and endIndex variable to keep track of the current command to perform
    int startIndex = 0;
    for( ; ; ){
        int endIndex = findLastIndexOfCurrentCommand(commandString, startIndex);
        // execute the appropiate commands based on the command letter
        if(commandString.at(startIndex) == 'H' || commandString.at(startIndex) == 'h'){
            int distance = stoi(commandString.substr(startIndex + 1, endIndex - startIndex + 1));
            bool success = plotLine(currentR, currentC, distance, HORIZ, plotChar, mode);
            // if the horizontal line is drawn successfully, update the column
            if(success){
                currentC += distance;
            }
            // if the horizontal line plots outside the grid, then set badPos to the start of the command and return 3
            else{
                if(currentC + distance > getCols() || currentC + distance < 1){
                    badPos = startIndex;
                    return 3;
                }
            }
        }
        else if(commandString.at(startIndex) == 'V' || commandString.at(startIndex) == 'v'){
            int distance = stoi(commandString.substr(startIndex + 1, endIndex - startIndex + 1));
            bool success = plotLine(currentR, currentC, distance, VERT, plotChar, mode);
            // if the vertical line is drawn successfully, update the row
            if(success){
                currentR += distance;
            }
            else{
                // if the vertical line plots outside the grad, then set badPos to the start of the command and return 3
                if(currentR + distance > getRows() || currentR + distance < 1){
                    badPos = startIndex;
                    return 3;
                }
            }
        }
        else if(commandString.at(startIndex) == 'F' || commandString.at(startIndex) == 'f'){
            mode = FG;
            plotChar = commandString.at(startIndex + 1);
        }
        else if(commandString.at(startIndex) == 'B' || commandString.at(startIndex) == 'b'){
            mode = BG;
            plotChar = commandString.at(startIndex + 1);
        }
        else if(commandString.at(startIndex) == 'C' || commandString.at(startIndex) == 'c'){
            clearGrid();
            currentR = 1;
            currentR = 1;
            plotChar = '*';
            mode = FG;
        }
        // where a plotting command is expecting to begin, a character is not a vlid command letter
        else{
            badPos = startIndex;
            return 1;
        }
        // update startIndex to move on to the next command in the string
        startIndex = endIndex + 1;
        // done executing all the commands, exit the loop
        if(endIndex == commandString.size() - 1){
            break;
        }
    }
    return 0;
}

bool entireStringHasValidSyntax (string commandString, int& badPos) {
    int startIndex = 0;
    // if the commandString is an empty string, it is considered syntactically valid
    if(commandString == ""){
        return true;
    }
    // check the syntax validity COMMAND BY COMMAND, using variables startIndex and endIndex to keep track of the current command
    for( ; ; ){
        int endIndex = findLastIndexOfCurrentCommand(commandString, startIndex);
        if(commandString.at(startIndex) == 'H' || commandString.at(startIndex) == 'h' || commandString.at(startIndex) == 'V' || commandString.at(startIndex) == 'v' || commandString.at(startIndex) == 'B' || commandString.at(startIndex) == 'b' || commandString.at(startIndex) == 'F' || commandString.at(startIndex) == 'f' || commandString.at(startIndex) == 'C' || commandString.at(startIndex) == 'c'){
            if(!commandHasValidSyntax(startIndex, endIndex, commandString, badPos))
                return false;
        }
        // startIndex is not a proper command letter
        else{
            badPos = startIndex;
            return false;
        }
        // iterate: now check the validity of the next command
        startIndex = endIndex + 1;
        // the loop has finished checking the validity for all commands, it's done! exit the loop
        if(endIndex == commandString.size() - 1){
            break;
        }
    }
    return true;
}

int findLastIndexOfCurrentCommand(string commandString, int startIndex){
    int endIndex = startIndex + 1;
    
    // if startIndex happens to be the last character
    if(endIndex >= commandString.size()){
        endIndex = static_cast<int> (commandString.size()) - 1;
        return endIndex;
    }
    
    // if the command is foreground/background and the character it is followed by is a plotting command, make sure NOT to treat it as a regular plotting command
    if(commandString.at(startIndex) == 'F' || commandString.at(startIndex) == 'f' || commandString.at(startIndex) == 'B' || commandString.at(startIndex) == 'b'){
        if(commandString.at(startIndex + 1) == 'H' || commandString.at(startIndex + 1) == 'h' || commandString.at(startIndex + 1) == 'V' || commandString.at(startIndex + 1) == 'v' || commandString.at(startIndex + 1) == 'F' || commandString.at(startIndex + 1) == 'f' || commandString.at(startIndex + 1) == 'B' || commandString.at(startIndex + 1) == 'b' || commandString.at(startIndex + 1) == 'C' || commandString.at(startIndex + 1) == 'c'){
            endIndex++;
        }
    }
    for( ; ; ){
        if(commandString.at(endIndex) == 'H' || commandString.at(endIndex) == 'h' || commandString.at(endIndex) == 'V' || commandString.at(endIndex) == 'v' || commandString.at(endIndex) == 'B' || commandString.at(endIndex) == 'b' || commandString.at(endIndex) == 'F' || commandString.at(endIndex) == 'f' || commandString.at(endIndex) == 'C' || commandString.at(endIndex) == 'c'){
            break;
        }
        if(endIndex == commandString.size() - 1){
            return endIndex;
        }
        endIndex++;
    }
    // decrement endIndex and return it because we want it to be the LAST index of the CURRENT command
    return --endIndex;
}

bool commandHasValidSyntax(int startIndex, int endIndex, string commandString, int& badPos){
    if(commandString.at(startIndex) == 'H' || commandString.at(startIndex) == 'h' || commandString.at(startIndex) == 'V' || commandString.at(startIndex) == 'v'){
        // if command letter is H/h/V/v and it is not followed by an argument --> error! return the position where it expects the argument
        if(endIndex == startIndex){
            badPos = startIndex + 1;
            return false;
        }
        // if the argument starts with '-', do not count this as a digit
        if(commandString.at(startIndex + 1) == '-'){
            // if the argument is only '-' --> error! return the position where it expects an integer
            if(endIndex == startIndex + 1){
                badPos = endIndex + 1;
                return false;
            }
            startIndex++;
        }
        for(int i = startIndex + 1; i <= endIndex; i++){
            // if the argument is more than 2 digits --> error! set badPos to the 3rd digit
            if(i == startIndex + 3){
                badPos = i;
                return false;
            }
            // if the argument is not composed entirely of digits --> error! set badPos to where it is not a digit
            if(!isdigit(commandString.at(i))){
                badPos = i;
                return false;
            }
        }
    }
    else if(commandString.at(startIndex) == 'B' || commandString.at(startIndex) == 'b' || commandString.at(startIndex) == 'F' || commandString.at(startIndex) == 'f'){
        // if B/b/F/f has no following arguments --> error! set badPos to where it expects the argument
        if(endIndex == startIndex){
            badPos = startIndex + 1;
            return false;
        }
        // if the argument is greater than one char --> error! set badPos to the position after the first char
        if(endIndex > startIndex + 1){
            badPos = startIndex + 2;
            return false;
        }
        // if the argument is a non-printable character, then set badPos to where the index of that character
        if(!isprint(commandString.at(startIndex + 1))){
            badPos = startIndex + 1;
            return false;
        }
    }
    // else, the command letter is C
    else{
        // there should only be ONE character in the clear command ('C' or 'c'), if not --> error! set badPos to the index after C/c
        if(!(endIndex == startIndex)){
            badPos = startIndex + 1;
            return false;
        }
    }
    return true;
}

bool plotHorizontalLine(int r, int c, int distance, char ch, int fgbg){
    if(r > getRows() || r < 1 || c > getCols() || c < 1)
        return false;
    if(distance == 0){
        if(fgbg == FG || (fgbg == BG && getChar(r, c) == ' '))
            setChar(r, c, ch);
    }
    else{
        // check if all the points will be in bounds by checking if the endpoint is in bounds
        if(c + distance > getCols() || c + distance < 1)
            return false;
        // all the points are in bounds, so plot
        else{
            for(int i = c; i <= c + distance; i++){
                // accounts for background plotting
                if(fgbg == FG || (fgbg == BG && getChar(r, i) == ' '))
                    setChar(r, i, ch);
            }
        }
    }
    return true;
}

bool plotVerticalLine(int r, int c, int distance, char ch, int fgbg){
    if(r > getRows() || r < 1 || c > getCols() || c < 1)
        return false;
    if(distance == 0){
        if(fgbg == FG || (fgbg == BG && getChar(r, c) == ' '))
            setChar(r, c, ch);
    }
    else{
        // check if all the points will be in bounds by checking if the endpoint will be in bounds
        if(r + distance > getRows() || r + distance < 1)
            return false;
        for(int i = r; i <= r + distance; i++){
            if(fgbg == FG || (fgbg == BG && getChar(i, c) == ' '))
                setChar(i, c, ch);
        }
    }
    return true;
}

//required function implementation
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg){
    // parameter checking: if invalid inputs are passed as the dir, fgbg, or the plotChar parameter, then return false
    if(dir != HORIZ && dir != VERT)
        return false;
    if(fgbg != FG && fgbg != BG)
        return false;
    if(!isprint(plotChar))
        return false;
    if(dir == HORIZ){
        if(distance > 0)
            return plotHorizontalLine(r, c, distance, plotChar, fgbg);
        else
            // if distance is negative, then begin plotting at the endpoint (not the start point r, c)
            return plotHorizontalLine(r, c + distance, -distance, plotChar, fgbg);
    }
    else{
        if(distance > 0)
            return plotVerticalLine(r, c, distance, plotChar, fgbg);
        else
            // if distance is negative, then begin plotting at the endpoint (not the start point r, c)
            return plotVerticalLine(r + distance, c, -distance, plotChar, fgbg);
    }
}





