// Name: Rachel Chan
// UID: 306280535

#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

#include <cassert>
#include <streambuf>
#include <sstream>

using namespace std;

void printWordPortions(char word[], int& count, bool& firstWordInLine, int& lineLength, ostream& outf, bool& pBreak, bool& hasPunc, bool& firstWord, bool& printingWordPortions, bool& returnOne, bool& firstWordPortion, bool& justPerfectlyCompletedALine);

bool moreThanOneWordPortion(char word[]){
    for(int i = 0; word[i] != '\0'; i++){
        if(word[i] == '-')
            return true;
    }
    return false;
}

void printWord(char word[], int& count, bool& firstWordInLine, int& lineLength, ostream& outf, bool& pBreak, bool& hasPunc, bool& firstWord, bool& printingWordPortions, bool& returnOne, bool& firstWordPortion, bool& justPerfectlyCompletedALine){
    
    if(strcmp(word, "@P@") != 0)
        count += strlen(word);
    
    int prependSpaces = 1;
    if(printingWordPortions){
        if(!firstWordPortion)
            prependSpaces = 0;
    }
    if(hasPunc){
        prependSpaces = 2;
        hasPunc = false;
    }
    if(firstWordInLine)
        prependSpaces = 0;
    
    // prepend the paragraph break from the previous token
    if(pBreak && strcmp(word, "@P@") != 0){
        if(justPerfectlyCompletedALine){
            outf << endl;
        }
        else
            outf << endl << endl;
        firstWordInLine = true;
        pBreak = false;
    }

    if(strcmp(word, "@P@") == 0){
        if(firstWord){
            // do nothing
        }
        else{
            pBreak = true;
            count = 0;
            firstWordInLine = true;
        }
    }
    // can fit in the current line
    else if(count + prependSpaces < lineLength){
        for(int i = 0; i < prependSpaces; i++)
            outf << " ";
        outf << word;
        count += prependSpaces;
        firstWordInLine = false;
        justPerfectlyCompletedALine = false;
    }
    // perfectly completes the current line; start a new line
    else if(count + prependSpaces == lineLength){
        for(int i = 0; i < prependSpaces; i++)
            outf << " ";
        outf << word << endl;
        count = 0;
        justPerfectlyCompletedALine = true;
        firstWordInLine = true;
    }
    // the word will not fit in the current line; print a new line and print the word in the next line
    else{
        if(!printingWordPortions && moreThanOneWordPortion(word)){
            count -= strlen(word);
            printingWordPortions = true;
            firstWordPortion = true;
            printWordPortions(word, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord, printingWordPortions, returnOne, firstWordPortion, justPerfectlyCompletedALine);
        }
        else{
            if(strlen(word) > lineLength){
                returnOne = true;
                /** TO BE COMPLETED **/
            }
            outf << endl << word;
            count = int(strlen(word));
            firstWordInLine = false;
        }
        justPerfectlyCompletedALine = false;
    }
    if(firstWord && strcmp(word, "@P@") != 0)
        firstWord = false;
    // checks if the word has punctuation: if so, hasPunc turns true and will prepend 2 spaces in the next token
    if(word[strlen(word) - 1] == '.' || word[strlen(word) - 1] == '?' || word[strlen(word) - 1] == '!' || word[strlen(word) - 1] == ':')
        hasPunc = true;
}

void printWordPortions(char word[], int& count, bool& firstWordInLine, int& lineLength, ostream& outf, bool& pBreak, bool& hasPunc, bool& firstWord, bool& printingWordPortions, bool& returnOne, bool& firstWordPortion, bool& justPerfectlyCompletedALine){
    
    int portioncount = 0;
    char portion[180] = "";
    
    while(portioncount < strlen(word)){
        int i;
        for(i = 0; word[portioncount + i] != '\0'; i++){
            portion[strlen(portion)] = word[portioncount + i];
            if(portion[strlen(portion) - 1] == '-')
                break;
        }
        portioncount += i + 1;
        printWord(portion, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord, printingWordPortions, returnOne, firstWordPortion, justPerfectlyCompletedALine);
        firstWordPortion = false;
        // reset wordportion in preperation for the next wordportion
        for(int i = 0; portion[i] != '\0'; i++){
            portion[i] = '\0';
        }
    }
    printingWordPortions = false;
}

int render(int lineLength, istream& inf, ostream& outf){
    if(lineLength < 1)
        return 2;
    char c;
    char word[180] = "";
    
    int count = 0;
    bool firstWordInLine = true;
    // bool so @P@ doesn't print if it is the first token
    bool firstWord = true;
    bool pBreak = false;
    bool hasPunc = false;
    bool printingWordPortions = false;
    bool firstWordPortion = false;
    bool returnOne = false;
    bool justPerfectlyCompletedALine = false; // for pbreaks: if the word perfectly completed the preceding line, then only prine 1 new line (not 2)
    
    while(inf.get(c)){
        if(!isspace(c)){
            word[strlen(word)] = c;
        }
        else if(strlen(word) != 0){
            printWord(word, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord, printingWordPortions, returnOne, firstWordPortion, justPerfectlyCompletedALine);
            if(strcmp(word, "@P@") != 0){
                firstWord = false;
            }
            // reset word
            for(int i = 0; word[i] != '\0'; i++){
                word[i] = '\0';
            }
        }
    }
    if(strcmp(word, "@P@") != 0 && strlen(word) != 0)
        // print the last token
        printWord(word, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord, printingWordPortions, returnOne, firstWordPortion, justPerfectlyCompletedALine);
    if(!justPerfectlyCompletedALine)
        outf << endl;
    if(returnOne)
        return 1;
    return 0;
}

void testRender(int lineLength, const char input[], const char expectedOutput[], int expectedReturnValue)
{
    istringstream iss(input);
    ostringstream oss;
    ostringstream dummy;
    streambuf* origCout = cout.rdbuf(dummy.rdbuf());
    int retval = render(lineLength, iss, oss);
    cout.rdbuf(origCout);
    if ( ! dummy.str().empty())
        cerr << "WROTE TO COUT INSTEAD OF THIRD PARAMETER FOR: " << input << endl;
    else if (retval != expectedReturnValue)
        cerr << "WRONG RETURN VALUE FOR: " << input << endl;
    else if (retval == 2)
    {
        if ( ! oss.str().empty())
            cerr << "WROTE OUTPUT WHEN LINELENGTH IS " << lineLength << endl;
    }
    else if (oss.str() != expectedOutput)
        cerr << "WRONG RESULT FOR: " << input << endl;
}

int main(){
    ifstream infile("/Users/rachel/Desktop/input.txt");
    if (!infile)
        cerr << "Cannot open input.txt!" << endl;
    cout << "Enter maximum line length: ";
    int len;
    cin >> len;
    cin.ignore(10000, '\n');
    int returnCode = render(len, infile, cout);
    cout << "Return code is " << returnCode << endl;
}
