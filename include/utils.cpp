#include <iostream> 
#include <string>
#include <chrono>
#include <thread>
#include <sodium/utils.h>
#include <sodium/randombytes.h>
#include "../clip/clip.h" 
#include "utils.h"

// Exit from the program with an error message
void exitWithError(const char* msg){
	std::cerr << "solarpass: error: " << msg << std::endl;
	exit(1);
}

// Check if we are on Wayland
bool isWayland() {
    const char* wayland = std::getenv("WAYLAND_DISPLAY");
    return (wayland && *wayland);
}

// Get a random character from a base pool
char getRandChar(std::string basePool) {
    unsigned char index;
	randombytes_buf(&index, 1); 
    return basePool[index % basePool.length()];
}

// Clipboard method
bool copyToClip(const std::string& text){
	if(isWayland()){
		std::string cmd = "printf %s \"" + text + "\" | wl-copy";
		int res = system(cmd.c_str());
		return res == 0;
	}

	return clip::set_text(text);
}

// Wait 10 seconds and then clear clipboard
void waitForClear(){
	for(unsigned int i = 10; i > 0; i--){
		std::cout << "Clearing clipboard in " << i << "s..." << std::endl; 
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	if(isWayland()){
		std::string cmd = "wl-copy --clear";
		system(cmd.c_str());
	}else clip::clear();
	std::cout << "Clipboard cleared (but may still exist in clipboard history)." << std::endl;
}

// Cleanup password in memory
void pswCleanup(std:: string& psw){
	sodium_memzero(psw.data(), psw.size());
	psw.clear();
}
