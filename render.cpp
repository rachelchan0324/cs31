#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

using namespace std;

int render(int lineLength, istream& inf, ostream& outf){
    if(lineLength < 1)
        return 2;
    char c;
    char word[181];
    int count = 0;
    while(inf.get(c)){ // read in one char at a time
        if(!isspace(c)){
            word[strlen(word)] = c; // append char c to the current word
            count++;
        }
        else{ // if the program reaches a space character, print word and reset it to get ready for the next word
            if(strlen(word) != 0){
                if(count + 1 + strlen(word) <= lineLength){
                    outf << word << " ";
                    count++;
                }
                else{
                    if(count + strlen(word) <= lineLength){
                        outf << word << endl;
                        count = 0;
                    }
                    else{
                        outf << endl << word << " ";
                        count = 0;
                    }
                }
            }
            for(int i = 0; word[i] != '\0'; i++){
                word[i] = '\0';
            }
        }
    }
    outf << word << endl; // print the last word (it is not printed in the loop because our loop only prints after it reaches a space character)
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
