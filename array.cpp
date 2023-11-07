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
    // reduplicate() test cases
    string names[10] = {"Emilie", "Irene", "Lester", "Phoebe", "Aiden", "Rachel"};
    assert(reduplicate(names, -7) == -1 && names[0] == "Emilie"); // n is negative
    assert(reduplicate(names, 0) == 0 && names[0] == "Emilie"); // n is 0
    assert(reduplicate(names, 2) == 2 && names[1] == "IreneIrene" && names[2] == "Lester"); // n is less than the total amount of elements
    string stuff[6] = {"mahi", "bon", "cous", "", "tar", "mur"};
    assert(reduplicate(stuff, 6) == 6 && stuff[0] == "mahimahi" && stuff[5] == "murmur"); // n is equal to the total amount of elements
    string empty[0];
    assert(reduplicate(empty, 0) == 0); // empty list
    
    // locate() test cases
    string names2[10] = {"Emilie", "Irene", "Lester", "Phoebe", "Aiden", "Rachel", "Emilie"};
    assert(locate(names2, 7, "Brian") == -1); // target string is not included in the list
    assert(locate(names2, 2, "Lester") == -1); // target string exists in the list but is not in the bounds of what we are interested in
    assert(locate(names2, 3, "EMILIE") == -1); // testing case sensitivity
    assert(locate(names2, 7, "Emilie") == 0); // target string exists twice in the array
    assert(locate(names2, 0, "Emilie") == -1); // n is 0
    assert(locate(names2, -5, "Rachel") == -1); // n is negative
    assert(locate(empty, 0, "empty") == -1); // array is empty
    
    // locationOfMax() test cases
    string food[5] = {"sushi", "pasta", "pizza", "taco", "sandwhich"};
    assert(locationOfMax(food, 0) == -1); // n is 0
    assert(locationOfMax(food, -1) == -1); // n is negative
    assert(locationOfMax(food, 1) == 0); // the greatest string is not in the bounds of what we are interested in
    assert(locationOfMax(food, 5) == 3); // n is equal to the number of elements in the array
    string drinks[4] = {"Boba",  "boba", "boba", "BOBA"};
    assert(locationOfMax(drinks, 2) == 1); // lowercase should be greater than uppercase
    assert(locationOfMax(drinks, 4) == 1); // more than one greatest string
    string favColor[1] = {"pink"};
    assert(locationOfMax(favColor, 1) == 0); // only one element in the array
    
    // circleLeft() test cases
    string colors[7] = {"jade", "red", "pink", "lavendar", "navy", "grey", "white"};
    assert(circleLeft(colors, -1, 3) == -1); // n is negative
    assert(circleLeft(colors, 0, 0) == -1); // n is 0 and pos is 0
    assert(circleLeft(colors, 2, 4) == -1); // pos is greater than n
    assert(circleLeft(colors, 7, 0) == 0 && colors[0] == "red" && colors[1] == "pink" && colors[2] == "lavendar" && colors[3] == "navy" && colors[4] == "grey" && colors[5] == "white" && colors[6] == "jade") ; // pos is 0
    assert(circleLeft(colors, 4, -1) == -1); // position is negative
    assert(circleLeft(colors, 5, 2) == 2 && colors[0] == "red" && colors[1] == "pink" && colors[2] == "navy" && colors[3] == "grey" && colors[4] == "lavendar" && colors[5] == "white" && colors[6] == "jade"); // n is less than the total number of elements
    assert(circleLeft(colors, 3, 2) == 2 && colors[0] == "red" && colors[1] == "pink" && colors[2] == "navy" && colors[3] == "grey" && colors[4] == "lavendar" && colors[5] == "white" && colors[6] == "jade"); // pos is one less than n (shouldn't change anything)
    assert(circleLeft(favColor, 1, 0) == 0 && favColor[0] == "pink"); // only one element in the array
    
    // enumerateRuns() test cases
    string brands[3] = {"Nike", "Nike", "Nike"};
    assert(enumerateRuns(brands, 0) == 0); // n is 0
    assert(enumerateRuns(brands, -1) == -1); // n is negative
    assert(enumerateRuns(brands, 3) == 1); // one sequence
    string brands2[5] = {"Nike", "Adidas", "Reebok", "Vans", "Puma"};
    assert(enumerateRuns(brands2, 5) == 5); // no consecutive items
    assert(enumerateRuns(empty, 0) == 0); // empty string
    string brands3[10] = {"Nike", "nike", "Adidas", "Reebok", "Reebok", "Vans", "Vans", "Puma", "Reebok", "Vans"};
    assert(enumerateRuns(brands3, 2) == 2); // checks for case sensitivity
    assert(enumerateRuns(brands3, 4) == 4); // n is less than the total number of elements
    assert(enumerateRuns(brands3, 10) == 8); // sequence containing consecutive items
    
    // flip() test cases
    string folks[6] = {"donald", "tim", "", "chris", "nikki", "donald"};
    assert(flip(folks, -6) == -1); // n is negative
    assert(flip(folks, 0) == 0); // n is zero
    assert(flip(folks, 4) == 4 && folks[0] == "chris" && folks[1] == "" && folks[2] == "tim" && folks[3] == "donald" && folks[4] == "nikki" && folks[5] == "donald"); // n is less than all the elements in the array
    assert(flip(brands2, 3) == 3 && brands2[0] == "Reebok" && brands2[1] == "Adidas" && brands2[2] == "Nike" && brands2[3] == "Vans" && brands2[4] == "Puma"); // n is odd
    assert(flip(favColor, 1) == 1 && favColor[0] == "pink"); // array has only one element
    string parents[2] = {"mom", "dad"};
    assert(flip(parents, 1) == 1 && parents[0] == "mom" && parents[1] == "dad"); // n is 1 (nothing should change)
    assert(flip(parents, 2) == 2 && parents[0] == "dad" && parents[1] == "mom"); // array with 2 elements
    
    // locateDifference() test cases
    string cities[5] = {"fremont", "milpitas", "cupertino", "sunnyvale", "saratoga"};
    string cities2[4] = {"fremont", "milpitas", "sunnyvale", "sunnyvale"};
    assert(locateDifference(cities, 1, cities2, -8) == - 1); // n1 is negative
    assert(locateDifference(cities, -3, cities2, 2) == - 1); // n2 is negative
    assert(locateDifference(cities, 0, cities2, 4) == 0); // n1 is zero
    assert(locateDifference(cities, 2, cities2, 0) == 0); // n2 is zero
    assert(locateDifference(cities, 2, cities2, 2) == 2); // no difference
    assert(locateDifference(cities, 1, cities2, 2) == 1); // one of the arrays runs out
    assert(locateDifference(cities, 5, cities2, 4) == 2); // n1 and n2 equal the total number of elements in the array
    
    // subsequence() test cases
    string countries[7] = {"New Zealand", "India", "Japan", "Canada", "China", "Japan", "Australia"};
    string countries2[4] = {"Japan", "Canada", "China", "Japan"};
    string countries3[3] = {"India", "Mexico", "New Zealand"};
    assert(subsequence(countries, -1, countries2, 4) == -1); // n1 is negative
    assert(subsequence(countries, 5, countries2, -2) == -1); // n2 is negative
    assert(subsequence(countries, 0, countries2, 4) == -1); // n1 is 0
    assert(subsequence(countries, 4, countries2, 0) == 0); // n2 is 0
    assert(subsequence(countries, 0, countries2, 0) == 0); // n1 and n2 is 0
    assert(subsequence(countries, 7, countries2, 4) == 2); // n1 and n2 equal the total number of elements in the array
    assert(subsequence(countries, 1, countries3, 1) == -1); // subsequence is not in the bounds of what we are interested in
    assert(subsequence(countries2, 4, countries3, 3) == -1); // subsequence doesn't exist
    
    // locateAny() test cases
    assert(locateAny(countries, 0, countries2, 4) == -1); // n1 is 0
    assert(locateAny(countries, 5, countries2, 0) == -1); // n2 is 0
    assert(locateAny(countries, -2, countries2, 4) == -1); // n1 is negative
    assert(locateAny(countries, 5, countries2, -1) == -1); // n2 is negative
    assert(locateAny(countries, 7, countries2, 4) == 2); // multiple elements in a2 that are in a1
    assert(locateAny(countries2, 4, countries3, 3) == -1) ; // no elements in a2 that are in a1
    assert(locateAny(countries2, 3, countries, 2) == -1); // elements in a2 exist in a1 but are located outside the bounds
    
    // divide() test cases
    string names3[6] = {"Emilie", "Irene", "Lester", "Phoebe", "Aiden", "Rachel"};
    assert(divide(names3, -1, "") == -1); // n is negative
    assert(divide(names3, 0, "Irene") == 0); // n is 0
    assert(divide(names3, 4, "phoebe") == 4); // all elements are less than divider
    string names4[6] = {"Emilie", "Irene", "Lester", "Phoebe", "Aiden", "Rachel"};
    assert(divide(names4, 3, "Frank") == 1); // n is less than the total number of elements in the array
    assert(names4[0] == "Emilie" && names4[1] == "Irene" && names4[2] == "Lester" && names4[3] == "Phoebe" && names4[4] == "Aiden" && names4[5] == "Rachel");
    assert(divide(names4, 5, "Aiden") == 0); // divider is equal to one of the elements
    string greetings[4] = {"hi", "hello", "bonjour", "hey"};
    assert(divide(greetings, 4, "HI!") == 0); // all elements are greater than divider
    
    // smallberg's test cases
    
    string people[5] = { "donald", "vivek", "tim", "asa", "nikki" };
    assert(locate(people, 3, "nikki") == -1);
    string stuff2[6] = { "mahi", "bon", "cous", "", "tar", "mur" };
    assert(reduplicate(stuff2, 6) == 6 && stuff[0] == "mahimahi" && stuff[1] == "bonbon" && stuff[2] == "couscous" && stuff[3] == "" && stuff[4] == "tartar" && stuff[5] == "murmur");
    string cand[6] = { "donald", "asa", "ron", "vivek", "chris", "nikki"};
    assert(locationOfMax(cand, 6) == 3);
    string running[5] = { "nikki", "donald", "asa", "tim", "ron" };
    assert(circleLeft(running, 5, 1) == 1 && running[0] == "nikki" && running[1] == "asa" && running[2] == "tim" && running[3] == "ron" && running[4] == "donald");
    string d[9] = {"chris", "doug", "tim", "tim", "vivek", "vivek", "vivek", "tim", "tim"};
    assert(enumerateRuns(d, 9) == 5);
    string folks2[6] = { "donald", "tim", "", "chris", "nikki", "donald" };
    assert(flip(folks2, 4) == 4 && folks2[0] == "chris" && folks2[1] == "" && folks2[3] == "donald" && folks2[4] == "nikki" && folks2[5] == "donald");
    string folks3[6] = { "donald", "tim", "", "chris", "nikki", "donald" };
    string group[5] = { "donald", "tim", "donald", "", "chris" };
    assert(locateDifference(folks3, 6, group, 5) == 2);
    assert(locateDifference(folks3, 2, group, 1) == 1);
    string names5[10] = { "nikki", "ron", "tim", "vivek", "doug" };
    string names6[10] = { "ron", "tim", "vivek" };
    assert(subsequence(names5, 5, names6, 3) == 1);
    string names7[10] = { "nikki", "vivek" };
    assert(subsequence(names5, 4, names7, 2) == -1);
    string names8[10] = { "nikki", "ron", "tim", "vivek", "doug" };
    string set1[10] = { "donald", "doug", "vivek", "ron" };
    assert(locateAny(names8, 5, set1, 4) == 1);
    string set2[10] = { "chris", "asa" };
    assert(locateAny(names8, 5, set2, 2) == -1);
    string cand1[6] = { "donald", "asa", "ron", "vivek", "chris", "nikki" };
    assert(divide(cand1, 6, "doug") == 3);
    string cand2[4] = { "ron", "vivek", "asa", "tim" };
    assert(divide(cand2, 4, "tim") == 2);
}
