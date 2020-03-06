#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

int main()
{
	char path[256] = "test.txt";
	FILE *fp = fopen(path,"w+");
	fprintf(fp,"╡Бйтндвж1\n╡Бйтндвж2\n╡Бйтндвж3");
	fclose(fp);
	return 0;
}
