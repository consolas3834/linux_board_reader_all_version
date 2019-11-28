#include <iostream> 
#include <map> 
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <mntent.h> 
#include <sys/statfs.h>
#include <limits.h>
#include <search.h>
#include "errno.h"
#include <stdio.h>
#include <stddef.h>
#include <locale.h>
#include "termios.h"
#include "YBTRFAPI.h"
#include<stdlib.h>

/*#ifdef __cplusplus
extern "C" {
#endif
#include "comUtils.h"
#include "utils.h"
#ifdef __cplusplus
}
#endif*/
using namespace std;
char* GetPath(char* out)
{
    char current_absolute_path[PATH_MAX];
    //获取当前程序绝对路径
    int cnt = readlink("/proc/self/exe", current_absolute_path, PATH_MAX);
    if (cnt < 0 || cnt >= PATH_MAX)
    {
        strcpy(out,"./");
        return out;
    }
    //获取当前目录绝对路径，即去掉程序名
    int i;
    for (i = cnt; i >=0; --i)
    {
        if (current_absolute_path[i] == '/')
        {
            current_absolute_path[i+1] = '\0';
            break;
        }
    }
    strcpy(out,current_absolute_path);
    return out;
}

short Convert[55]={ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1,
     -1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,
     -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
     -1,10,11,12,13,14,15};

int HexToText(char* Text, unsigned char* Buffer, int BufSize)
//将以空格分开的十六进制字符串 Text 转换成 PChar 类型字符串 Buffer，BufSize
//指定了 Buffer 的大小，函数返回转换后 Buffer 的实际大小。
{
  int I;
  I = BufSize;
  while (I > 0)
  {
    if (Text[0]==' ')
    {
      Text++;
      continue;
    }
    if ((Text[0]<'0') || (Text[0]>'f') || (Text[1]<'0') || (Text[1]>'f'))
      break;
    Buffer[0]=(Convert[Text[0]-'0'] << 4) + Convert[Text[1]-'0'];
    Buffer++;
    Buffer[0]=0;
    Text+=2;
    I--;
  }
  return BufSize - I;
}

typedef pair <string, string> char_Pair;

int main()
{
    int m_hCom=0;
    char AppPath[256];
    GetPath(AppPath);
    string inifile=AppPath;
    inifile=inifile+"sysit.cfg";
    char* project=(char*)"Reader";
    int ret=MsgOpen(m_hCom,(char*)inifile.c_str(),project);
    if(ret==1)
    {
        printf("open succ.\n");
        ret=MsgPowerOff(m_hCom);

        unsigned char ucWType = 0; 
        unsigned char ucWRegion = 0;
        unsigned char ucType = 2;
        int iStart=0;
	unsigned char ucPwd[5];
	memset(ucPwd, 0, 5);
	unsigned char ucValue[128];
	memset(ucValue, 0, 128);
        unsigned char ucLen;

	unsigned char ucEpcStartPtr = 2; //Epc start from 2
	unsigned char ucEpcLen = 0;
	unsigned char pEpcData[128];
	memset(pEpcData, 0, 128);

	unsigned char ucUserStartPtr = 0;
	unsigned char ucUserLen = 0;
	unsigned char pUserData[256];
	memset(pUserData, 0, 256);

	unsigned char ucReservedStartPtr = 0;
	unsigned char ucReservedLen = 0;
	unsigned char pReservedData[128];
	memset(pReservedData, 0, 128);

        //e2801130200030089b4d0907
        //300f4f573ad001c1059205a2  
        
        string tag="e2801130200030089b4d0907";
        ucLen=HexToText((char*)tag.c_str(),ucValue,128); 
        tag="300f4f573ad001c1059205a6"; 
	ucEpcLen=HexToText((char*)tag.c_str(),pEpcData,128); 
        ret = MsgTagWrite(m_hCom, ucWType,
		ucPwd,
		ucType,
		iStart,
		ucLen,
		ucValue,
		ucEpcStartPtr,
		ucEpcLen,
		pEpcData,
		ucUserStartPtr,
		ucUserLen,
		pUserData,
		ucReservedStartPtr,
		ucReservedLen,
		pReservedData
	);        
        if(ret==1)
        {          
            printf("write Epc %s succ.\n",tag.c_str());                      
        }
	else
	    printf("write Epc %s fail.\n",tag.c_str()); 
        MsgClose(m_hCom);
    }
    else
        printf("open fail\n");
    
    return 0;
}
