#include <iostream>
#include <fstream>
#include <chrono>
#

//using namespace cv;
using namespace std;

// global variables
    int Lcount = 0;
    bool timer_on = false;
    double total_time_in_s = 0; 
    float Navg = 0;
    std::chrono::steady_clock::time_point Tstart;
    std::chrono::steady_clock::time_point Tend;

int main(int argc, char **argv) {
        	// files to read data 
		string filename = "N_people.txt";
		ifstream fin;
		fin.open(filename);
		ofstream T_file("T_hours.txt");
		int N;
		int i;
		if(fin.is_open()){
			//for(i=0; i<50; ++i)
			while(1)
			{
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
    			fin >> N;
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
            		if (N > 0)  // N was >=1 successively for 5 reads
            		{
                		if( !timer_on)
                		{
                        		// start timer
                        		Tstart = std::chrono::steady_clock::now();
                        		timer_on = true;
                        		cout << "\n...........Turning on Timer!.........";
                		}
            		}
            		else if (N == 0) // N was '0' successively for 5 reads
            		{
                		if(timer_on)
                		{
                        		cout << "\n......Turning off Timer!....";
                        		Tend = std::chrono::steady_clock::now();
                        		std::chrono::duration<double> elapsed_seconds = Tend-Tstart;

                        		std::cout << "elapsed time: " << elapsed_seconds.count();
                        		total_time_in_s += elapsed_seconds.count();
                        		//Tfile.open();
                        		T_file << total_time_in_s << endl;
                        		//Tfile.close();
                        		timer_on = false;
                		}
	    		}
	    		}// end of infinite loop
    		}
		else
		{
			cout << "\nError: file could not be opened!!";
			exit(1);
		}
		
		fin.close();
		return 0;
	}
	
