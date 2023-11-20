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
    // determines if a word has more than one word portion by checking if any of the chars are a '-'
    for(int i = 0; word[i] != '\0'; i++){
        if(word[i] == '-')
            return true;
    }
    return false;
}

void printWord(char word[], int& count, bool& firstWordInLine, int& lineLength, ostream& outf, bool& pBreak, bool& hasPunc, bool& firstWord, bool& printingWordPortions, bool& returnOne, bool& firstWordPortion, bool& justPerfectlyCompletedALine){
    
    // @P@ does not have length and is not considered a word UNLESS it is a word-portion
    if(strcmp(word, "@P@") != 0 || printingWordPortions)
        count += strlen(word);
    
    // determines how many spaces we need to prepend before printing our current word
    int prependSpaces = 1;
    if(printingWordPortions){ // word portions shouldn't have spaces before them EXCEPT if its the first word portion
        if(!firstWordPortion)
            prependSpaces = 0;
        else
            prependSpaces = 1;
    } // if our previous word has punctuation, we prepend 2 spaces
    if(hasPunc){
        prependSpaces = 2;
    }
    if(firstWordInLine)
        prependSpaces = 0;
    
    // prepend the paragraph break from the previous token UNLESS the current word is also a paragraph break (we ignore consecutive paragraph breaks and only print the LAST paragraph break)
    if(pBreak && strcmp(word, "@P@") != 0){
        // if we just completed a line, then one of the new lines is already printed
        if(justPerfectlyCompletedALine)
            outf << endl;
        else
            outf << endl << endl;
        firstWordInLine = true;
        pBreak = false;
    }

    if(!printingWordPortions && strcmp(word, "@P@") == 0){
        if(firstWord){
            // a paragraph break cannot be the first thing printed
        }
        else{
            // marks pBreak as true, so a new line will be prepended to the next token
            firstWordInLine = true;
            pBreak = true;
            count = 0;
        }
    }
    // the current word and the prepended spaces CAN fit in the current line
    else if(count + prependSpaces < lineLength){
        for(int i = 0; i < prependSpaces; i++)
            outf << " ";
        outf << word;
        count += prependSpaces;
        firstWordInLine = false;
        justPerfectlyCompletedALine = false;
    }
    // the current word and the prepended spaces PERFECTLY completes the current line: start a new line
    else if(count + prependSpaces == lineLength){
        for(int i = 0; i < prependSpaces; i++)
            outf << " ";
        outf << word << endl;
        count = 0;
        justPerfectlyCompletedALine = true;
        firstWordInLine = true;
    }
    // the current word and the prepended spaces will NOT fit in the current line
    else{
        // splits the word into different word portions if the word contains a hyphen
        if(moreThanOneWordPortion(word) && !printingWordPortions){
            count -= strlen(word); // we never printed out word (in its entirety), so subtract its length from the count
            printingWordPortions = true;
            firstWordPortion = true;
            printWordPortions(word, count, firstWordInLine, lineLength, outf, pBreak, hasPunc,firstWord, printingWordPortions, returnOne, firstWordPortion, justPerfectlyCompletedALine);
        }
        // the word portion is greater than the maxLength: set returnOne to true and print as much as will fit on a new line
        else if(strlen(word) > lineLength){
            returnOne = true;
            if(!firstWordInLine)
                outf << endl;
            int charsToPrint = lineLength;
            for(int i = 0; i < charsToPrint; i++)
                outf << word[i]; // print as much as will fit on a new line
            outf << endl;
            firstWordInLine = true;
            count = 0;
            char overflow[180] = "";
            // create a new cstring containing the remaining characters and pass it back into printWord()
            for(int i = charsToPrint; i < strlen(word); i++)
                overflow[strlen(overflow)] = word[i];
            printWord(overflow, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord, printingWordPortions, returnOne, firstWordPortion, justPerfectlyCompletedALine);
        }
        else{ // current word doesn't fit on current line --> print a new line and print the word in the next line
            outf << endl << word;
            count = int(strlen(word));
            firstWordInLine = false;
            justPerfectlyCompletedALine = false;
        }
    }
    hasPunc = false;
    // just finished printing a word, so firstWord is false UNLESS firstWord is @P@ (in which case the paragraph break is ignored)
    if(firstWord && strcmp(word, "@P@") != 0)
        firstWord = false;
    // checks if the word has punctuation: if so, hasPunc turns true and will prepend 2 spaces in the next token
    if(word[strlen(word) - 1] == '.' || word[strlen(word) - 1] == '?' || word[strlen(word) - 1] == '!' || word[strlen(word) - 1] == ':')
        hasPunc = true;
}

void printWordPortions(char word[], int& count, bool& firstWordInLine, int& lineLength, ostream& outf, bool& pBreak, bool& hasPunc, bool& firstWord, bool& printingWordPortions, bool& returnOne, bool& firstWordPortion, bool& justPerfectlyCompletedALine){
    
    int wordcount = 0; // stores the number of characters we have already printed from the word
    char portion[180] = "";
    
    while(wordcount < strlen(word)){
        int i;
        // identifies the next word portion by starting at index wordcount and appending chars from word until the for-loop reaches a '-' (inclusive) or '\0'
        for(i = 0; word[wordcount + i] != '\0'; i++){
            portion[strlen(portion)] = word[wordcount + i];
            if(portion[strlen(portion) - 1] == '-')
                break;
        }
        wordcount += i + 1;
        printWord(portion, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord, printingWordPortions, returnOne, firstWordPortion, justPerfectlyCompletedALine);
        if(firstWordPortion)
            firstWordPortion = false; // just printed a word portion in the previous line, so firstWordPortion becomes false
        // reset wordportion in preperation for the next wordportion
        for(int i = 0; portion[i] != '\0'; i++){
            portion[i] = '\0';
        }
    }
    printingWordPortions = false; // finish printing all word portions so printingWordPortions becomes false
}

int render(int lineLength, istream& inf, ostream& outf){
    // if line length is less than 1: error --> return 1
    if(lineLength < 1)
        return 2;
    char c;
    char word[180] = ""; // stores the current word we are printing
    int count = 0; // stores the number of characters already printed in the current line
    
    // boolean values that represent unique edge cases for our algorithm
    bool firstWordInLine = true;
    bool firstWord = true;
    bool pBreak = false;
    bool hasPunc = false;
    bool printingWordPortions = false;
    bool firstWordPortion = false;
    bool justPerfectlyCompletedALine = false; // used for printing pbreaks; if the word perfectly completed the preceding line, then only print 1 new line (not 2)
    bool returnOne = false;
    
    // reads the text file char by char
    while(inf.get(c)){
        if(!isspace(c)){
            word[strlen(word)] = c; // append the chars to word[] until the program hits a space
        }
        else if(strlen(word) != 0){
            // the program hits a space; now, we have stored all the chars to word[] and it is time to print
            printWord(word, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord, printingWordPortions, returnOne, firstWordPortion, justPerfectlyCompletedALine);
            // reset word
            for(int i = 0; word[i] != '\0'; i++){
                word[i] = '\0';
            }
        }
    }
    // print the last token (our last token is not printed in our while loop because it only prints the word AFTER hitting a space but our input doesn't necessarily have to end with a space) UNLESS the token is a space or paragraph break
    if(strcmp(word, "@P@") != 0 && strlen(word) != 0)
        printWord(word, count, firstWordInLine, lineLength, outf, pBreak, hasPunc, firstWord, printingWordPortions, returnOne, firstWordPortion, justPerfectlyCompletedALine);
    
    // print a new line at the end of the output UNLESS justPerfectlyCompeltedALine (already printed a newline at the end) or firstWord is true (no output was printed at all our output file should have nothing)
    if(!justPerfectlyCompletedALine && !firstWord)
        outf << endl;
    if(returnOne)
        return 1;
    return 0;
}

bool testRender(int lineLength, const char input[], const char expectedOutput[], int expectedReturnValue) {
    istringstream iss(input);
    ostringstream oss;
    int retval = render(lineLength, iss, oss);
    if (retval != expectedReturnValue) {
        cout << "WRONG RETURN VALUE FOR:" << endl << input << endl;
        return false;
    } else if (retval == 2) {
        if (!oss.str().empty()) {
            cout << "WROTE OUTPUT WHEN LINELENGTH IS " << lineLength << endl;
            return false;
        }
    } else if (oss.str() != expectedOutput) {
        cout << "-------------------" << endl;
        cout << "WRONG RESULT FOR: " << endl << input << endl;
        cout << "----------received:" << endl;
        cout << oss.str() << endl;
        cout << "----------expected:" << endl;
        cout << expectedOutput << endl;
        cout << "-------------------" << endl;
        return false;
    }
    return true;
}

int main(){
    // Smallberg's Test Cases
    testRender(7, "This\n\t\tis a\ntest\n", "This is\na test\n", 0);
    testRender(8, "  This is a test  \n", "This is\na test\n", 0);
    testRender(6, "Testing it\n", "Testin\ng it\n", 1);
    testRender(-5, "irrelevant", "irrelevant", 2);
    testRender(40, "It always does seem to me that I am doing more work than\nI should do. It is not that I object to the work, mind you;\nI like work: it fascinates me.       I can sit and look at it for hours.\nI love to keep     it by me: the idea of getting\nrid\nof it nearly breaks my heart. @P@ You cannot give me too\nmuch work; to accumulate work has almost become\na passion with me: my study is so full of it now, that there is hardly\nan inch of room for any more.", "It always does seem to me that I am\ndoing more work than I should do.  It is\nnot that I object to the work, mind you;\nI like work:  it fascinates me.  I can\nsit and look at it for hours.  I love to\nkeep it by me:  the idea of getting rid\nof it nearly breaks my heart.\n\nYou cannot give me too much work; to\naccumulate work has almost become a\npassion with me:  my study is so full of\nit now, that there is hardly an inch of\nroom for any more.\n", 0);
    
    // Andrew's Test Cases
    assert(testRender(0, "Hello", "", 2)); // max length can't be 0
    assert(testRender(100, " Hello!", "Hello!\n", 0)); // space at beginning, remove it
    assert(testRender(100, "\nHello!", "Hello!\n", 0)); // newline at beginning, remove it
    assert(testRender(100, "  \n\n\nHello!", "Hello!\n", 0)); // space + newlines at beginning, remove them
    assert(testRender(100, "Hello! ", "Hello!\n", 0)); // space at end, remove it
    assert(testRender(100, "Hello! \n\n  ", "Hello!\n", 0)); // spaces + newlines at end, remove them
    assert(testRender(8, "Hello world", "Hello\nworld\n", 0)); // break at space
    assert(testRender(8, "Hello   world", "Hello\nworld\n", 0)); // consecutive spaces are ignored
    assert(testRender(100, "Henley-on-Thames", "Henley-on-Thames\n", 0)); // normal behavior where it fits
    assert(testRender(12, "Henley-on-Thames", "Henley-on-\nThames\n", 0)); // broken up after a hyphen
    assert(testRender(5, "1234-1234-1234", "1234-\n1234-\n1234\n", 0)); // multiple hyphen breaks
    assert(testRender(4, "1234-1234-1234", "1234\n-\n1234\n-\n1234\n", 1)); // hyphen couldn't fit and is on its own line
    assert(testRender(2, "1234-1234-1234", "12\n34\n-\n12\n34\n-\n12\n34\n", 1)); // both word and hyphen are split
    assert(testRender(1, "1234-1234-1234", "1\n2\n3\n4\n-\n1\n2\n3\n4\n-\n1\n2\n3\n4\n", 1)); // one single vertical line
    assert(testRender(100, "Hello- world", "Hello- world\n", 0)); // space after hyphen
    assert(testRender(100, "Hello-   world", "Hello- world\n", 0)); // multiple spaces after hyphen
    assert(testRender(100, "Hello! My name is Andrew.", "Hello!  My name is Andrew.\n", 0)); // 2 spaces after !
    assert(testRender(100, "Hello?!! World", "Hello?!!  World\n", 0)); // consecutive punctuation
    assert(testRender(100, "What time is it? Time to get a watch.", "What time is it?  Time to get a watch.\n", 0)); // test question mark
    assert(testRender(100, "A wise man once said: I love CS 31.", "A wise man once said:  I love CS 31.\n", 0)); // test colon
    assert(testRender(100, "Hi... how are you doing?", "Hi...  how are you doing?\n", 0)); // 2 spaces after end of ellipsis
    assert(testRender(100, "!-!-!-!-!", "!-!-!-!-!\n", 0)); // punctuation with hyphen
    assert(testRender(5, "!-!-!-!-!", "!-!-\n!-!-!\n", 0)); // break in middle
    assert(testRender(16, "What time is it? Time to get a watch.", "What time is it?\nTime to get a\nwatch.\n", 0)); // no space after question mark when breaking
    assert(testRender(6, "Hello world!", "Hello\nworld!\n", 0)); // remove the extra space from the exclamation mark
    assert(testRender(5, "Hello, world!", "Hello\n,\nworld\n!\n", 1)); // comma and exclamation mark on separate line
    assert(testRender(3, "abcdef", "abc\ndef\n", 1)); // split across 2 lines, returning 1
    assert(testRender(3, "abcdefg", "abc\ndef\ng\n", 1)); // single character at the end
    assert(testRender(10, "A long word is supercalifragilisticexpialidocious", "A long\nword is\nsupercalif\nragilistic\nexpialidoc\nious\n", 1)); // only last word is too long
    assert(testRender(100, "Hello @P@ World", "Hello\n\nWorld\n", 0)); // paragraph break in middle
    assert(testRender(100, "Hello @P@ @P@ World", "Hello\n\nWorld\n", 0)); // duplicate paragraph breaks have no effect
    assert(testRender(100, "@P@ Hello @P@ World", "Hello\n\nWorld\n", 0)); // paragraph break at start is ignored
    assert(testRender(100, "Hello @P@ World @P@", "Hello\n\nWorld\n", 0)); // paragraph break at end is ignored
    assert(testRender(100, "Hello @P@ World @P@ ", "Hello\n\nWorld\n", 0)); // paragraph break at end and extra space are both ignored
    assert(testRender(100, "banjo@P@", "banjo@P@\n", 0)); // @P@ is part of the previous word
    assert(testRender(100, "banjo@P@ banjo@P@", "banjo@P@ banjo@P@\n", 0)); // space after the @P@ isn't enough, need space before too
    assert(testRender(100, "Hello @P@world", "Hello @P@world\n", 0)); // space before the @P@ isn't enough, need space after too
    assert(testRender(100, "Hello @P@@P@ World", "Hello @P@@P@ World\n", 0)); // consecutive @P@'s without space just renders it as-is
    assert(testRender(100, "@P@-@P@", "@P@-@P@\n", 0)); // should print out as a single word
    assert(testRender(100, "@P@ @P@- -@P@ @P@", "@P@- -@P@\n", 0)); // ignore leading and ending paragraph break
    assert(testRender(100, "@P@ @P@- -@P@World! @P@- !-@P@ Hello-@P@-World.", "@P@- -@P@World!  @P@- !-@P@ Hello-@P@-World.\n", 0)); // lots of weird combinations
    assert(testRender(20, "@P@ @P@- -@P@World! @P@- !-@P@ Hello-@P@-World.", "@P@- -@P@World!\n@P@- !-@P@ Hello-\n@P@-World.\n", 0)); // split on the hyphen after "Hello"
    assert(testRender(10, "@P@ @P@- -@P@World! @P@- !-@P@ Hello-@P@-World.", "@P@- -\n@P@World!\n@P@- !-@P@\nHello-@P@-\nWorld.\n", 0)); // more splits
    assert(testRender(1, "@P@ @P@- -@P@World! @P@- !-@P@ Hello-@P@-World.", "@\nP\n@\n-\n-\n@\nP\n@\nW\no\nr\nl\nd\n!\n@\nP\n@\n-\n!\n-\n@\nP\n@\nH\ne\nl\nl\no\n-\n@\nP\n@\n-\nW\no\nr\nl\nd\n.\n", 1)); // single vertical line of output
    assert(testRender(500, "The input word @P@ is not to be processed as a word according to the above rules; instead, it indicates a paragraph break. The first word in the input following a paragraph break will be the first word of a new paragraph in the output. If a paragraph has already been output, the new paragraph must be separated from the one that precedes it by an empty line (i.e., a line with no characters other than the terminating newline). The very first output paragraph must not be preceded by an empty line. The very last output paragraph must not be followed by an empty line. Two or more consecutive @P@ words in the input are treated as just one paragraph break. Notice that banjo@P@ is one eight-character word; it does not cause a paragraph break, because in that string, @P@ is not a word because of the immediately preceding non-whitespace character o.", "The input word\n\nis not to be processed as a word according to the above rules; instead, it indicates a paragraph break.  The first word in the input following a paragraph break will be the first word of a new paragraph in the output.  If a paragraph has already been output, the new paragraph must be separated from the one that precedes it by an empty line (i.e., a line with no characters other than the terminating newline).  The very first output paragraph must not be preceded by an empty line.  The very last\noutput paragraph must not be followed by an empty line.  Two or more consecutive\n\nwords in the input are treated as just one paragraph break.  Notice that banjo@P@ is one eight-character word; it does not cause a paragraph break, because in that string,\n\nis not a word because of the immediately preceding non-whitespace character o.\n", 0)); // first parameter greater than 250
    assert(testRender(25, "asdlfdasoifjaspodfj-\n1239ij9 fsdjna9fusd-\n14890r413\n9fjasdjioasdcj9asdf-dfa-\naqwe-qwe-=q=q-q-=qfnjvc\nvf @P@-\ndfiosj90vji0af0jaw @P@@P@\nafsdokfja-@P@\n\nfd9as0ad0-s-09\n", "asdlfdasoifjaspodfj-\n1239ij9 fsdjna9fusd-\n14890r413\n9fjasdjioasdcj9asdf-dfa-\naqwe-qwe-=q=q-q-=qfnjvc\nvf @P@-\ndfiosj90vji0af0jaw @P@@P@\nafsdokfja-@P@ fd9as0ad0-\ns-09\n", 0)); // lots of combinations
    assert(testRender(40, "It always does seem to me that I am doing more work than\nI should do. It is not that I object to the work, mind you;\nI like work: it fascinates me.   I can sit and look at it for hours.\nI love to keep it by me: the idea of getting\nrid\nof it nearly breaks my heart. @P@ You cannot give me too\nmuch work; to accumulate work has almost become\n\n\na passion with me: my study is so full of it now, that there is hardly\nan inch of room for any more.\n\n", "It always does seem to me that I am\ndoing more work than I should do.  It is\nnot that I object to the work, mind you;\nI like work:  it fascinates me.  I can\nsit and look at it for hours.  I love to\nkeep it by me:  the idea of getting rid\nof it nearly breaks my heart.\n\nYou cannot give me too much work; to\naccumulate work has almost become a\npassion with me:  my study is so full of\nit now, that there is hardly an inch of\nroom for any more.\n", 0));
    assert(testRender(30, "This text was rendered with max line length 30. @P@ This is paragraph 2!", "This text was rendered with\nmax line length 30.\n\nThis is paragraph 2!\n", 0));
    assert(testRender(7, "This\n\t\tis a\ntest\n", "This is\na test\n", 0));
    assert(testRender(8, "  This is a test  \n", "This is\na test\n", 0));
    assert(testRender(6, "Testing it\n", "Testin\ng it\n", 1));
    assert(testRender(-5, "irrelevant", "irrelevant", 2));
    
    // Rachel's Test Case
    testRender(10, "\n", "", 0); // input is empty line
    testRender(10, " ", "", 0); // input is empty space
    // Error Cases
    testRender(3, "one two three", "one\ntwo\nthr\nee\n", 1); // input word longer than max length
    testRender(6, "hello-there and good-bye", "hello-\nthere\nand\ngood-\nbye\n", 0); // input word longer than max length but individual word portions are not
    testRender(4, "hello-you\n\ngoodbye-you", "hell\no-\nyou\ngood\nbye-\nyou\n", 1); // word portions are longer than max length
    testRender(0, "boo!", "", 2); // desired maximum length is < 1
    // Punctuation
    testRender(10, "!", "!\n", 0); // input is a singular punctuation mark
    testRender(6, ".?!:", ".?!:\n", 0); // ends and starts with a punctuation mark (should not have spaces at beginning or the end)
    testRender(6, ". ? ! : ", ".  ?\n!  :\n", 0); // testing all punctuation marks specified in the spec
    // Paragraph Breaks
    testRender(10, "@P@", "", 0); // input is a paragraph break
    testRender(12, ".banjo@P@.", ".banjo@P@.\n", 0); // paragraph break is incorporated in a word (does not count!)
    testRender(6, "@P@ Hello @P@ Bye @P@", "Hello\n\nBye\n", 0); // leading and ending paragraph breaks
    testRender(6, "@P@@P@", "@P@@P@\n", 0); // @P@@P@ should be treated as a word, not a paragraph break
    testRender(4, "@P@ @P@ @P@ @P@", "", 0); // contains only repeated paragraph breaks
    testRender(2, "hi @P@ hi", "hi\n\nhi\n", 0); // @P@ should have no length
    testRender(3, "@P@-@P@", "@P@\n-\n@P@\n", 1); // @P@ should be treated as a word portion, not a word
    testRender(10, "hi\n\n@P@ bye", "hi\n\nbye\n", 0); // paragraph break with new lines
    // Hyphen
    testRender(10, "-", "-\n", 0); // input is a hyphen
    testRender(10, "hi!--!-december", "hi!--!-\ndecember\n", 0); // punctuation included inside word portion
    testRender(2, "---------", "--\n--\n--\n--\n-\n", 0); // input has consecutive hyphens
    testRender(3, "-a-a-a-a-a", "-a-\na-\na-\na-a\n", 0); // input is a large word portion with multiple hyphens
    testRender(10, "Thames so-called Henley-on-Thames so--called so- -so", "Thames so-\ncalled\nHenley-on-\nThames so-\n-called\nso- -so\n", 0); //  includes all pathological examples of word portions from smallberg's spec
    testRender(20, "Here's a wild mix of dashes-connecting ideas in a jumble-making a merry mess-of words and thoughts-like a crazy puzzle-full of dashes-jumping around-just for fun!", "Here's a wild mix of\ndashes-connecting\nideas in a jumble-\nmaking a merry mess-\nof words and\nthoughts-like a\ncrazy puzzle-full of\ndashes-jumping\naround-just for fun!\n", 0); // multiple word portions broken at a hyphen to fit on an output line
    testRender(6, "@P@ @P@ @P@\n\neric-chou\nhello\n@P@\nalsdjf-j--j\nsevens-fives\nhi fives\n\n\n@P@\nfives-bye\n\n\n", "eric-\nchou\nhello\n\nalsdjf\n-j--j\nsevens\n-fives\nhi\nfives\n\nfives-\nbye\n", 1); // multiple split word portions AND paragraph breaks
    // Exceeds Maximum Line Length
    testRender(5, "i'm rach-chan", "i'm\nrach-\nchan\n", 0); // overflow word with hyphen
    testRender(6, "i'm rachchan", "i'm\nrachch\nan\n", 1); // overflow word without hyphen
    testRender(6, "oopsiesdaises! my-bad", "oopsie\nsdaise\ns!\nmy-bad\n", 1); // overflow word includes punctuation
    testRender(6, "crashlandingonyouisaveryverygoodshow", "crashl\nanding\nonyoui\nsavery\nverygo\nodshow\n", 1); // input line length is a multiple of the max line length
    testRender(7, "crashlandingonyouisaveryverygoodshow", "crashla\nndingon\nyouisav\neryvery\ngoodsho\nw\n", 1); // input line length is not a multiple of max line length
    testRender(20, "@P@ @P@ @P@ @P@- -@P@World! -@P@ !-@P@ Hello-@P@-World.", "@P@- -@P@World!  -\n@P@ !-@P@ Hello-@P@-\nWorld.\n", 0); // overflow word has a hyphen and can be split into word portions
    testRender(7, "zen-me-le- - ni-hao-bu-hao", "zen-me-\nle- -\nni-hao-\nbu-hao\n", 0); // word length exceeds maximum line length but word portion lengths do not (should split the word portions and NOT return 1)
    // More Complex Test Cases
    testRender(1, "h-i! @P@ ya-y!. @P@", "h\n-\ni\n!\n\ny\na\n-\ny\n!\n.\n", 1); // line length is 1
    testRender(10, "!: . @P@ laj-jal3-ncl-  \neric-chou . . !@P@\n\n@P@  \n\n. -\n\n-- -.-?- \nhello-my-name-is-rachel-this-project-is-hard\n\n@P@ @P@ @P@\n\n", "!:  .\n\nlaj-jal3-\nncl- eric-\nchou .  .\n!@P@\n\n.  - -- -\n.-?-\nhello-my-\nname-is-\nrachel-\nthis-\nproject-\nis-hard\n", 0); // combination of punctuation, paragraph breaks, and hyphens
    testRender(4, "forrestgump once-said life-is likea boxofchocolates", "forr\nestg\nump\nonce\n-\nsaid\nlife\n-is\nlike\na\nboxo\nfcho\ncola\ntes\n", 1); // combination of overflow words AND hyphens
    testRender(20, "@P@ Once-upon a-time. . . -in-a- \n faraway? land: @P@ there--lived-a- curious-fox @P@ \n The-real--treasure- is- the adventures! you- embrace.", "Once-upon a-time.  .\n.  -in-a- faraway?\nland:\n\nthere--lived-a-\ncurious-fox\n\nThe-real--treasure-\nis- the adventures!\nyou- embrace.\n", 0); // combination of punctuation, paragraph breaks, hyphens, and new lines
    testRender(37, "On-ce, a cu-ri-ous fox roa-med fa-ra-wa-y lands @P@ see-king ad-ven-tures. Am-id-st fo-re-sts and va-lle-ys, he st-umb-led up-on an an-ci-ent map @P@ ex-ci-te-ment sur-g-ed wit-hin. Off he das-h-ed, see-king hid-de-n trea-su-res mar-k-ed on the map @P@ thr-ou-gh tho-rny bu-sh-es and wi-nd-ing ri-ve-rs. His jou-rney led to a gli-mme-ring chest @P@ ea-ge-rly o-pe-ning it, he found not go-ld or ge-ms, but a no-te that re-ad, The re-al trea-su-re is the ad-ven-tu-res you e-mbra-ce. @P@ With a chu-ck-le, he re-al-iz-ed the tru-e va-lu-e of his jou-rney and co-nti-nu-ed ex-plor-ing the wo-rld with gle-e.", "On-ce, a cu-ri-ous fox roa-med fa-ra-\nwa-y lands\n\nsee-king ad-ven-tures.  Am-id-st fo-\nre-sts and va-lle-ys, he st-umb-led\nup-on an an-ci-ent map\n\nex-ci-te-ment sur-g-ed wit-hin.  Off\nhe das-h-ed, see-king hid-de-n trea-\nsu-res mar-k-ed on the map\n\nthr-ou-gh tho-rny bu-sh-es and wi-nd-\ning ri-ve-rs.  His jou-rney led to a\ngli-mme-ring chest\n\nea-ge-rly o-pe-ning it, he found not\ngo-ld or ge-ms, but a no-te that re-\nad, The re-al trea-su-re is the ad-\nven-tu-res you e-mbra-ce.\n\nWith a chu-ck-le, he re-al-iz-ed the\ntru-e va-lu-e of his jou-rney and co-\nnti-nu-ed ex-plor-ing the wo-rld with\ngle-e.\n", 0); // extra credit: long input line with over 250 characters
    
    // Emily's Test Cases
    testRender(10, "hello this is emily @P@ @P@ @P@ hehe", "hello this\nis emily\n\nhehe\n", 0);
    testRender(10, "hello hellolllloooollllooo hello", "hello\nhellollllo\nooollllooo\nhello\n", 1);
    testRender(10, "hello @P@this is emily @P@ @P@ @P@ hehe", "hello\n@P@this is\nemily\n\nhehe\n", 0);
    testRender(10, "hello-@P@ sdhjkghjkfg h3-jkhr-sfdghjk-dfg", "hello-@P@\nsdhjkghjkf\ng h3-jkhr-\nsfdghjk-\ndfg\n", 1);
    testRender(10, "sdgbj,gbjsd-jksdhfhjksdf    hjksd---jkhsdfhjkef bjmfghjk", "sdgbj,gbjs\nd-\njksdhfhjks\ndf hjksd--\n-\njkhsdfhjke\nf bjmfghjk\n", 1);
    testRender(10, "sdjkfh sdfhjksdfjkhsdghjksdghjk   \n  shauhaf-----sdkghj-jksdhf82jk--jdnfsdhjksdghjk ksdhjdduu @P@\nsdjkghsdghjsdkhjfhjksdhjsdjssdfhjsfhjsdfjsdfjsdjksdfjhjksdfsdfhjksdfjsdfjjksdfsdfhjk", "sdjkfh\nsdfhjksdfj\nkhsdghjksd\nghjk\nshauhaf---\n--sdkghj-\njksdhf82jk\n--\njdnfsdhjks\ndghjk\nksdhjdduu\n\nsdjkghsdgh\njsdkhjfhjk\nsdhjsdjssd\nfhjsfhjsdf\njsdfjsdjks\ndfjhjksdfs\ndfhjksdfjs\ndfjjksdfsd\nfhjk\n", 1);
    testRender(10, "fdhjksf238 iuhfsdu--- - - - sjkdhg- - disd -dfjkgh -sdjkfh", "fdhjksf238\niuhfsdu---\n- - -\nsjkdhg- -\ndisd -\ndfjkgh -\nsdjkfh\n", 0);
    testRender(10, "-dhjksf238", "-dhjksf238\n", 0);
    testRender(-1, "hi", "hi", 2);
    testRender(10, "sjfgshjgfhdgfshdgf", "sjfgshjgfh\ndgfshdgf\n", 1);
    testRender(10, "-sjfgshjgfhdgfshdgf", "-\nsjfgshjgfh\ndgfshdgf\n", 1);
    testRender(10, "o. he ! h; e!  ", "o.  he !\nh; e!\n", 0);
    
    // Brandon's Test Cases
    assert(testRender(0, "Hello", "", 2)); //max length can't be 0
    assert(testRender(-10, "Hello", "", 2)); //max length can't be negative
    assert(testRender(5, "Hello", "Hello\n", 0));
    assert(testRender(15, " asdf", "asdf\n", 0)); //leading space
    assert(testRender(15, "asdf ", "asdf\n", 0)); //trailing space
    assert(testRender(15, "\nasdf", "asdf\n", 0)); //leading line break
    assert(testRender(15, "asdf\n", "asdf\n", 0)); //trailing line break
    assert(testRender(15, "     \n\n\n\n\n\nHello", "Hello\n", 0)); //multiple leading line breaks
    assert(testRender(15, "Hello\n\n\n\n\n\n     ", "Hello\n", 0)); //multiple trailing line breaks
    assert(testRender(40, "It always does seem to me that I am doing more work than\nI should do. It is not that I object to the work, mind you;\nI like work: it fascinates me.       I can sit and look at it for hours.\nI love to keep     it by me: the idea of getting\nrid\nof it nearly breaks my heart. @P@ You cannot give me too\nmuch work; to accumulate work has almost become\n\n\na passion with me: my study is so full of it now, that there is hardly\nan inch of room for any more.", "It always does seem to me that I am\ndoing more work than I should do.  It is\nnot that I object to the work, mind you;\nI like work:  it fascinates me.  I can\nsit and look at it for hours.  I love to\nkeep it by me:  the idea of getting rid\nof it nearly breaks my heart.\n\nYou cannot give me too much work; to\naccumulate work has almost become a\npassion with me:  my study is so full of\nit now, that there is hardly an inch of\nroom for any more.\n", 0)); //multiple word splitting tests that cover most cases
    assert(testRender(10, "Thames so-called Henley-on-Thames so--called so- -so\n", "Thames so-\ncalled\nHenley-on-\nThames so-\n-called\nso- -so\n", 0)); //multiple hyphen tests
    assert(testRender(10, "asdf-asdf", "asdf-asdf\n", 0)); //hyphen token fits on one line
    assert(testRender(6, "asdf-asdf", "asdf-\nasdf\n", 0)); //hyphen token that needs splitting
    assert(testRender(6, "asdf-asdf-asdf", "asdf-\nasdf-\nasdf\n", 0)); //multiple hyphen tokens
    assert(testRender(12, "asdf-asdf-asdf", "asdf-asdf-\nasdf\n", 0)); //multiple hyphen tokens broken at second hyphen
    assert(testRender(12, "asdf-asdf-asdf", "asdf-asdf-\nasdf\n", 0)); //multiple hyphen tokens broken at second hyphen
    assert(testRender(15, "asdf- asdf", "asdf- asdf\n", 0)); //hyphens split by space
    assert(testRender(15, "asdf-  asdf", "asdf- asdf\n", 0)); //hyphens split by multiple spaces
    assert(testRender(5, "----------", "-----\n-----\n", 0)); //only hyphens
    assert(testRender(5, "- - - - - - - - - -", "- - -\n- - -\n- - -\n-\n", 0)); //hyphens with spaces
    assert(testRender(15, "asdf @P@ asdf", "asdf\n\nasdf\n", 0)); //basic case
    assert(testRender(15, "@P@ asdf", "asdf\n", 0)); //leading breaks ignored
    assert(testRender(15, "asdf @P@", "asdf\n", 0)); //trailing breaks ignored
    assert(testRender(15, "asdf @P@ @P@ asdf", "asdf\n\nasdf\n", 0)); //multiple breaks ignored
    assert(testRender(25, "asdf @P@@P@", "asdf @P@@P@\n", 0)); //only '@P@' is considered a break
    assert(testRender(15, "@P@-@P@", "@P@-@P@\n", 0)); //with hyphen
    assert(testRender(4, "@P@-@P@", "@P@-\n@P@\n", 0)); //breaks at hyphen but doesnt not paragraph break
    assert(testRender(2, "as @P@ df", "as\n\ndf\n", 0)); //length less than length of break token
    assert(testRender(15, "asdfasdfasdfasdfasdf", "asdfasdfasdfasd\nfasdf\n", 1)); //basic case
    assert(testRender(4, "asdfasdfasdfasdfasdf", "asdf\nasdf\nasdf\nasdf\nasdf\n", 1)); //split to multiple lines
    assert(testRender(4, "asdfasdfasdfasdfasdf", "asdf\nasdf\nasdf\nasdf\nasdf\n", 1)); //split to multiple lines
    assert(testRender(5, "asdf-asdfasdf--asdf asdf", "asdf-\nasdfa\nsdf--\nasdf\nasdf\n", 1)); //with hyphens
    assert(testRender(6, "asdf!-asdfasdf!-asdf asdf", "asdf!-\nasdfas\ndf!-\nasdf\nasdf\n", 1)); //with hyphens and punctuation
    assert(testRender(15, "", "", 0)); //empty input
    assert(testRender(15, "@P@ @P@ @P@", "", 0)); //only breaks
    assert(testRender(179, "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasd", "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasd\n", 0)); //179 character long token
    assert(testRender(500, "It always does seem to me that I am doing more work than\nI should do. It is not that I object to the work, mind you;\nI like work: it fascinates me.       I can sit and look at it for hours.\nI love to keep     it by me: the idea of getting\nrid\nof it nearly breaks my heart. @P@ You cannot give me too\nmuch work; to accumulate work has almost become\n\n\na passion with me: my study is so full of it now, that there is hardly\nan inch of room for any more.", "It always does seem to me that I am doing more work than I should do.  It is not that I object to the work, mind you; I like work:  it fascinates me.  I can sit and look at it for hours.  I love to keep it by me:  the idea of getting rid of it nearly breaks my heart.\n\nYou cannot give me too much work; to accumulate work has almost become a passion with me:  my study is so full of it now, that there is hardly an inch of room for any more.\n", 0)); //long output line (EXTRA CREDIT CASE)
    
    cerr << "All tests complete!" << endl;
    return 0;
}

// input.txt main
/** int main() {
    ifstream infile("/Users/rachel/Desktop/input.txt");
    if (!infile)
        cerr << "Cannot open input.txt!" << endl;
    cout << "Enter maximum line length: ";
    int len;
    cin >> len;
    cin.ignore(10000, '\n');
    int returnCode = render(len, infile, cout);
    cout << "Return code is " << returnCode << endl;
} */
