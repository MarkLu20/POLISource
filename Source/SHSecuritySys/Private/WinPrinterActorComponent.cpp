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
	//PD_RETURNDEFAULT 意味着直接返回当前系统默认的打印机设置，若没有这个标识，则会弹出对话框让用户自己选择  
	//PD_RETURNDC 意味着返回的是dc而不是ic（information context）  
	//PD_ALLPAGES 指定“全部”单选钮在初始时被选中(缺省标志)  
	//对于错误的时候，若需要知道更多的错误消息，请执行CommDlgError来查看返回值  

	//PrintDlg目的是获取当前系统设置的默认打印机相关信息，供后面使用  
	if (!PrintDlg(&printInfo))
	{
		printInfo.Flags = 0;    //清除标志，然后执行将会弹出对话框让用户选择打印机  
		if (!PrintDlg(&printInfo))
		{
			UE_LOG(LogStreaming, Warning, TEXT("没有选择打印机."));
			return;
		}
	}

	//获取打印的时候的dc，然后往这个dc上绘图就是打印出来的样子了  
	HDC hPrintDC = printInfo.hDC;

	//锁定全局对象，获取对象指针。 devmode是有关设备初始化和打印机环境的信息  
	DEVMODE* devMode = (DEVMODE*)GlobalLock(printInfo.hDevMode);
	if (devMode == 0)
	{
		UE_LOG(LogStreaming, Warning, TEXT("获取打印机设置时发生了错误."));
		return;
	}

	devMode->dmPaperSize = DMPAPER_A4;               //打印机纸张设置为A4。  
	devMode->dmOrientation = DMORIENT_PORTRAIT;        //打印方向设置成纵向打印  
													   //DMORIENT_LANDSCAPE 是横向打印  
													   //对打印方向的设置，会影响hPrintDC的大小，假设宽度为1024，高度为300  
													   //则在横向打印的时候dc大小会是宽1024 * 高300  
													   //而纵向打印的时候dc大小会是宽300 * 高1024  

	int printQuality = devMode->dmPrintQuality;          //获取打印机的打印质量  
														 //devMode->dmPrintQuality = DMRES_MEDIUM;  
														 //设置打印质量的，因为像素被打印到纸上的时候是有做转换的  
														 //单位是dpi，意为像素每英寸(dots per inch)。就是一英寸的纸张上  
														 //打印多少个像素点，意味着这个质量越高，打印结果越精细，越低，越粗糙  
														 //设置的质量可以是具体数值，也可以是宏DMRES_MEDIUM  
														 //一般我们选择300，或者600，DMRES_MEDIUM = 600dpi  

														 //应用我们修改过后的设置.  
	ResetDC(hPrintDC, devMode);

	//解锁全局对象，对应GlobalLock  
	GlobalUnlock(printInfo.hDevMode);

	//设置绘图模式，以保证绘图可以不失真的绘制上去，因为StretchDIBits函数要求设置这个才能够不是失真的绘图  
	//当你用StretchDIBits绘图往窗口显示的时候就会发现，24位图，若没有这个设置，是会失真的  
	SetStretchBltMode(hPrintDC, HALFTONE);

	int width, height;

 Image image(*PrintScreenPath);
	if (image.GetLastStatus() != Status::Ok) {
		UE_LOG(LogStreaming, Warning, TEXT("读取位图%s失败了."), *PrintScreenPath);
		return;
	}

	width = image.GetWidth();
	height = image.GetHeight();


	//好了，可以开始打印了  
	DOCINFO doc_info = { sizeof(DOCINFO), L"测试打印机" };
	//cbSize  
	//结构体的字节大小  
	//lpszDocName  
	//指明文档名的字符串指针，该字符串以null为尾。  
	//lpszOutput  
	//指明输出文件的名称的字符串指针，该字符串以null为尾。如果指针为null，那么输出将会发送至某个设备，该设备将由传递至 StartDoc 函数的‘设备上下文句柄’HDC来指明。  
	//lpszDatatype  
	//指针指向代表某种数据类型的字符串，而数据用于记录打印工作，该字符串以null为尾。合法值可通过调用函数 EnumPrintProcessorDatatypes 可得到，亦可为 NULL。  
	//fwType  
	//指明打印工作的其它信息。可为0或者以下值之一：  
	//DI_APPBANDING  
	//DI_ROPS_READ_DESTINATION  

	//开始一个档案，打印的时候是按照档案来区分的，返回作业编号（大于0的为正常）  
	int doc_idd = StartDoc(hPrintDC, &doc_info);
	if (doc_idd <= 0)
	{
		UE_LOG(LogStreaming, Warning, TEXT("StartDoc 错误，错误代码：%d."), GetLastError());
		return;
		//goto last_code;
	}
	//printf("作业编号：%d\n", doc_idd);

	//开始一个打印页面，大于等于0为正确  
	if (StartPage(hPrintDC) < 0)
	{
		UE_LOG(LogStreaming, Warning, TEXT("StartPage 错误."));
		AbortDoc(hPrintDC); //结束doc打印  
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

	//结束这一页，其实可以循环的startpage、endpage，这样在一个文档中进行多个页面的打印  
	EndPage(hPrintDC);
	EndDoc(hPrintDC);
	//printf("打印顺利完成了. o(∩_∩)o \n");
}

void UWinPrinterActorComponent::PrintItem(FString content)
{
	/*
	char szprinter[80];
	char *szDevice, *szDriver, *szOutput;
	HDC hdcprint; // 定义一个设备环境句柄
	//定义一个打印作业
	static DOCINFO di = { sizeof(DOCINFO),L"printer",NULL };
	// 得到设备字符串存入数组szprinter中
	GetProfileString(L"windows", L"device", L",,,", (LPWSTR)szprinter, 80);
	// 将设备字符串分解
	if (NULL != (szDevice = strtok(szprinter, ",")) && NULL != (szDriver = strtok(NULL, ",")) &&
	NULL != (szOutput = strtok(NULL, ",")))
	{
	if ((hdcprint = CreateDC((LPWSTR)szDriver, (LPWSTR)szDevice, (LPWSTR)szOutput, NULL)) != 0) {
	if (StartDoc(hdcprint, &di)>0) //开始执行一个打印作业
	{

	StartPage(hdcprint); //打印机走纸,开始打印
	SaveDC(hdcprint); //保存打印机设备句柄
	TextOut(hdcprint, 1, 1, L"testPrint", strlen("testPrint"));
	RestoreDC(hdcprint, -1); //恢复打印机设备句柄
	EndPage(hdcprint); //打印机停纸,停止打印
	EndDoc(hdcprint); //结束一个打印作业
	//        MessageBox("打印完毕!","提示",MB_ICONINFORMATION);
	}
	// 用API函数DeleteDC销毁一个打印机设备句柄
	DeleteDC(hdcprint);
	}
	else {
	//MessageBox("没有默认打印机,或者没有安装打印机!");
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