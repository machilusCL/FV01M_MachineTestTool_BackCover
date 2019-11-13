#include "stdafx.h"
#include "SaveTestDate.h"
#include "MakeSure.h"

SaveTestDate::SaveTestDate()
{
}


SaveTestDate::~SaveTestDate()
{
}

CString  SaveTestDate::CreatFile(CString hand)
{
	m_csvPath.Empty();
	m_csvPath.Append(_T("D:\\FV01M生产测试\\整机测试(有后盖)\\"));

	SYSTEMTIME time;
	CString timestr;
	GetLocalTime(&time);
	timestr.Format(_T("%d-"), time.wYear);
	m_csvPath.Append(timestr);
	timestr.Format(_T("%d-"), time.wMonth);
	m_csvPath.Append(timestr);
	timestr.Format(_T("%d\\"), time.wDay);
	m_csvPath.Append(timestr);

	MakeSureDirectoryPathExists(m_csvPath);
	m_csvPath.Append(_T("Result.csv"));

	fpath = CStringA(m_csvPath.GetBuffer());
	m_csvPath.ReleaseBuffer();
	printf("%s\n", fpath);
	const char *fhand = CStringA(hand.GetBuffer());
	hand.ReleaseBuffer();
	FILE *fb = fopen(fpath, "r");
	if (fb) {

		printf("open the file!\n");
	}
	else
	{
		printf("creat the file!\n");
		fb = fopen(fpath, "a+");
		if (!fb) {
			printf("creat the file fail!\n");
			return false;
		}
		fprintf(fb, "%s", "编号,");
		fprintf(fb, "%s", "测试结果,");
		fprintf(fb, "%s", "ZYNQ版本,");
		fprintf(fb, "%s", "MCU版本,");
		fprintf(fb, "%s", fhand);
		fprintf(fb, "\n");
		m_hand = hand;
	}
	fclose(fb);

	printf("Close the file!\n");
	return m_csvPath;
}

int SaveTestDate::WriteToFile(CString SaveDataName, CString data){

	const char *fpath1 = CStringA(SaveDataName.GetBuffer());
	const char *fdata = CStringA(data.GetBuffer());
	SaveDataName.ReleaseBuffer();
	data.ReleaseBuffer();
	printf("%s\n", fpath1);
	FILE *fb = fopen(fpath1, "a+");
	if (!fb)
	{
		return -1;
	}
	else
	{
		printf("写入数据\n");
		fprintf(fb, "%s", fdata);
		fprintf(fb, "\n");
		fclose(fb);
	}
	
	return 0;
}

int SaveTestDate::ChangeHead(CString SaveDataName, CString hand) {

	const char *fhand = CStringA(hand);
	const char *fpath1 = CStringA(SaveDataName.GetBuffer());
	if (0 != m_hand.CompareNoCase(hand))
	{
		FILE *fb = fopen(fpath1, "a+");
		if (!fb) {
			printf("creat the file fail!\n");
			return -1;
		}
		printf("change the file!\n");
		fprintf(fb, "%s", "编号,");
		fprintf(fb, "%s", "测试结果,");
		fprintf(fb, "%s", "ZYNQ版本,");
		fprintf(fb, "%s", "MCU版本,");
		fprintf(fb, "%s", fhand);
		fprintf(fb, "\n");
		m_hand = hand;
		fclose(fb);
	}
	return 0;
}

void SaveTestDate::CreatInit() {

	FILE *fb = fopen(".\\setting.ini", "r");
	if (fb) {
		printf("open the file!\n");
	}
	else
	{
		printf("creat the file!\n");
		fb = fopen(fpath, "a+");
		if (!fb) {
			printf("creat the file fail!\n");
			return ;
		}
	}

	fclose(fb);
}