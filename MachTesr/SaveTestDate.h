#pragma once
class SaveTestDate
{
public:
	SaveTestDate();
	~SaveTestDate();


	CString CreatFile(CString hand);
	int WriteToFile(CString SaveDataName,CString data);
	void CreatInit();
	int ChangeHead(CString SaveDataName,CString hand);
	CString m_csvPath;
	CString m_hand;
private:
	const char* fpath;
};

