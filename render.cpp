#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

using namespace std;

void printWords(char word[], int& count, bool& firstWordInLine, int lineLength, ostream& outf){
    if(firstWordInLine && strlen(word) < lineLength){
        outf << word;
        firstWordInLine = false;
    }
    else if(count + 1 < lineLength){
        outf << " " << word;
        count++;
    }
    else if(firstWordInLine && strlen(word) == lineLength){
        outf << word << endl;
        count = 0;
    }
    // if the word perfectly completes the line, print a new line
    else if(count + 1 == lineLength){
        outf << " " << word << endl;
        count = 0;
        firstWordInLine = true;
    }
    // the word will not fit in the current line, print a new line and print the word in the next line
    else{
        outf << endl << word;
        count = 0;
        count += strlen(word);
    }
    // reset word
    for(int i = 0; word[i] != '\0'; i++){
        word[i] = '\0';
    }
}

int render(int lineLength, istream& inf, ostream& outf){
    if(lineLength < 1)
        return 2;
    char c;
    char word[181];
    int count = 0;
    bool firstWordInLine = true;
    while(inf.get(c)){ // read in one char at a time
        if(!isspace(c)){
            word[strlen(word)] = c; // append char c to the current word
            count++;
        }
        else{
            if(strlen(word) != 0)
                printWords(word, count, firstWordInLine, lineLength, outf);
        }
    }
    printWords(word, count, firstWordInLine, lineLength, outf); // print the last word (it is not printed in the loop because our loop only prints after it reaches a space character)
    outf << endl;
    return 0;
}

int main(){
    const int MAX_FILENAME_LENGTH = 100;
    for (;;)
    {
        cout << "Enter input file name (or q to quit): ";
        char filepath [MAX_FILENAME_LENGTH + 22];
        strcat(filepath, "/Users/rachel/Desktop/");
        char filename [MAX_FILENAME_LENGTH];
        cin.getline(filename, MAX_FILENAME_LENGTH);
        if (strcmp(filename, "q") == 0)
            break;
        strcat(filepath, filename);
        ifstream infile(filepath);
        if (!infile)
        {
            cerr << "Cannot open " << filename << "!" << endl;
            continue;
        }
        cout << "Enter maximum line length: ";
        int len;
        cin >> len;
        cin.ignore(10000, '\n');
        int returnCode = render(len, infile, cout);
        cout << "Return code is " << returnCode << endl;
    }
}
