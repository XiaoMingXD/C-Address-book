#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

/*------���峣��------*/

#define CHARLENTH 256
#define PHONENUMBERLENTH 256
#define DATAPATH "database"
#define FILELENTH 1024 

/*--�����û����Ԫ--*/ 

typedef struct userN{
	char usn[CHARLENTH]; 
	char pwd[CHARLENTH];
	struct userN *next;
};

/*--�������ݴ��Ԫ--*/ 

typedef struct contact{
	char name[CHARLENTH];
	char brithplace[CHARLENTH];
	char number1[PHONENUMBERLENTH];
	char number2[PHONENUMBERLENTH];
	char group[CHARLENTH];
	char email[CHARLENTH];
} *Contact,ContactNode;

/*------��������------*/
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
/*-------��ʼ��-------*/

//������ݿ�Ŀ¼�Ƿ���ڣ��粻�����򴴽�Ŀ¼
int initialize()
{
	if(access(DATAPATH,00) == -1)//00��F_OK�ĺ궨�壬-1����Ȩ�޲���ʧ�ܣ���Ŀ¼������ 
	{
		printf("����Ϊ�״�ʹ�ý��г�ʼ����ʱ����ܽϳ������Ժ�...\n"); 
		if(mkdir(DATAPATH) == -1)//�����ļ��У��ɹ�����0��ʧ�ܷ���-1����������ļ���ʧ���򱨴� 
			return -1;  //main()�м�⵽-1���˳����� 
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

/*------С�����------*/ 


int contactioninfomation(char name[CHARLENTH])//��ӡ��ϵ����ϸ��Ϣ���ɹ�����1��ʧ�ܷ���0 
{
	Contact contaction = (Contact)calloc(1,sizeof(ContactNode));
	if(readconfig(contaction,name) == -1)
	{
			printf("��ϵ����Ϣ��ȡʧ�ܣ�\n");
			return 0 ;
	}
	printf("��ϵ�ˣ�%s ����ϸ��Ϣ���£�\n",name);
	printf("����: %s\n",contaction->name); 
	printf("����: %s\n",contaction->brithplace); 
	printf("�绰1: %s\n",contaction->number1); 
	printf("�绰2: %s\n",contaction->number2); 
	printf("����: %s\n",contaction->group); 
	printf("����: %s\n",contaction->email); 
	return 1;
}

void RemoveSuffix(char *str)//ȥ���ļ�����׺ 
{
	char *p;//��̬�ַ�ָ�� 
	char temp[CHARLENTH] = "";//������ 
	p = str;//ָ�붨λ�ַ���ͷ 
	int i=0,j=0;//�������� 
	for(i=0;i<strlen(str);i++,j++)//ѭ�������ļ�����ÿһ���ַ���ֱ��'.'������ѭ�� 
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

void GetEXEPath(char path[CHARLENTH])  //��ȡ��������Ŀ¼ 
{ 
    char EXEPath[255];
    GetModuleFileName(NULL,EXEPath,255); 
    (strrchr(EXEPath,'\\'))[1] = 0; 
   
    strcpy(path,EXEPath);
}

int ifnumbervaild(char number[CHARLENTH])//�ж�����ĵ绰�����Ƿ����0-9֮����ַ� ���߳��ȷǷ� ,�Ϸ�����1���Ƿ�����0 
{
	//printf("�ж��Ƿ�Ϸ����������������ǣ�%s\n",number);
	//printf("���ȣ�%d\n",strlen(number)); 
	if(strlen(number) != 11)
	{
		//���ȷǷ�
		return 0; 
	}
	else
	{
		char *p;
		p = number;//��λָ�� 
		
		int i = 0;
		for(i=0;i<10;i++)
		{
			if(*p < '0' || *p > '9')
			{
				printf("�Ƿ��ַ���%c\n",*p);
				return 0;
			}
			p++;
		}
		//���ݺϷ�
		return 1; 
	} 
}
 
void trim(char *strIn, char *strOut)//ȥ���ַ�����β�ո� 
{
    char *start, *end, *temp;//ȥ���ո���ַ�����ͷβָ��ͱ���ָ��
    temp = strIn;
    while (*temp == ' ')
	{
        ++temp;
    }
    start = temp; //ͷָ��
    temp = strIn + strlen(strIn) - 1; //�õ�ԭ�ַ������һ���ַ���ָ��
    while (*temp == ' ')
	{
        --temp;
    }
    end = temp; //βָ��
    for(strIn = start; strIn <= end; )
	{
        *strOut++ = *strIn++;
    }
 	*strOut = '\0';
}

void getkeyvalue(char keyAndValue[CHARLENTH], char key[CHARLENTH], char value[CHARLENTH])//ͨ�������н���valueֵ 
{
	char *p = keyAndValue;//����ָ�� 
    p = strstr(keyAndValue, key);//��λkey 
    if(p == NULL)
	{
        printf("��ֵ����\n");
        return;
    }
    p += strlen(key);//����key�ַ��� 
    trim(p, value);//ȥ���ո� 
    p = strstr(value, "=");//��λ= 
    if(p == NULL)
	{
        printf("����ֵ����û��=\n");
        return;
    }
    p+= strlen("=");//����= 
    trim(p, value);
    p = strstr(value, "=");
    if(p != NULL)
	{
        printf("����ֵ���󣡶����=\n");
        return;
    }
    p = value;
    trim(p, value);

} 
/*------�˵�����------*/

int firstmenu()//ע���û���Ϣ 
{
	int Ret=-1;
	do{
		int x = 0;
		printf("==========ͨѶ¼����ϵͳ v1.0==========\n");
		printf("���ȵ����ע�ᣡ\n");
		printf("1.����\n"); 
		printf("2.ע��\n");
		do{
			printf("��ѡ��Ҫִ�еĲ�����");
			scanf("%d",&x);
    		switch(x){
	    		case 1:Ret=loginmenu();break;
    			case 2:registermenu();break;
    			default:printf("����ֵ����\n");break;
    		}
		}while(x != 1 && x != 2);
	}while(Ret != 2);//����ֵΪ2�����½�ɹ� 
	mainmenu();
}

int loginmenu(){
	userN *head = loadUser();
	userN *here;
	char usn[CHARLENTH]="",pwd[CHARLENTH]="",pwdc[CHARLENTH]="";//pwd���û����������
	printf("�������û���:"); 
	scanf("%s",&usn);
	here=findUser(head,usn);
	if(here==NULL){
		printf("�û��������ڣ�������\n");
		return -1;
	}
	printf("����������:"); 
	scanf("%s",&pwd);
	if(strcmp(here->pwd,pwd)!=0){
		printf("�������������\n");
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
	printf("��½�ɹ���\n");
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
	printf("��������Ҫע����û���:"); 
	scanf("%s",&usn);
	printf("���ڲ�ѯ�û���%s�Ƿ����...\n",usn);
	strcat(path,usn);
	if(access(path,00) == 0){//00��F_OK�ĺ궨�壬0����Ȩ�޲��Գɹ�����Ŀ¼����  
 		printf("�û���%s�ѱ�ע�ᣬ�볢�Ե�½\n",&usn);
 		return -1; 
	} 
	do{
		printf("�û���%s����,����������:",usn); 
		scanf("%s",&pwd);
		printf("��ȷ������:"); 
		scanf("%s",&pwdc);
		if(*pwd==*pwdc)
			break;
		else
			printf("������������벻һ�£�������\n");
	}while(true);
	printf("����Ϊ�������û������Ժ�...\n");
	if(registing(usn,pwd)==-1){
		printf("�����û�ʧ�ܣ���������Ȩ�ޣ�������������ԡ�\n"); 
		return -1;
	}
 	printf("�û������ɹ�,����������������ҳ��...\n");	
 	return 1;
}

int registing(char* usn, char* pwd){
	char path[CHARLENTH];
	strcpy(path,DATAPATH);
	strcat(strcat(path,"\\"),usn);
	if(mkdir(path) == -1)//�����ļ��У��ɹ�����0��ʧ�ܷ���-1����������ļ���ʧ���򱨴� 
		return -1;
	char filePath[CHARLENTH];
	strcpy(filePath,DATAPATH);
	strcat(filePath,"\\user.ini");
	FILE *user=fopen(filePath,"a");
	char filechar[FILELENTH] = "";//�����ļ��ַ���
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
	userN *head,*node,*end;//����ͷ�ڵ㣬��ͨ�ڵ㣬β���ڵ㣻
	int rowc=row(user);
	rowc/=2;
    head = (userN*)malloc(sizeof(userN));//�����ַ
    end = head;//���ǿ�������ͷβ�ڵ�һ��
    for (int i = 0; i < rowc; i++) {
        node = (userN*)malloc(sizeof(userN));
        //��ȡ��ֵ 
		//usn
		fgets(filechar,FILELENTH,user);
		char usn[CHARLENTH] = "usn";
		getkeyvalue(filechar,usn,value);
		if(value[strlen(value) - 1] == '\n') 
			value[strlen(value) - 1] = '\0';
		strcpy(node->usn,value);
		//��ȡ��ֵ 
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
    end->next = NULL;//��������
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
	printf("������ϵ�������Բ鿴���飬����quit������һ��\n");
	char name[CHARLENTH] = "";
	char quit[CHARLENTH] = "quit";
	do{		
		scanf("%s",name);
	    if(strstr(name,quit) != NULL)
	       return 1; 
	    else
	    	if(findcontaction(1,name) == 0 )//����0��ʾ�Ҳ����ļ������ļ���ȡʧ�� 
    			printf("����ϵ�˲����ڻ����ݴ���\n������Ҫ��ѯ����ϵ��������");
     		else
     			break;//��ѯ�ɹ�������ѭ�� 
	}while(1); 
}

int viewcontaction()//���������ϵ�� �����������ϵ�˻��ߴ�ʧ�ܷ���0���ɹ��г�����1 
{
	printf("\n-=-ȫ����ϵ��-=-\n\n");
	DIR *dir_p;//����Ŀ¼ָ�� 
	struct dirent *entry;//����dirent�е�Ŀ¼���Խṹ�� 
	char path[CHARLENTH];
	GetEXEPath(path);
	char current[CHARLENTH]="";
	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
	fclose(cnt);
	dir_p = opendir(strcat(strcat(strcat(path,DATAPATH),"//"),current));//��Ŀ¼��д��Ŀ¼ָ�� 
	if(dir_p == NULL)//���Ŀ¼�����ڻ���ϵͳȨ����ֹ 
	{
		printf("�����ݿ�ʧ�ܣ�\n");
		return 0; 
	}
	else
		while((entry = readdir(dir_p)) != NULL)//ѭ����ȡ�����������ļ����ٶ������NULL 
			if(entry->d_name[0] != '.'){
				RemoveSuffix(entry->d_name);
				printf("%s\n",entry->d_name);//��ӡ��ȡ�����ļ��� 
		    }
	closedir(dir_p);
	printf("\n-=-��ȫ���г�-=-\n");
	
} 

int menu_createcontaction()//�½���ϵ�˲˵� 
{
	char temp_path[CHARLENTH] = "";
	Contact newcontaction;
	newcontaction = (Contact)calloc(1,sizeof(ContactNode));
	printf("==============�½���ϵ��==============\n");
	do{
		printf("��������ϵ������:\n");
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
	    if(access(temp_path,00) == -1)//00��F_OK�ĺ궨�壬-1����Ȩ�޲���ʧ�ܣ����ļ�������
	    {
	    	break;
		}
		else
		{
			printf("����ϵ���Ѵ��ڣ�����������\n");
		}
	} while(1);
	
	
	printf("��������ϵ�����ڵ�(����):\n");
	scanf("%s",newcontaction->brithplace);
	
	do{
	    printf("��������ϵ�˵绰����1:\n");
	    scanf("%s",newcontaction->number1);
	    if(ifnumbervaild(newcontaction->number1) == 0)
	    {
	    	printf("�������ݷǷ������������룡\n");
	    }
	    else
	    {
	    	break;
		}
	}while(1);
	
	do{
	    printf("��������ϵ�˵绰����2:\n");
	    scanf("%s",newcontaction->number2);
	    if(ifnumbervaild(newcontaction->number2) == 0)
	    {
	    	printf("�������ݷǷ������������룡\n");
	    }
	    else
	    {
	    	break;
		}
	}while(1);
	
	printf("��������ϵ�˷���:\n");
	scanf("%s",newcontaction->group);
	printf("��������ϵ������:\n");
	scanf("%s",newcontaction->email);
	//����������ɣ����ڴ�����ϵ��...... 
	printf("����������ɣ����ڴ�����ϵ��...... \n");
	if(createcontaction(newcontaction) == 0) 
	{
		printf("�½���ϵ��ʧ�ܣ�\n");
		return 0;
    }
    else
    {
    	printf("�½���ϵ�����\n");
    	return 1;
	}
}

int createcontaction(Contact newcontaction)//�½���ϵ�ˣ��ɹ�����1��ʧ�ܷ���0
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

int menu_findcontaction()//��ѯ��ϵ�˲˵� ,��ϵ�˲����ڷ���0���ɹ���ѯ����1���г���Ϣ 
{
	
	printf("\n========��ѯ��ϵ��========\n");
	printf("��ѡ���ѯ���ͣ�\n");
	printf("1.����������ѯ\n");
	printf("2.���ռ����ѯ\n");
	printf("3.���յ绰��ѯ\n");
	printf("4.���շ����ѯ\n");
	printf("5.���������ѯ\n");
	int mode = 0;
	do{
		printf("�������ѯ���ͣ�\n");
        scanf("%d",&mode);
    	if(mode < 0 || mode > 5)
    		printf("����ֵ�������������룺\n");
	    else
    		break;
	}while(1);
	char value[CHARLENTH] = "";
	printf("������ؼ��֣�\n");
	scanf("%s",value);
	if(findcontaction(mode,value) == 0)
	{
		printf("�޷��ҵ���Ӧ����\n");
	}
	
}

int findcontaction(int mode,char *value)
//������ϵ��(������) ��ѯ�ɹ�����1��ʧ�ܷ���0 
//����1Ϊ��ѯ��ǩ����
//1.����������ѯ
//2.���ռ����ѯ
//3.���յ绰��ѯ
//4.���շ����ѯ
//5.���������ѯ 
//����2Ϊ��ǩֵ 
{
	int statu = 0;//searchitem����ֵ��ʱ��� 
	switch (mode){
		case 1: //1.����������ѯ 
		{ 
			if(contactioninfomation(value) == 0)
			{
				return 0;
			}
			break; 
		} 
		case 2: //2.���ռ����ѯ
		    {
			    printf("���ڲ��Ҽ���Ϊ��%s����ϵ��......\n������������ϵ�����£�\n",value);
			    statu = searchitem(2,value);
	    	    if(statu == 0)
	    	    {
	    	    	printf("�޷�����������ϵ�ˡ�\n");
				}
				else if(statu == 1)
				{
					printf("�������\n");
				}
				else
				{
					printf("���ҳ���\n");
				}
	    	    break; 
			}
		case 3: //3.���յ绰��ѯ
		    {
			    printf("���ڲ��ҵ绰Ϊ��%s����ϵ��......\n������������ϵ�����£�\n",value);
			    statu = searchitem(3,value);
	    	    if(statu == 0)
	    	    {
	    	    	printf("�޷�����������ϵ�ˡ�\n");
				}
				else if(statu == 1)
				{
					printf("�������\n");
				}
				else
				{
					printf("���ҳ���\n");
				}
	    	    break; 
			}
		case 4: //4.���շ����ѯ
		    {
			    printf("���ڲ��ҷ���Ϊ��%s����ϵ��......\n������������ϵ�����£�\n",value);
			    statu = searchitem(4,value);
	    	    if(statu == 0)
	    	    {
	    	    	printf("�޷�����������ϵ�ˡ�\n");
				}
				else if(statu == 1)
				{
					printf("�������\n");
				}
				else
				{
					printf("���ҳ���\n");
				}
	    	    break; 
			}
		case 5: //5.���������ѯ
		    {
			    printf("���ڲ�������Ϊ��%s����ϵ��......\n������������ϵ�����£�\n",value);
			    statu = searchitem(5,value);
	    	    if(statu == 0)
	    	    {
	    	    	printf("�޷�����������ϵ�ˡ�\n");
				}
				else if(statu == 1)
				{
					printf("�������\n");
				}
				else
				{
					printf("���ҳ���\n");
				}
	    	    break; 
			}
		default: return 0;
	}
	printf("������ϵ�������Բ鿴���飬����quit�������˵���");
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

int menu_editcontaction()//�༭��ϵ�� �˵� 
{
	viewcontaction();//���г�������ϵ��
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
		printf("������Ҫ�޸ĵ���ϵ��������");
		scanf("%s",name);
    	strcpy(temp_path,DATAPATH);
    	strcat(temp_path,"\\");
        strcat(temp_path,current);
    	strcat(temp_path,"\\");
        strcat(temp_path,name);
     	strcat(temp_path,".ini");
    	if(access(temp_path,00) == -1)
    	{
    		printf("\n����ϵ�˲����ڣ�\n");
		}
		else
		{
			break;
		}
	}while(1);
	Contact contaction = (Contact)calloc(1,sizeof(ContactNode));
	if(readconfig(contaction,name) == -1)
	{
			printf("��ϵ����Ϣ��ȡʧ�ܣ�\n");
			return -1 ;
	}
	printf("��ϵ�ˣ�%s ����ϸ��Ϣ���£�\n",name);
	printf("1.����: %s\n",contaction->brithplace); 
	printf("2.�绰1: %s\n",contaction->number1); 
	printf("3.�绰2: %s\n",contaction->number2); 
	printf("4.����: %s\n",contaction->group); 
	printf("5.����: %s\n",contaction->email); 
	
	int item = 0;
	do{
		printf("\n��ѡ��Ҫ�޸ĵ���Ŀ��");
    	scanf("%d",&item);
    	if(item > 0 || item < 6)
    	{
    		break;
		}
		else
		{
			printf("\n����ֵ�������������룺");
		}
	}while(1);
	
	char value[CHARLENTH] = "";
	printf("\n�������µ����ݣ�");
    scanf("%s",value);
	
	
	
	if(editcontaction(name,item,value) == -1)
	{
		printf("��ϵ����Ϣ�޸�ʧ�ܣ�\n");
		return 0;
	}
	else
	{
		printf("��ϵ����Ϣ�޸���ɣ�\n");
		return 1;
	}
	
}

int editcontaction(char name[CHARLENTH],int item,char value[CHARLENTH])//�޸���ϵ�ˣ�����1Ϊ��ϵ������,����2Ϊ�޸���Ŀ������3Ϊ�µ�ֵ 
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
	remove(temp_filepath);//��ɾ�����ļ�Ȼ����д�����ļ� 
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

int menu_deletecontaction()//ɾ����ϵ�˲˵� 
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
		printf("������Ҫɾ������ϵ��������");
		scanf("%s",name);
     	strcpy(temp_path,DATAPATH);
     	strcat(temp_path,"\\");
        strcat(temp_path,current);
     	strcat(temp_path,"\\");
        strcat(temp_path,name);
     	strcat(temp_path,".ini");
     	if(access(temp_path,00) == -1)
    	{
        	printf("\n����ϵ�˲����ڣ�\n");
    	}
    	else
    	{
    		break;
		}
	}while(1);
	
	if(deletecontaction(name) == 0)
	{
		printf("ɾ����ϵ��ʧ�ܣ�\n");
		return 0;
	}
	else
	{
		return 1;
	}
	
}

int deletecontaction(char name[CHARLENTH])//ɾ����ϵ�ˣ��ɹ�����1��ʧ�ܷ���0 
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

int searchitem(int item,char value[CHARLENTH])//������-1���ɹ�����1 ,δ�ҵ�����0������1Ϊ��Ŀ���ƣ�ȡֵ2-5 
{
	int t = 0 ;//�ҵ��ķ�����������ϵ������ 
	Contact contaction = (Contact)calloc(1,sizeof(ContactNode));
	DIR *dir_p;//����Ŀ¼ָ�� 
	struct dirent *entry;//����dirent�е�Ŀ¼���Խṹ�� 
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
	dir_p = opendir(path);//��Ŀ¼��д��Ŀ¼ָ�� 
	if(dir_p == NULL)//���Ŀ¼�����ڻ���ϵͳȨ����ֹ 
	{
		printf("�����ݿ�ʧ�ܣ�\n");
		return -1; 
	}
	else
	{
		while((entry = readdir(dir_p)) != NULL)//ѭ����ȡ�����������ļ����ٶ������NULL 
		{
			if(entry->d_name[0] != '.')
			{
		    	RemoveSuffix(entry->d_name);
		    	if(readconfig(contaction,entry->d_name) == -1)
		    	{
		    		return -1; 
		    	}
		    	//��ʼ��֧�ж� 
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

/*------�����ļ�------*/

//д������

int writeconfig(Contact contaction)//д��ɹ�����1��ʧ�ܷ���0
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
	strcat(filename,".ini");//�����ļ��� 
	strcat(strcat(filepath,"\\"),filename);//ƴ���ļ�·�� 
	FILE *config = fopen(filepath,"w+");//�����ļ�ָ�� 
	char filechar[FILELENTH] = "";//�����ļ��ַ���
	//ƴ���ַ��� 
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

//��ȡ����
int readconfig(Contact contaction,char name[])//������-1 
{
	char filepath[CHARLENTH] = "";//�����ļ�·������ 
   	char current[CHARLENTH]="";
   	char cntPath[CHARLENTH]="";
   	strcpy(cntPath,DATAPATH);
   	strcat(cntPath,"\\current.ini");
   	FILE *cnt = fopen(cntPath,"r");
   	fgets(current,FILELENTH,cnt);
   	fclose(cnt);
	strcat(strcat(strcat(strcat(strcat(strcat(filepath,DATAPATH),"\\"),current),"\\"),name),".ini");//ƴ���ļ�·�� 
	FILE *config = fopen(filepath,"r");//���ļ�
	if(config == NULL)
	{   //�ļ���ʧ�� 
    	printf("readconfig faild");
		fclose(config);
		return(-1);
	}
	char filechar[FILELENTH] = "";
	char keyandvalue[CHARLENTH] = "";
	char key[CHARLENTH] = "";
	char value[CHARLENTH] = "";
	//��ȡ��ֵ 
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


int mainmenu()//��ʾ�˵�
{
	int x = 0;
	do{
    	printf("==========ͨѶ¼����ϵͳ v1.0==========\n");
    	printf("ϵͳ�����˵���\n");
    	printf("1.���ͨѶ¼\n"); 
    	printf("2.�½���ϵ��\n");
    	printf("3.��ѯ��ϵ��\n");
	    printf("4.�޸���ϵ��\n");
	    printf("5.ɾ����ϵ��\n");
	    printf("��������������˳�ϵͳ\n");
	    printf("��ѡ��Ҫִ�еĲ�����");
	    scanf("%d",&x);
	    switch(x){
		    case 1: menu_viewcontaction();break; 
		    case 2: menu_createcontaction();break;//�ж����˵� 
		    case 3: menu_findcontaction();break;//�ж����˵� 
		    case 4: menu_editcontaction();break;//�ж����˵� 
		    case 5: menu_deletecontaction();break;//�ж����˵� 
		    default: printf("����ֵ�������������룡\n");break; 
		    case 0: printf("ϵͳ�����˳�.....");exit(0);break;
    	}
	}while(1);
}

int main()
{
	if(initialize() == -1)//��ʼ������������˳����� 
	{
		printf("Initializing ERROR");//����
		return 0;
	}
	
/* �����ô��� 

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

/* �����ô��� 

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






