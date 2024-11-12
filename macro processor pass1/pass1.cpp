#include<iostream>
#include<fstream>
#include<vector>
#include<tuple>
#include<string>
#include<sstream>

using namespace std;

vector<tuple<string, int, int, int, int>> MNT;
vector<tuple<string, string>> KPDTAB;
vector<vector<string>> MDT;
vector<vector<string>> PNTAB;
vector<vector<string>> ourInputFile;

int findintempPNTAB(vector<string> temp, string n, int no)
{
    for(int i=0; i<no; i++)
    {
        if(temp[i] == n)
        {
            return i;
        }
    }
    return -1;
}

int processMacro(int lineNum)
{
    vector<string> tempPNTAB;
    int p = 0;
    int kp = 0;
    int KDTP = KPDTAB.size();
    int MDTP = MDT.size();
    string name = ourInputFile[lineNum][0];
    for(int j=1; j<ourInputFile[lineNum].size(); j++)
    {
        string token = ourInputFile[lineNum][j];
        auto pos = token.find('=');
        if(pos == string::npos)
        {
            tempPNTAB.push_back(token);
            p++;
        }
        else
        {
            string keyword = token.substr(0, pos);
            string value = token.substr(pos+1);
            tempPNTAB.push_back(keyword);
            kp++;
            KPDTAB.push_back(make_tuple(keyword, value));
        }
    }
    PNTAB.push_back(tempPNTAB);
    MNT.push_back(make_tuple(name, p, kp, MDTP, KDTP));

    lineNum++;
    while(lineNum < ourInputFile.size() && ourInputFile[lineNum][0] != "MEND")
    {
        vector<string> tokens;
        for(int i=0; i<ourInputFile[lineNum].size(); i++)
        {
            if(ourInputFile[lineNum][i][0] == '&')
            {
                
                int a = findintempPNTAB(tempPNTAB, ourInputFile[lineNum][i], tempPNTAB.size());
                string line = "(P,"+to_string(a)+")";
                tokens.push_back(line);
            }
            else
            {
                tokens.push_back(ourInputFile[lineNum][i]);
            }
        }
        MDT.push_back(tokens);
        lineNum++;
    }
    MDT.push_back(ourInputFile[lineNum]);
    return lineNum;

}

void pass1()
{
    for(int i=0; i<ourInputFile.size(); i++)
    {
        if(ourInputFile[i][0] == "MACRO")
        {
            
            i = processMacro(i+1);
            
        }
    }
    
}

void enterVecsIntoFiles() {
    ofstream mntfile("mnt.txt"), mdtfile("mdt.txt"), pntfile("pnt.txt"), kdtfile("kdt.txt");
    for(const auto& entry : MNT)
    {
        mntfile<<get<0>(entry)<<" "<<get<1>(entry)<<" "<<get<2>(entry)<<" "<<get<3>(entry)<<" "<<get<4>(entry)<<endl;
    }
    for(const auto& pnt : PNTAB)
    {
        for(const auto& param : pnt)
        {
            pntfile<<param<<" ";
        }
        pntfile<<endl;
    }

    for(const auto& line : MDT)
    {
        for(const auto& token : line)
        {
            mdtfile<<token<<" ";
        }
        mdtfile<<endl;
    }

    for(const auto& line : KPDTAB)
    {
        kdtfile<<get<0>(line)<<" "<<get<1>(line)<<endl;
    }

    mdtfile.close();
    mntfile.close();
    kdtfile.close();
    pntfile.close();
}


int main()
{
    ifstream ipfile("macroinput.txt");\

    string line;
    while(getline(ipfile, line))
    {
        stringstream ss(line);
        vector<string> lineTokens;
        string word;
        while(ss >> word)
        {
            lineTokens.push_back(word);
        }
        ourInputFile.push_back(lineTokens);
    }
    ipfile.close();

    pass1();
    enterVecsIntoFiles();
    return 0;
}