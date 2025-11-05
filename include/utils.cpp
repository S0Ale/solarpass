#include <iostream> 
//#include <unistd.h>
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

// Clipboard method
bool copyToClip(const std::string& text){
	if(isWayland()){
		std::string cmd = "printf %s \"" + text + "\" | wl-copy";
		int res = system(cmd.c_str());
		return res == 0;
	}

	return clip::set_text(text);
}
