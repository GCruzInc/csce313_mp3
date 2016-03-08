/*
 * CSCE-313-502, Dr. A.Tyagi
 * Machine Problem 3: Unix Processes 
 * By Eric Chung & Kyle Preston
 *
 * Filename: proctest.cpp
 * Date: 03/07/2016
 *
 *
 *
 *
 * -------------------------------------------------------------------------------- //
 */
#include <sstream>
#include <cctype>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
using namespace std;




// HELPER FUNCTION(S)
// -------------------------------------------------------------------------------- //
enum stat_index 
{
    PID, 
    STATE, 
    PPID, 
    UTIME, 
    STIME, 
    CUTIME, 
    CSTIME, 
    PRIORITY, 
    NICE, 
    NUM_THREADS,
    ITREALVALUE, 
    STARTCODE, 
    ENDCODE, 
    KSTKESP, 
    KSTKEIP,
    PROCESSOR, 
};


string int2string(int number) {
    stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}


bool valid_pid_entry(string s) {
    for(int i = 0; i < s.size(); i++) {
        char input = s[i];
        if(input < 48 || input > 57) {
            ///i.e. char is not between 0-9
            return false;
        }
    }
    return true;
}


string my_tolower(string s) {
    for (int i=0; s[i]; i++) 
        s[i] = tolower(s[i]);
    return s;
}


vector<string> &split(const string &s, char delim,vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if (item.length() > 0) {
            elems.push_back(item);  
        }
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}


int buffer_menu() {
    cout << "\n----------------------------------------\n";
    cout << "To view a list of processes enter 'ps'\n";
    cout << "To quit enter 'q'\n";
    cout << "To clear the terminal enter 'clear'\n";
    cout << "----------------------------------------\n";
    cout << "Please enter a process id: ";
    string input;
    cin >> input;
    cout << endl;
    input = my_tolower(input);

    if (input == "ps") {
        system("clear");
        system("ls /proc");
        return buffer_menu();
    }
    else if (input == "q") {
        system("clear");
        cout << "Bye!" << endl;
        return -1;
    }
    else if (valid_pid_entry(input)) {
        system("clear");
        int pid = atoi(input.c_str());
        cout << "Returned pid: " << pid << endl;
        return pid; 
    }
    else {
        cout << "Error! Invalid input. Please try again.\n";
        return buffer_menu();
    }
}




// MAIN FUNCTION 
// -------------------------------------------------------------------------------- //
int main(int argc, char * argv[]) {
    int pid = buffer_menu();

    while(pid != -1) {
        ifstream stat,
                 status,
                 maps;

        stat.open("/proc/" + int2string(pid) + "/stat");
        status.open("/proc/" + int2string(pid) + "/status");

        if(stat && status) {
            cout << "\n/proc/" << pid << "/stat opened..." << endl << endl;
            string raw_input,
                   buffer;

            while(getline(stat, buffer))
                raw_input += buffer;

            cout << "/proc/" << pid << "/stat raw input:" << endl;
            cout << raw_input << endl << endl;

            istringstream iss(raw_input);
            istream_iterator<string> beg(iss), end;

            vector<string> stat_values(beg, end);

            cout << "/proc/" << pid << "/status opened..." << endl << endl;

            raw_input = "";

            vector <vector <string> > status_values;

            while(getline(status, buffer)) {
                vector<string> split_vec = split(buffer, ':');
                string info = split_vec[1];
                istringstream iss2(info);
                istream_iterator<string> beq(iss2), end;
                vector<string> buffer_vec(beq, end);
                status_values.push_back(buffer_vec);
                raw_input += buffer + '\n';
            }

            cout<< "/proc/" << pid << "/status raw input:" << endl
                << raw_input << endl 
                << endl
                << "Raw input parsed..." << endl 
                << endl;

            //Outputing data
            cout<<"REQUESTED DATA" << endl 
                << endl
                << "1) Identifiers" << endl
                << "Process ID (PID): " << stat_values.at(PID) << endl
                << "Parent process ID (PPID): " << stat_values.at(PPID) << endl
                << "Effective user ID (EUID): " << status_values.at(6).at(1) << endl
                << "Effective group ID (EGID): " << status_values.at(7).at(1) << endl
                << "Real user ID (RUID): " << status_values.at(6).at(0) << endl
                << "Real group ID (RGID): " << status_values.at(7).at(0) << endl
                << "User file system ID (FSUID): " << status_values.at(6).at(3) << endl
                << "Group file system ID (FSGID): " << status_values.at(7).at(3) << endl 
                << endl;

            cout<< "2) State" << endl;
            string state_val = stat_values.at(STATE);
            cout << "State (STATE): " << state_val;
            if (state_val == "R")	
                cout << " Running\n\n";
            else if (state_val == "S")	
                cout << " (Sleeping)\n\n";
            else if (state_val == "D")	
                cout << " (Disk sleeping)\n\n";
            else if (state_val == "T")	
                cout << " (Stopped)\n\n";
            else if (state_val == "Z")	
                cout << " (Zombie)\n\n";
            else if (state_val == "X")	
                cout << " (Dead(x))\n\n";
            else	
                cout << "(Unkown)\n\n";

            cout<< "3) Thread Information" << endl
                << "Thread_info (NUM_THREADS): " << stat_values.at(NUM_THREADS) << endl << endl;	

            cout<< "4) Priorty" << endl
                << "Priorty number (PRIORITY): " << stat_values.at(PRIORITY) << endl
                << "Nice value (NICE): " << stat_values.at(NICE) << endl << endl;

            cout<< "5) Time Information" << endl
                << "Time in scheduler in kernal mode (STIME): " << stat_values.at(STIME) << endl
                << "Time in scheduler in user mode (UTIME): " << stat_values.at(UTIME) << endl
                << "Time waiting on children in kernal mode (CSTIME): " << stat_values.at(CSTIME) << endl
                << "Time waiting on children in user mode (CUTIME): " << stat_values.at(CUTIME) << endl 
                << endl;

            cout<< "6) Address Space" << endl
                << "Startcode(STARTCODE): " << stat_values.at(STARTCODE) << endl
                << "Endcode (ENDCODE): " << stat_values.at(ENDCODE) << endl
                << "ESP (KSTKESP): " << stat_values.at(KSTKESP) << endl
                << "EIP (KSTKEIP): " << stat_values.at(KSTKEIP) << endl 
                << endl;

            cout << "7) Resources" << endl
                << "Number of file descriptors (FDSize): " << status_values.at(8).at(0) << endl
                << "Number of voluntary context switches (vountary_ctxt_switches): " << status_values.at(37).at(0) << endl
                << "Number of nonvoluntary context switches (nonvountary_ctxt_switches): " << status_values.at(38).at(0) << endl 
                << endl;

            cout << "8) Processors" << endl
                << "Allowed processors: " << status_values.at(33).at(0) << endl
                << "Last executed processor (PROCESSOR): " << stat_values.at(PROCESSOR)  << endl 
                << endl;
        }

        else 
            cout<< "ERROR: pid " << pid << " could not be opened" << endl
                << "Note: This process may not exist, enter 'list' to find available processes" << endl;
        

        cout << "\nAttempting to access memory map..." << endl << endl;
        maps.open("/proc/" + int2string(pid) + "/maps");

        if (maps) {
            cout << "Memory map accessed..." << endl << endl;
            string buffer;
            string raw_input = "";
            int i = 0;

            while(getline(maps, buffer) && i < 3){
                raw_input += buffer + '\n';
                i++;
            }

            if (raw_input == "")
                cout << "Memory map is empty...\n";
            
            else {
                cout<< "9) Memory Map" << endl
                    << raw_input << endl << endl;
                ofstream out;
                out.open("out.txt");
                out << raw_input;
                out.close();
            }				
        }

        else
            cout<< "Error! Could not access memory map :/" << endl
                << "Note that the process may not be running" << endl;

        stat.close();
        status.close();
        maps.close();
        pid = buffer();
    }

    cout << "Program terminated." << endl;
    return 0;
}
