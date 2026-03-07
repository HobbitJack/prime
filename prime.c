#define _POSIX_C_SOURCE 200809L

#include <libgen.h>
#include <stdio.h>
#include <unistd.h>

#include <gmp.h>

#include "prime.g.h"
#include "prime.l.h"

static char *progname;
static int status;

static struct gengetopt_args_info args;

int
isprime(mpz_t num)
{
	return (mpz_probab_prime_p(num, args.repetitions_arg)) ? 1 : 0;
}

int
main(int argc, char *argv[])
{
	unsigned int i;
	mpz_t target;
	
	progname = basename(argv[0]);
	mpz_init(target);

	if (ggo(argc, argv, &args))
		return 1;

	if (args.help_given)
	{
		ggo_print_help();
		return 0;
	}
	if (args.version_given)
	{
		ggo_print_version();
		return 0;
	}

	if (args.repetitions_arg < 0)
	{
		fprintf(stderr, "%s: %d: Bad number of repetitions\n", progname, args.repetitions_arg);
		return 1;
	}
	
	if (args.inputs_num)
	{
		for (i=0; i<args.inputs_num; i++)
		{
			if (mpz_set_str(target, args.inputs[i], 0) || (mpz_cmp_d(target, 0) < 0))
			{
				if (!args.silent_given)
					fprintf(stderr, "%s: %s: Bad argument\n", progname, args.inputs[i]);

				status = 1;
				continue;
			}

			if (!args.quiet_given)
			{
				mpz_out_str(NULL, 10, target);
				fputs(": ", stdout);
			}

			if (isprime(target))
				puts("prime");
			else
				puts("composite");
		}
	}
	else
	{
		while (yylex() != -1)
		{
			if ((errno < 0) || mpz_set_str(target, yytext, 0) || (mpz_cmp_d(target, 0) < 0))
			{
				if (!args.silent_given)
					fprintf(stderr, "%s: %s: Bad argument\n", progname, yytext);

				errno = 0;
				status = 1;
				continue;
			}

			if (!args.quiet_given)
			{
				mpz_out_str(NULL, 10, target);
				fputs(": ", stdout);
			}
			
			if (isprime(target))
				puts("prime");
			else
				puts("composite");
		}
	}

	return args.loose_exit_status_given ? 0 : status;
}
