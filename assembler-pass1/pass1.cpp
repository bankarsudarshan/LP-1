#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
using namespace std;

struct optabentry
{
    string opcode, mclass, mneumonic;
};
optabentry optab[18] = {
    {"STOP", "IS", "00"},
    {"ADD", "IS", "01"},
    {"SUB", "IS", "02"},
    {"MULT", "IS", "03"},
    {"MOVER", "IS", "04"},
    {"MOVEM", "IS", "05"},
    {"COMP", "IS", "06"},
    {"BC", "IS", "07"},
    {"DIV", "IS", "08"},
    {"READ", "IS", "09"},
    {"PRINT", "IS", "10"},
    {"START", "AD", "01"},
    {"END", "AD", "02"},
    {"ORIGIN", "AD", "03"},
    {"EQU", "AD", "04"},
    {"LTORG", "AD", "05"},
    {"DC", "DL", "01"},
    {"DS", "DL", "02"},
};
int getId(string opcode) {
    for(int i=0; i<18; i++) {
        if(optab[i].opcode == opcode) {
            return i;
        }
    }
    return 1;
}


int scnt = 0;
int lcnt = 0;

struct stentry
{
    int no;
    string name, addr;
};
struct stentry stab[10];
bool ispresentsym(string sym) {
    for(int i=0; i<10; i++) {
        if(stab[i].name == sym) {
            return true;
        }
    }
    return false;
}
int getsymid(string sym) {
    for(int i=0; i<10; i++) {
        if(stab[i].name == sym) {
            return i;
        }
    }
    return -1;
}
void printstab() {
    cout<<"Symbol table"<<endl;
    for(int i=0; i<scnt; i++) {
        cout<<stab[i].no<<"-"<<stab[i].name<<"-"<<stab[i].addr<<endl;
    }
    cout<<endl;
}

struct ltentry
{
    int no;
    string name, addr;
};
struct ltentry ltab[10];
int getlitid(string lit) {
    for(int i=0; i<10; i++) {
        if(ltab[i].name == lit) {
            return i;
        }
    }
    return -1;
}
void printltab() {
    cout<<"Literal table"<<endl;
    for(int i=0; i<lcnt; i++) {
        cout<<ltab[i].no<<"-"<<ltab[i].name<<"-"<<ltab[i].addr<<endl;
    }
    cout<<endl;
}

string getregid(string reg) {
    if(reg == "AREG") {
        return "1";
    } else if(reg == "BREG") {
        return "2";
    } else if(reg == "CREG") {
        return "3";
    } else if(reg == "DREG") {
        return "4";
    } else {
        return "-1";
    }
}
string getccid(string cc) {
    if(cc == "LT") {
        return "1";
    } else if(cc == "LE") {
        return "2";
    } else if(cc == "EQ") {
        return "3";
    } else if(cc == "GT") {
        return "4";
    } else if(cc == "GE") {
        return "5";
    }  else if(cc == "ANY") {
        return "6";
    }  else {
        return "-1";
    }
}


int main() {
    ifstream fin("input.txt");
    ofstream ic("ic.txt");
    int LC;
    string label, opcode, op1, op2;
    int nlcnt = 0;

    while(!fin.eof()) {

        string IC, lc;
        fin >> label >> opcode >> op1 >> op2;
        cout<<opcode<<"start - ";
        int id = getId(opcode);
        IC = "\t("+optab[id].mclass+","+optab[id].mneumonic+")";

        if(opcode == "START") {
            lc = "---";
            IC += "\t(C,"+op1+")\tNAN";
            LC = stoi(op1);
        }
        if(opcode == "EQU") {
            lc = "---";
            if(ispresentsym(label)) {
                stab[getsymid(label)].addr = stab[getsymid(op1)].addr;
            } else {
                stab[scnt].no = scnt+1;
                stab[scnt].name = label;
                stab[getsymid(label)].addr = stab[getsymid(op1)].addr;
                scnt++;
            }
            IC += "\tNAN\tNAN";
        }
        else if(label != "NAN") {
            if(!ispresentsym(label)) {
                stab[scnt].no = scnt+1;
                stab[scnt].name = label;
                scnt++;
            }
            stab[getsymid(label)].addr = to_string(LC);
        }
        if(opcode == "ORIGIN") {
            string token1, token2;
            char op;
            stringstream ss(op1);
            int pos = op1.find('+');
            if(pos == string::npos) {
                op = '-';
            } else {
                op = '+';
            }

            getline(ss, token1, op);
            getline(ss, token2, op);
            cout<<token1<<"-"<<token2<<"-";
            string string_address = stab[getsymid(token1)].addr;
            cout<<string_address<<"-";
            int addr = stoi(string_address);
            int inc = stoi(token2);
            if(op == '+') {
                LC = addr+inc;
            } else {
                LC = addr-inc;
            }
            lc = "---";
            IC += "\t(S,0"+to_string(stab[getsymid(token1)].no)+")"+op+token2+"\tNAN";
        }
        if(opcode == "LTORG") {
            for(int i=lcnt-nlcnt; i<lcnt; i++) {
                lc = to_string(LC);
                IC = "\t(DL,01)\t(C,"+(ltab[i].name).substr(2, 1)+")\tNAN";
                ltab[i].addr = lc;
                ic << lc << IC << endl;
                LC++;
            }
            nlcnt = 0;
            continue;
        }
        if(opcode == "END") {
            lc = "---";
            IC += "\tNAN\tNAN";
            ic << lc << IC << endl;
            for(int i=lcnt-nlcnt; i<lcnt; i++) {
                lc = to_string(LC);
                IC = "\t(DL, 01)\t(C,"+(ltab[i].name).substr(2, 1)+")\tNAN";
                ltab[i].addr = lc;
                ic << lc << IC << endl;
                LC++;
            }
            nlcnt = 0;
            break;
        }
        if(opcode == "DS" || opcode == "DC") {
            lc = to_string(LC);
            IC += "\t(C,"+op1+")\tNAN";
            if(opcode == "DS") {
                LC += stoi(op1);
            } else {
                LC++;
            }
        }

        if(opcode!="START" && opcode!="END" && opcode!="ORIGIN" && opcode!="EQU" && opcode!="LTORG" && opcode!="DC" && opcode!="DS") {
            cout<<"inside imperative statements condition - ";
            if(op2 == "NAN") {
                if(op1 == "NAN") {
                    IC += "\tNAN\tNAN";
                } else {
                    cout<<"hello"<<endl;
                }
            } else {
                if(opcode == "BC") {
                    IC += "("+getccid(op1)+")";
                } else {
                    IC += "("+getregid(op1)+")";
                }

                if(op2[0] == '=') {
                    ltab[lcnt].no = lcnt+1;
                    ltab[lcnt].name = op2;
                    lcnt++;
                    nlcnt++;
                    IC += "\t(L,0"+to_string(ltab[getlitid(op2)].no)+")";
                } else {
                    if(!ispresentsym(op2)) {
                        stab[scnt].no = scnt+1;
                        stab[scnt].name = op2;
                        scnt++;
                    }
                    IC += "\t(S,0"+to_string(stab[getsymid(op2)].no)+")";
                }
            }
            lc = to_string(LC);
            LC++;
        }

        ic << lc << IC <<endl;
        cout<<opcode<<"done"<<endl;

        printltab();
        printstab();

    }
    ic.close();

    ofstream st("st.txt");
    for(int i=0; i<scnt; i++) {
        st << stab[i].no << "\t" << stab[i].name << "\t" << stab[i].addr << endl;
    }
    st.close();

    ofstream lt("lt.txt");
    st << "NO" << "\tNAME" << "\tADDR" << endl;
    for(int i=0; i<lcnt; i++) {
        lt << ltab[i].no << "\t" << ltab[i].name << "\t" << ltab[i].addr << endl;
    }
    lt.close();

    return 0;
}