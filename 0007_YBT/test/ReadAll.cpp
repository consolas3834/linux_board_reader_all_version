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

        int iTidLen = 0;
        int iUserLen = 0;
        int iSavlen = 0;
        int iNeedEpc = 1;

        iTidLen = 6;
        iUserLen = 0;
        iSavlen = 0;
                
        unsigned char pPwd[4];
        memset(pPwd, 0, 4);
        ret = MsgTagRead(m_hCom,
                             1,                  //0单次 1表示连续
                             pPwd,
                             iNeedEpc,            //1，返回epc,0不要返回epc
                             0,  //tid开始地址
                             iTidLen,
                             0,   //0 tid固定 1自适应长度
                             0,    //用户数据开始地址
                             iUserLen, //用户数据长度
                             0,  //保留区开始地址
                             iSavlen,  //保留区长度
                             0,   //设置读多少次停止
                             0    //设置读多少时间停止
                            );        
        if(ret==1)
        {          
            unsigned char pEpc[128];
            unsigned char pEpcLen = 0;
            unsigned char ucAntenna = 0;
            unsigned char ucRssi;
            map<string,string> mplist;
            
            map<HANDLE, char*>::const_iterator map_iter;
            while(1)
            {
                unsigned char  pEpc[128];
                memset(pEpc, 0, 128);
                unsigned char pEpcLen = 0;
                unsigned char  pTid[128];
                memset(pTid, 0, 128);
                unsigned char pTidLen = 0;
                unsigned char pUser[256];
                memset(pUser, 0, 256);
                unsigned char pUserLen = 0;
                unsigned char pReverse[128];
                memset(pReverse, 0, 0);
                unsigned char ucReverseLen = 0;
                unsigned char ucAntenna = 0;
                unsigned char ucRssi = 0;
                string str = "";
                string strTemp="";
                string strValue = "";
                int i_index = -1;
                        
                ret = MsgTagInventoryAllReport(m_hCom,
                                               pEpc,
                                               &pEpcLen,
                                               pTid,
                                               &pTidLen,
                                               pUser,
                                               &pUserLen,
                                               pReverse,
                                               &ucReverseLen,
                                               &ucAntenna,
                                               &ucRssi);  
                if (ret == 1)
                {
                    string strID="";
                    string strTianXian="";
                    string strRSSI="";
                    string strUser="";
                    string strEpc="";
                    string strReverse="";
                    char  strTemp[10];
                    for (int i = 0; i < pTidLen; i++)
                    {
                        sprintf(strTemp,"%02x", pTid[i]);
                        strID += strTemp;
                    }                    
                    if (strID != "")
                    {
                        for (int i = 0; i < pEpcLen; i++)
                        {
                            sprintf(strTemp,"%02x", pEpc[i]);
                            strEpc += strTemp;
                        }
                                                
                        for (int i = 0; i < pUserLen; i++)
                        {
                            sprintf(strTemp,"%02x", pUser[i]);
                            strUser += strTemp;
                        }
                                                
                        for (int i = 0; i < ucReverseLen; i++)
                        {
                            sprintf(strTemp,"%02x", pReverse[i]);
                            strReverse += strTemp;
                        }
                        

                        sprintf(strTemp,"%d#", ucAntenna);
                        strTianXian=strTemp;

                                                
                        sprintf(strTemp,"%d", ucRssi);
                        strRSSI=strTemp;
                        
                        if (mplist.end() == mplist.find(strID))
                        {        
                            mplist.insert(char_Pair(strID, strID));
                            printf("EPC:%s    TID:%s    天线:%s    RSSI:%s\n",strEpc.c_str(),strID.c_str(),strTianXian.c_str(),strRSSI.c_str()); 
                        }
                        else
                            break;                             
                    }
                    //break;
                }
                usleep(200*1000);
            }            
                       
        }
        MsgClose(m_hCom);
    }
    else
        printf("open fail\n");
    
    return 0;
}
