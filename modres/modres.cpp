// modresource.cpp: 定义控制台应用程序的入口点。
#include <tchar.h>
#include <windows.h>
#include <string>
#include <vector>

int _tmain(int argc, wchar_t** argv)
{
	/*
	argv[0] 本程序路径文件名
	argv[1] 要求改的程序路径文件名
	argv[2] 修改的资源ID
	argv[3] 修改为的内容
	*/
	//Sleep(2000);
	/*if (argc < 4)
		return 0;*/
	HANDLE hResource = BeginUpdateResource(_T("F:\\Work\\ResourceUsageRecoder\\Debug\\ResourceUsageRecoder.exe")/*argv[1]*/, TRUE);
	if (hResource != NULL)
	{
		std::wstring arryData[] =
		{ _T(""), _T(""), _T(""), _T(""),
			_T(""), _T(""), _T(""), _T(""),
			_T(""), _T(""), _T(""), _T(""),
			_T(""), _T(""), _T(""), _T("") };
		//int iResID = _wtoi(argv[2]);
		int iResID = 160;
		arryData[iResID % 16] = _T("8")/*argv[3]*/;
		std::vector< WORD > szBuffer;
		for (size_t index = 0; index < sizeof(arryData) / sizeof(arryData[0]); ++index)
		{
			size_t pos = szBuffer.size();
			szBuffer.resize(pos + arryData[index].size() + 1);
			szBuffer[pos++] = static_cast< WORD >(arryData[index].size());
			copy(arryData[index].begin(), arryData[index].end(), szBuffer.begin() + pos);
		}
		//参数hResource是要修改的PE文件资源的句柄
		//参数RT_STRING是指定要修改的资源类型(一个项目中只能有一个stringtable,是唯一的)
		//想改资源ID为101的字符串7*16=112 MAKEINTRESOURCE(7)对应修改的范围就是16*(7-1)~(16*7-1),配合上arryData数组中的第5字符串对应的就是资源ID为101的字符串
		//MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED),区域/语言相关
		//buffer
		//buffer size
		int i = szBuffer.size() * sizeof(WORD);
		int iUpdateRet = UpdateResource(hResource, RT_STRING, MAKEINTRESOURCE(iResID / 16 + 1), MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), \
			reinterpret_cast< void* >(&szBuffer[0]), szBuffer.size() * sizeof(WORD));
		if (iUpdateRet != 0)
		{
			int iEndRet = EndUpdateResource(hResource, FALSE);
			if (iEndRet != 0)
				OutputDebugString(_T("修改PE文件资源字符串成功\n"));
		}
	}
	return 0;
}
