#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <sstream>
#include <string>

using namespace std;

vector<tuple<string, int, int, int, int>> MNT; // Macro Name Table
vector<vector<string>> MDT;                      // Macro Definition Table
vector<vector<string>> PNTAB;                    // Parameter Name Table
vector<tuple<string, string>> KPTAB;             // Keyword Default Table
vector<vector<string>> ourInputFile;                  // Tokenized input


// Find the macro index in MNT
int findInMNT(const string& name) {
    for (int i = 0; i < MNT.size(); i++) {
        if (get<0>(MNT[i]) == name) {
            return i;
        }
    }
    return -1;
}

int expandMacro(int linenum)
{
    vector<string> APTAB; // Actual Parameter Table
    int mntindex = findInMNT(ourInputFile[linenum][0]);
    // we are pushing all the actual parameters in the actual parameter table
    for(int i=1; i<ourInputFile[linenum].size(); i++)
    {
        APTAB.push_back(ourInputFile[linenum][i]);
    }
    // for first macro, APTAB = ["X", "&E=AREG"]

    int mdtindex = get<3>(MNT[mntindex]);
    int pp = get<1>(MNT[mntindex]); // no of simple parameters of that macro


    // with this for loop, we will iterate over each line in mdt
    for(int i=mdtindex; i<MDT.size(); i++)
    {
        if(MDT[i][0] == "MEND")
        {
            break;
        }

        // with this for each loop, we will iterate over each word/string/token in one single line in mdt
        string token;
        for(int z=0; z<MDT[i].size(); z++)
        {
            token = MDT[i][z];
            if(token[0] == '(') // sample line - SUB (P,1) (P,0)
            {
                int pos = token[3] - '0'; // pos refers to the position number of the parameter in the pntab
                if(pos < pp) // if it is a positional parameter, then directly print value from APTAB
                {
                    cout<<APTAB[pos]<<" ";
                }
                else // else if it is a keyword parameter
                // Search APTAB for a match. If found, output the provided value.
                // If not found in APTAB, then find the default value in of that keyword parameter in the KPTAB 
                {
                    string arg = PNTAB[mntindex][pos]; // PNTAB madhun, apan hya kp cha name gheun aalo
                    string val = "";
                    int i;
                    for(i=pp; i<APTAB.size(); i++)
                    {
                        auto n = APTAB[i].find('=');
                        if(APTAB[i].substr(0,n) == arg)
                        {
                            val = APTAB[i].substr(n+1);
                            cout<<val<<" ";
                            break;
                        }
                    }

                    if(i == APTAB.size())
                    {
                        int kptabp = get<4>(MNT[mntindex]);
                        for(int j=kptabp; j<KPTAB.size(); j++)
                        {
                            auto entry = KPTAB[j];
                            if(get<0>(entry) == arg)
                            {
                                string val = get<1>(entry);
                                cout<<val<<" ";
                            }
                        }
                    }
                }
            }
            else
            {
                cout<<token<<" ";
            }
        }
        cout<<endl;
    }

    return linenum;
}

// Pass 2 to process each line
void pass2() {
    for (int i = 0; i < ourInputFile.size(); i++) {
        if(ourInputFile[i][0] == "MACRO") {
            while(i<ourInputFile.size() && ourInputFile[i][0] != "MEND") {
                i++;
            }
        } else if(findInMNT(ourInputFile[i][0]) != -1 && ourInputFile[i-1][0] != "MACRO") {
            // cout<<"macro call found. macro expansion below"<<endl;
            i = expandMacro(i);
        } else {
            for(int j=0; j<ourInputFile[i].size(); j++) {
                cout<<ourInputFile[i][j]<<" ";
            }
            cout<<endl;
        }
    }
}

// Display the tables for debugging purposes
void displayTables() {
    cout << "MNT (Macro Name Table):" << endl;
    cout << "Name\tPP\tKP\tMDTP\tKDTP" << endl;
    for (const auto& entry : MNT) {
        cout << get<0>(entry) << "\t" << get<1>(entry) << "\t" 
             << get<2>(entry) << "\t" << get<3>(entry) << "\t" 
             << get<4>(entry) << endl;
    }
    cout << endl;

    cout << "MDT (Macro Definition Table):" << endl;
    for (int i = 0; i < MDT.size(); i++) {
        cout << i << ":\t";
        for (const auto& token : MDT[i]) {
            cout << token << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "PNTAB (Parameter Name Table):" << endl;
    for (int i = 0; i < PNTAB.size(); i++) {
        cout << "Macro " << i << ":\t";
        for (const auto& param : PNTAB[i]) {
            cout << param << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "KPTAB (Keyword Default Table):" << endl;
    cout << "Keyword\tDefault Value" << endl;
    for (const auto& entry : KPTAB) {
        cout << get<0>(entry) << "\t" << get<1>(entry) << endl;
    }
    cout << endl;
}

// Load the tables from files
void loadFiles() {
    ifstream mntFile("mnt.txt"), mdtFile("mdt.txt"), pntFile("pnt.txt"), kdtFile("kpt.txt"), inputFile("macroinput.txt");
    string line, word;

    // Load MNT
    while (getline(mntFile, line)) {
        stringstream iss(line);
        string name;
        int PP, KP, MDTP, KDTP;
        iss >> name >> PP >> KP >> MDTP >> KDTP;
        MNT.push_back(make_tuple(name, PP, KP, MDTP, KDTP));
    }

    // Load MDT
    while (getline(mdtFile, line)) {
        stringstream iss(line);
        vector<string> tokens;
        while (iss >> word) {
            tokens.push_back(word);
        }
        MDT.push_back(tokens);
    }

    // Load PNTAB
    while (getline(pntFile, line)) {
        stringstream iss(line);
        vector<string> tokens;
        while (iss >> word) {
            tokens.push_back(word);
        }
        PNTAB.push_back(tokens);
    }

    // Load KPTAB
    while (getline(kdtFile, line)) {
        stringstream iss(line);
        string key, val;
        
        // Extract key
        iss >> key;
        
        // Check if a value is provided
        if (iss >> val) {
            // Both key and value are present
            KPTAB.push_back(make_tuple(key, val));
        } else {
            // Only key is present, use an empty string or default
            KPTAB.push_back(make_tuple(key, ""));
        }
    }

    while (getline(inputFile, line)) {
        stringstream iss(line);
        vector<string> tokens;
        string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        ourInputFile.push_back(tokens);
    }

}

int main() {
    // Tokenize and load all the files in our global variables declared at the top
    loadFiles();

    displayTables();

    // Run Pass2
    cout<<endl<<"***OUTPUT***"<<endl;
    pass2();

    return 0;
}