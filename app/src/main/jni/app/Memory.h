#ifndef NATIVESURFACE_MEMREAD_H
#define NATIVESURFACE_MEMREAD_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include <dlfcn.h>
#include <map>
#include <array>
	// 定义圆周率
#define PI 3.141592653589793238
typedef unsigned int ADDRESS;
typedef char PACKAGENAME;
typedef unsigned short UTF16;
typedef char UTF8;
#define UNI_SUR_HIGH_START (UTF32)0xD800
#define UNI_SUR_HIGH_END (UTF32)0xDBFF
#define UNI_SUR_LOW_START (UTF32)0xDC00
#define UNI_SUR_LOW_END (UTF32)0xDFFF
	// Some fundamental constants
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF


int ProcessID; 
int GetProcessID(char bm[64]);
unsigned long BaseAddress, GWorld, GName, GEngine, GUObjectArray;

// syscall内存读写
#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

ssize_t process_v(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count, const struct iovec *__remote_iov, unsigned long __remote_iov_count, unsigned long __flags, bool iswrite) {
	return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

// Process Memory
bool ProcessMemory(void *address, void *buffer, size_t size, bool iswrite)
{
	struct iovec local[1];
	struct iovec remote[1];
	local[0].iov_base = buffer;
	local[0].iov_len = size;
	remote[0].iov_base = address;
	remote[0].iov_len = size;
	if (ProcessID < 0)
	{
		return false;
	}
	ssize_t bytes = process_v(ProcessID, local, 1, remote, 1, 0, iswrite);
	return bytes == size;
}

// Read Memory
bool _Read(unsigned long address, void *buffer, size_t size) {
	return ProcessMemory(reinterpret_cast<void*>(address), buffer, size, false);
}
// Write Memory
bool _Write(unsigned long address, void *buffer, size_t size) {
	return ProcessMemory(reinterpret_cast<void*>(address), buffer, size, true);
}
// Read Template
template<typename ReadTemplate>
ReadTemplate Read(long Address) {
    ReadTemplate Data; 
    _Read(Address, reinterpret_cast<void*>(&Data), sizeof(ReadTemplate)); 
    return Data;
}
// Write Template
template<typename WriteTemplate>
void Write(long Address, WriteTemplate Data) { 
    _Write(Address, reinterpret_cast<void*>(&Data), sizeof(WriteTemplate));
}



void OpenMemory() {
   char Mem[64];
   sprintf(Mem, "/proc/%d/mem", ProcessID);
   int Handle = open(Mem, O_RDWR);
   if (Handle == 0) return;
}

// Function for obtaining the ProcessID
int GetProcessID(const char *packageName) {
	int id = -1;
	DIR *dir;
	FILE *fp;
	char filename[64];
	char cmdline[64];
	struct dirent *entry;
	dir = opendir("/proc");
	while ((entry = readdir(dir)) != NULL)
	{
		id = atoi(entry->d_name);
		if (id != 0)
		{
			sprintf(filename, "/proc/%d/cmdline", id);
			fp = fopen(filename, "r");
			if (fp)
			{
				fgets(cmdline, sizeof(cmdline), fp);
				fclose(fp);
				if (strcmp(packageName, cmdline) == 0)
				{
					return id;
				}
			}
		}
	}
	closedir(dir);
	return -1;
}
// Function for obtaining the base address of the game, used to offset from it
// if a game uses ASLR
unsigned long GetBaseAddress(int pid, const char *module_name) {
	FILE *fp;
	unsigned long addr = 0;
	char *pch;
	char filename[64];
	char line[1024];
	snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
	fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			if (strstr(line, module_name))
			{
				pch = strtok(line, "-");
				addr = strtoul(pch, NULL, 16);
				if (addr == 0x8000)
					addr = 0;
				break;
			}
		}
		fclose(fp);
	}
	return addr;
}
// Function to check if a package is running
bool IsPackageRunning(PACKAGENAME * bm) {
	DIR *dir = NULL;
	struct dirent *ptr = NULL;
	FILE *fp = NULL;
	char filepath[64];
	char filetext[64];
	dir = opendir("/proc/");
	if (dir != NULL)
	{
		while ((ptr = readdir(dir)) != NULL)
		{
			if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
				continue;
			if (ptr->d_type != DT_DIR)
				continue;
			sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
			fp = fopen(filepath, "r");
			if (NULL != fp)
			{
				fgets(filetext, sizeof(filetext), fp);
				if (strcmp(filetext, bm) == 0)
				{
					closedir(dir);
					return true;
				}
				fclose(fp);
			}
		}
	}
	closedir(dir);
	return false;
}
// Check to see if a Pointer / Address is valid
bool IsValidAddress(long addr) {
   if (addr != 0x0) return true;
   return false;
}
// Unreal Engines way to see if an Object belongs to a Class
bool IsA(long Actor, long StaticClass) {
    long ActorClass = Read<long>(Actor + 0x10);
    if (Read<int>(StaticClass + 0x90) <= Read<int>(ActorClass + 0x90  && Read<long>(Read<long>(ActorClass + 0x88) + Read<int>(StaticClass + 0x90) * 8) == StaticClass + 0x88)) {
        return true;
    } 
}
// checks if a string contains a string
bool ContainsString(const std::string& str, const char* check) {
    size_t found = str.find(check);
    return (found != std::string::npos);
}




#endif
