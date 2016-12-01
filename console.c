#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"

#define PROMPT	"pCLI > "

int cmd_test1(int argc, char* argv[])
{
	printf("test1 command\n");

	return 0;
}
int cmd_test2(int argc, char* argv[])
{
	printf("test2 command\n");

	return 0;
}
int cmd_test3(int argc, char* argv[])
{
	printf("test3 command\n");

	return 0;
}

int cmd_example1(int argc, char* argv[])
{
	printf("cmd_example1 command\n");

	return 0;
}

int cmd_example2(int argc, char* argv[])
{
	printf("cmd_example2 command\n");

	return 0;
}

int cmd_quit(int argc, char* argv[])
{
	exit(0);
}
#define CMD_TBL_QUIT	CMD_TBL_ENTRY(		\
	"quit",		4,	cmd_quit,	\
	"quit		- Exit feicli"		\
),


#define CMD_TBL_TEST1	CMD_TBL_ENTRY(		\
	"test1",	5,	cmd_test1,	\
	"test1		- test1 test command"	\
),
#define CMD_TBL_TEST2	CMD_TBL_ENTRY(		\
	"test2",	5,	cmd_test2,	\
	"test2		- test2 test command"	\
),
#define CMD_TBL_TEST3	CMD_TBL_ENTRY(		\
	"test3",	5,	cmd_test3,	\
	"test3		- test3 test command"	\
),

#define CMD_TBL_EXAMPLE1	CMD_TBL_ENTRY(		\
	"example1",	8,	cmd_example1,	\
	"example1	- example1 command"	\
),
#define CMD_TBL_EXAMPLE2	CMD_TBL_ENTRY(		\
	"example2",	8,	cmd_example2,	\
	"example2	- example2 command"	\
),

struct cmd_table cmd_new_tbl[4] = {
	CMD_TBL_TEST1
	CMD_TBL_TEST2
	CMD_TBL_TEST3
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

struct cmd_table cmd_example_tbl[3] = {
	CMD_TBL_EXAMPLE1
	CMD_TBL_EXAMPLE2
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

struct cmd_table cmd_system_tbl[2] = {
	CMD_TBL_QUIT
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

struct cmd_table *topcmd = NULL;
struct cmd_table *currcmd = NULL;


int show_cmd_help()
{
	struct cmd_table *curr = topcmd;

	while (curr->name != NULL) {
		printf("%s\n", curr->usage);

		if (curr->next_cmd != NULL) {
			curr = curr->next_cmd;
		} else {
			break;
		}
	}

	return 0;
}

int parse_line()
{
	return 0;
}

int parse_cmd2(char *cmd)
{
	struct cmd_table *curr = topcmd;
	int found = 0;

	if (strlen(cmd) == 0)
		return 0;

	while (curr->name != NULL) {
		found = strncmp(curr->name, cmd, curr->len_name);
		if (!found) {
			if (curr->Func != NULL) {
				curr->Func(0,0);
				break;
			}
		}

		if (curr->next_cmd != NULL)
			curr = curr->next_cmd;
	}

	return found;
}

#if 0
int cmd_register(struct cmd_table new[])
{
	int i = 0;

	while (new[i].name != NULL) {
		if (topcmd == NULL) {
			topcmd = &new[i];
			currcmd = topcmd;
		} else {
			currcmd->next_cmd = &new[i];
			currcmd = &new[i];
		}
		i++;
	}

	return 0;
}
#endif

void init_cmd_register()
{
	if (topcmd == NULL) {
		topcmd = malloc(sizeof(struct cmd_table));
		topcmd->name = NULL;
		topcmd->next_cmd = NULL;
		currcmd = topcmd;
	}
}

int cmd_register(struct cmd_table new[])
{
	int i = 0;

	while (new[i].name != NULL) {
		memcpy(currcmd, &new[i], sizeof(struct cmd_table));

		currcmd->next_cmd = malloc(sizeof(struct cmd_table));
		currcmd = currcmd->next_cmd;
		currcmd->name = NULL;
		currcmd->next_cmd = NULL;

		i++;
	}

	return 0;
}

int main()
{
	int c = 0;
	int i = 0;
	char history_cmd[CMDBUF_SIZE];
	char cmdbuf[CMDBUF_SIZE];
	int ret = 0;

	memset(history_cmd, 0, sizeof(history_cmd));
	memset(cmdbuf, 0, sizeof(cmdbuf));

	printf("%s ", PROMPT);

	// Init command list
	init_cmd_register();
	cmd_register(cmd_system_tbl);
	cmd_register(cmd_new_tbl);
	cmd_register(cmd_example_tbl);

	while (1) {
		c = getchar();

		if (c == 0xa) { /* enter key */
			if (i > 0) {
				//printf("\n");
				cmdbuf[i] = 0x0;
				ret = parse_cmd2(cmdbuf);
				if (ret != 0) { // Command not found!!
					show_cmd_help();
				}
				memset(cmdbuf, 0, sizeof(cmdbuf));
				i = 0;
			}
			printf("%s ",PROMPT);

		} else {
			//printf("%c", c);
			cmdbuf[i] = c;
			i++;
                        
			if (i > (CMDBUF_SIZE - 1)) {
				i = 0;
				memset(cmdbuf, 0, sizeof(cmdbuf));
			} 
		}
	}

	return 0;
}
