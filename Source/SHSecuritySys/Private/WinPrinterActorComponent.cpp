// Fill out your copyright notice in the Description page of Project Settings.

#include "WinPrinterActorComponent.h"
#include "windows.h"
#include <string>
#include "Engine/Engine.h"
#include "FileHelper.h"
#include "LogMacros.h"
#include "UnrealClient.h"
#include "Paths.h"
#include "gdiplus.h"

#pragma comment(lib, "gdiplus")

using namespace Gdiplus;

// Sets default values for this component's properties
UWinPrinterActorComponent::UWinPrinterActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	GdiplusStartupInput gdiplusInput;
	GdiplusStartup(&gdiplusToken, &gdiplusInput, NULL);
}

UWinPrinterActorComponent::~UWinPrinterActorComponent() {
	GdiplusShutdown(gdiplusToken);
}


// Called when the game starts
void UWinPrinterActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PrintScreenState = EPrintScreenState::kIdle;
}


// Called every frame
void UWinPrinterActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//PrintScreenCheck();
}

void UWinPrinterActorComponent::PrintScreen(FString filename)
{
	//if (PrintScreenState != EPrintScreenState::kIdle)
	//	return;	

	FScreenshotRequest::RequestScreenshot(filename, true, false);
	FString fileNameNextScreenshot = FScreenshotRequest::GetFilename();
	FScreenshotRequest::CreateViewportScreenShotFilename(filename);
	PrintScreenPath = FPaths::ConvertRelativePathToFull(fileNameNextScreenshot);
	//PrintScreenState = EPrintScreenState::kPrepare;
}

void UWinPrinterActorComponent::PrintScreenCheck() {
	if (PrintScreenState != EPrintScreenState::kPrepare)
		return;

	if (!FPaths::FileExists(PrintScreenPath))
		return;

	PrintScreenState = EPrintScreenState::kIdle;
	PRINTDLG printInfo = { 0 };
	printInfo.lStructSize = sizeof(printInfo);
	printInfo.Flags = PD_RETURNDC | PD_RETURNDEFAULT | PD_ALLPAGES;
	//PD_RETURNDEFAULT ��ζ��ֱ�ӷ��ص�ǰϵͳĬ�ϵĴ�ӡ�����ã���û�������ʶ����ᵯ���Ի������û��Լ�ѡ��  
	//PD_RETURNDC ��ζ�ŷ��ص���dc������ic��information context��  
	//PD_ALLPAGES ָ����ȫ������ѡť�ڳ�ʼʱ��ѡ��(ȱʡ��־)  
	//���ڴ����ʱ������Ҫ֪������Ĵ�����Ϣ����ִ��CommDlgError���鿴����ֵ  

	//PrintDlgĿ���ǻ�ȡ��ǰϵͳ���õ�Ĭ�ϴ�ӡ�������Ϣ��������ʹ��  
	if (!PrintDlg(&printInfo))
	{
		printInfo.Flags = 0;    //�����־��Ȼ��ִ�н��ᵯ���Ի������û�ѡ���ӡ��  
		if (!PrintDlg(&printInfo))
		{
			UE_LOG(LogStreaming, Warning, TEXT("û��ѡ���ӡ��."));
			return;
		}
	}

	//��ȡ��ӡ��ʱ���dc��Ȼ�������dc�ϻ�ͼ���Ǵ�ӡ������������  
	HDC hPrintDC = printInfo.hDC;

	//����ȫ�ֶ��󣬻�ȡ����ָ�롣 devmode���й��豸��ʼ���ʹ�ӡ����������Ϣ  
	DEVMODE* devMode = (DEVMODE*)GlobalLock(printInfo.hDevMode);
	if (devMode == 0)
	{
		UE_LOG(LogStreaming, Warning, TEXT("��ȡ��ӡ������ʱ�����˴���."));
		return;
	}

	devMode->dmPaperSize = DMPAPER_A4;               //��ӡ��ֽ������ΪA4��  
	devMode->dmOrientation = DMORIENT_PORTRAIT;        //��ӡ�������ó������ӡ  
													   //DMORIENT_LANDSCAPE �Ǻ����ӡ  
													   //�Դ�ӡ��������ã���Ӱ��hPrintDC�Ĵ�С��������Ϊ1024���߶�Ϊ300  
													   //���ں����ӡ��ʱ��dc��С���ǿ�1024 * ��300  
													   //�������ӡ��ʱ��dc��С���ǿ�300 * ��1024  

	int printQuality = devMode->dmPrintQuality;          //��ȡ��ӡ���Ĵ�ӡ����  
														 //devMode->dmPrintQuality = DMRES_MEDIUM;  
														 //���ô�ӡ�����ģ���Ϊ���ر���ӡ��ֽ�ϵ�ʱ��������ת����  
														 //��λ��dpi����Ϊ����ÿӢ��(dots per inch)������һӢ���ֽ����  
														 //��ӡ���ٸ����ص㣬��ζ���������Խ�ߣ���ӡ���Խ��ϸ��Խ�ͣ�Խ�ֲ�  
														 //���õ����������Ǿ�����ֵ��Ҳ�����Ǻ�DMRES_MEDIUM  
														 //һ������ѡ��300������600��DMRES_MEDIUM = 600dpi  

														 //Ӧ�������޸Ĺ��������.  
	ResetDC(hPrintDC, devMode);

	//����ȫ�ֶ��󣬶�ӦGlobalLock  
	GlobalUnlock(printInfo.hDevMode);

	//���û�ͼģʽ���Ա�֤��ͼ���Բ�ʧ��Ļ�����ȥ����ΪStretchDIBits����Ҫ������������ܹ�����ʧ��Ļ�ͼ  
	//������StretchDIBits��ͼ��������ʾ��ʱ��ͻᷢ�֣�24λͼ����û��������ã��ǻ�ʧ���  
	SetStretchBltMode(hPrintDC, HALFTONE);

	int width, height;

 Image image(*PrintScreenPath);
	if (image.GetLastStatus() != Status::Ok) {
		UE_LOG(LogStreaming, Warning, TEXT("��ȡλͼ%sʧ����."), *PrintScreenPath);
		return;
	}

	width = image.GetWidth();
	height = image.GetHeight();


	//���ˣ����Կ�ʼ��ӡ��  
	DOCINFO doc_info = { sizeof(DOCINFO), L"���Դ�ӡ��" };
	//cbSize  
	//�ṹ����ֽڴ�С  
	//lpszDocName  
	//ָ���ĵ������ַ���ָ�룬���ַ�����nullΪβ��  
	//lpszOutput  
	//ָ������ļ������Ƶ��ַ���ָ�룬���ַ�����nullΪβ�����ָ��Ϊnull����ô������ᷢ����ĳ���豸�����豸���ɴ����� StartDoc �����ġ��豸�����ľ����HDC��ָ����  
	//lpszDatatype  
	//ָ��ָ�����ĳ���������͵��ַ��������������ڼ�¼��ӡ���������ַ�����nullΪβ���Ϸ�ֵ��ͨ�����ú��� EnumPrintProcessorDatatypes �ɵõ������Ϊ NULL��  
	//fwType  
	//ָ����ӡ������������Ϣ����Ϊ0��������ֵ֮һ��  
	//DI_APPBANDING  
	//DI_ROPS_READ_DESTINATION  

	//��ʼһ����������ӡ��ʱ���ǰ��յ��������ֵģ�������ҵ��ţ�����0��Ϊ������  
	int doc_idd = StartDoc(hPrintDC, &doc_info);
	if (doc_idd <= 0)
	{
		UE_LOG(LogStreaming, Warning, TEXT("StartDoc ���󣬴�����룺%d."), GetLastError());
		return;
		//goto last_code;
	}
	//printf("��ҵ��ţ�%d\n", doc_idd);

	//��ʼһ����ӡҳ�棬���ڵ���0Ϊ��ȷ  
	if (StartPage(hPrintDC) < 0)
	{
		UE_LOG(LogStreaming, Warning, TEXT("StartPage ����."));
		AbortDoc(hPrintDC); //����doc��ӡ  
		return;
		//goto last_code;
	}


	Graphics graphics(hPrintDC);
	Gdiplus::Rect dstRect;
	graphics.GetVisibleClipBounds(&dstRect);
	dstRect.X += 30;
	dstRect.Y += 30;
	dstRect.Width -= 60;
	dstRect.Height -= 60;
	dstRect.Height = dstRect.Width * image.GetHeight() / image.GetWidth();
	graphics.DrawImage(&image, dstRect);

	//������һҳ����ʵ����ѭ����startpage��endpage��������һ���ĵ��н��ж��ҳ��Ĵ�ӡ  
	EndPage(hPrintDC);
	EndDoc(hPrintDC);
	//printf("��ӡ˳�������. o(��_��)o \n");
}

void UWinPrinterActorComponent::PrintItem(FString content)
{
	/*
	char szprinter[80];
	char *szDevice, *szDriver, *szOutput;
	HDC hdcprint; // ����һ���豸�������
	//����һ����ӡ��ҵ
	static DOCINFO di = { sizeof(DOCINFO),L"printer",NULL };
	// �õ��豸�ַ�����������szprinter��
	GetProfileString(L"windows", L"device", L",,,", (LPWSTR)szprinter, 80);
	// ���豸�ַ����ֽ�
	if (NULL != (szDevice = strtok(szprinter, ",")) && NULL != (szDriver = strtok(NULL, ",")) &&
	NULL != (szOutput = strtok(NULL, ",")))
	{
	if ((hdcprint = CreateDC((LPWSTR)szDriver, (LPWSTR)szDevice, (LPWSTR)szOutput, NULL)) != 0) {
	if (StartDoc(hdcprint, &di)>0) //��ʼִ��һ����ӡ��ҵ
	{

	StartPage(hdcprint); //��ӡ����ֽ,��ʼ��ӡ
	SaveDC(hdcprint); //�����ӡ���豸���
	TextOut(hdcprint, 1, 1, L"testPrint", strlen("testPrint"));
	RestoreDC(hdcprint, -1); //�ָ���ӡ���豸���
	EndPage(hdcprint); //��ӡ��ֽͣ,ֹͣ��ӡ
	EndDoc(hdcprint); //����һ����ӡ��ҵ
	//        MessageBox("��ӡ���!","��ʾ",MB_ICONINFORMATION);
	}
	// ��API����DeleteDC����һ����ӡ���豸���
	DeleteDC(hdcprint);
	}
	else {
	//MessageBox("û��Ĭ�ϴ�ӡ��,����û�а�װ��ӡ��!");
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "No Printer!");
	}
	}
	else
	{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "No Device String!");
	}

	*/

	std::string hello = "!!! Hello Printer !!!\f";
	HANDLE hPrinter = NULL;
	DOC_INFO_1 DocInfo;
	DWORD      dwJob;
	DWORD      dwBytesWritten = 0L;

	FString csDefault;
	GetDefaultPrinterMy(csDefault);


	if (OpenPrinter((LPTSTR)(const_cast<TCHAR*>(*csDefault)), &hPrinter, NULL)) {

		DocInfo.pDocName = (LPTSTR)_T("My Document");
		DocInfo.pOutputFile = NULL;
		DocInfo.pDatatype = (LPTSTR)_T("RAW");
		dwJob = StartDocPrinter(hPrinter, 1, (LPBYTE)&DocInfo);
		if (dwJob != 0) {

			if (StartPagePrinter(hPrinter)) {

				// Send the data to the printer.  
				if (WritePrinter(hPrinter, (void*)hello.c_str(), hello.length(), &dwBytesWritten)) {
					if (dwBytesWritten == hello.length())
					{
						GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "Message sent to printer!");
					}
				}
				EndPagePrinter(hPrinter);

			}
			// Inform the spooler that the document is ending.  
			EndDocPrinter(hPrinter);

		}
		else {

		}
		// Close the printer handle.  
		ClosePrinter(hPrinter);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "No Printer!");
	}


}

void UWinPrinterActorComponent::GetDefaultPrinterMy(FString& csPrinter)
{
	HANDLE hPrinter = NULL;
	DWORD dwSize = 0;
	if ((!::GetDefaultPrinter(NULL, &dwSize)) && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		TCHAR *szPrinter = new TCHAR[dwSize + 1];
		if (::GetDefaultPrinter(szPrinter, &dwSize))
		{
			csPrinter = szPrinter;
		}
		delete[]szPrinter;
	}
}