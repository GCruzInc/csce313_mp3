/* 
    File: simpleclient.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/11

    Simple client main program for MP2 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#include <errno.h>
#include <unistd.h>

#include "reqchannel.H"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}


string local_request(string _request){
   if (_request.compare(0, 5, "hello") == 0) {
    return "hello to you too";
  }
  else if (_request.compare(0, 4, "data") == 0) {
    return int2string(rand() % 100);
  }
  else if (_request.compare(0, 9, "newthread") == 0) {
    return "data: No threads";
	
  }
  else {
    return "unknown request";
  }

}

void done(){
	printf("Program is complete");
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/


int main(int argc, char * argv[]) {
	atexit(done);
	
 pid_t pid = fork();
 if (pid == 0){
	cout << "CLIENT STARTED:" << endl;
	
	cout << "Establishing control channel... " << flush;
	RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
	cout << "done." << endl;
	
	
	 /*-- Start sending a sequence of requests */

 
    string reply1 = chan.send_request("hello");
	cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;
    
    string reply2 = chan.send_request("data Joe Smith");
    cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;

    string reply3 = chan.send_request("data Jane Smith");
	cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;
	
	

	for(int i = 0; i < 100; i++) {
	  string request_string("data TestPerson" + int2string(i));
	  string reply_string = chan.send_request(request_string);
	  cout << "reply to request " << i << ":" << reply_string << endl;
	}


	string reply4 = chan.send_request("quit");
    cout << "Reply to request 'quit' is '" << reply4 << endl;
	usleep(1000000);
	
  }
  else { 
    execv("dataserver", argv);
  }
  return 0;	
}
