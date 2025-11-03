#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

void printUsage(char* name){
	cout << "Usage: " << name << " [-hp]" << endl << endl;
	cout << "Options: " << endl;
	cout << " -p		Print password to terminal." << endl;
	cout << " -h		Display the help message." << endl;
	cout << endl;

	exit(0);
}

int main(int argc, char *argv[]){
	int opt;
	bool printToStd{false};

	while((opt = getopt(argc, argv, "hp")) != -1){
		switch(opt){
			case 'h':
				printUsage(argv[0]);	
			case 'p':
				std::cout << "Print the new password to stdin..." << endl;
				printToStd = true;
				break;
		}
	}

	return 0;
}
