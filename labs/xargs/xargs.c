#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_INP_SIZE 1024
#define MAX_ARG_NUM 256
#define MAX_EXE_PATH_LEN 1024

char to_lower(char c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return c += 'a' - 'A';
	}
	return c;
}

char* find_PATH(char** envp)
{
	static char PATH[] = "PATH";
	char* pPATH = PATH;
	char* res = NULL;

	int found = 1;

	for(char** varp = envp; *varp; varp++)
	{
		pPATH = PATH;
		found = 1;
		char* var = *varp;
		while(*pPATH && *var)
		{
			if(to_lower(*pPATH) != to_lower(*var))
			{
				found = 0;
				break;
			}
			pPATH++;
			var++;
		}

		if(found)
		{
			res = var;
			res += 5;
			return res;
		}
	}
	return res;
}

int main(int argc, char** argv, char** envp)
{
	// no args use echo

	char to_run[MAX_EXE_PATH_LEN];
	if(argc == 1)
	{
		return -1;
	}
	else{

		char candidate[MAX_EXE_PATH_LEN];
		char stat_res[256];

		if(stat(to_run, (struct stat*) stat_res) != 0)
		{
			to_run[0] = 0;

			char* PATH = find_PATH(envp);
			char* start = PATH;

			while(1)
			{
				if(*PATH == 0)
				{
					break;
				}

				if(*PATH == ':')
				{
					*PATH = 0;
					char* cand_p = candidate;
					while(*cand_p++ = *start++);

					char *exe = argv[1];
					*(cand_p-1) = '/';
					while(*cand_p++ = *exe++);

					

					if(stat(candidate,(struct stat*) stat_res) == 0)
					{
						cand_p = candidate;
						exe = to_run;
						while(*exe++ = *cand_p++);
					}
				}
				PATH++;
			}
		}
	}
	
	char* res_args [MAX_ARG_NUM];
	res_args[0] = argc == 1 ? "echo" : argv[1];

	char args[MAX_ARG_NUM * MAX_INP_SIZE];
	char* line = args;

	int read_bytes = -1;
	int i = 1;
	while(i < MAX_ARG_NUM && (read_bytes = read(STDIN_FILENO, line, MAX_INP_SIZE)) > 0)
	{
		int idx = 0;
		char* begin = line;
		while(idx < read_bytes)
		{
			if(line[idx] == '\n' || line[idx] == '\t' || line[idx] == ' ' || line[idx] == 0)
			{
				line[idx] = 0;
				res_args[i] = begin;
				begin = line + idx + 1;
				i++;
			}
			idx++;
		}
		line += read_bytes;
	}
	res_args[i] = 0;

	execve(to_run, res_args, envp);
}
