#include <sstream>
#include <cctype>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
using namespace std;

enum stat_index {PID, COMM, STATE, PPID, PGRP, SESSION, TTY_NR, TPGID, FLAGS, MINFLT, CMINFLT, 
				   MAJFLT, CMAJFLT, UTIME, STIME, CUTIME, CSTIME, PRIORITY, NICE, NUM_THREADS,
				   ITREALVALUE, STARTTIME, VSIZE, RSS, RSSLIM, STARTCODE, ENDCODE, STARTSTACK,
				   KSTKESP, KSTKEIP, SIGNAL, BLOCKED, SIGIGNORE, SIGCATCH, WCHAN, NSWAP, ONSWAP,
				   EXIT_SIGNAL, PROCESSOR, RT_PRIORITY, POLICY, DELAYACCT_BLKIO_TICKS, GUEST_TIME,
				   CGUEST_TIME};
				   
bool valid_pid_entry(string s){
	for(int i = 0; i < s.size(); i++){
		char buffer = s[i];
		if(buffer < 48 || buffer > 57){
			///i.e. char is not between 0-9
			return false;
		}
	}
	return true;
}
string to_lower(string s){
	for (int i=0; s[i]; i++) s[i] = tolower(s[i]);
	return s;
}
string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
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

int prompt_pid(){
	cout << "\n----------------------------------------\n";
	cout << "To view a list of processes enter 'list'\n";
	cout << "To quit enter 'quit'\n";
	cout << "To clear the terminal enter 'clear'\n";
	cout << "----------------------------------------\n";
	cout << "Please enter a process id: ";
	string input;
	cin >> input;
	cout << endl;
	input = to_lower(input);
	if(input == "list"){
		system("ls /proc");
		return prompt_pid();
	}else if(input == "quit"){
		cout << "Quitting..." << endl;
		return -1;
	}else if(input == "clear"){
		system("clear");
		return prompt_pid();
	}else if(valid_pid_entry(input)){
		int pid = atoi(input.c_str());
		cout << "Pid: " << pid << " submited..." << endl;
		return pid; 
	}else{
		cout << "ERROR: Invalid input... \n";
		return prompt_pid();
	}
	
}

int main(int argc, char * argv[]) {
	int pid = prompt_pid();
	
	//maps 
	//status
	
	while(pid != -1){
		ifstream stat;
		ifstream status;
		ifstream maps;
		stat.open("/proc/" + int2string(pid) + "/stat");
		status.open("/proc/" + int2string(pid) + "/status");
		if(stat && status){
			cout << "\n/proc/" << pid << "/stat opened..." << endl << endl;
			string raw_input;
			string buffer;
			while(getline(stat, buffer)){
				raw_input += buffer;
			}
			cout << "/proc/" << pid << "/stat raw input:" << endl;
			cout << raw_input << endl << endl;
			istringstream iss(raw_input);
			istream_iterator<string> beg(iss), end;
			vector<string> stat_values(beg, end);
			
			cout << "/proc/" << pid << "/status opened..." << endl << endl;
			raw_input = "";
			vector<vector<string>> status_values;
			while(getline(status, buffer)){
				vector<string> split_vec = split(buffer, ':');
				string info = split_vec[1];
				istringstream iss2(info);
				istream_iterator<string> beq(iss2), end;
				vector<string> buffer_vec(beq, end);
				status_values.push_back(buffer_vec);
				raw_input += buffer + '\n';
			}
			cout << "/proc/" << pid << "/status raw input:" << endl;
			cout << raw_input << endl << endl;
			
			
			
			cout << "Raw input parsed..." << endl << endl;
			
			//Outputing data
			cout <<"*****REQUESTED DATA*****" << endl << endl;
						
			cout << "1) Identifiers" << endl; 
			cout << "Process ID (PID): " << stat_values.at(PID) << endl;
			cout << "Parent process ID (PPID): " << stat_values.at(PPID) << endl;
			cout << "Effective user ID (EUID): " << status_values.at(6).at(1) << endl;
			cout << "Effective group ID (EGID): " << status_values.at(7).at(1) << endl;
			cout << "Real user ID (RUID): " << status_values.at(6).at(0) << endl;
			cout << "Real group ID (RGID): " << status_values.at(7).at(0) << endl;
			cout << "User file system ID (FSUID): " << status_values.at(6).at(3) << endl;
			cout << "Group file system ID (FSGID): " << status_values.at(7).at(3) << endl << endl;
			
			cout << "2) State" << endl;
			string state_val = stat_values.at(STATE);
			cout << "State (STATE): " << state_val;
			if(state_val == "R")	cout << " Running\n\n";
			else if(state_val == "S")	cout << " (Sleeping)\n\n";
			else if(state_val == "D")	cout << " (Disk sleeping)\n\n";
			else if(state_val == "T")	cout << " (Stopped)\n\n";
			else if(state_val == "Z")	cout << " (Zombie)\n\n";
			else if(state_val == "X")	cout << " (Dead(x))\n\n";
			else	cout << "(Unkown)\n\n";
			
			cout << "3) Thread Information" << endl;
			cout << "Thread_info (NUM_THREADS): " << stat_values.at(NUM_THREADS) << endl << endl;	
			
			cout << "4) Priorty" << endl;
			cout << "Priorty number (PRIORITY): " << stat_values.at(PRIORITY) << endl;
			cout << "Nice value (NICE): " << stat_values.at(NICE) << endl << endl;
			
			cout << "5) Time Information" << endl;
			cout << "Time in scheduler in kernal mode (STIME): " << stat_values.at(STIME) << endl;
			cout << "Time in scheduler in user mode (UTIME): " << stat_values.at(UTIME) << endl;
			cout << "Time waiting on children in kernal mode (CSTIME): " << stat_values.at(CSTIME) << endl;
			cout << "Time waiting on children in user mode (CUTIME): " << stat_values.at(CUTIME) << endl << endl;
			
			cout << "6) Address Space" << endl;
			cout << "Startcode(STARTCODE): " << stat_values.at(STARTCODE) << endl;
			cout << "Endcode (ENDCODE): " << stat_values.at(ENDCODE) << endl;
			cout << "ESP (KSTKESP): " << stat_values.at(KSTKESP) << endl;
			cout << "EIP (KSTKEIP): " << stat_values.at(KSTKEIP) << endl << endl;
			
			cout << "7) Resources" << endl;
			cout << "Number of file descriptors (FDSize): " << status_values.at(8).at(0) << endl;
			cout << "Number of voluntary context switches (vountary_ctxt_switches): " << status_values.at(37).at(0) << endl;
			cout << "Number of nonvoluntary context switches (nonvountary_ctxt_switches): " << status_values.at(38).at(0) << endl << endl;
			
			cout << "8) Processors" << endl;
			cout << "Allowed processors (Cpus_allowed): " << status_values.at(33).at(0) << endl;
			cout << "Last executed processor (PROCESSOR): " << stat_values.at(PROCESSOR)  << endl << endl;
			
		}else{
			cout << "ERROR: pid " << pid << " could not be opened" << endl;
			cout << "Note: This process may not exist, enter 'list' to find available processes" << endl;
		}
		
		cout << "\nAttempting to access memory map..." << endl << endl;
		maps.open("/proc/" + int2string(pid) + "/maps");
		if(maps){
			cout << "Memory map accessed..." << endl << endl;
			string buffer;
			string raw_input = "";
			int i = 0;
			while(getline(maps, buffer) && i < 3){
				raw_input += buffer + '\n';
				i++;
			}
			if(raw_input == ""){
				cout << "Memory map is empty...\n";
			}else{
				cout << "9) Memory Map" << endl;
				cout << raw_input << endl << endl;
				ofstream out;
				out.open("out.txt");
				out << raw_input;
				out.close();
			}				
		}else{
			cout << "ERROR: Unable to access memory map\n";
			cout << "Note: process may not be running\n";
		}
		
		stat.close();
		status.close();
		maps.close();
		pid = prompt_pid();
	}
	
	cout << "Program terminated." << endl;
	return 0;
}