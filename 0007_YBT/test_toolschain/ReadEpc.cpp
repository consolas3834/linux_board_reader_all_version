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
        unsigned char ucAntenna;
        unsigned char ucRssi;    
        bool m_bAnter=false;
        bool m_bRSSI=false;
        ret=Msg6CTagFieldQuery(m_hCom, &ucAntenna, &ucRssi);
        if(ret==1)
        {
            if (ucAntenna == 1)
                m_bAnter=true;
            
            if (ucRssi == 1)
                m_bRSSI=true; 
            ret = MsgTagInventory(m_hCom, 0, 0); 
            if(ret==1)
            {
                unsigned char pEpc[128];
                unsigned char pEpcLen = 0;
                unsigned char ucAntenna = 0;
                unsigned char ucRssi;
                string strID="";
                string strTianXian="";
                string strRSSI="";
                map<string,string> mplist;
                
                map<HANDLE, char*>::const_iterator map_iter;
                while(1)
                {
                    ret = MsgTagInventoryReport(m_hCom, pEpc, &pEpcLen, &ucAntenna, &ucRssi);
                    if (ret == 1)
                    {
                        strID="";
                        strTianXian="";
                        strRSSI="";
                        char  strIDTemp[10];
                        for (int i = 0; i < pEpcLen; i++)
                        {
                            sprintf(strIDTemp,"%02x", pEpc[i]);
                            strID += strIDTemp;
                        }
                        if (strID != "")
                        {
                            if (m_bAnter)
                            {
                                sprintf(strIDTemp,"%d#", ucAntenna);
                                strTianXian=strIDTemp;
                            }
                            else
                            {
                                strTianXian = "";
                            }
                            if (m_bRSSI)
                            {
                                sprintf(strIDTemp,"%d", ucRssi);
                                strRSSI=strIDTemp;
                            }
                            else
                            {
                                strRSSI = "";
                            }
                            
                            if (mplist.end() == mplist.find(strID))
                            {        
                                mplist.insert(char_Pair(strID, strID));
                                printf("EPC:%s    天线:%s    RSSI:%s\n",strID.c_str(),strTianXian.c_str(),strRSSI.c_str()); 
                            }
                            else
                                break;                             
                        }
                        //break;
                    }
                    usleep(200*1000);
                }            
            }           
        }
        MsgClose(m_hCom);
    }
    else
        printf("open fail\n");
    
    return 0;
}
