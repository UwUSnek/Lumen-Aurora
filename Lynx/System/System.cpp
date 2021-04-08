#include "Lynx/System/System.hpp"
#include "Lynx/Types/Containers/String.hpp"
#include "Lynx/Core/AutoInit.hpp"








namespace lnx::sys{
	String dir::thisDir;	//Path to the current directory //Initialized in init function


	LnxAutoInit(LNX_H_SYSTEM) {
		char buff[FILENAME_MAX];					//Create char array to store the path
		__lp_get_cwd(buff, FILENAME_MAX);			//Get path
		dir::thisDir = buff;						//Save path
		_wds(dir::fixWindowsPath(dir::thisDir);)	//Replace silly windows backslashes with normal slashes
	}








	namespace dir{
		//Replaces backslashes with normal slashes
		void fixWindowsPath(const String& pStr) {
			for (char8& i : pStr) {
				if (i == '\\') i = '/';
				//else if(i == ' ') i = '' //TODO spaces
			}
		}

		//Returns a lnx::String containing the extension of the file
		//e.g. getExtensionFromPath("/home/dir/file.txt") returns "txt"
		//Directories or files with no exention returns ""
		lnx::String getExtensionFromPath(const lnx::String& pStr) {
			uint32 i = pStr.count() - 1;
			for(; pStr[i] != '.' && i > 0; --i) if(pStr[i] == '/') return lnx::String("");
			return lnx::String(pStr.begin() + i + 1);
		}

		//Returns a lnx::String containing the name of the file
		//e.g. getFileNameFromPath("/home/dir/file.txt") returns "file.txt"
		lnx::String getFileNameFromPath(const lnx::String& pStr) {
			uint32 i = pStr.count() - 1;
			for(; pStr[i] != '/' && i > 0; --i);
			return lnx::String(pStr.begin() + i + 1);
		}
	}
}



