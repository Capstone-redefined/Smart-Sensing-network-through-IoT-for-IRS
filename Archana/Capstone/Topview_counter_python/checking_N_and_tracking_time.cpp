#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <unistd.h>
//using namespace cv;
using namespace std;

// global variables
    unsigned int second = 1000000;
    int hours, minutes, seconds;
    int Lcount = 0;
    bool timer_on = false;
    double total_time_in_s = 0; 
    float Navg = 0;
    std::chrono::steady_clock::time_point Tstart;
    std::chrono::steady_clock::time_point Tend;

void convertSecToHours()
{
	// int seconds, hours, minutes;
	
	minutes = seconds / 60;
	hours = (int) minutes / 60;
	minutes = int(minutes%60);
	seconds = int(seconds%60);

}
int main(int argc, char **argv) {
        	// files to read data 
		string filename = "N_1value.txt";
		ifstream fin;
                int N;
                int i;
		if ( remove("T_hours.txt") !=0 )
			perror("Error deleting T_hours.txt!!");
		else
			puts("T_hours.txt successfully deleted");

                //while(1)
		for(i=0; i<65; ++i)
                {

		fin.open(filename);
		

		if(fin.is_open()){
			//
/*
 			//Got to the last character before EOF
    			fin.seekg(-1, std::ios_base::end);
    			if(fin.peek() == '\n')
    			{
      				//Start searching for \n occurrences
      				fin.seekg(-1, std::ios_base::cur);
      				int i = fin.tellg();
      				for(i;i > 0; i--)
      				{
        				if(fin.peek() == '\n')
        				{
          					//Found
          					fin.get();
          					break;
        				}
        				//Move one character back
        				fin.seekg(i, std::ios_base::beg);
      				}
    			}

  */  			fin >> N;
			fin.close();
    			std::cout << endl << "Current No. of people in the zone : " << N << std::endl;
    			/*
    			Navg += N;
    			++Lcount;
    			if(Lcount = 5)
            		{
                		Navg /= 5;
                		Lcount = 0;

            		} 
            		*/          
            		if (N > 0)  
            		{
                		if( !timer_on)
                		{
                        		// start timer
                        		Tstart = std::chrono::steady_clock::now();
                        		timer_on = true;
                        		cout << "\n...........Turning on Timer!.........";
                		}
            		}
            		else if (N == 0) 
            		{
                		if(timer_on)
                		{
                        		cout << "\n......Turning off Timer!....";
                        		Tend = std::chrono::steady_clock::now();
                        		std::chrono::duration<double> elapsed_seconds = Tend-Tstart;

                        		std::cout << "elapsed time: " << elapsed_seconds.count();
                        		seconds += elapsed_seconds.count();
					convertSecToHours();
                        		//Tfile.open();
                        		ofstream T_file;
					T_file.open("T_hours.txt", std::ios_base::app);
                        		T_file << hours <<":" <<  minutes << ":" << seconds << endl;
                        		T_file.close();
                        		timer_on = false;
					system("./ble_write");
                		}
	    		}
	    	}
		else
		{
			cout << "\nError: file could not be opened!!";
			exit(1);
		}
		
		usleep(0.5*second);	
    		}
		
		
		return 0;
	}
	
