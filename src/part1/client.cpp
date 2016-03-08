/*
 * CSCE-313-502, Dr. A.Tyagi
 * Machine Problem 3: Unix Processes 
 * By Eric Chung & Kyle Preston
 *
 * Filename: client.cpp
 * Date: 03/07/2016
 *
 *
 *
 *
 * -------------------------------------------------------------------------------- //
 */
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>

#include <cmath>
#include <chrono> // Added to measure time of requests
#include <iomanip> // For setw()
#include <vector>

#include "reqchannel.h"
using namespace std;




// HELPER FUNCTION(S)
// -------------------------------------------------------------------------------- //

// Casts type int to type string 
string int2string(int number) {
    stringstream ss; // create a stringstream
    ss << number; // add number to the stream
    return ss.str(); // return a string with the contents of the stream
}


int cwrite(string msg) {
    const char *s = msg.c_str();
    return 1;
}


void hello(RequestChannel &channel, const string &request) {
    cout << "LOCAL: hello to you too!" << endl;
}


void data(RequestChannel &channel, const string &request) {
    cwrite(int2string(rand() % 100));
}


void local_process_request(RequestChannel &channel, const string &request) {
    if (request.compare(0, 5, "hello") == 0)
        hello(channel, request);
    else if (request.compare(0, 4, "data") == 0)
        data(channel, request);
    else 
        cwrite("unknown request");
}


vector<long double> calc_stats(int num_req, vector<double> req_times, long double avg, long double stdev, vector<long double> calc) {
    for(int i = 0; i < num_req; i++)
        avg += req_times[i];

    avg = avg/num_req;

    for(int i = 0; i < num_req; i++)
        stdev += ((req_times[i]-avg)*(req_times[i]-avg));

    stdev = sqrt(stdev/(num_req-1));

    calc.push_back(avg);
    calc.push_back(stdev);

    return calc;
}




// MAIN FUNCTION 
// -------------------------------------------------------------------------------- //
int main(int argc, char * argv[]) {
    if (fork() == 0)
        //this process is the 'child', so run the dataserver
        system("./dataserver");

    else {
        int num_requests = 1000;
        double run_time,
               run_time_local;
        vector<double> request_times, request_times_local;

        cout << "CLIENT STARTED:" << endl;

        cout << "Establishing control channel... " << flush;
        RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
        cout << "done." << endl;

        // Declare execution time handling variables
        // Time at start of server request
        std::chrono::high_resolution_clock::time_point pre_request_time,
            post_request_time,        // Time at end of server request
            pre_local_request_time,   // Time at start of local request
            post_local_request_time;  // Time at end of local request


        /* -- Start sending a sequence of requests */

        string reply1 = chan.send_request("hello");
        cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;

        string reply2 = chan.send_request("data Joe Smith");
        cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;

        string reply3 = chan.send_request("data Jane Smith");
        cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;

        for(int i = 0; i < num_requests; i++) {
            string request_string("data TestPerson" + int2string(i));

            // Send request to dataserver
            pre_request_time = std::chrono::high_resolution_clock::now();
            string reply_string = chan.send_request(request_string);
            post_request_time = std::chrono::high_resolution_clock::now();

            // Capture dataserver execution time and same to array
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>( post_request_time - pre_request_time ).count();
            run_time += duration;
            request_times.push_back(duration);

            // Print out the details of the request response
            cout << "reply to request took " << duration << " microseconds round-trip " << i << ":" << reply_string << endl;;

            // Handle request locally as if it were the dataserver
            pre_local_request_time = std::chrono::high_resolution_clock::now();
            local_process_request(chan, request_string);
            post_local_request_time = std::chrono::high_resolution_clock::now();

            // Calculate the time it takes, in order to compare with server request
            auto local_duration = std::chrono::duration_cast<std::chrono::microseconds>( post_local_request_time - pre_local_request_time ).count();
            run_time_local += local_duration;
            request_times_local.push_back(local_duration);
        }

        string reply4 = chan.send_request("quit");
        cout << "Reply to request 'quit' is '" << reply4 << "'"<<endl;

        usleep(1000000);

        // Calculate time related information for server requests
        long double avg = 0.0, stdev = 0.0, avg_local = 0.0, stdev_local = 0.0;
        vector<long double> calc, calc_local;

        calc = calc_stats(num_requests, request_times, avg, stdev, calc);
        calc_local = calc_stats(num_requests, request_times_local, avg_local, stdev_local, calc_local);
        

        // Print out server request results
        cout << " -------------------------------------------- " << endl << flush
            << "|                Server Results              |" << endl << flush
            << "|--------------------------------------------|" << endl << flush
            << "| Run-time:         |" << setw(20) << run_time/1000 << " ms |" << endl << flush
            << "| # of requests     |" << setw(23) << num_requests << " |" << endl << flush
            << "| Avg. Request Time |" << setw(23) << calc[0]<< " |" << endl << flush
            << "| Stand. Deviation  |" << setw(23) << calc[1]<< " |" << endl << flush
            << " -------------------------------------------- " << endl << flush;


        // Print out local request results
        cout << " -------------------------------------------- " << endl << flush
            << "|                Local Results               |" << endl << flush
            << "|--------------------------------------------|" << endl << flush
            << "| Run-time:         |" << setw(20) << run_time_local/1000 << " ms |" << endl << flush
            << "| # of requests     |" << setw(23) << num_requests << " |" << endl << flush
            << "| Avg. Request Time |" << setw(23) << calc_local[0]<< " |" << endl << flush
            << "| Stand. Deviation  |" << setw(23) << calc_local[1]<< " |" << endl << flush
            << " -------------------------------------------- " << endl << flush;
    }
}
