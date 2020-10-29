
#include "LuxEngine/System/System.h"
#include "LuxEngine/Types/Containers/LuxString.h"

#include <new>
//TODO remove nothing constructors
//TODO remove LuxNoInit and VarNoInit macros

lux::String lux::sys::dir::thisDir;	//Path to the current directory //Initialized in init function
//TODO move to lux::thr
// uint32		lux::sys::threadNum = lux::sys::threadNum;						//Number of threads in the main CPU
uint32		lux::sys::threadNum;						//Number of threads in the main CPU




namespace lux::sys{
	namespace dir{
		//Replaces backslashes with normal slashes
		void fixWindowsPath(String& pStr) {
			//pStr.empty();
			//pStr.begin();
			//pStr.end();
			for (char8& i : pStr) {
				if (i == '\\') i = '/';
				//else if(i == ' ') i = '' //TODO spaces
			}
		}

		//Returns a lux::String containing the extension of the file
		//e.g. getExtensionFromPath("/home/dir/file.txt") returns "txt"
		//Directories or files with no exention returns ""
		lux::String getExtensionFromPath(const lux::String& pStr) {
			uint32 i = pStr.count( ) - 1;
			for(; pStr[i] != '.' && i > 0; --i) if(pStr[i] == '/') return lux::String("");
			return lux::String(pStr.begin( ) + i + 1);
		}

		//Returns a lux::String containing the name of the file
		//e.g. getFileNameFromPath("/home/dir/file.txt") returns "file.txt"
		lux::String getFileNameFromPath(const lux::String& pStr) {
			uint32 i = pStr.count( ) - 1;
			for(; pStr[i] != '/' && i > 0; --i);
			return lux::String(pStr.begin( ) + i + 1);
		}
	}


	void init( ) {
		static bool once = true;
		if(once) {								//Execute only once
			once = false;
			char buff[FILENAME_MAX];				//Create char array to store the path
			__lp_get_cwd(buff, FILENAME_MAX);		//Get path
			// dir::thisDir.String::String(lux::String(buff));		//Save path
			dir::thisDir = String(lux::String(buff));		//Save path
			dir::fixWindowsPath(dir::thisDir);		//Replace silly windows backslashes with normal slashes
			__lp_get_nopt(threadNum);				//Get number of physical threads
		}
	}
}