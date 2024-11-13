#include<bits/stdc++.h>
using namespace std;

string table(ifstream & f, string c) {
    string no, name, addr;
    while( f >> no >> name >> addr) {
        if(c == no) {
            f.clear();
            f.seekg(0, ios::beg);
            return addr;
        }
    }
    f.clear();
    f.seekg(0, ios::beg);
    return "NAN";
}

int main() {
    ofstream mc("machinecode.txt");
    ifstream st("st.txt"), ic("ic.txt"), lt("lt.txt");

    string lc, ic1, ic2, ic3;
    while(ic >> lc >> ic1 >> ic2 >> ic3) {
        string machine_code;
        if(ic1.substr(1, 2) == "AD" || (ic1.substr(1, 2) == "DL" && ic1.substr(4, 2) == "02")) {
            machine_code = "--no mc code--";
        } else if(ic1.substr(1, 2) == "DL" && ic1.substr(4, 2) == "01") {
            machine_code = "00\t0\t00";
            char c = ic2[3];
            machine_code += c;
        } else {
            if(ic1 == "(IS,00)") {
                machine_code = "00\t0\t000";
            } else {
                if(ic3[1] == 'S') {
                    machine_code = ic1.substr(4, 2) + "\t" + ic2[1] + "\t" + table(st, ic3.substr(4,1));
                } else {
                    machine_code = ic1.substr(4, 2) + "\t" + ic2[1] + "\t" + table(lt, ic3.substr(4,1));
                }
            }
        }

        mc << lc << "\t" << machine_code << endl;
    }

    return 0;
}