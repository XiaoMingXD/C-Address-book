#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <windows.h>

char *GF_GetEXEPath()
{
    char FilePath[255];
    GetModuleFileName(NULL,FilePath,255); 
    (strrchr(FilePath,'\\'))[1] = 0; 
   
    return FilePath;
}
int main()
{
	DIR *dir_p;
	struct dirent *entry;
	dir_p = opendir();
	if(dir_p == NULL)
	{
		printf("open dir error\n");
	}
	else
	{
		
		while((entry = readdir(dir_p)) != NULL)
		{
			printf("%s\n",entry->d_name);
		}
	}
	closedir(dir_p);
	printf("ÔËÐÐÄ¿Â¼£º%s",GF_GetEXEPath());
}
