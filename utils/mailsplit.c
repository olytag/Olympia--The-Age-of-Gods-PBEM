
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"z.h"


usage()
{
    fprintf(stderr, "usage: mailsplit [-s size] [-l lines] [-c cmd]\n");
    fprintf(stderr, "       size defalts to 60,000\n");
    fprintf(stderr, "       lines defalts to 0 (no line split)\n");
    exit(1);
}

char *command = "/usr/lib/sendmail -t";
int pid;

main(argc, argv)
int argc;
char **argv;
{
	extern int optind, opterr;
	extern char *optarg;
	int errflag = 0;
	int c;
	int size = 60000;
	int lines = 0;
	int part;

	pid = getpid();

	while ((c = getopt(argc, argv, "c:l:s:?")) != EOF)
	{
		switch (c)
		{
		case 'c':
			command = str_save(optarg);
			break;

		case 'l':
			lines = atoi(optarg);
			break;

		case 's':
			size = atoi(optarg);
			break;

		case '?':
		default:
			errflag++;
			break;
		}
	}

	if (errflag)
		usage();	/* doesn't return */

	if (size > 0 && size < 10000)
	{
		size = 10000;
		fprintf(stderr, "min size is 10000\n");
	}

	if (lines > 0 && lines < 500)
	{
		lines = 500;
		fprintf(stderr, "min lines is 500\n");
	}

	part = read_message(lines, size);
	send_message(part);
	unlink_files(part);

	exit(0);
}


unlink_files(int part)
{
	char fnam[LEN];
	int i;

	sprintf(fnam, "/tmp/split%d.head", pid);
	unlink(fnam);

	for (i = 1; i <= part; i++)
	{
		sprintf(fnam, "/tmp/split%d.%d", pid, i);
		unlink(fnam);
	}
}


FILE *
open_head(char *mode)
{
	char fnam[LEN];
	FILE *fp;

	sprintf(fnam, "/tmp/split%d.head", pid);

	fp = fopen(fnam, mode);

	if (fp == NULL)
	{
		fprintf(stderr, "can't open %s:", fnam);
		perror("");
		exit(1);
	}

	return fp;
}


FILE *
open_part(int part, char *mode)
{
	char fnam[LEN];
	FILE *fp;

	sprintf(fnam, "/tmp/split%d.%d", pid, part);

	fp = fopen(fnam, mode);

	if (fp == NULL)
	{
		fprintf(stderr, "can't open %s:", fnam);
		perror("");
		unlink_files(part);
		exit(1);
	}

	return fp;
}


feed_headers(int part, int highest, FILE *fp)
{
	char *line;
	FILE *head_fp;

	head_fp = open_head("r");

	while (line = getlin(head_fp))
	{
		if (highest > 1 && i_strncmp(line, "Subject:", 8) == 0)
			fprintf(fp, "%s [part %d of %d]\n", line, part, highest);
		else
			fprintf(fp, "%s\n", line);
	}

	fclose(head_fp);
	fprintf(fp, "\n");
}


feed_body(int part, FILE *fp)
{
	char *line;
	FILE *part_fp;

	part_fp = open_part(part, "r");

	while (line = getlin(part_fp))
		fprintf(fp, "%s\n", line);

	fclose(part_fp);
}


read_message(lines, size)
int lines;
int size;
{
	char *line;
	int linecount;
	int sizecount;
	int header_lines = 0;
	int header_size = 0;
	int part = 1;
	FILE *fp;

	fp = open_head("w");

	while (line = getlin(stdin))
	{
		header_lines++;
		header_size += strlen(line) + 1;

		if (*line == '\0')
			break;

		fprintf(fp, "%s\n", line);
	}

	fclose(fp);
	fp = open_part(part, "w");

	linecount = header_lines;
	sizecount = header_size;

	while (line = getlin(stdin))
	{
		linecount++;
		sizecount += strlen(line) + 1;

		if ((lines > 0 && linecount > lines) ||
		    (size > 0 && sizecount > size))
		{
			linecount = header_lines;
			sizecount = header_size;

			fclose(fp);
			part++;
			fp = open_part(part, "w");
		}

		fprintf(fp, "%s\n", line);
	}

	fclose(fp);

	return part;
}


send_message(int part)
{
	FILE *fp;
	int i;

	for (i = 1; i <= part; i++)
	{
		fp = popen(command, "w");

		if (fp == NULL)
		{
			fprintf(stderr, "can't invoke %s:", command);
			perror("");
			unlink_files(part);
			exit(1);
		}

		feed_headers(i, part, fp);
		feed_body(i, fp);

		pclose(fp);
	}
}

