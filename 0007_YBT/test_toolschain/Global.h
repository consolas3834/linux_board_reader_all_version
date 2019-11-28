#if !defined _GLOBLE
#define   _GLOBLE
#include <string>
#include <stdbool.h>
#include <cstring>
#define _MAX_PATH 256
#define INVALID_HANDLE_VALUE -1
#define TRUE 1
#define FALSE 0
using namespace std;

typedef bool BOOL;
typedef unsigned int UINT;
typedef int HANDLE;
typedef unsigned int DWORD;


unsigned short crc16(unsigned char pMsg, unsigned short crc);

long MemLoad(string cInitFile,string cMemItem,string cMemKey);

string MemLoadStr(string cInitFile,string cMemItem,string cMemKey);

void MemSet(string cInitFile,string cMemItem,string cMemKey,string cValue);

unsigned sleep2(unsigned long milliseconds);


template<typename T>
inline BOOL GetBit(T t, UINT nBit)
{
	return ((t >> nBit) & 1);
}

template<typename T>
inline void SetBitOn(T *t, UINT nBit)
{
	*t |= 0x01 << nBit;
}

template<typename T>
inline void SetBitOff(T *t, UINT nBit)
{
	*t &= ~(0x01 << nBit);
}

template<typename T>
void SetBit(T *t, UINT nBit, BOOL bFlag)
{
	bFlag ? SetBitOn(t, nBit) : SetBitOff(t, nBit);
}

#endif