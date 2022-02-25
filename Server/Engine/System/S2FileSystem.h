#pragma once

namespace s2 {
namespace S2FileSystem
{
	bool IsDirectory(const char* folder);
	bool CreateDirectory(const char* folder);

	void GetDrive(const char* path, char* drive);
	void GetDir(const char* path, char* dir);
	void GetFileName(const char* path, char* fileName);
	void GetExt(const char* path, char* ext);
}
}
