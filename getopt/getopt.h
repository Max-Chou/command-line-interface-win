#pragma once
#include <windows.h>
#include <tchar.h>

extern PTCHAR optarg;
extern int optind;
extern int opterr;
extern int optopt;

struct option
{
	LPCTSTR name;
	int has_arg;
	int* flag;
	int val;
};

#define no_argument						0
#define required_argument				1
#define optional_argument				2

extern int getopt(int argc, TCHAR *const *argv, const TCHAR *shortopts);
extern int getopt_long(int argc, TCHAR *const *argv, const TCHAR *shortopts, const struct option* longopts, int* longind);

extern int _getopt_internal(
	int argc, TCHAR *const *argv,
	const TCHAR *shortopts,
	const struct option* longopts, int* longind,
	int long_only
);
