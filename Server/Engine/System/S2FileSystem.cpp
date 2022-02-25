#include "S2Engine.h"
#include "S2FileSystem.h"

namespace s2 {

bool S2FileSystem::IsDirectory(const char* folder)
{
    return PathIsDirectory(folder);
}

bool S2FileSystem::CreateDirectory(const char* path)
{
	return ::CreateDirectory(path, NULL);
}

void S2FileSystem::GetDrive(const char* path, char* drive)
{
	char dir[MAX_PATH];             // 디렉토리 경로
	char fileName[MAX_PATH];        // 파일명
	char ext[MAX_PATH];             // 확장자 명

	_splitpath_s(path, drive, MAX_PATH, dir, MAX_PATH, fileName, MAX_PATH, ext, MAX_PATH);
}

void S2FileSystem::GetDir(const char* path, char* dir)
{
	char drive[MAX_PATH];           // 드라이브 명
	char fileName[MAX_PATH];        // 파일명
	char ext[MAX_PATH];             // 확장자 명

	_splitpath_s(path, drive, MAX_PATH, dir, MAX_PATH, fileName, MAX_PATH, ext, MAX_PATH);
}

void S2FileSystem::GetFileName(const char* path, char* fileName)
{
	char drive[MAX_PATH];           // 드라이브 명
	char dir[MAX_PATH];             // 디렉토리 경로
	char ext[MAX_PATH];             // 확장자 명

	_splitpath_s(path, drive, MAX_PATH, dir, MAX_PATH, fileName, MAX_PATH, ext, MAX_PATH);
}

void S2FileSystem::GetExt(const char* path, char* ext)
{
	char drive[MAX_PATH];           // 드라이브 명
	char dir[MAX_PATH];             // 디렉토리 경로
	char fileName[MAX_PATH];        // 파일명
	
	_splitpath_s(path, drive, MAX_PATH, dir, MAX_PATH, fileName, MAX_PATH, ext, MAX_PATH);
}

}