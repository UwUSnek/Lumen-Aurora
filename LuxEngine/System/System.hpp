#pragma once
#define LUX_H_SYSTEM
#include <filesystem>
#include <stdio.h>
#include <thread>
#include "LuxEngine/Types/Containers/LuxString.hpp"




#if !defined __GNUC__ || !defined __cplusplus		//Use only gcc
#	error gcc compiler required
#endif


#ifdef _WIN64														//Windows
#	include <direct.h>													//  DIR | For _getcwd()
#	define __lp_get_cwd _getcwd											//  DIR | Get current working directory
#	define __lp_get_nopt(n)	n = std::thread::hardware_concurrency();	//  THR | Get number of physical threads
#	include <windows.h>													//  THR | For SuspendThread() and ResumeThread()
#	define __lp_suspend_thr(th) SuspendThread(th)						//  THR | Function to suspend a thread
#	define __lp_resume_thr(th)  ResumeThread(th)						//  THR | Function to resume a thread
#	define __lp_get_thr()  GetCurrentThread()						//  THR | Function to get the current thread ID
#	define win(...) __VA_ARGS__
#	define linux(...)

#elif defined __linux__												//Linux
#	include <unistd.h>													//  DIR | For getcwd()
#	define __lp_get_cwd getcwd											//  DIR | Get current working directory
#	define __lp_get_nopt(n) n = std::thread::hardware_concurrency();	//  THR | Get nuber of physical threads
#	include <pthread.h>													//  THR | For SuspendThread() and ResumeThread()
#	include <signal.h>													//  THR | For SuspendThread() and ResumeThread()
#	define __lp_suspend_thr(th) pthread_kill(th, SIGSTOP)				//  THR | Function to suspend a thread
#	define __lp_resume_thr(th)  pthread_kill(th, SIGCONT)				//  THR | Function to resume a thread
#	define __lp_get_thr()  gettid()						//  THR | Function to get the current thread ID
#	define win(...)
#	define linux(...) __VA_ARGS__

#else																//Other operating systems
#	error Unsupported operating system
#endif








namespace lux::sys {
	namespace dir {
		extern String thisDir;
		void fixWindowsPath(lux::String& pStr);
		lux::String getExtensionFromPath(const lux::String& pStr);
		lux::String getFileNameFromPath(const lux::String& pStr);
	}

	extern uint32 threadNum;
	void init( );
}