#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopt.h"

PTCHAR optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = '?';

static PTCHAR nextchar = NULL;
static int first_nonopt = 1;
static int last_nonopt = 1;

static void
exchange(TCHAR** argv)
{
	int bottom = first_nonopt;
	int middle = last_nonopt;
	int top = optind;
	TCHAR* temp;

	while (top > middle && middle > bottom)
	{
		if (top - middle > middle - bottom)
		{
			int len = middle - bottom;
			int i;

			for (i = 0; i < len; i++)
			{
				temp = argv[bottom + i];
				argv[bottom + i] = argv[top - (middle - bottom) + i];
				argv[top - (middle - bottom) + i] = temp;
			}

			top -= len;
		}
		else
		{
			int len = top - middle;
			int i;
			for (i = 0; i < len; i++)
			{
				temp = argv[bottom + i];
				argv[bottom + i] = argv[middle + i];
				argv[middle + i] = temp;
			}
			bottom += len;
		}
	}
	first_nonopt += (optind - last_nonopt);
	last_nonopt = optind;
}

int _getopt_internal(int argc, TCHAR *const *argv, const TCHAR *optstring,
	const struct option* longopts, int* longind, int long_only)
{
	optarg = NULL;
	if (nextchar == NULL || *nextchar == '\0')
	{
		if (last_nonopt > optind)
		{
			last_nonopt = optind;
		}

		if (first_nonopt > optind)
		{
			first_nonopt = optind;
		}

		if (first_nonopt != last_nonopt && last_nonopt != optind)
			exchange((TCHAR**)argv);
		else if (last_nonopt != optind)
			first_nonopt = optind;

		/* skip non-options */
		while (optind < argc && (argv[optind][0] != '-' || argv[optind][1] == '\0'))
			optind++;
		last_nonopt = optind;

		/* special ARGV-element `--' means permature end of options */
		if (optind != argc && !_tcscmp(argv[optind], _T("--")))
		{
			optind++;
			if (first_nonopt != last_nonopt && last_nonopt != optind)
				exchange((TCHAR**)argv);
			else if (first_nonopt == last_nonopt)
				first_nonopt = optind;
			last_nonopt = argc;

			optind = argc;
		}

		if (optind == argc)
		{
			if (first_nonopt != last_nonopt)
				optind = first_nonopt;
			return -1;
		}

		if (argv[optind][0] != '-' || argv[optind][1] == '\0')
		{
			optarg = argv[optind++];
			return 1;
		}
		nextchar = (argv[optind] + 1 + (longopts != NULL && argv[optind][1] == '-'));
	}

	/* long options */
	if (longopts != NULL
		&& (argv[optind][1] == '-'
			|| (long_only && (argv[optind][2] || !_tcschr(optstring, argv[optind][1])))))
	{
		TCHAR* nameend;
		const struct option* p;
		const struct option* pfound = NULL;
		int indfound = -1;
		int option_index;

		for (nameend = nextchar; *nameend && *nameend != '='; nameend++)
			;

		/* find the option */
		for (p = longopts, option_index = 0; p->name; p++, option_index++)
		{
			if (!_tcsncmp(p->name, nextchar, nameend - nextchar))
			{
				if ((unsigned int)(nameend - nextchar) == (unsigned int)_tcslen(p->name))
				{
					pfound = p;
					indfound = option_index;
					break;
				}
				else if (pfound == NULL)
				{
					pfound = NULL;
					indfound = option_index;
				}
			}
		}

		if (pfound != NULL)
		{
			option_index = indfound;
			optind++;
			if (*nameend)
			{
				if (pfound->has_arg)
				{
					optarg = nameend + 1;
				}
				else
				{
					if (opterr)
					{
						if (argv[optind - 1][1] == '-')
						{
							_ftprintf(stderr, _T("%s: option `--%s' doesn't allow an argument\n"), argv[0], pfound->name);
						}
						else
						{
							_ftprintf(stderr, _T("%s: option `%c%s' doesn't allow an argument\n"), argv[0], argv[optind - 1][0], pfound->name);
						}
					}
					nextchar += _tcslen(nextchar);
					optopt = pfound->val;
					return '?';
				}
			}
			else if (pfound->has_arg == 1)
			{
				if (optind < argc)
				{
					optarg = argv[optind++];
				}
				else
				{
					if (opterr)
					{
						_ftprintf(stderr, _T("%s: option `%s' requires an argument\n"), argv[0], argv[optind - 1]);
					}
					nextchar += _tcslen(nextchar);
					optopt = pfound->val;
					return optstring[0] == ':' ? ':' : '?';
				}
			}
			nextchar += _tcslen(nextchar);
			if (longind != NULL)
			{
				*longind = option_index;
			}

			if (pfound->flag)
			{
				*(pfound->flag) = pfound->val;
				return 0;
			}
			return pfound->val;
		}
		if (!long_only || argv[optind][1] == '-' || _tcschr(optstring, *nextchar) == NULL)
		{
			if (opterr)
			{
				if (argv[optind][1] == '-')
				{
					_ftprintf(stderr, _T("%s: unrecongnized option `--%s'\n"), argv[0], nextchar);
				}
				else
				{
					_ftprintf(stderr, _T("%s: unrecongnized option `%c%s'\n"), argv[0], argv[optind][0], nextchar);
				}
			}
			nextchar = (PTCHAR) _T("");
			optind++;
			optopt = 0;
			return '?';
		}
	}

	/* short options */
	{
		TCHAR c = *nextchar++;
		TCHAR* temp = _tcschr(optstring, c);

		if (*nextchar == '\0')
		{
			++optind;
		}

		if (temp == NULL || c == ':')
		{
			if (opterr)
			{
				_ftprintf(stderr, _T("%s: invalid option -- %c\n"), argv[0], c);
			}
			optopt = c;
			return '?';
		}

		if (temp[1] == ':')
		{
			if (temp[2] == ':')
			{
				if (*nextchar != '\0')
				{
					optarg = nextchar;
					optind++;
				}
				else
				{
					optarg = NULL;
				}
				nextchar = NULL;
			}
			else
			{
				if (*nextchar != '\0')
				{
					optarg = nextchar;
					optind++;
				}
				else if (optind == argc)
				{
					if (opterr)
					{
						_ftprintf(stderr, _T("%s: option requires an argument -- %c\n"), argv[0], c);
					}
					optopt = c;
					if (optstring[0] == ':')
					{
						c = ':';
					}
					else
					{
						c = '?';
					}
				}
				else
				{
					optarg = argv[optind++];
				}
				nextchar = NULL;
			}
		}
		return c;
	}
}

int
getopt(int argc, TCHAR *const *argv, const TCHAR *optstring)
{
	return _getopt_internal(argc, argv, optstring,
		(const struct option*)0,
		(int*)0,
		0);
}

int
getopt_long(int argc, TCHAR *const *argv, const TCHAR *options,
	const struct option* long_options, int* opt_index)
{
	return _getopt_internal(argc, argv, options, long_options, opt_index, 0);
}

int
getopt_long_only(int argc, TCHAR *const *argv, const TCHAR *options,
	const struct option* long_options, int* opt_index)
{
	return _getopt_internal(argc, argv, options, long_options, opt_index, 1);
}