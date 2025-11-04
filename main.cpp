#include <iostream> 
#include <sstream>
#include <string>
#include <unistd.h>
#include <sodium.h>
#include "clip/clip.h" 
using namespace std;

const char* gCMDNAME = nullptr;
const unsigned int MAX_LEN{1024};

// Print helper message
void printUsage(const char* name){
	cout << "Usage: " << name << " [-hp] <length>" << endl << endl;
	cout << "Description:" << endl;
	cout << " Randomly generate a new password with the specified length." << endl;
	cout << " It copies the result to the clipboard as default." << endl << endl;
	cout << "Options:" << endl;
	cout << " -p		Print password to terminal." << endl;
	cout << " -h		Display the help message." << endl;
	cout << endl;

	exit(0);
}

// Check if we are on Wayland
bool isWayland() {
    const char* wayland = std::getenv("WAYLAND_DISPLAY");
    return (wayland && *wayland);
}

// Exit from the program with an error message
void exitWithError(const char* msg){
	cerr << gCMDNAME << ": error: " << msg << endl;
	exit(1);
}

// Clipboard method
bool copyToClip(const string& text){
	if(isWayland()){
		string cmd = "printf %s \"" + text + "\" | wl-copy";
		int res = system(cmd.c_str());
		return res == 0;
	}

	return clip::set_text(text);
}

// Get random character from a pool
char getRandomCharacter() {
    const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    unsigned char index;
	randombytes_buf(&index, 1); 
    return characters[index % characters.length()];
}

// Pawword generation function
string generatePsw(const unsigned int len){
	if(len <= 0)
		exitWithError("Length cannot be zero or negative.");
	if(len > MAX_LEN)
		exitWithError("Password cannot be longer than 1024 characters.");

	string psw(len, ' ');
	unsigned int i = 0;
	for(; i < len; i++)
		psw[i] = getRandomCharacter();

	return psw;
}

int main(int argc, char *argv[]){
	gCMDNAME = argv[0];

	int opt;
	bool printToStd{false};

	while((opt = getopt(argc, argv, "hp")) != -1){
		switch(opt){
			case 'h':
				printUsage(argv[0]);	
				exit(0);
			case 'p':
				std::cout << "Printing the new password to stdin..." << endl;
				printToStd = true;
				break;
		}
	}

	if(argc <= 1){
		printUsage(argv[0]);
		exit(0);
	}
	if(optind >= argc)
		exitWithError("No length specified");

	char* arg = argv[optind];
	unsigned int len{};
	stringstream stream(arg);
	if (!(stream >> len) || !(stream.eof())){ 
		exitWithError("invalid length");
	}

	string newPsw = generatePsw(len);
	if(printToStd)
		cout << newPsw << endl;
	else{
		if(copyToClip(newPsw))
			cout << "Password copied to clipboard" << endl;
		else
			exitWithError("failed to copy to clipboard");
	}

	return 0;
}
