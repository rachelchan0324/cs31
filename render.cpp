#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

using namespace std;

void printWord(char word[], int& count, bool& firstWordInLine, int lineLength, ostream& outf, bool& pBreak, bool &hasPunc, bool firstWord){
    // paragraph break
    if(strcmp(word, "@P@") == 0){
        if(!pBreak && !firstWord){
            outf << endl << endl;
            count = 0;
            firstWordInLine = true;
            pBreak = true;
        }
    }
    // if this is the first word in the current line, do not print a space before it
    else if(firstWordInLine && strlen(word) < lineLength){
        outf << word;
        firstWordInLine = false;
        pBreak = false;
    }
    else if(hasPunc && count + 2 < lineLength){
        outf << "  " << word;
        count += 2;
        pBreak = false;
    }
    // if the word can fit in the current line, print it
    else if(count + 1 < lineLength){
        outf << " " << word;
        count++;
        pBreak = false;
    }
    // if this is the first word in the current line AND it completely complets the line print, do not print a space before it and print a new line
    else if(firstWordInLine && strlen(word) == lineLength){
        outf << word << endl;
        count = 0;
        pBreak = false;
    }
    else if(hasPunc && count + 2 == lineLength){
        outf << "  " << word << endl;
        count = 0;
        firstWordInLine = true;
        pBreak = false;
    }
    // if the word perfectly completes the line, print a new line
    else if(count + 1 == lineLength){
        outf << " " << word << endl;
        count = 0;
        firstWordInLine = true;
        pBreak = false;
    }
    // the word will not fit in the current line, print a new line and print the word in the next line
    else{
        outf << endl << word;
        count = 0;
        count += strlen(word);
        pBreak = false;
    }
    // checks if the word has punctuation; if so, appends a space for before the next word
    if(word[strlen(word) - 1] == '.' || word[strlen(word) - 1] == '?' || word[strlen(word) - 1] == '!' || word[strlen(word) - 1] == ':'){
        hasPunc = true;
    }
    else{
        hasPunc = false;
    }
}

int render(int lineLength, istream& inf, ostream& outf){
    if(lineLength < 1)
        return 2;
    char c;
    char word[181] = "";
    int count = 0;
    bool firstWordInLine = true;
    bool firstWord = true;
    bool pBreak = false;
    bool hasPunc = false;
    while(inf.get(c)){ // read in one char at a time
        if(!isspace(c)){
            word[strlen(word)] = c; // append char c to the current word
            count++;
        }
        else{
            if(strlen(word) != 0){
                printWord(word, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord);
                // reset word
                for(int i = 0; word[i] != '\0'; i++){
                    word[i] = '\0';
                }
            }
            firstWord = false;
        }
    }
    if(strcmp(word, "@P@") != 0)
        printWord(word, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord); // print the last word (it is not printed in the loop because our loop only prints after it reaches a space character) **/
    outf << endl;
    return 0;
}

int main(){
    ifstream infile("/Users/rachel/Desktop/input.txt");
    if (!infile)
    {
        cerr << "Cannot open input.txt!" << endl;
    }
    cout << "Enter maximum line length: ";
    int len;
    cin >> len;
    cin.ignore(10000, '\n');
    int returnCode = render(len, infile, cout);
    cout << "Return code is " << returnCode << endl;
}
