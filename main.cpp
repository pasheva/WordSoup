#include <iostream>
#include <fstream>      // For file input
#include <vector>		// For dictionary vector
#include <iomanip>		// For setw()
#include <cstdlib>  // For exit and abs  exit(-1);
#include <cctype>   // For tolower() method.
#include <string> //for the non-static replace() method.
#include <algorithm> // For find() non static method.
#include  <ctime> // time
using namespace std;

void readInWordsFromFile(vector<string>& dictionary, int lengthOfWordsToUse); //Readging the dictionary stream and assigning the words to the dictionary vectors by size.
void displayMenu(vector<string>& dictionary, int lengthOfWordsToUse, string startWord, string endWord, int& menuOption);
void getWord(string& startWord, string& endWord, int lengthOfWordsToUse, vector<string>& dictionary); // Getting user set words or random ones for case 3.
void reverseOrder(vector<string>& sequence, vector<string>& winningSeq, vector<int>& childFirst, vector<int>& childLast); // Case 7 ouputing the winning sequence in reverse order.
int levelOrderSearch(string startWord, string endWord, int lengthOfWordsToUse, vector<string>& dictionary, vector<string>& sequence, vector<int>& childFirst, vector<int>& childLast, int checkCase); // Debug mode. Outputing all words inbetween startWord and endWord.
long binarySearch(const string& searchWord, vector< string> dictionary); //Binary search used for the dictionary vector in order to obtain Olog(n) and avoid timeout.
bool checkValid(string searchWord, int lengthOfWordsToUse, vector<string>& dictionary); //Checking if a word is the correct size and exists in the dictionary.
string lowerCase(string word); //Turning all words (strings) from the dictionary.txt file to lowercase.

//-----------------------------------------------------------------------------------------
int main()
{
    vector< string> dictionary;    		// Vector of dictionary words read in from the dictionary file.
    vector<string> sequence;            // Vector containing the whole sequence from the startWord to the endWord.
    vector<int> childFirst, childLast; /** Vectors containing indexes of the children for each parent.
                                        -----------------------------------------------------------------------------------
                                        0. cat: 1:bat 2:eat 3:fat 4:gat 5:hat 6:jat 7:kat 8:lat 9:mat 10:nat 11:oat 12:pat
                                        13:rat 14:sat 15:tat 16:vat 17:wat 18:yat 19:zat 20:cit 21:cot 22:cut 23:cab 24:cad
                                        25:cag 26:cal 27:cam 28:can 29:cap 30:car 31:caw 32:cay
                                        1. bat: 33:bet 34:bit 35:bot 36:but 37:baa 38:bab 39:bac 40:bad 41:bae 42:bag 43:bah
                                        44:bal 45:bam 46:ban 47:bap 48:bar 49:bas 50:baw 51:bay
                                        -------------------------------------------------------------------------------------
                                        Parent: cat | Children: (childFirst) bat (childLast) cay
                                        Parent: bat | Children: (childFirst) bet (childLast) bay
                                        **/
    vector<string>  winnningSeq(sequence.size()); //Storing the winning sequence for the reverse order.
    int lengthOfWordsToUse = 3;         // Default length of word to use in word transformation
    string startWord = "cat";           // The start word for transformation
    string endWord = "bay";             // The end word for transformation
    int userInput = -1;                // User menu option selection -1


    // Reading the dictionary.txt stream into the dictionary vector based on the size of words.
    readInWordsFromFile(dictionary, lengthOfWordsToUse); //Making the default size 3.

    // Seed the random number generator.
    srand( (unsigned) time(NULL));

    // Menu displays as infinite loop
    do {
        displayMenu(dictionary, lengthOfWordsToUse, startWord, endWord, userInput);
        switch (userInput){
            case 1:
            {
                cout << "What length words do you want to use? ";
                cin >> lengthOfWordsToUse;
                // Giving the opportunity to switch to a different length of words.
                readInWordsFromFile(dictionary, lengthOfWordsToUse);
                startWord = ""; //Set to default empty.
                endWord = ""; // Set to default empty.
            }
                break;
            case 2:
            {
                // Promting the user to set the start and end word.
                getWord(startWord, endWord, lengthOfWordsToUse, dictionary);
            }
                break;
            case 3:
                childLast.clear();
                childFirst.clear();
                sequence.clear();
                winnningSeq.clear();
                if (levelOrderSearch(startWord, endWord, lengthOfWordsToUse, dictionary, sequence, childFirst, childLast, 5) == 1){
                    cout << "\n\nWinning sequence was found!" << endl;
                }else{
                    cout << "\n\nWinning sequence was not found!" << endl;
                }
                break;
            case 4:
                childLast.clear();
                childFirst.clear();
                sequence.clear();
                winnningSeq.clear();
                levelOrderSearch(startWord, endWord, lengthOfWordsToUse, dictionary, sequence, childFirst, childLast, 7);
                reverseOrder(sequence, winnningSeq, childFirst, childLast);
                cout << "\nWinning sequence in reverse order is:" << endl;
                for (int i = 0; i < winnningSeq.size(); ++i){
                    int pos = std::find(sequence.begin(), sequence.end(), winnningSeq.at(i)) - sequence.begin();
                    cout << pos << ". " << winnningSeq.at(i) << endl;
                }
                winnningSeq.clear();
                break;
            case 5:
                cout << "Exit" << endl;
                exit(-1);
            default:
                cout << "Invalid menu option.  Please retry." << endl;
        }// end switch statement
    } while( true);
}//end main()

//-----------------------------------------------------------------------------------------
// Creates an array with all the words of a certain size.
void readInWordsFromFile(vector<string>& dictionary, int lengthOfWordsToUse)
{
    ifstream inputFileStream;
    string fileName = "dictionary.txt"; // C string (array of char) to store filename
    string inputWord;				    // stores each word as it is read

    // Open input filefff
    inputFileStream.open( fileName.c_str() );

    // Verify that the open worked
    if( !inputFileStream.is_open())
    {
        cout << "Could not find input file " << fileName << "  Exiting..." << endl;
        exit( -1);
    }
    // Read all the words from the file, and display them
    dictionary.clear();
    while( inputFileStream >> inputWord)
    {
        if (inputWord.size() == lengthOfWordsToUse)
        {
            dictionary.push_back(lowerCase(inputWord));
        }
    }
    inputFileStream.close();    // Close the input file.
} // end readInWordsFromFile

//-----------------------------------------------------------------------------------------
//Gives statistics, displays the menu and asks the user for input.
void displayMenu(vector<string>& dictionary, int lengthOfWordsToUse, string startWord, string endWord, int& menuOption){
    cout << "\nCurrently we have " << dictionary.size() << " words of length "
         << lengthOfWordsToUse << " in the dictionary.  \n"
         << "Changing from '" << startWord << "' to '" << endWord << "'" << endl
         << endl;

    cout << "Choose from the following options:  \n"
         << "   1. Change the word length        \n"
         << "   2. Change start and end words       \n"
         << "   3. Find the end word sequence  \n"
         << "   4. Display the winning sequence    \n"
         << "   5. Exit the program              \n"
         << "Your choice -> ";
    cin >> menuOption;
    cout << endl;
} // end displayMenu

//-----------------------------------------------------------------------------------------
bool checkValid(string searchWord, int lengthOfWordsToUse, vector<string>& dictionary)
{
    if (searchWord.length() == lengthOfWordsToUse)
    {
        if ((binarySearch(searchWord, dictionary)) > 0)
        {
            return true;
        }
        cout << "\n\t*** \'" << searchWord << "\' is not in the dictionary. Please retry." << endl;
        return false;
    }
    if (searchWord.length() <= 0)
    {
        return false;
    }
    cout << "\n\t*** \'" << searchWord << "\' is not of length " << lengthOfWordsToUse << ". Please retry." << endl;
    return false;
} // end checkValid

//-----------------------------------------------------------------------------------------
void getWord(string& startWord, string& endWord, int lengthOfWordsToUse, vector<string>& dictionary)
{
    int numberOfWords = dictionary.size();
    bool check = false;
    while(!(check)) {
        cout << "\nEnter starting word, or 'r' for a random word: ";
        cin >> startWord;
        if (startWord == "r")
        {
            startWord = dictionary.at(rand() % numberOfWords);
            check = true;
        }else if(startWord == "exit")
        {
            cout << "\nExiting program." << endl;
            exit(-1);
        }else{
            cout << endl;
            check = checkValid(startWord, lengthOfWordsToUse, dictionary);
        }
    }
    check = false;
    while(!(check))
    {
        cout << "\nEnter ending word, or 'r' for a random word: ";
        cin >> endWord;
        if (endWord == "r") {
            endWord = dictionary.at(rand() % numberOfWords);
            check = true;
        }else if(endWord == "exit")
        {
            cout << "\nExiting program." << endl;
            exit(-1);
        }else{
            cout << endl;
            check = checkValid(endWord, lengthOfWordsToUse, dictionary);
        }
    }
} // end getWord

//-----------------------------------------------------------------------------------------
//Converts all the characters to lowers case,.
string lowerCase(string inputWord)
{
    string lowCaseWord;
    for(char c : inputWord)
    {
        lowCaseWord += tolower(c);
    }
    return lowCaseWord;
} // end lowerCase

//--------------------------------------------------------------------------------------
int levelOrderSearch(string startWord, string endWord, int lengthOfWordsToUse, vector<string>& dictionary, vector<string>& sequence, vector<int>& childFirst, vector<int>& childLast, int checkCase)
{
    bool found = false;
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    string testWord = startWord; // Assigning the aphalbet chars to the index and using it as a dummy word to check its validity.

    int index = 0; //Declaring it here so I can change it later inside the loops. Because we iterate over the first index only the first time.
    int sequenceIndexCache = 0; // In order to help with outputing the correct children to the correct parrent.
    int parentIndex = 0;

    sequence.push_back(startWord); // [dog, ] //initialize


    while (!(found))
    {
        if (checkCase == 5)
        {
            cout << "\n" << parentIndex << ". " << sequence[parentIndex] << ":\t";
        }
        testWord = sequence[parentIndex];
        childFirst.push_back(sequence.size());
        for (index = 0; index < lengthOfWordsToUse; ++index)
        {
            //Iterating over the alphabet of characters.
            for (char c : alphabet)
            {
                testWord = sequence[parentIndex];
                testWord[index] = c;
                if (binarySearch(testWord, dictionary) >= 0) //Checking if the word is valid and exists in the dictionary.
                {
                    if (find(sequence.begin(), sequence.end(), testWord) == sequence.end()) //Checking whether it already exists in the sequence vector.
                    {
                        sequence.push_back(testWord); //Adding the word to the vector.
                        sequenceIndexCache++;
                        if (checkCase == 5)
                        {
                            cout << sequenceIndexCache << ":" << testWord << " "; //COUTING THE SEQUENCE VECTOR
                        }
                        if (testWord == endWord) {
                            childLast.push_back(sequence.size() - 1);
                            found = true;
                            return 1;
                        } // Break if found.
                    }
                }
            }
            if (found)
                break;
        }
        parentIndex++; //Moving to the next parent
        childLast.push_back(sequence.size() - 1);
    }
    return 0;
} // end levelOrderSearch

//--------------------------------------------------------------------------------------
//
void reverseOrder(vector<string>& sequence, vector<string>& winningSeq, vector<int>& childFirst, vector<int>& childLast)
{
    int lastWordFound = sequence.size() - 1;
    winningSeq.push_back(sequence.at(lastWordFound));
    for (int i = (childFirst.size() - 1); i >= 0; --i)
    {
        if ((lastWordFound >= childFirst.at(i)) && (lastWordFound <= childLast.at(i)))
        {
            winningSeq.push_back(sequence.at(i));
            lastWordFound = i;
        }
    }
}// end reverseOrder

//--------------------------------------------------------------------------------------
// Use binary search to look up the search word in the dictionary vector, returning
// the word's index if found, -1 otherwise.
long binarySearch(const string& searchWord, vector< string> dictionary)
{
    long low, mid, high;     // array indices for binary search
    long searchResult = -1;  // Stores index of word if search succeeded, else -1

    low = 0;
    high = dictionary.size() - 1;
    while ( low <= high)
    {
        mid = (low + high) / 2;
        searchResult = searchWord.compare( dictionary[ mid] );
        if ( searchResult == 0)
        {
            // return the index where the word was found
            return mid;
        }
        else if (searchResult < 0)
        {
            high = mid - 1; // word located before the mid location
        }
        else  {
            low = mid + 1; // word located after the mid location
        }
    }
    return -1;
}//end binarySearch()
