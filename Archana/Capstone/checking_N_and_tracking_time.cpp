#include <iostream>
#include <fstream>
#include <chrono>

using namespace cv;
using namespace std;

// global variables
    int count = 0;
    bool timer_on = false;
    double total_time_in_s = 0; 
    float Navg = 0;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;

int main(int argc, char **argv) {
        // files to write data 
	    ofstream N_file("Npeople.txt");
	    ofstream T_file("Toccupied.txt");
            Navg += num_people;
            ++count;

            if(count = 10)
            {
                Navg /= 10;
                count = 0;

            }           
            if (Navg >= 1)  // N was >=1 successively for 10 reads
            {
                if( !timer_on)
                {
                        // start timer
                        start = std::chrono::steady_clock::now();
                        timer_on = true;
                        cout << "\n...........Turning on Timer!.........";
                }
            }
            else if (Navg == 0) // N was '0' successively for 10 reads
            {
                if(timer_on)
                {
                        cout << "\n......Turning off Timer!....";
                        end = std::chrono::steady_clock::now();
                        std::chrono::duration<double> elapsed_seconds = end-st$

                        std::cout << "elapsed time: " << elapsed_seconds.count$
                        total_time_in_s += elapsed_seconds.count();
                        //Tfile.open();
                        T_file << total_time_in_s << endl;
                        //Tfile.close();
                        timer_on = false;
                }
	    }
