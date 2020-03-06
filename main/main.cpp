#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

/*------定义常量------*/

#define CHARLENTH 256
#define PHONENUMBERLENTH 256
#define DATAPATH "database"
#define FILELENTH 1024 

/*--创建用户储蓄单元--*/ 

typedef struct userN{
	char usn[CHARLENTH]; 
	char pwd[CHARLENTH];
	struct userN *next;
};

/*--创键数据储蓄单元--*/ 

typedef struct contact{
	char name[CHARLENTH];
	char brithplace[CHARLENTH];
	char number1[PHONENUMBERLENTH];
	char number2[PHONENUMBERLENTH];
	char group[CHARLENTH];
	char email[CHARLENTH];
} *Contact,ContactNode;

/*------函数声明------*/
int initialize();
int mainmenu();
int ifnumbervaild(char number[CHARLENTH]);
void trim(char *strIn, char *strOut);
void getkeyvalue(char * keyAndValue, char * key, char * value);
int viewcontaction();
int menu_createcontaction();
int createcontaction(Contact newcontaction);
int menu_findcontaction();
int findcontaction(int mode,char *name);
int menu_editcontaction();
int editcontaction(char name[CHARLENTH],int item,char value[CHARLENTH]);
int menu_deletecontaction();
int deletecontaction(char name[CHARLENTH]);
int writeconfig(Contact contaction);
int readconfig(Contact contaction,char name[]);
int menu();
int loginmenu();
int registermenu();
void GetEXEPath(char path[CHARLENTH]);
void RemoveSuffix(char *str);
int menu_viewcontaction();
int contactioninfomation(char name[CHARLENTH]);
int searchitem(int item,char value[CHARLENTH]);
int registing(char* usn, char* pwd);
userN* loadUser();
userN* findUser(userN* head,char* usn);
int row(FILE *fp);
/*-------初始化-------*/

//检测数据库目录是否存在，如不存在则创建目录
int initialize()
{
	if(access(DATAPATH,00) == -1)//00是F_OK的宏定义，-1代表权限测试失败，即目录不存在 
	{
		printf("正在为首次使用进行初始化，时间可能较长。请稍后...\n"); 
		if(mkdir(DATAPATH) == -1)//创建文件夹，成功返回0，失败返回-1，如果创建文件夹失败则报错 
			return -1;  //main()中检测到-1则退出程序 
		char userconf[CHARLENTH];
		/*
		strcpy(userconf,DATAPATH);
		strcat(userconf,"\\");
		strcat(userconf,"user.ini");
		*/
	}
	char current[CHARLENTH]="";
	strcpy(current,DATAPATH);
	strcat(current,"\\current.ini");
	remove(current);
	return 0;
 } 

/*------小程序块------*/ 


int contactioninfomation(char name[CHARLENTH])//打印联系人详细信息，成功返回1，失败返回0 
{
	Contact contaction = (Contact)calloc(1,sizeof(ContactNode));
	if(readconfig(contaction,name) == -1)
	{
			printf("联系人信息读取失败！\n");
			return 0 ;
	}
	printf("联系人：%s 的详细信息如下：\n",name);
	printf("姓名: %s\n",contaction->name); 
	printf("籍贯: %s\n",contaction->brithplace); 
	printf("电话1: %s\n",contaction->number1); 
	printf("电话2: %s\n",contaction->number2); 
	printf("分组: %s\n",contaction->group); 
	printf("邮箱: %s\n",contaction->email); 
	return 1;
}

void RemoveSuffix(char *str)//去除文件名后缀 
{
	char *p;//动态字符指针 
	char temp[CHARLENTH] = "";//缓冲区 
	p = str;//指针定位字符串头 
	int i=0,j=0;//计数变量 
	for(i=0;i<strlen(str);i++,j++)//循环遍历文件名的每一个字符，直到'.'则跳出循环 
	{
		if(str[i] == '.')
		{
			break;
		}
		temp[j] = str[i];
	}
	//strcat(temp,'\0');
    //strcpy(str,temp);
    strcpy(str,temp);
} 

void GetEXEPath(char path[CHARLENTH])  //获取程序运行目录 
{ 
    char EXEPath[255];
    GetModuleFileName(NULL,EXEPath,255); 
    (strrchr(EXEPath,'\\'))[1] = 0; 
   
    strcpy(path,EXEPath);
}

int ifnumbervaild(char number[CHARLENTH])//判断输入的电话号码是否存在0-9之外的字符 或者长度非法 ,合法返回1，非法返回0 
{
	//printf("判断是否合法函数的输入数据是：%s\n",number);
	//printf("长度：%d\n",strlen(number)); 
	if(strlen(number) != 11)
	{
		//长度非法
		return 0; 
	}
	else
	{
		char *p;
		p = number;//定位指针 
		
		int i = 0;
		for(i=0;i<10;i++)
		{
			if(*p < '0' || *p > '9')
			{
				printf("非法字符：%c\n",*p);
				return 0;
			}
			p++;
		}
		//数据合法
		return 1; 
	} 
}
 
void trim(char *strIn, char *strOut)//去除字符串首尾空格 
{
    char *start, *end, *temp;//去除空格后字符串的头尾指针和遍历指针
    temp = strIn;
    while (*temp == ' ')
	{
        ++temp;
    }
    start = temp; //头指针
    temp = strIn + strlen(strIn) - 1; //得到原字符串最后一个字符的指针
    while (*temp == ' ')
	{
        --temp;
    }
    end = temp; //尾指针
    for(strIn = start; strIn <= end; )
	{
        *strOut++ = *strIn++;
    }
 	*strOut = '\0';
}

void getkeyvalue(char keyAndValue[CHARLENTH], char key[CHARLENTH], char value[CHARLENTH])//通过配置行解析value值 
{
	char *p = keyAndValue;//遍历指针 
    p = strstr(keyAndValue, key);//定位key 
    if(p == NULL)
	{
        printf("键值错误\n");
        return;
    }
    p += strlen(key);//跳过key字符串 
    trim(p, value);//去除空格 
    p = strstr(value, "=");//定位= 
    if(p == NULL)
	{
        printf("数据值错误！没有=\n");
        return;
    }
    p+= strlen("=");//跳过= 
    trim(p, value);
    p = strstr(value, "=");
    if(p != NULL)
	{
        printf("数据值错误！多余的=\n");
        return;
    }
    p = value;
    trim(p, value);

} 
/*------菜单引导------*/

int firstmenu()//注册用户信息 
{
	int Ret=-1;
	do{
		int x = 0;
		printf("==========通讯录管理系统 v1.0==========\n");
		printf("请先登入或注册！\n");
		printf("1.登入\n"); 
		printf("2.注册\n");
		do{
			printf("请选择要执行的操作：");
			scanf("%d",&x);
    		switch(x){
	    		case 1:Ret=loginmenu();break;
    			case 2:registermenu();break;
    			default:printf("输入值有误！\n");break;
    		}
		}while(x != 1 && x != 2);
	}while(Ret != 2);//返回值为2代表登陆成功 
	mainmenu();
}

int loginmenu(){
	userN *head = loadUser();
	userN *here;
	char usn[CHARLENTH]="",pwd[CHARLENTH]="",pwdc[CHARLENTH]="";//pwd是用户输入的密码
	printf("请输入用户名:"); 
	scanf("%s",&usn);
	here=findUser(head,usn);
	if(here==NULL){
		printf("用户名不存在，请重试\n");
		return -1;
	}
	printf("请输入密码:"); 
	scanf("%s",&pwd);
	if(strcmp(here->pwd,pwd)!=0){
		printf("密码错误，请重试\n");
		return -1;
	}
	char current[CHARLENTH]="";
   	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"w+");
   	strcpy(current,usn);
   	fputs(current,cnt);
   	fclose(cnt);
	printf("登陆成功！\n");
	return 2; 
}

userN* findUser(userN* head,char* usn){
	userN *p=head;
 	while (p->next != NULL) {
		p = p->next;
		if(p->usn != usn)
			return p;
	}
	return NULL;
}
int registermenu(){
	char usn[CHARLENTH],pwd[CHARLENTH],pwdc[CHARLENTH];
	char path[CHARLENTH];
	strcpy(path,DATAPATH);
	strcat(path,"\\"); 
	printf("请输入想要注册的用户名:"); 
	scanf("%s",&usn);
	printf("正在查询用户名%s是否可用...\n",usn);
	strcat(path,usn);
	if(access(path,00) == 0){//00是F_OK的宏定义，0代表权限测试成功，即目录存在  
 		printf("用户名%s已被注册，请尝试登陆\n",&usn);
 		return -1; 
	} 
	do{
		printf("用户名%s可用,请输入密码:",usn); 
		scanf("%s",&pwd);
		printf("请确认密码:"); 
		scanf("%s",&pwdc);
		if(*pwd==*pwdc)
			break;
		else
			printf("两次输入的密码不一致，请重试\n");
	}while(true);
	printf("正在为您创建用户，请稍后...\n");
	if(registing(usn,pwd)==-1){
		printf("创建用户失败，请检查您的权限，重启软件后再试。\n"); 
		return -1;
	}
 	printf("用户创建成功,即将将您返回至主页面...\n");	
 	return 1;
}

int registing(char* usn, char* pwd){
	char path[CHARLENTH];
	strcpy(path,DATAPATH);
	strcat(strcat(path,"\\"),usn);
	if(mkdir(path) == -1)//创建文件夹，成功返回0，失败返回-1，如果创建文件夹失败则报错 
		return -1;
	char filePath[CHARLENTH];
	strcpy(filePath,DATAPATH);
	strcat(filePath,"\\user.ini");
	FILE *user=fopen(filePath,"a");
	char filechar[FILELENTH] = "";//创建文件字符串
	char usne[CHARLENTH] = "usn=";
	char pwde[CHARLENTH] = "pwd=";
	strcat(filechar,strcat(usne,usn));
	strcat(filechar,"\n");
	strcat(filechar,strcat(pwde,pwd));
	strcat(filechar,"\n");
	if(fprintf(user,"%s",filechar) == -1)
	{
		fclose(user); 
    	return -1;
	}
	else
	{
		fclose(user);
		return 1;
	}
}

userN* loadUser(){
	char filePath[CHARLENTH];
	strcpy(filePath,DATAPATH);
	strcat(filePath,"\\user.ini");
	FILE *user=fopen(filePath,"r");
	char filechar[FILELENTH] = "";
	char value[CHARLENTH] = "";
	userN *head,*node,*end;//定义头节点，普通节点，尾部节点；
	int rowc=row(user);
	rowc/=2;
    head = (userN*)malloc(sizeof(userN));//分配地址
    end = head;//若是空链表则头尾节点一样
    for (int i = 0; i < rowc; i++) {
        node = (userN*)malloc(sizeof(userN));
        //读取键值 
		//usn
		fgets(filechar,FILELENTH,user);
		char usn[CHARLENTH] = "usn";
		getkeyvalue(filechar,usn,value);
		if(value[strlen(value) - 1] == '\n') 
			value[strlen(value) - 1] = '\0';
		strcpy(node->usn,value);
		//读取键值 
		//pwd
		fgets(filechar,FILELENTH,user);
		char pwd[CHARLENTH] = "pwd";
		getkeyvalue(filechar,pwd,value);
		if(value[strlen(value) - 1] == '\n') 
			value[strlen(value) - 1] = '\0';
		strcpy(node->pwd,value);
        end->next = node;
        end = node;
    }
    end->next = NULL;//结束创建
    return head;
}
int row(FILE *fp)
{
	char c;
	int h=0;
  	while(!feof(fp))
  	{
    	c = fgetc(fp);
    	if(c == '\n')
      		h++;
  	}
 	rewind(fp);
	return h;
}
int menu_viewcontaction()
{
	viewcontaction();
	printf("键入联系人姓名以查看详情，键入quit返回上一级\n");
	char name[CHARLENTH] = "";
	char quit[CHARLENTH] = "quit";
	do{		
		scanf("%s",name);
	    if(strstr(name,quit) != NULL)
	       return 1; 
	    else
	    	if(findcontaction(1,name) == 0 )//返回0表示找不到文件或者文件读取失败 
    			printf("该联系人不存在或数据错误\n请输入要查询的联系人姓名：");
     		else
     			break;//查询成功则跳出循环 
	}while(1); 
}

int viewcontaction()//浏览所有联系人 如果不存在联系人或者打开失败返回0，成功列出返回1 
{
	printf("\n-=-全部联系人-=-\n\n");
	DIR *dir_p;//创建目录指针 
	struct dirent *entry;//调用dirent中的目录属性结构体 
	char path[CHARLENTH];
	GetEXEPath(path);
	char current[CHARLENTH]="";
	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
	fclose(cnt);
	dir_p = opendir(strcat(strcat(strcat(path,DATAPATH),"//"),current));//打开目录，写入目录指针 
	if(dir_p == NULL)//如果目录不存在或者系统权限阻止 
	{
		printf("打开数据库失败！\n");
		return 0; 
	}
	else
		while((entry = readdir(dir_p)) != NULL)//循环读取，读完所有文件后再读会读到NULL 
			if(entry->d_name[0] != '.'){
				RemoveSuffix(entry->d_name);
				printf("%s\n",entry->d_name);//打印读取到的文件名 
		    }
	closedir(dir_p);
	printf("\n-=-已全部列出-=-\n");
	
} 

int menu_createcontaction()//新建联系人菜单 
{
	char temp_path[CHARLENTH] = "";
	Contact newcontaction;
	newcontaction = (Contact)calloc(1,sizeof(ContactNode));
	printf("==============新建联系人==============\n");
	do{
		printf("请输入联系人姓名:\n");
	    scanf("%s",newcontaction->name);
	    char current[CHARLENTH]="";
   		char cntPath[CHARLENTH]="";
   		strcpy(cntPath,DATAPATH);
   		strcat(cntPath,"\\current.ini");
   		FILE *cnt = fopen(cntPath,"r");
   		fgets(current,FILELENTH,cnt);
		fclose(cnt);
   		
		strcpy(temp_path,DATAPATH);
	    strcat(temp_path,"\\");
	    strcat(temp_path,current);
	    strcat(temp_path,"\\");
	    strcat(temp_path,newcontaction->name);
	    strcat(temp_path,".ini");
	    if(access(temp_path,00) == -1)//00是F_OK的宏定义，-1代表权限测试失败，即文件不存在
	    {
	    	break;
		}
		else
		{
			printf("该联系人已存在，请重新输入\n");
		}
	} while(1);
	
	
	printf("请输入联系人所在地(籍贯):\n");
	scanf("%s",newcontaction->brithplace);
	
	do{
	    printf("请输入联系人电话号码1:\n");
	    scanf("%s",newcontaction->number1);
	    if(ifnumbervaild(newcontaction->number1) == 0)
	    {
	    	printf("输入数据非法！请重新输入！\n");
	    }
	    else
	    {
	    	break;
		}
	}while(1);
	
	do{
	    printf("请输入联系人电话号码2:\n");
	    scanf("%s",newcontaction->number2);
	    if(ifnumbervaild(newcontaction->number2) == 0)
	    {
	    	printf("输入数据非法！请重新输入！\n");
	    }
	    else
	    {
	    	break;
		}
	}while(1);
	
	printf("请输入联系人分组:\n");
	scanf("%s",newcontaction->group);
	printf("请输入联系人邮箱:\n");
	scanf("%s",newcontaction->email);
	//数据输入完成，正在创建联系人...... 
	printf("数据输入完成，正在创建联系人...... \n");
	if(createcontaction(newcontaction) == 0) 
	{
		printf("新建联系人失败！\n");
		return 0;
    }
    else
    {
    	printf("新建联系人完成\n");
    	return 1;
	}
}

int createcontaction(Contact newcontaction)//新建联系人，成功返回1，失败返回0
{
	if(writeconfig(newcontaction))
	{
		return 1;
	}
	else
	{
		return 0;
	}
} 

int menu_findcontaction()//查询联系人菜单 ,联系人不存在返回0，成功查询返回1并列出信息 
{
	
	printf("\n========查询联系人========\n");
	printf("请选择查询类型：\n");
	printf("1.按照姓名查询\n");
	printf("2.按照籍贯查询\n");
	printf("3.按照电话查询\n");
	printf("4.按照分组查询\n");
	printf("5.按照邮箱查询\n");
	int mode = 0;
	do{
		printf("请输入查询类型：\n");
        scanf("%d",&mode);
    	if(mode < 0 || mode > 5)
    		printf("输入值错误！请重新输入：\n");
	    else
    		break;
	}while(1);
	char value[CHARLENTH] = "";
	printf("请输入关键字：\n");
	scanf("%s",value);
	if(findcontaction(mode,value) == 0)
	{
		printf("无法找到相应数据\n");
	}
	
}

int findcontaction(int mode,char *value)
//查找联系人(多条件) 查询成功返回1，失败返回0 
//参数1为查询标签类型
//1.按照姓名查询
//2.按照籍贯查询
//3.按照电话查询
//4.按照分组查询
//5.按照邮箱查询 
//参数2为标签值 
{
	int statu = 0;//searchitem返回值临时存放 
	switch (mode){
		case 1: //1.按照姓名查询 
		{ 
			if(contactioninfomation(value) == 0)
			{
				return 0;
			}
			break; 
		} 
		case 2: //2.按照籍贯查询
		    {
			    printf("正在查找籍贯为：%s的联系人......\n符合条件的联系人如下：\n",value);
			    statu = searchitem(2,value);
	    	    if(statu == 0)
	    	    {
	    	    	printf("无符合条件的联系人。\n");
				}
				else if(statu == 1)
				{
					printf("查找完毕\n");
				}
				else
				{
					printf("查找出错\n");
				}
	    	    break; 
			}
		case 3: //3.按照电话查询
		    {
			    printf("正在查找电话为：%s的联系人......\n符合条件的联系人如下：\n",value);
			    statu = searchitem(3,value);
	    	    if(statu == 0)
	    	    {
	    	    	printf("无符合条件的联系人。\n");
				}
				else if(statu == 1)
				{
					printf("查找完毕\n");
				}
				else
				{
					printf("查找出错\n");
				}
	    	    break; 
			}
		case 4: //4.按照分组查询
		    {
			    printf("正在查找分组为：%s的联系人......\n符合条件的联系人如下：\n",value);
			    statu = searchitem(4,value);
	    	    if(statu == 0)
	    	    {
	    	    	printf("无符合条件的联系人。\n");
				}
				else if(statu == 1)
				{
					printf("查找完毕\n");
				}
				else
				{
					printf("查找出错\n");
				}
	    	    break; 
			}
		case 5: //5.按照邮箱查询
		    {
			    printf("正在查找邮箱为：%s的联系人......\n符合条件的联系人如下：\n",value);
			    statu = searchitem(5,value);
	    	    if(statu == 0)
	    	    {
	    	    	printf("无符合条件的联系人。\n");
				}
				else if(statu == 1)
				{
					printf("查找完毕\n");
				}
				else
				{
					printf("查找出错\n");
				}
	    	    break; 
			}
		default: return 0;
	}
	printf("输入联系人姓名以查看详情，输入quit返回主菜单：");
	char userin[CHARLENTH] = "";
	scanf("%s",userin);
	char quit[CHARLENTH] = "quit";
	if(strstr(userin,quit))
	{
		return 1;
	}
	else
	{
		contactioninfomation(userin);
	}
	return 1;
	
} 

int menu_editcontaction()//编辑联系人 菜单 
{
	viewcontaction();//先列出所有联系人
	char name[CHARLENTH] = "";
	char temp_path[CHARLENTH] = "";
	char current[CHARLENTH]="";
   	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
   	fclose(cnt);
	do{
		printf("请输入要修改的联系人姓名：");
		scanf("%s",name);
    	strcpy(temp_path,DATAPATH);
    	strcat(temp_path,"\\");
        strcat(temp_path,current);
    	strcat(temp_path,"\\");
        strcat(temp_path,name);
     	strcat(temp_path,".ini");
    	if(access(temp_path,00) == -1)
    	{
    		printf("\n该联系人不存在！\n");
		}
		else
		{
			break;
		}
	}while(1);
	Contact contaction = (Contact)calloc(1,sizeof(ContactNode));
	if(readconfig(contaction,name) == -1)
	{
			printf("联系人信息读取失败！\n");
			return -1 ;
	}
	printf("联系人：%s 的详细信息如下：\n",name);
	printf("1.籍贯: %s\n",contaction->brithplace); 
	printf("2.电话1: %s\n",contaction->number1); 
	printf("3.电话2: %s\n",contaction->number2); 
	printf("4.分组: %s\n",contaction->group); 
	printf("5.邮箱: %s\n",contaction->email); 
	
	int item = 0;
	do{
		printf("\n请选择要修改的项目：");
    	scanf("%d",&item);
    	if(item > 0 || item < 6)
    	{
    		break;
		}
		else
		{
			printf("\n输入值有误！请重新输入：");
		}
	}while(1);
	
	char value[CHARLENTH] = "";
	printf("\n请输入新的数据：");
    scanf("%s",value);
	
	
	
	if(editcontaction(name,item,value) == -1)
	{
		printf("联系人信息修改失败！\n");
		return 0;
	}
	else
	{
		printf("联系人信息修改完成！\n");
		return 1;
	}
	
}

int editcontaction(char name[CHARLENTH],int item,char value[CHARLENTH])//修改联系人，参数1为联系人姓名,参数2为修改项目，参数3为新的值 
{
	Contact contaction;
	contaction = (Contact)calloc(1,sizeof(ContactNode));
	if(readconfig(contaction,name) == -1)
	{
		return -1;
	}
	switch(item){
		case 1: strcpy(contaction->brithplace,value);break;
		case 2: strcpy(contaction->number1,value);break;
		case 3: strcpy(contaction->number2,value);break;
		case 4: strcpy(contaction->group,value);break;
		case 5: strcpy(contaction->email,value);break;
	}
	char current[CHARLENTH]="";
   	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
   	fclose(cnt);
	char temp_filepath[CHARLENTH] = "";
	strcat(temp_filepath,DATAPATH);
	strcat(temp_filepath,"\\");
	strcat(temp_filepath,current);
	strcat(temp_filepath,"\\");
	strcat(temp_filepath,name);
	strcat(temp_filepath,".ini");
	remove(temp_filepath);//先删除老文件然后在写入新文件 
	Sleep(2);
    if(writeconfig(contaction) == -1)
    {
    	return -1;
	}
	else
	{
		return 1;
	}
	
}

int menu_deletecontaction()//删除联系人菜单 
{
	viewcontaction();
	char name[CHARLENTH] = "";
	char temp_path[CHARLENTH] = "";
	char current[CHARLENTH]="";
   	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
   	fclose(cnt);
   	
	do{
		printf("请输入要删除的联系人姓名：");
		scanf("%s",name);
     	strcpy(temp_path,DATAPATH);
     	strcat(temp_path,"\\");
        strcat(temp_path,current);
     	strcat(temp_path,"\\");
        strcat(temp_path,name);
     	strcat(temp_path,".ini");
     	if(access(temp_path,00) == -1)
    	{
        	printf("\n该联系人不存在！\n");
    	}
    	else
    	{
    		break;
		}
	}while(1);
	
	if(deletecontaction(name) == 0)
	{
		printf("删除联系人失败！\n");
		return 0;
	}
	else
	{
		return 1;
	}
	
}

int deletecontaction(char name[CHARLENTH])//删除联系人，成功返回1，失败返回0 
{
	char path[CHARLENTH] = "";
	char current[CHARLENTH]="";
   	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
   	fclose(cnt);
   	
 	strcpy(path,DATAPATH);
   	strcat(path,"\\");
   	strcat(path,current);
   	strcat(path,"\\");
    strcat(path,name);
   	strcat(path,".ini");
   	if(remove(path) == -1)
   	{
   		return 0;
    }
    else
    {
    	return 1;
	}
} 

int searchitem(int item,char value[CHARLENTH])//出错返回-1，成功返回1 ,未找到返回0，参数1为项目名称，取值2-5 
{
	int t = 0 ;//找到的符合条件的联系人数量 
	Contact contaction = (Contact)calloc(1,sizeof(ContactNode));
	DIR *dir_p;//创建目录指针 
	struct dirent *entry;//调用dirent中的目录属性结构体 
	char path[CHARLENTH];
   	char current[CHARLENTH]="";
   	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
   	GetEXEPath(path);
   	strcat(path,DATAPATH);
   	strcat(path,"\\");
   	strcat(path,current);
	dir_p = opendir(path);//打开目录，写入目录指针 
	if(dir_p == NULL)//如果目录不存在或者系统权限阻止 
	{
		printf("打开数据库失败！\n");
		return -1; 
	}
	else
	{
		while((entry = readdir(dir_p)) != NULL)//循环读取，读完所有文件后再读会读到NULL 
		{
			if(entry->d_name[0] != '.')
			{
		    	RemoveSuffix(entry->d_name);
		    	if(readconfig(contaction,entry->d_name) == -1)
		    	{
		    		return -1; 
		    	}
		    	//开始分支判断 
		    	switch(item){
		    		case 2: 
		    		{
		    		     if(strstr(contaction->brithplace , value) != NULL)
						 {
						      printf("%s\n",entry->d_name);
						      t++;
						 }
						 break;
					}
				    case 3: 
		    		{
		    		     if(strstr(contaction->number1 , value) != NULL || strstr(contaction->number2 , value) != NULL)
						 {
						      printf("%s\n",entry->d_name);
						      t++;
						 }
						 break;
					}
					case 4: 
		    		{
		    		     if(strstr(contaction->group , value) != NULL)
						 {
						      printf("%s\n",entry->d_name);
						      t++;
						 }
						 break;
					}
					case 5: 
		    		{
		    		     if(strstr(contaction->email , value) != NULL)
						 {
						      printf("%s\n",entry->d_name);
						      t++;
						 }
						 break;
					}
				}
			
		    }
		}
	}
	closedir(dir_p);
	if(t == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
 } 

/*------配置文件------*/

//写入配置

int writeconfig(Contact contaction)//写入成功返回1，失败返回0
{
	char filename[CHARLENTH] = "";
	char filepath[CHARLENTH] = "";
	strcpy(filename,contaction->name);
   	char current[CHARLENTH]="";
   	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
   	fclose(cnt);
   	strcat(filepath,DATAPATH);
	strcat(filepath,"\\");
	strcat(filepath,current);
	strcat(filename,".ini");//生成文件名 
	strcat(strcat(filepath,"\\"),filename);//拼接文件路径 
	FILE *config = fopen(filepath,"w+");//创建文件指针 
	char filechar[FILELENTH] = "";//创建文件字符串
	//拼接字符串 
	char name[CHARLENTH] = "name=";
	char brithplace[CHARLENTH] = "brithplace=";
	char number1[CHARLENTH] = "number1=";
	char number2[CHARLENTH] = "number2=";
	char group[CHARLENTH] = "group=";
	char email[CHARLENTH] = "email=";
	
	strcat(filechar,strcat(name,contaction->name));
	strcat(filechar,"\n");
	strcat(filechar,strcat(brithplace,contaction->brithplace));
	strcat(filechar,"\n");
	strcat(filechar,strcat(number1,contaction->number1));
	strcat(filechar,"\n");
	strcat(filechar,strcat(number2,contaction->number2));
	strcat(filechar,"\n");
	strcat(filechar,strcat(group,contaction->group));
	strcat(filechar,"\n");
	strcat(filechar,strcat(email,contaction->email));
	strcat(filechar,"\0");
	//printf("%s",filechar);
	if(fprintf(config,"%s",filechar) == -1)
	{
		fclose(config);
		getchar(); 
    	return -1;
	}
	else
	{
		fclose(config);
		return 1;
	}

}

//读取配置
int readconfig(Contact contaction,char name[])//出错返回-1 
{
	char filepath[CHARLENTH] = "";//创建文件路径变量 
   	char current[CHARLENTH]="";
   	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
   	fclose(cnt);
	strcat(strcat(strcat(strcat(strcat(strcat(filepath,DATAPATH),"\\"),current),"\\"),name),".ini");//拼接文件路径 
	FILE *config = fopen(filepath,"r");//打开文件
	if(config == NULL)
	{   //文件打开失败 
    	printf("readconfig faild");
		fclose(config);
		return(-1);
	}
	char filechar[FILELENTH] = "";
	char keyandvalue[CHARLENTH] = "";
	char key[CHARLENTH] = "";
	char value[CHARLENTH] = "";
	//读取键值 
	//name
	fgets(filechar,FILELENTH,config);
	char n[CHARLENTH] = "name";
	getkeyvalue(filechar,n,value);
	if(value[strlen(value) - 1] == '\n') value[strlen(value) - 1] = '\0';
	strcpy(contaction->name,value);
	//printf("ReadedFileCahr:\n%s\n",filechar);
	//printf("%s\n",value);
	//brithplace
	memset(value, 0x00, sizeof(value));
	fgets(filechar,FILELENTH,config);
	char b[CHARLENTH] = "brithplace";
	getkeyvalue(filechar,b,value);
	if(value[strlen(value) - 1] == '\n') value[strlen(value) - 1] = '\0';
	strcpy(contaction->brithplace,value);
	//printf("ReadedFileCahr:\n%s\n",filechar);
	//printf("%s\n",value);
	//number1
	memset(value, 0x00, sizeof(value));
	fgets(filechar,FILELENTH,config);
	char num1[CHARLENTH] = "number1";
	getkeyvalue(filechar,num1,value);
	if(value[strlen(value) - 1] == '\n') value[strlen(value) - 1] = '\0';
	strcpy(contaction->number1,value);
	//printf("ReadedFileCahr:\n%s\n",filechar);
	//printf("%s\n",value);
	//number2
	memset(value, 0x00, sizeof(value));
	fgets(filechar,FILELENTH,config);
	char num2[CHARLENTH] = "number2";
	getkeyvalue(filechar,num2,value);
	if(value[strlen(value) - 1] == '\n') value[strlen(value) - 1] = '\0';
	strcpy(contaction->number2,value);
	//printf("ReadedFileCahr:\n%s\n",filechar);
	//printf("%s\n",value);
	//group
	memset(value, 0x00, sizeof(value));
	fgets(filechar,FILELENTH,config);
	char g[CHARLENTH] = "group";
	getkeyvalue(filechar,g,value);
	if(value[strlen(value) - 1] == '\n') value[strlen(value) - 1] = '\0';
	strcpy(contaction->group,value);
	//printf("ReadedFileCahr:\n%s\n",filechar);
	//printf("%s\n",value);
	//email
	memset(value, 0x00, sizeof(value));
	fgets(filechar,FILELENTH,config);
	char e[CHARLENTH] = "email";	
	getkeyvalue(filechar,e,value);
	if(value[strlen(value) - 1] == '\n') value[strlen(value) - 1] = '\0';
	strcpy(contaction->email,value);
	//printf("ReadedFileCahr:\n%s\n",filechar);
	//printf("%s\n",value);
	fclose(config);
}


int mainmenu()//显示菜单
{
	int x = 0;
	do{
    	printf("==========通讯录管理系统 v1.0==========\n");
    	printf("系统操作菜单：\n");
    	printf("1.浏览通讯录\n"); 
    	printf("2.新建联系人\n");
    	printf("3.查询联系人\n");
	    printf("4.修改联系人\n");
	    printf("5.删除联系人\n");
	    printf("★键入其他数据退出系统\n");
	    printf("请选择要执行的操作：");
	    scanf("%d",&x);
	    switch(x){
		    case 1: menu_viewcontaction();break; 
		    case 2: menu_createcontaction();break;//有二级菜单 
		    case 3: menu_findcontaction();break;//有二级菜单 
		    case 4: menu_editcontaction();break;//有二级菜单 
		    case 5: menu_deletecontaction();break;//有二级菜单 
		    default: printf("输入值有误，请重新输入！\n");break; 
		    case 0: printf("系统正在退出.....");exit(0);break;
    	}
	}while(1);
}

int main()
{
	if(initialize() == -1)//初始化如果出错则退出程序 
	{
		printf("Initializing ERROR");//报错
		return 0;
	}
	
/* 测试用代码 

Contact test,test2;
	test = (Contact)calloc(1,sizeof(Contact));
	strcpy(test->name , "BayMax3");
	strcpy(test->brithplace , "JiangXi");
	strcpy(test->number1 , "18876544321");
	strcpy(test->number2 , "15512345678");
	strcpy(test->group , "classmate");
	strcpy(test->email , "baymaxcloud@foxmail.com");
	writeconfig(test);
	test2 = (Contact)calloc(1,sizeof(Contact));
	readconfig(test2,"BayMax");
	printf("test2->name=%s",test2->name);
	printf("test2->brithplace=%s",test2->brithplace);
	printf("test2->number1=%s",test2->number1);
	printf("test2->number2=%s",test2->number2);
	printf("test2->group=%s",test2->group);
	printf("test2->email=%s",test2->email);
*/  
	
	
	firstmenu(); 
}

/* 测试用代码 

Contact test,test2;
	test = (Contact)calloc(1,sizeof(Contact));
	strcpy(test->name , "BayMax3");
	strcpy(test->brithplace , "JiangXi");
	strcpy(test->number1 , "18876544321");
	strcpy(test->number2 , "15512345678");
	strcpy(test->group , "classmate");
	strcpy(test->email , "baymaxcloud@foxmail.com");
	writeconfig(test);
	test2 = (Contact)calloc(1,sizeof(Contact));
	readconfig(test2,"BayMax");
	printf("test2->name=%s",test2->name);
	printf("test2->brithplace=%s",test2->brithplace);
	printf("test2->number1=%s",test2->number1);
	printf("test2->number2=%s",test2->number2);
	printf("test2->group=%s",test2->group);
	printf("test2->email=%s",test2->email);
	
//*/  






