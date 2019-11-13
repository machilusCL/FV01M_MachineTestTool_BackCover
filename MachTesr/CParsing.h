#pragma once
#include"CanComm.h"


class CParsing
{
public:
	CParsing();
	~CParsing();

	CString ParsingCString(CString str);
	CString ParsingCString1(CString str);
	bool FillingFrameData(FrameData *mFrameData);
	bool FillingFrameData1(FrameData *mFrameData);

	
};

