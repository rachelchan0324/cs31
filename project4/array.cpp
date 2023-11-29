// Name: Rachel Chan
// UID: 306280535

#include <iostream>
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

int reduplicate(string a[], int n){
    if(n < 0)
        return -1;
    // for every string in the array, append the string to itself
    for(int i = 0; i < n; i++){
        a[i] += a[i];
    }
    return n;
}

int locate(const string a[], int n, string target){
    if(n < 0)
        return -1;
    // iterate through array, if the string at the index matches the target string then return that index
    for(int i = 0; i < n; i++){
        if(a[i] == target)
            return i;
    }
    // the target string was never found, return -1
    return -1;
}

int locationOfMax(const string a[], int n){
    // if n is <= 0, then the function should examine no elements of the array
    if(n <= 0)
        return -1;
    // represents the location of the latest string in alphabetic order
    int maxLoc = 0;
    for(int i = 0; i < n; i++){
        // if the string at index i is alphabetically greater than the one at maxLoc, update maxLoc
        if(a[i] > a[maxLoc])
            maxLoc = i;
    }
    return maxLoc;
}

int circleLeft(string a[], int n, int pos){
    if(n < 0)
        return -1;
    // requires looking at the contents of an element PAST the last element we're interested in
    if(pos > n - 1 || pos < 0)
        return -1;
    // stores the eliminated item to place at the end, later
    string temp = a[pos];
    for(int i = pos; i < n - 1; i++){
        // shifts elements to the left
        a[i] = a[i + 1];
    }
    // places the eliminated item at the end
    a[n - 1] = temp;
    return pos;
}

int enumerateRuns(const string a[], int n){
    if(n < 0)
        return -1;
    if(n == 0)
        return 0;
    string currString = a[0];
    // number of sequences
    int seq = 1;
    for(int i = 0; i < n; i++)
        // if a "new" string appears, then increment sequences
        if(a[i] != currString){
            currString = a[i];
            seq++;
        }
    return seq;
}

int flip(string a[], int n){
    if(n < 0)
        return -1;
    // swap the current element with the element at the corresponding position on the other half the array (its mirrored position)
    for(int i = 0; i < n/2; i++){
        string temp = a[n-1-i];
        a[n-1-i] = a[i];
        a[i] = temp;
    }
    return n;
}

int locateDifference(const string a1[], int n1, const string a2[], int n2){
    if(n1 < 0 || n2 < 0)
        return -1;
    int pos;
    // pos goes up to whenever n1 or n2 run out
    for(pos = 0; pos < n1 && pos < n2; pos++){
        if(a1[pos] != a2[pos])
            return pos;
    }
    if(n1 <= n2)
        return n1;
    else
        return n2;
}

int subsequence(const string a1[], int n1, const string a2[], int n2){
    if(n1 < 0 || n2 < 0)
        return -1;
    // a sequence of 0 elements is a subsequence of any subsequence
    if(n2 == 0)
        return 0;
    int candidate = -1;
    // tracks which n2 element we are currently comparing
    int n2index = 0;
    for(int i = 0; i < n1; i++){
        if(a1[i] == a2[n2index]){
            if(n2index == 0)
                // the beginning of a potential subsequence, store it in variable candidates
                candidate = i;
            if(i - candidate + 1 == n2)
                break;
            // if this is the last index of n1 and not all of n2 elements has appeared yet, then this subsequence is no longer a valid candidate
            if(i == n1 - 1 && i - candidate + 1 < n2)
                candidate = -1;
            n2index++;
        }
        // if element in n1 doesn't match n2, it is no longer a valid candidate --> reset
        else{
            n2index = 0;
            candidate = -1;
        }
    }
    return candidate;
}

int locateAny(const string a1[], int n1, const string a2[], int n2){
    if(n1 <= 0 || n2 <= 0)
        return -1;
    // nested loop: iterate through each element in a1 and compare it to each element in a2
    for(int i = 0; i < n1; i++){
        for(int j = 0; j < n2; j++){
            // if the elements match, then return the current index of a1
            if(a1[i] == a2[j])
                return i;
        }
    }
    return -1;
}

int divide(string a[], int n, string divider){
    if(n < 0)
        return -1;
    // selection sort algorithm: sort the entire array in order by identifying the smallest element from the unsorted portion of the list and swapping it with the first element of the unsorted portion
    for(int i = 0; i < n - 1; i++){
        int minIndex = i;
        // identifies the minimum element in the unsorted array
        for(int j = i + 1; j < n; j++){
            if(a[j] < a[minIndex])
                minIndex = j;
        }
        // swaps the minimum element with first element of unsorted array
        if(minIndex != i){
            string temp = a[i];
            a[i] = a[minIndex];
            a[minIndex]= temp;
        }
    }
    // returns the first element that is greater than or equal to divider
    for(int i = 0; i < n; i++){
        if(a[i] >= divider)
            return i;
    }
    return n;
}

int main() {
    
    string h[7] = { "nikki", "ron", "asa", "vivek", "", "chris", "donald" };
    assert(locate(h, 7, "chris") == 5);
    assert(locate(h, 7, "asa") == 2);
    assert(locate(h, 2, "asa") == -1);
    assert(locationOfMax(h, 7) == 3);

    string g[4] = { "nikki", "ron", "chris", "tim" };
    assert(locateDifference(h, 4, g, 4) == 2);
    assert(circleLeft(g, 4, 1) == 1 && g[1] == "chris" && g[3] == "ron");

    string c[4] = { "ma", "can", "tu", "do" };
    assert(reduplicate(c, 4) == 4 && c[0] == "mama" && c[3] == "dodo");

    string e[4] = { "asa", "vivek", "", "chris" };
    assert(subsequence(h, 7, e, 4) == 2);

    string d[5] = { "ron", "ron", "ron", "chris", "chris" };
    assert(enumerateRuns(d, 5) == 2);

    string f[3] = { "vivek", "asa", "tim" };
    assert(locateAny(h, 7, f, 3) == 2);
    assert(flip(f, 3) == 3 && f[0] == "tim" && f[2] == "vivek");

    assert(divide(h, 7, "donald") == 3);

    cout << "All tests succeeded" << endl;
}
