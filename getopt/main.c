#include <Windows.h>
#include <tchar.h>

#include "getopt.h"

#ifdef LONGOPTION

int _tmain(int argc, TCHAR** argv)
{
	int c;
	int digit_optind = 0;

	while (1)
	{
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] =
		{
	  {_T("add"), 1, 0, 0},
	  {_T("append"), 0, 0, 0},
	  {_T("delete"), 1, 0, 0},
	  {_T("verbose"), 0, 0, 0},
	  {_T("create"), 0, 0, 0},
	  {_T("file"), 1, 0, 0},
	  {0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, _T("abc:d:0123456789"),
			long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
		{
		case 0:
			_tprintf(_T("option %s"), long_options[option_index].name);
			if (optarg)
				_tprintf(_T(" with arg %s"), optarg);
			_tprintf(_T("\n"));
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (digit_optind != 0 && digit_optind != this_option_optind)
				_tprintf(_T("digits occur in two different argv-elements.\n"));
			digit_optind = this_option_optind;
			_tprintf(_T("option %c\n"), c);
			break;

		case 'a':
			_tprintf(_T("option a\n"));
			break;

		case 'b':
			_tprintf(_T("option b\n"));
			break;

		case 'c':
			_tprintf(_T("option c with value `%s'\n"), optarg);
			break;

		case 'd':
			_tprintf(_T("option d with value `%s'\n"), optarg);
			break;

		case '?':
			break;

		default:
			_tprintf(_T("?? getopt returned character code 0%o ??\n"), c);
		}
	}

	if (optind < argc)
	{
		_tprintf(_T("non-option ARGV-elements: "));
		while (optind < argc)
			_tprintf(_T("%s "), argv[optind++]);
		_tprintf(_T("\n"));
	}

	exit(0);
}


#else

int _tmain(int argc, TCHAR **argv)
{
	int c;
	int digit_optind = 0;

	while (1)
	{
		int this_option_optind = optind ? optind : 1;

		c = getopt(argc, argv, _T("abc:d:0123456789"));
		if (c == -1)
			break;

		switch (c)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (digit_optind != 0 && digit_optind != this_option_optind)
				_tprintf(_T("digits occur in two different argv-elements.\n"));
			digit_optind = this_option_optind;
			_tprintf(_T("option %c\n"), c);
			break;

		case 'a':
			_tprintf(_T("option a\n"));
			break;

		case 'b':
			_tprintf(_T("option b\n"));
			break;

		case 'c':
			_tprintf(_T("option c with value `%s'\n"), optarg);
			break;

		case '?':
			break;

		default:
			_tprintf(_T("?? getopt returned character code 0%o ??\n"), c);
		}
	}

	if (optind < argc)
	{
		_tprintf(_T("non-option ARGV-elements: "));
		while (optind < argc)
			_tprintf(_T("%s "), argv[optind++]);
		_tprintf(_T("\n"));
	}

	exit(0);
}
#endif