// fatiguedetect-dlgDlg.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////Basic include
#pragma warning(disable:4996)
#include "stdafx.h"
#include "fatiguedetect-dlg.h"
#include "fatiguedetect-dlgDlg.h"
#include "resource.h"
#include <windows.h>
#include <windowsx.h>
#include <cstring>
#include <cctype>
#include <list>
#include <commctrl.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;
#define LOAD_OMRON
///////////////////////////////////////////////////////////////////////Omron include
#ifdef LOAD_OMRON
#include "OkaoAPI.h"
#include "CommonDef.h"
#include "OkaoDtAPI.h"
#include "OkaoPtAPI.h"
#include "OkaoAgAPI.h"
#include "OkaoGnAPI.h"
#include "OkaoEoAPI.h"//Eye-open Rate Detection
#include "OkaoSmAPI.h"
#include "OkaoFrAPI.h"
#include "OkaoGbAPI.h"
#endif
///////////////////////////////////////////////////////////////////////Defines

#define MAX2 30  //����ۼ�  30
#define MAX1 30  //����ۼ�  30
#define MAX3 50 //����
#define MAX4 2 //��λ ����
#define MAX5 100  //��λ�ۼ�
#define MAX5_MIN 30	//�����
#define PI 3.1415
#define FRAME_WIDTH		320
#define FRAME_HEIGHT	240
#define TPL_WIDTH 		16
#define TPL_HEIGHT 		12
#define WIN_WIDTH		TPL_WIDTH * 2
#define WIN_HEIGHT		TPL_HEIGHT * 2
#define TM_THRESHOLD	0.4
#define STAGE_INIT		1
#define STAGE_TRACKING	2
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define sign(a)             (((a) >=(0)) ? (1) : (-1)) 
#define pow(a)				((a)*(a))
#define POINT_TL(r)		cvPoint(r.x, r.y)
#define POINT_BR(r)		cvPoint(r.x + r.width, r.y + r.height)
#define POINTS(r)		POINT_TL(r), POINT_BR(r)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CvPoint2D32f CenterGaze;
CvPoint2D32f rCenterGaze;
CvPoint2D32f ScreenLocation[2] = { { 0, 0 }, { 0, 0 } };  //0:���Ͻ�;1:���½�
CvPoint2D32f rScreenLocation[2] = { { 0, 0 }, { 0, 0 } }; //ͬ��


double A = GetSystemMetrics(SM_CXSCREEN);   //��ȡ��Ļ���
double B = GetSystemMetrics(SM_CYSCREEN);	//��ȡ��Ļ�߶�

int WindowPositionFlag = -1;					
int horizontal[MAX5];			//Ϊϵͳ��ʼ����λ����λ�õ�ʱ�򱣴�ˮƽ�н�����
int vertical[MAX5];			//Ϊϵͳ��ʼ����λ����λ�õ�ʱ�򱣴洹ֱ�н�����

int rHorizontal[MAX5];			//Ϊϵͳ��ʼ����λ����λ�õ�ʱ�򱣴�ˮƽ�н�����
int rVertical[MAX5];			//Ϊϵͳ��ʼ����λ����λ�õ�ʱ�򱣴洹ֱ�н�����
//////////////////////////////////////////////////////////
struct Angle
{
	INT32 data[MAX2];
	int insert, erase;
};

struct Coordinates
{
	INT32 data[MAX1];
	int insert, erase;
};

int FrameCount = 0;
bool ReadyToEstimation = false;


//////////////////////////////////////////////////////////////////////////


//�ṩ�����ڡ���������

class CAboutDlg : public CDialog
{
public:
	CAboutDlg():CDialog(CAboutDlg::IDD){};
	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)   // DDX/DDV support
	{
		CDialog::DoDataExchange(pDX);
	};    
protected:
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////

//ϵͳ��ʼ��

fatiguedetectDlg::fatiguedetectDlg(CWnd* pParent): CDialog(fatiguedetectDlg::IDD, pParent)
												//pParent=NULL
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	flag_CamState = 0;
	isCamOpen = FALSE;
	isRightCamOpen = FALSE;
	ServMainThread = NULL;
	ServMainThread2 = NULL;
	iLEyeOpen = iREyeOpen = 250;//ϵͳ��ʼ����
	iLEyeClose = iREyeClose = 230;
	iThreshold_OpenClose = 50;
	nMaxUserNum = 6; 
	nMaxDataNum = 10;
	flag_faceRecognitionState = 0;
	nRegisteredUserNum = 0; 
	iMaxFaceCount = 1;
	imageNo = 0;
}

//��ܵ��ô˺�������д��ȷ�϶Ի�������

void fatiguedetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_GENDER, eBox_gender);
	//DDX_Text(pDX, IDC_AGE, eBox_age);
	DDX_Control(pDX, IDC_ALL_USER, combo_allUser);
}

//������Ϣӳ�䶨��

BEGIN_MESSAGE_MAP(fatiguedetectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_CAM, &fatiguedetectDlg::OnBnClickedOpenCam)
	ON_BN_CLICKED(IDC_PAUSE, &fatiguedetectDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_EXIT, &fatiguedetectDlg::OnBnClickedExit)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SET, &fatiguedetectDlg::OnBnClickedSet)
	ON_BN_CLICKED(IDC_ADDUSER, &fatiguedetectDlg::OnBnClickedAdduser)
	ON_BN_CLICKED(IDC_MANUAL_SET, &fatiguedetectDlg::OnBnClickedManualSet)
	ON_BN_CLICKED(IDC_REGISTER_FEATURE, &fatiguedetectDlg::OnBnClickedRegisterFeature)
	ON_BN_CLICKED(IDC_PRINT, &fatiguedetectDlg::OnBnClickedPrint)
	ON_EN_CHANGE(IDC_GAZELR, &fatiguedetectDlg::OnEnChangeGazelr)
	ON_EN_CHANGE(IDC_GAZEUD, &fatiguedetectDlg::OnEnChangeGazeud)
	ON_BN_CLICKED(IDC_BUTTON1, &fatiguedetectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &fatiguedetectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &fatiguedetectDlg::OnBnClickedButton3)
	ON_STN_CLICKED(IDC_PICTURE, &fatiguedetectDlg::OnStnClickedPicture)
	ON_EN_CHANGE(IDC_EDIT1, &fatiguedetectDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON6, &fatiguedetectDlg::OnBnClickedButton6)
	ON_EN_CHANGE(IDC_LEYE1, &fatiguedetectDlg::OnEnChangeLeye1)
	ON_EN_CHANGE(IDC_REYE1, &fatiguedetectDlg::OnEnChangeReye1)
	ON_EN_CHANGE(IDC_MANUAL_SET, &fatiguedetectDlg::OnEnChangeManualSet)
	ON_EN_CHANGE(IDC_REYE2, &fatiguedetectDlg::OnEnChangeReye2)
	ON_CBN_SELCHANGE(IDC_ALL_USER, &fatiguedetectDlg::OnCbnSelchangeAllUser)
	//ON_STN_CLICKED(IDC_PICTURE2, &fatiguedetectDlg::OnStnClickedPicture2)
END_MESSAGE_MAP()


//�Ի����ʼ������

BOOL fatiguedetectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	

	return TRUE; 
}

/*�ػ����������û�������Ʋ˵��İ�ť���ߵ�������С����ť*/

void fatiguedetectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

/*�����ػ�����*/

void fatiguedetectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1)/2;
		int y = (rect.Height() - cyIcon + 1)/2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

/*�ص���������ϵͳ���ã����ù�*/

HCURSOR fatiguedetectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*Click OpenCam button*/

void fatiguedetectDlg::OnBnClickedOpenCam()
{

	if (flag_CamState > 0)
		return;
	CRect rect;
	CvFont font;

	rightCamera = cvCreateCameraCapture(1);
	capture = cvCreateCameraCapture(0);/*��������ͷ*/

	/*capture = cvCreateFileCapture("video//tongNa.wmv");*/
	if (!capture||!rightCamera)
	{
		AfxMessageBox("Cannot initialize camera!");
		return;
	}
	/*����������Ƶ�ߴ硢Ԥ������*/
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	cvSetCaptureProperty(rightCamera, CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	cvSetCaptureProperty(rightCamera, CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, 8);

	cvGrabFrame(rightCamera);
	cvGrabFrame(capture);
	rightFrame = cvRetrieveFrame(rightCamera);
	frame = cvRetrieveFrame(capture);

	pWnd = GetDlgItem(IDC_PICTURE);
	rPWnd = GetDlgItem(IDC_PICTURE2);

	pWnd->GetClientRect(&videoRect);
	rPWnd->GetClientRect(&videoRect);

	pGlobalDC = pWnd->GetDC();
	hGlobalDC = pGlobalDC->GetSafeHdc();
	rPGlobalDC = rPWnd->GetDC();
	rHGlobalDC = rPGlobalDC->GetSafeHdc();


	combo_allUser.Clear();

	isCamOpen = TRUE;
	isRightCamOpen = TRUE;
	CString str1, str2;
	str1.Format("%d", iLEyeOpen);
	str2.Format("%d", iLEyeClose);

	SetDlgItemText(IDC_LEYE1, str1);
	SetDlgItemText(IDC_LEYE2, str2);
	flag_CamState = 1;
#ifdef LOAD_OMRON
	ServMainThread = AfxBeginThread(ComputeThreadProc, (LPVOID)this, THREAD_PRIORITY_NORMAL);
#endif
}

void fatiguedetectDlg::MatrixPreprocessing()
{
}


/*����ŷķ���⺯���ⶨ����*/

//double XCoordinateCalculation(double gaze,double positiongazeL,double positiongazeR,int distance) 
//{
//	int offset = 0;
//	offset = distance*tan((gaze - (positiongazeL+positiongazeR) / 2) / 180 * PI);
//	return A / 2+offset;
//}

//double YCoordinateCalculation(double gaze, double positiongazeU, int distance)
//{
//	int offset = 0;
//	offset = distance*fabs(tan((gaze -  positiongazeU) / 180 * PI));
//	return 0 + offset;
//}


#ifdef LOAD_OMRON
UINT fatiguedetectDlg::ComputeThreadProc(LPVOID pParam)
{
	double VerticalGaze = 0, HorizontalGaze = 0;  /*ÿ���ۼƵ�ƽ���� ��ֱֵ��ˮƽֵ*/
	double rVerticalGaze = 0, rHorizontalGaze = 0;  /*ÿ���ۼƵ�ƽ���� ��ֱֵ��ˮƽֵ*/
	double minx = 10000, maxx = -10000;
	double miny = 10000, maxy = -10000;

	double rMinx = 10000, rMaxx = -10000;
	double rMiny = 10000, rMaxy = -10000;

	double TotalXOf30fps0 = 0, TotalYOf30fps0 = 0;	/*���30��������ֵ���ܺ�*/
	double rTotalXOf30fps0 = 0, rTotalYOf30fps0 = 0;	/*���30��������ֵ���ܺ�*/

	double HorizontalCoordinate = 0, VerticalCoordinate = 0;	/*���30��������ֵ��ƽ��ֵ*/
	double OldHorizontalCoordinate = 0, OldVerticalCoordinate = 0;	/*��һ��30��������ֵ��ƽ��ֵ*/
//	double HorizontalLocationLeft, HorizontalLocationRight, VerticalLocationLeft = -30, VerticalLocationRight = 30;
//	double rHorizontalLocationLeft, rHorizontalLocationRight, rVerticalLocationLeft = -30, rVerticalLocationRight = 30;

	int MouseControlCode = 0;//������
	int t_ce = 0;
	int stay = 0;	//�Ƿ�ά������״̬��0:��ά��;1:ά��;
	int a_num = 0, b_num = 0, c_num = 0;  //a�����b�һ�,c˫��

	int distance = 0;
	int rdistance = 0;

	//gaze����
	Angle GazeHangleData = { { 0 }, 0, 0 }; //ˮƽ
	Angle GazeVangleData = { { 0 }, 0, 0 }; //��ֱ

	Angle rGazeHangleData = { { 0 }, 0, 0 }; //ˮƽ
	Angle rGazeVangleData = { { 0 }, 0, 0 }; //��ֱ


	GazeHangleData.erase = 0; GazeHangleData.insert = 0;
	GazeVangleData.erase = 0; GazeVangleData.insert = 0;

	rGazeHangleData.erase = 0; rGazeHangleData.insert = 0;
	rGazeVangleData.erase = 0; rGazeVangleData.insert = 0;

	double TotalHAngleOf30fps = 0; //INT32
	double TotalVAngleOf30fps = 0; //INT32

	double rTotalHAngleOf30fps = 0; //INT32
	double rTotalVAngleOf30fps = 0; //INT32

	//���꣨x,y������
	Coordinates GazeXData = { { 0 }, 0, 0 };
	Coordinates GazeYData = { { 0 }, 0, 0 };
	double TotalXOf30fps = 0; //INT32
	double TotalYOf30fps = 0; //INT32
	GazeXData.erase = 0; GazeXData.insert = 0;
	GazeYData.erase = 0; GazeYData.insert = 0;

	IplImage* img = cvLoadImage("d:\\1.jpg");
	cvNamedWindow("exp", 0);  //��������
	cvShowImage("exp", img);    //��������ʾͼ��

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fatiguedetectDlg * pointer;
	pointer = (fatiguedetectDlg *)pParam;
	CRect rect,rRect;
	CDC *pDC,*rPDC;
	HDC hDC,rHDC;

	pointer->pWnd->GetClientRect(&rect);
	pDC = pointer->pWnd->GetDC();
	hDC = pDC->GetSafeHdc();

	pointer->rPWnd->GetClientRect(&rRect);
	rPDC = pointer->rPWnd->GetDC();
	rHDC = rPDC->GetSafeHdc();

	int iLReady, iRReady;
	iLReady = iRReady = 0;

	double X0, Y0;
	double rX0, rY0;

	//IplImage* pCurFrame;
	BYTE *pImgData;//���֡ͼ����������
	BYTE *pRImgData;
	CString strTemp1, strTemp2, strTemp3;

	RAWIMAGE r, g, b;
	int iBytePerLine;
	int width, height;//frame��ȣ��߶�

	int iTemp1, iTemp2, iTemp3, iTemp4, iTemp5;//��ʱ����
	int i, j;//ѭ������

	vector<INT32>vLEye, vREye, vAge;
	vector<INT32>rVLEye, rVREye, rVAge;
	INT32 totalSmile, totalREye, totalLEye, totalAge;
	INT32 rTotalSmile, rTotalREye, rTotalLEye, rTotalAge;
	totalSmile = totalREye = totalLEye = totalAge = 0;
	rTotalSmile = rTotalREye = rTotalLEye = rTotalAge = 0;
	//���������ر�������
	INT32 FunctionReturnCode, rFunctionReturnCode;  /* Return code */
	INT32 FaceCount = 0, rFaceCount = 0;               /* The number of detected face */

	INT32 LeftEyeOpenLevel, RightEyeOpenLevel;
	INT32 rLeftEyeOpenLevel, rRightEyeOpenLevel;

	INT32 SmileDegree, SmileDetectConfidence;//smile degree, 
	INT32 rSmileDegree, rSmileDetectConfidence;//smile degree, 
	FACEINFO info,rInfo;                  /* Detection Result */

	//////////////////////////////////////////////////////////////
	INT32 pnGazeLeftRight, pnGazeUpDown;//gaze
	INT32 rPnGazeLeftRight, rPnGazeUpDown;//gaze

	/////////////////////////////////////////////////////////////

	INT32 anConf[PT_POINT_KIND_MAX];
	POINT aptPoint[PT_POINT_KIND_MAX];
	INT32 rAnConf[PT_POINT_KIND_MAX];
	POINT rAptPoint[PT_POINT_KIND_MAX];



	vector<FACEINFO>vInfo;//���ÿ��������������

	INT32 Age;
	INT32 Confidence;
	INT32 rAge;
	INT32 rConfidence;

	INT32 FetureSimilarityTemp1, FetureSimilarityTemp2;
	//data num of current user

	INT32 Gender;
	INT32 rGender;
	//INT32 Confidence;

	RAWIMAGE *pGray = NULL;         /* Raw image data (gray) */
	RAWIMAGE *rPGray = NULL;
	const INT32 PictureWidth = 320;       /* Image width */
	const INT32 PictureHeight = 240;      /* Image height */

	char pStr[2048];                /* String Buffer for logging output */
//********************************************************************************************************************
//ʶ��������
	//�������
	HDETECTION FaceDetect = NULL;   
	HDETECTION rFaceDetect = NULL;
	HDTRESULT FaceDetectResult = NULL;    
	HDTRESULT rFaceDetectResult = NULL;

	//�������ټ��
	HPOINTER FacePartsDetect = NULL;   
	HPOINTER rFacePartsDetect = NULL;
	HPTRESULT FacePartsDetectResult = NULL;   
	HPTRESULT rFacePartsDetectResult = NULL;

	//����ʶ��		
	HFEATURE FaceFeature = NULL;
	HFEATURE rFaceFeature = NULL;
	HALBUM FaceFeatureAlbum = NULL;
	HALBUM rFaceFeatureAlbum = NULL;

	//�����ſ��̶ȼ��
	HEYEOPEN EyeOpenDetect = NULL;
	HEYEOPEN rEyeOpenDetect = NULL;


	//��������
	HGAZEBLINK EyeGazeBlinkDetect = NULL;
	HGAZEBLINK rEyeGazeBlinkDetect = NULL;
	HGBRESULT EyeGazeBlinkDetectResult = NULL;
	HGBRESULT rEyeGazeBlinkDetectResult = NULL;

	//΢Ц�̶ȼ��
	HSMILE SmileDetect = NULL;
	HSMILE rSmileDetect = NULL;
	HSMRESULT SmileDetectResult = NULL;
	HSMRESULT rSmileDetectResult = NULL;

	//�������
	HAGE AgeDetect = NULL;        
	HAGE rAgeDetect = NULL;
	HAGRESULT AgeDetectResult = NULL;     
	HAGRESULT rAgeDetectResult = NULL;

	//�Ա����
	HGENDER GenderDetect = NULL;        
	HGENDER rGenderDetect = NULL;
	HGNRESULT GenderDetectResult = NULL;    
	HGNRESULT rGenderDetectResult = NULL;
//ʶ�����������

//��ʼ������
	
	for (i = 0; i < 10; i++)   //����10���沿ʶ������������
	{
		FaceDetect = OKAO_CreateDetection();
		if (FaceDetect != NULL)
			break;
	}
	if (i == 10)   //�沿ʶ��������ʧ��
	{
		AfxMessageBox("Creating Face Detection handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)   //����10���沿ʶ������������
	{
		rFaceDetect = OKAO_CreateDetection();
		if (rFaceDetect != NULL)
			break;
	}
	if (i == 10)   //�沿ʶ��������ʧ��
	{
		AfxMessageBox("Creating Face Detection handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)  //����10���沿ʶ������������������
	{
		FaceDetectResult = OKAO_CreateDtResult(pointer->iMaxFaceCount, 0);
		if (FaceDetectResult != NULL)
			break;
	}
	if (i == 10)  //�沿ʶ��������������ʧ��
	{
		AfxMessageBox("Creating Face Detection result handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)  //����10���沿ʶ������������������
	{
		rFaceDetectResult = OKAO_CreateDtResult(pointer->iMaxFaceCount, 0);
		if (rFaceDetectResult != NULL)
			break;
	}
	if (i == 10)  //�沿ʶ��������������ʧ��
	{
		AfxMessageBox("Creating Face Detection result handle failed"); return 0;
	}

	OKAO_SetDtMode(FaceDetect, DT_MODE_DEFAULT);  //�����沿ʶ��ģʽ
	OKAO_SetDtFaceSizeRange(FaceDetect, 60, 220);   //����ʶ��������ʶ�����С�����������С

	OKAO_SetDtMode(rFaceDetect, DT_MODE_DEFAULT);  //�����沿ʶ��ģʽ
	OKAO_SetDtFaceSizeRange(rFaceDetect, 60, 220);   //����ʶ��������ʶ�����С�����������С


	
	for (i = 0; i < 10; i++)    //����10����������ʶ������������
	{
		FacePartsDetect = OKAO_PT_CreateHandle();
		if (FacePartsDetect != NULL)
			break;
	}
	if (i == 10)      //�沿����ʶ��������ʧ��
	{
		AfxMessageBox("Creating Facial Parts Detection handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10����������ʶ������������
	{
		rFacePartsDetect = OKAO_PT_CreateHandle();
		if (rFacePartsDetect != NULL)
			break;
	}

	if (i == 10)      //�沿����ʶ��������ʧ��
	{
		AfxMessageBox("Creating Facial Parts Detection handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10����������ʶ������������������
	{
		FacePartsDetectResult = OKAO_PT_CreateResultHandle();
		if (FacePartsDetectResult != NULL)
			break;
	}
	if (i == 10)    //�沿����ʶ��������������ʧ��
	{
		AfxMessageBox("Creating Facial Parts Detection result handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10����������ʶ������������������
	{
		rFacePartsDetectResult = OKAO_PT_CreateResultHandle();
		if (rFacePartsDetectResult != NULL)
			break;
	}
	if (i == 10)    //�沿����ʶ��������������ʧ��
	{
		AfxMessageBox("Creating Facial Parts Detection result handle failed"); return 0;
	}


	for (i = 0; i < 10; i++)     	//����10����������ʶ������������
	{
		FaceFeature = OKAO_FR_CreateFeatureHandle();
		if (FaceFeature != NULL)
			break;
	}
	if (i == 10)      //��������ʶ��������ʧ��
	{
		AfxMessageBox("Creating Face recognition handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)     	//����10����������ʶ������������
	{
		rFaceFeature = OKAO_FR_CreateFeatureHandle();
		if (rFaceFeature != NULL)
			break;
	}
	if (i == 10)      //��������ʶ��������ʧ��
	{
		AfxMessageBox("Creating Face recognition handle failed"); return 0;
	}


	for (i = 0; i < 10; i++)    //����Ԥ��Ĳ���������������ʶ���ಾ�Ĵ�������,����û�����6,ÿ�����������10
	{
		FaceFeatureAlbum = OKAO_FR_CreateAlbumHandle(pointer->nMaxUserNum, pointer->nMaxDataNum);//maxUserNum=6,maxDataNumPerUser=10
		if (FaceFeatureAlbum != NULL)
			break;
	}
	if (i == 10)    //�����ಾ����ʧ��
	{
		AfxMessageBox("Creating Album handle failed"); return 0;
	}


	for (i = 0; i < 10; i++)    //����Ԥ��Ĳ���������������ʶ���ಾ�Ĵ�������,����û�����6,ÿ�����������10
	{
		rFaceFeatureAlbum = OKAO_FR_CreateAlbumHandle(pointer->nMaxUserNum, pointer->nMaxDataNum);//maxUserNum=6,maxDataNumPerUser=10
		if (rFaceFeatureAlbum != NULL)
			break;
	}
	if (i == 10)    //�����ಾ����ʧ��
	{
		AfxMessageBox("Creating Album handle failed"); return 0;
	}


	
	for (i = 0; i < 10; i++)     //����10������ʶ�����Ĵ�������
	{
		EyeOpenDetect = OKAO_CreateEyeOpen();
		if (EyeOpenDetect != NULL)
			break;
	}
	if (i == 10)    //����ʶ��������ʧ��
	{
		AfxMessageBox("Creating Eye Open-close Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)     //����10������ʶ�����Ĵ�������
	{
		rEyeOpenDetect = OKAO_CreateEyeOpen();
		if (rEyeOpenDetect != NULL)
			break;
	}
	if (i == 10)    //����ʶ��������ʧ��
	{
		AfxMessageBox("Creating Eye Open-close Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10������ʶ�����Ĵ�������
	{
		EyeGazeBlinkDetect = OKAO_GB_CreateHandle();
		if (EyeGazeBlinkDetect != NULL)
			break;
	}
	if (i == 10)      //����ʶ��������ʧ��
	{
		AfxMessageBox("Creating Creates Gaze and Blink Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10������ʶ�����Ĵ�������
	{
		rEyeGazeBlinkDetect = OKAO_GB_CreateHandle();
		if (rEyeGazeBlinkDetect != NULL)
			break;
	}
	if (i == 10)      //����ʶ��������ʧ��
	{
		AfxMessageBox("Creating Creates Gaze and Blink Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)     //����10������ʶ�����������Ĵ�������
	{
		EyeGazeBlinkDetectResult = OKAO_GB_CreateResultHandle();
		if (EyeGazeBlinkDetectResult != NULL)
			break;
	}
	if (i == 10)     //����ʶ��������������ʧ��
	{
		AfxMessageBox("Creates Gaze and Blink Estimation Result handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)     //����10������ʶ�����������Ĵ�������
	{
		rEyeGazeBlinkDetectResult = OKAO_GB_CreateResultHandle();
		if (rEyeGazeBlinkDetectResult != NULL)
			break;
	}
	if (i == 10)     //����ʶ��������������ʧ��
	{
		AfxMessageBox("Creates Gaze and Blink Estimation Result handle failed"); return 0;
	}
	
	for (i = 0; i < 10; i++)    //����10��΢Цʶ�����Ĵ�������
	{
		SmileDetect = OKAO_SM_CreateHandle();
		if (SmileDetect != NULL)
			break;
	}
	if (i == 10)     //΢Цʶ��������ʧ��
	{
		AfxMessageBox("Creating Smile Degree Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10��΢Цʶ�����Ĵ�������
	{
		rSmileDetect = OKAO_SM_CreateHandle();
		if (rSmileDetect != NULL)
			break;
	}
	if (i == 10)     //΢Цʶ��������ʧ��
	{
		AfxMessageBox("Creating Smile Degree Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)     //����10��΢Цʶ�����������Ĵ�������
	{
		SmileDetectResult = OKAO_SM_CreateResultHandle();
		if (SmileDetectResult != NULL)
			break;
	}
	if (i == 10)     //΢Цʶ��������������ʧ��
	{
		AfxMessageBox("Creating Smile Degree Estimation result handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)     //����10��΢Цʶ�����������Ĵ�������
	{
		rSmileDetectResult = OKAO_SM_CreateResultHandle();
		if (rSmileDetectResult != NULL)
			break;
	}
	if (i == 10)     //΢Цʶ��������������ʧ��
	{
		AfxMessageBox("Creating Smile Degree Estimation result handle failed"); return 0;
	}


	for (i = 0; i < 10; i++)     //����10������ʶ�����Ĵ�������
	{
		AgeDetect = OKAO_AG_CreateHandle();
		if (AgeDetect != NULL)
			break;
	}
	if (i == 10)      //����ʶ��������ʧ��
	{
		AfxMessageBox("Creating Age Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)     //����10������ʶ�����Ĵ�������
	{
		rAgeDetect = OKAO_AG_CreateHandle();
		if (rAgeDetect != NULL)
			break;
	}
	if (i == 10)      //����ʶ��������ʧ��
	{
		AfxMessageBox("Creating Age Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10������ʶ�����������Ĵ�������
	{
		AgeDetectResult = OKAO_AG_CreateResultHandle();
		if (AgeDetectResult != NULL)
			break;
	}
	if (i == 10)    //����ʶ��������������ʧ��
	{
		AfxMessageBox("Creating Age Estimation result handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10������ʶ�����������Ĵ�������
	{
		rAgeDetectResult = OKAO_AG_CreateResultHandle();
		if (rAgeDetectResult != NULL)
			break;
	}
	if (i == 10)    //����ʶ��������������ʧ��
	{
		AfxMessageBox("Creating Age Estimation result handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10���Ա�ʶ�����Ĵ�������
	{
		GenderDetect = OKAO_GN_CreateHandle();
		if (GenderDetect != NULL)
			break;
	}
	if (i == 10)    //�Ա�ʶ��������ʧ��
	{
		AfxMessageBox("Creating Gender Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10���Ա�ʶ�����Ĵ�������
	{
		rGenderDetect = OKAO_GN_CreateHandle();
		if (rGenderDetect != NULL)
			break;
	}
	if (i == 10)    //�Ա�ʶ��������ʧ��
	{
		AfxMessageBox("Creating Gender Estimation handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10���Ա�ʶ�����������Ĵ�������
	{
		GenderDetectResult = OKAO_GN_CreateResultHandle();
		if (GenderDetectResult != NULL)
			break;
	}
	if (i == 10)    //�Ա�ʶ��������������ʧ��
	{
		AfxMessageBox("Creating Gender Estimation result handle failed"); return 0;
	}

	for (i = 0; i < 10; i++)    //����10���Ա�ʶ�����������Ĵ�������
	{
		rGenderDetectResult = OKAO_GN_CreateResultHandle();
		if (rGenderDetectResult != NULL)
			break;
	}
	if (i == 10)    //�Ա�ʶ��������������ʧ��
	{
		AfxMessageBox("Creating Gender Estimation result handle failed"); return 0;
	}

//��ʼ�����
//********************************************************************************************************************


	int framenum = 0;

	width = pointer->frame->width;   //���
	height = pointer->frame->height;    //�߶�
	iBytePerLine = pointer->frame->widthStep;
	CPen pen_male, pen_female, pen_noGender, pen_open;

	pen_male.CreatePen(PS_SOLID, 1, RGB(20, 20, 255));//��������
	pen_female.CreatePen(PS_SOLID, 1, RGB(255, 20, 20));//
	pen_noGender.CreatePen(PS_SOLID, 1, RGB(255, 255, 20));//
	pen_open.CreatePen(PS_SOLID, 1, RGB(20, 255, 20));//


	char cGender;

	int state_lEye, state_rEye;//0:����,1:�뿪,2:����
	int rState_lEye, rState_rEye;//0:����,1:�뿪,2:����
	CString sSmile, sAge, sUser;
	CString LeftEyeState, RightEyeState;
	CString rLeftEyeState, rRightEyeState;

	POINT mouthPoint_l, mouthPoint_c, mouthPoint_r, mouthPoint_t;
	POINT startPoint_smile, endPoint_smile;
	POINT rMouthPoint_l, rMouthPoint_c, rMouthPoint_r, rMouthPoint_t;
	POINT rStartPoint_smile, rEndPoint_smile;


	RECT rect_face, rect_lEye, rect_rEye;//�沿�������۾���
	RECT rRect_face, rRect_lEye, rRect_rEye;

	RECT rect_smile;   //Ц�ݾ���
	RECT rRect_smile;

	pGray = (RAWIMAGE *)malloc(PictureWidth * PictureHeight * sizeof(RAWIMAGE));   //ΪͼƬ�����ڴ�
	rPGray = (RAWIMAGE *)malloc(PictureWidth * PictureHeight * sizeof(RAWIMAGE));
	memset(pStr, 0, sizeof(pStr));    //�ڴ��ʼ��
	pDC->SetBkMode(TRANSPARENT);     //ȡ������ַ�����ͼ�εı���ɫ
	pDC->SetTextColor(RGB(255, 0, 0));   //����������ɫ
	while (pointer->isCamOpen)    //������ͷ����ʱ����
	{
	//	pointer->frame = cvQueryFrame(pointer->capture);  //����ͼ��

		cvGrabFrame(pointer->rightCamera);
		cvGrabFrame(pointer->capture);
		pointer->rightFrame = cvRetrieveFrame(pointer->rightCamera);
		pointer->frame = cvRetrieveFrame(pointer->capture);

		if (pointer->frame->origin == 1)		//��ͼ��ԭ��ת��������
		{
			cvFlip(pointer->frame, NULL, 0);
			cvFlip(pointer->rightFrame, NULL, 0);
		}
	
		//��ȡ�Ҷ�ͼ��
		pImgData = (BYTE*)pointer->frame->imageData;
		pRImgData = (BYTE*)pointer->rightFrame->imageData;

		if (pointer->frame->nChannels == 1)//�Ҷ�ͼ��
		{
			for (i = 0; i < PictureHeight; i++)
			{
				for (j = 0; j < PictureWidth; j++)
				{
					pGray[i*PictureWidth + j] = pImgData[i*iBytePerLine + j];
					rPGray[i*PictureWidth + j] = pRImgData[i*iBytePerLine + j];
				}
			}
		}
		else if (pointer->frame->nChannels == 3)//��ɫͼ��
		{
			for (i = 0; i < PictureHeight; i++)
			{
				for (j = 0; j < PictureWidth; j++)
				{
					int ColorTemp = i*iBytePerLine + j * 3;
					r = pImgData[ColorTemp + 2];
					g = pImgData[ColorTemp + 1];
					b = pImgData[ColorTemp];
					pGray[i*PictureWidth + j] = RAWIMAGE(r*0.30 + g*0.59 + b*0.11);
					r = pRImgData[ColorTemp + 2];
					g = pRImgData[ColorTemp + 1];
					b = pRImgData[ColorTemp];
					rPGray[i*PictureWidth + j] = RAWIMAGE(r*0.30 + g*0.59 + b*0.11);
				}
			}
		}
		else
		{
			AfxMessageBox("Image Format error");
			break;
		}

		if (pGray == NULL||rPGray==NULL)
			continue;
		//��ȡ�Ҷ�ͼ��ɹ�
		//�����沿ʶ��
		FunctionReturnCode = OKAO_Detection(FaceDetect, pGray, PictureWidth, PictureHeight, ACCURACY_NORMAL, FaceDetectResult);
		rFunctionReturnCode = OKAO_Detection(rFaceDetect, rPGray, PictureWidth, PictureHeight, ACCURACY_NORMAL, rFaceDetectResult);

		if (FunctionReturnCode != OKAO_NORMAL || rFunctionReturnCode != OKAO_NORMAL)
			continue;   //ʶ��ʧ��,���²���ͼ��ʶ��

		//��ȡ�沿ʶ������
		FunctionReturnCode = OKAO_GetDtFaceCount(FaceDetectResult, &FaceCount);
		rFunctionReturnCode = OKAO_GetDtFaceCount(rFaceDetectResult, &rFaceCount);

		if (FaceCount != rFaceCount)
			continue;

		if (FunctionReturnCode != OKAO_NORMAL || rFunctionReturnCode != OKAO_NORMAL)
			continue;   //ʶ��ʧ��,���²���ͼ��ʶ��

		//�����ǰͼ����ֻʶ����һ����
		if (FaceCount < 1)
		{
			pointer->cimg.CopyOf(pointer->frame);
			pointer->cimg.DrawToHDC(hDC, &rect);

			pointer->cimg.CopyOf(pointer->rightFrame);
			pointer->cimg.DrawToHDC(rHDC, &rect);
			continue;   //���û����������,��������һ��ʶ��
		}
		else
		{   
			FunctionReturnCode = OKAO_GetDtFaceInfo(FaceDetectResult, 0, &info);//��ȡʶ�𵽵��沿������������Ϣ
			rFunctionReturnCode = OKAO_GetDtFaceInfo(rFaceDetectResult, 0, &rInfo);

			if (FunctionReturnCode != OKAO_NORMAL || rFunctionReturnCode != OKAO_NORMAL)
				continue;   //ʶ��ʧ��,���²���ͼ��ʶ��
			if (info.ptLeftTop.x > 0 && info.ptRightBottom.x<320 && info.ptLeftTop.y>0 && info.ptRightBottom.y < 240)
			{              //���ʶ�����Ϸ�,��Ϊ�沿�������ò����Դ����
				rect_face.top = info.ptLeftTop.y;
				rect_face.bottom = info.ptRightBottom.y;
				rect_face.left = info.ptLeftTop.x;
				rect_face.right = info.ptRightBottom.x;
			}
			else
				continue;

			if (rInfo.ptLeftTop.x > 0 && rInfo.ptRightBottom.x<320 && rInfo.ptLeftTop.y>0 && rInfo.ptRightBottom.y < 240)
			{              //���ʶ�����Ϸ�,��Ϊ�沿�������ò����Դ����
				rRect_face.top = rInfo.ptLeftTop.y;
				rRect_face.bottom = rInfo.ptRightBottom.y;
				rRect_face.left = rInfo.ptLeftTop.x;
				rRect_face.right = rInfo.ptRightBottom.x;
			}
			else
				continue;

			//��������沿ϸ��ʶ��
			iTemp1 = 0;
			//ͨ��ʶ�𵽵��沿��������沿λ��
			FunctionReturnCode = OKAO_PT_SetPositionFromHandle(FacePartsDetect, FaceDetectResult, 0);
			rFunctionReturnCode = OKAO_PT_SetPositionFromHandle(rFacePartsDetect, rFaceDetectResult, 0);
			if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)
			{					//ִ���沿ϸ��ʶ��
				FunctionReturnCode = OKAO_PT_DetectPoint(FacePartsDetect, pGray, PictureWidth, PictureHeight, FacePartsDetectResult);
				rFunctionReturnCode = OKAO_PT_DetectPoint(rFacePartsDetect, rPGray, PictureWidth, PictureHeight, rFacePartsDetectResult);

				if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)
				{				//��ȡ�沿ϸ�ڽ��
					FunctionReturnCode = OKAO_PT_GetResult(FacePartsDetectResult, PT_POINT_KIND_MAX, aptPoint, anConf);
					rFunctionReturnCode = OKAO_PT_GetResult(rFacePartsDetectResult, PT_POINT_KIND_MAX, rAptPoint, rAnConf);

					if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)
					{
						iTemp1 = 1;
					}
				}
			}
			if (iTemp1 == 0)//�沿ϸ��ʶ��ʧ��
			{
				pointer->cimg.CopyOf(pointer->frame);
				pointer->cimg.DrawToHDC(hDC, &rect);
				pDC->SelectObject(&pen_noGender);
				pDC->MoveTo(rect_face.left, rect_face.top);  //�ƶ���ĳ��
				pDC->LineTo(rect_face.right, rect_face.top);   //����
				pDC->LineTo(rect_face.right, rect_face.bottom);
				pDC->LineTo(rect_face.left, rect_face.bottom);
				pDC->LineTo(rect_face.left, rect_face.top);     //��ɾ��εĻ���

				pointer->cimg.CopyOf(pointer->rightFrame);
				pointer->cimg.DrawToHDC(rHDC, &rRect);
				rPDC->SelectObject(&pen_noGender);
				rPDC->MoveTo(rRect_face.left, rRect_face.top);  //�ƶ���ĳ��
				rPDC->LineTo(rRect_face.right, rRect_face.top);   //����
				rPDC->LineTo(rRect_face.right, rRect_face.bottom);
				rPDC->LineTo(rRect_face.left, rRect_face.bottom);
				rPDC->LineTo(rRect_face.left, rRect_face.top);     //��ɾ��εĻ���

				continue;   //ִ����һ��ͼƬ�Ĳ���
			}
			//�沿ϸ��ʶ��ɹ�
			mouthPoint_l = aptPoint[PT_POINT_MOUTH_LEFT];    //����������
			mouthPoint_r = aptPoint[PT_POINT_MOUTH_RIGHT];     //����ҽ�����
			mouthPoint_c = aptPoint[PT_POINT_MOUTH];    //����в�����
			mouthPoint_t = aptPoint[PT_POINT_MOUTH_UP];    //����ϲ�����

			rMouthPoint_l = rAptPoint[PT_POINT_MOUTH_LEFT];    //����������
			rMouthPoint_r = rAptPoint[PT_POINT_MOUTH_RIGHT];     //����ҽ�����
			rMouthPoint_c = rAptPoint[PT_POINT_MOUTH];    //����в�����
			rMouthPoint_t = rAptPoint[PT_POINT_MOUTH_UP];    //����ϲ�����


			//���۳̶Ȳ���
			//ͨ��ʶ�𵽵��沿ϸ��Ϊ���۳̶Ȳ�������ʶ��������
			FunctionReturnCode = OKAO_SetEoPoint(EyeOpenDetect, FacePartsDetectResult);
			rFunctionReturnCode = OKAO_SetEoPoint(rEyeOpenDetect, rFacePartsDetectResult);
			if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)
			{		//ͨ������ʶ��������ָ�����ֿ��ϳ̶�
				FunctionReturnCode = OKAO_SetEoParts(EyeOpenDetect, EO_PARTS_LEFTEYE | EO_PARTS_RIGHTEYE);
				rFunctionReturnCode = OKAO_SetEoParts(rEyeOpenDetect, EO_PARTS_LEFTEYE | EO_PARTS_RIGHTEYE);
				if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)
				{			//�ж��۾�����
					FunctionReturnCode = OKAO_EyeOpen(EyeOpenDetect, pGray, PictureWidth, PictureHeight);
					rFunctionReturnCode = OKAO_EyeOpen(rEyeOpenDetect, rPGray, PictureWidth, PictureHeight);
					if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)
					{		
						if ((OKAO_GetEoLevelL(EyeOpenDetect, &LeftEyeOpenLevel) == OKAO_NORMAL) //��ȡ���ۿ��ϳ̶�
							&& (OKAO_GetEoLevelR(EyeOpenDetect, &RightEyeOpenLevel) == OKAO_NORMAL)
							&& (OKAO_GetEoLevelL(rEyeOpenDetect, &rLeftEyeOpenLevel) == OKAO_NORMAL)
							&& (OKAO_GetEoLevelR(rEyeOpenDetect, &rRightEyeOpenLevel) == OKAO_NORMAL))//��ȡ���ۿ��ϳ̶�
						{	//��ֻ�۾�����ȡ�ɹ�
							if (pointer->flag_CamState == 1)//�������ͷ״̬,�����沿ʶ��״̬
							{
								//�����µ����ۿ��ϳ̶ȷ���vector������ǰ��
								vLEye.insert(vLEye.begin(), LeftEyeOpenLevel);
								rVLEye.insert(rVLEye.begin(), rLeftEyeOpenLevel);

								totalLEye += LeftEyeOpenLevel;
								rTotalLEye += rLeftEyeOpenLevel;
								//����ͬ��
								vREye.insert(vREye.begin(), RightEyeOpenLevel);
								rVREye.insert(rVREye.begin(), rRightEyeOpenLevel);

								totalREye += RightEyeOpenLevel;
								rTotalREye += rRightEyeOpenLevel;

								iTemp3 = vLEye.size();
								if (iTemp3 > 10)  //���vector�����л�ȡ��10�����ϵ�����
								{
									//ɾȥ��ĩ�˵�һ������,ͬʱ��total�����м�ȥ��ֵ
									totalLEye = totalLEye - vLEye.back();
									totalREye = totalREye - vREye.back();

									vLEye.pop_back();
									vREye.pop_back();
									iTemp3--;
								}

								//��ȡvector����������Ԫ�ص�ƽ��ֵ
								iTemp1 = totalLEye / iTemp3;
								iTemp2 = totalREye / iTemp3;

								
								if (iTemp2 <= pointer->iREyeClose)//���۱պ�
								{
									state_rEye = 0;		//��־λ��¼�պ�״̬
									RightEyeState = "Close";
								}
								else if (iTemp2 <= pointer->iREyeOpen) //���۰뿪
								{
									state_rEye = 1;
									RightEyeState = "Half-Open";
								}
								else    //��������
								{
									state_rEye = 2;
									RightEyeState = "Open";
								}
								
								//����ͬ��

								if (iTemp1 <= pointer->iLEyeClose)
								{
									state_lEye = 0;
									LeftEyeState = "Close";
								}
								else if (iTemp1 <= pointer->iLEyeOpen)
								{
									state_lEye = 1;
									LeftEyeState = "Half-Open";
								}
								else
								{
									state_lEye = 2;
									LeftEyeState = "Open";
								}

								iTemp3 = rVLEye.size();
								if (iTemp3 > 10)  //���vector�����л�ȡ��10�����ϵ�����
								{
									//ɾȥ��ĩ�˵�һ������,ͬʱ��total�����м�ȥ��ֵ
									rTotalLEye = rTotalLEye - rVLEye.back();
									rTotalREye = rTotalREye - rVREye.back();

									rVLEye.pop_back();
									rVREye.pop_back();
									iTemp3--;
								}


								//��ȡvector����������Ԫ�ص�ƽ��ֵ
								iTemp1 = rTotalLEye / iTemp3;
								iTemp2 = rTotalREye / iTemp3;


								if (iTemp2 <= pointer->iREyeClose)//���۱պ�
								{
									rState_rEye = 0;		//��־λ��¼�պ�״̬
									rRightEyeState = "Close";
								}
								else if (iTemp2 <= pointer->iREyeOpen) //���۰뿪
								{
									rState_rEye = 1;
									rRightEyeState = "Half-Open";
								}
								else    //��������
								{
									rState_rEye = 2;
									rRightEyeState = "Open";
								}

								//����ͬ��

								if (iTemp1 <= pointer->iLEyeClose)
								{
									rState_lEye = 0;
									rLeftEyeState = "Close";
								}
								else if (iTemp1 <= pointer->iLEyeOpen)
								{
									rState_lEye = 1;
									rLeftEyeState = "Half-Open";
								}
								else
								{
									rState_lEye = 2;
									rLeftEyeState = "Open";
								}
							}
							else if (pointer->flag_CamState == 2)//��������״̬
							{
								iLReady = 0;
								vLEye.clear();
								totalLEye = 0;
								rVLEye.clear();
								rTotalLEye = 0;

								iRReady = 0;
								vREye.clear();
								totalREye = 0;
								rVREye.clear();
								rTotalREye = 0;

								pointer->flag_CamState = 3;  //����Ϊ������̬
							}

							//��¼����֡��ֵ��Ȼ��ȡƽ��ֵ����ƽ��ֵ��������������ֵ����Ϊ��С������
							if (pointer->flag_CamState == 3)//��������״̬
							{
								if (iLReady == 0)//���ۻ�δ���ȶ�״̬
								{
									vLEye.push_back(LeftEyeOpenLevel);
									totalLEye += LeftEyeOpenLevel;

									rVLEye.push_back(rLeftEyeOpenLevel);
									rTotalLEye += rLeftEyeOpenLevel;

									if (vLEye.size() >= 60 && rVLEye.size() >= 60)   //����ȡ����60��ͼƬ������ʱ������һ��
									{
											pointer->iLEyeOpen = (totalLEye + rTotalLEye) / (vLEye.size() + rVLEye.size()) - 50;
											pointer->iLEyeClose = pointer->iLEyeOpen - 40;
											iLReady = 1;    //�������У׼
									}
								}

								if (iRReady == 0)//����ͬ��
								{
									vREye.push_back(RightEyeOpenLevel);
									totalREye += RightEyeOpenLevel;

									rVREye.push_back(rRightEyeOpenLevel);
									rTotalREye += rRightEyeOpenLevel;

									if (vREye.size() >= 60 && rVREye.size() >= 60)
									{
											pointer->iREyeOpen = (totalREye+rTotalREye) / (vREye.size()+rVREye.size()) - 50;
											pointer->iREyeClose = pointer->iREyeOpen - 40;
											iRReady = 1;    
									}
								}
								if ((iLReady == 1) && (iRReady == 1))//�����۶������У׼
								{
									pointer->flag_CamState = 1;   //�л����沿ʶ��״̬

									//�������,����
									vLEye.clear();
									totalLEye = 0;
									vREye.clear();
									totalREye = 0;

									rVLEye.clear();
									rTotalLEye = 0;
									rVREye.clear();
									rTotalREye = 0;

									//�����������ø�����ؼ�,�������ò���
									pointer->SetDlgItemInt(IDC_LEYE1, pointer->iLEyeOpen);
									pointer->SetDlgItemInt(IDC_LEYE2, pointer->iLEyeClose);
									pointer->SetDlgItemInt(IDC_REYE1, pointer->iREyeOpen);
									pointer->SetDlgItemInt(IDC_REYE2, pointer->iREyeClose);
									AfxMessageBox("Testing success!");
								}
							}
						}
					}
				}
			}

			//�������þ��α�ʶ����			
			iTemp4 = min(aptPoint[PT_POINT_LEFT_EYE_OUT].y, aptPoint[PT_POINT_LEFT_EYE_IN].y);//�����ھ��߶�
			iTemp5 = max(aptPoint[PT_POINT_LEFT_EYE_OUT].y, aptPoint[PT_POINT_LEFT_EYE_IN].y);//�����⾶�߶�
			iTemp4 -= 10;  //�ݲ����
			iTemp5 += 5;
			//���β�����ʼ��
			rect_lEye.top = iTemp4;
			rect_lEye.bottom = iTemp5;
			rect_lEye.left = aptPoint[PT_POINT_LEFT_EYE_OUT].x - 5;
			rect_lEye.right = aptPoint[PT_POINT_LEFT_EYE_IN].x + 3;

			//r����
			iTemp4 = min(rAptPoint[PT_POINT_LEFT_EYE_OUT].y, rAptPoint[PT_POINT_LEFT_EYE_IN].y);//�����ھ��߶�
			iTemp5 = max(rAptPoint[PT_POINT_LEFT_EYE_OUT].y, rAptPoint[PT_POINT_LEFT_EYE_IN].y);//�����⾶�߶�
			iTemp4 -= 10;  //�ݲ����
			iTemp5 += 5;
			//���β�����ʼ��
			rRect_lEye.top = iTemp4;
			rRect_lEye.bottom = iTemp5;
			rRect_lEye.left = rAptPoint[PT_POINT_LEFT_EYE_OUT].x - 5;
			rRect_lEye.right = rAptPoint[PT_POINT_LEFT_EYE_IN].x + 3;


			//����ͬ��				
			iTemp4 = min(aptPoint[PT_POINT_RIGHT_EYE_OUT].y, aptPoint[PT_POINT_RIGHT_EYE_IN].y);
			iTemp5 = max(aptPoint[PT_POINT_RIGHT_EYE_OUT].y, aptPoint[PT_POINT_RIGHT_EYE_IN].y);
			iTemp4 -= 10;
			iTemp5 += 5;
			rect_rEye.top = iTemp4;
			rect_rEye.bottom = iTemp5;
			rect_rEye.left = aptPoint[PT_POINT_RIGHT_EYE_IN].x - 3;
			rect_rEye.right = aptPoint[PT_POINT_RIGHT_EYE_OUT].x + 5;

			//r����
			iTemp4 = min(rAptPoint[PT_POINT_RIGHT_EYE_OUT].y, rAptPoint[PT_POINT_RIGHT_EYE_IN].y);
			iTemp5 = max(rAptPoint[PT_POINT_RIGHT_EYE_OUT].y, rAptPoint[PT_POINT_RIGHT_EYE_IN].y);
			iTemp4 -= 10;
			iTemp5 += 5;
			rRect_rEye.top = iTemp4;
			rRect_rEye.bottom = iTemp5;
			rRect_rEye.left = rAptPoint[PT_POINT_RIGHT_EYE_IN].x - 3;
			rRect_rEye.right = rAptPoint[PT_POINT_RIGHT_EYE_OUT].x + 5;



			//�沿����ʶ��
			FunctionReturnCode = 0;
			rFunctionReturnCode = 0;
			strTemp1 = "";
			if (pointer->nRegisteredUserNum > 0)// �Ѿ���������û���������0
				//���沿����ʶ������ȡ�沿����
			{
				FunctionReturnCode = OKAO_FR_ExtractFeatureFromPtHdl(FaceFeature, pGray, PictureWidth, PictureHeight, FacePartsDetectResult);
				rFunctionReturnCode = OKAO_FR_ExtractFeatureFromPtHdl(rFaceFeature, rPGray, PictureWidth, PictureHeight, rFacePartsDetectResult);
			}
			
			if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)  //�����ȡ�������
			{
				INT32 similarity = 0;

				FetureSimilarityTemp1 = 0; 
				j = 0; 
				FetureSimilarityTemp2 = 0;
				for (i = 0; i<pointer->nRegisteredUserNum; i++)
				{
					OKAO_FR_Verify(FaceFeature, FaceFeatureAlbum, i, &FetureSimilarityTemp1);//���沿����ʶ�������ಾ�е����ݽ���ƥ��
					
					if (FetureSimilarityTemp1 > FetureSimilarityTemp2)		//�����ǰ�������ƶȴ���֮ǰ���ƶȣ�������滻
					{
						FetureSimilarityTemp2 = FetureSimilarityTemp1; 
						j = i;
						if (FetureSimilarityTemp1 > 900)break;//������ƶȴ���90%,��ƥ��ɹ�
					}
				}

				similarity += FetureSimilarityTemp2;

				FetureSimilarityTemp1 = 0;
				j = 0;
				FetureSimilarityTemp2 = 0;
				for (i = 0; i<pointer->nRegisteredUserNum; i++)
				{
					OKAO_FR_Verify(FaceFeature,rFaceFeatureAlbum, i, &FetureSimilarityTemp1);//���沿����ʶ�������ಾ�е����ݽ���ƥ��

					if (FetureSimilarityTemp1 > FetureSimilarityTemp2)		//�����ǰ�������ƶȴ���֮ǰ���ƶȣ�������滻
					{
						FetureSimilarityTemp2 = FetureSimilarityTemp1;
						j = i;
						if (FetureSimilarityTemp1 > 900)break;//������ƶȴ���90%,��ƥ��ɹ�
					}
				}

				similarity += FetureSimilarityTemp2;
				similarity /= 2;

				if (similarity > 700)//���ƶȴ���70%����Ϊƥ��ɹ�
				{
					strTemp1 = pointer->pStrUserList[j];	//��ȡƥ��ɹ����û�����
				}
			}

			sUser = strTemp1;  //����沿����ʶ��ɹ���Ϊ����ƥ����û����ƣ�����Ϊ��

			//����û���Ϣ
			if (pointer->flag_faceRecognitionState == 2)//������Ϣע��
			{
				if (pointer->nRegisteredUserNum == 0) //������ݿ���û��ע����Ϣ,��ʹ�ò����ͼ���Լ�ʶ����沿���ٽ���ʶ��
				{
					FunctionReturnCode = OKAO_FR_ExtractFeatureFromPtHdl(FaceFeature, pGray, PictureWidth, PictureHeight, FacePartsDetectResult);
					rFunctionReturnCode = OKAO_FR_ExtractFeatureFromPtHdl(rFaceFeature, rPGray, PictureWidth, PictureHeight, rFacePartsDetectResult);
				}
				if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)	//���������ȡ�ɹ�
				{
							//����������ע�ᵽ�沿����ʶ���������
					OKAO_FR_RegisterData(FaceFeatureAlbum, FaceFeature, pointer->nRegisteredUserNum, pointer->nCurDataNum);
					OKAO_FR_RegisterData(rFaceFeatureAlbum, rFaceFeature, pointer->nRegisteredUserNum, pointer->nCurDataNum);

					pointer->nCurDataNum++;   //�ı䵱ǰע��������
					pointer->flag_faceRecognitionState = 1;    //��������ʶ��״̬
					if (pointer->nCurDataNum < pointer->nMaxDataNum)	//���ע����������δ�ﵽ���ֵ
					{
						strTemp1.Format("%d/10", pointer->nCurDataNum + 1);	//���õ�ǰע����������
						pointer->GetDlgItem(IDC_FEATURE_NUM)->SetWindowText(strTemp1);	//��ʾ��ǰע����������
					}
					else
					{
						pointer->OnBnClickedAdduser();	//��������û���ť�ĵ������
					}
				}
			}


			//ʹ���沿����ʶ����Ϊ΢Ц�̶�ʶ��������������
			FunctionReturnCode = OKAO_SM_SetPointFromHandle(SmileDetect, FacePartsDetectResult);
			rFunctionReturnCode = OKAO_SM_SetPointFromHandle(rSmileDetect, rFacePartsDetectResult);
			strTemp1 = "";
			if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)	//����������ɹ�
			{
				FunctionReturnCode = OKAO_SM_Estimate(SmileDetect, pGray, PictureWidth, PictureHeight, SmileDetectResult);	//ʶ��΢Ц�̶ȣ�������΢Цʶ������������
				rFunctionReturnCode = OKAO_SM_Estimate(rSmileDetect, rPGray, PictureWidth, PictureHeight, rSmileDetectResult);	//ʶ��΢Ц�̶ȣ�������΢Цʶ������������

				if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)		//ʶ��΢Ц�̶ȳɹ�
				{
					FunctionReturnCode = OKAO_SM_GetResult(SmileDetectResult, &SmileDegree, &SmileDetectConfidence);//��ȡ΢Ц�̶�ʶ�������Լ�����Ŷ�
					rFunctionReturnCode = OKAO_SM_GetResult(rSmileDetectResult, &rSmileDegree, &rSmileDetectConfidence);//��ȡ΢Ц�̶�ʶ�������Լ�����Ŷ�

					if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)		//��ȡ����ɹ�
					{
						if (SmileDegree <= 20)		//���΢Ц�̶�û�дﵽ��ֵ
							rect_smile.top = -1;
						else
						{
							startPoint_smile.y = (mouthPoint_l.y + mouthPoint_r.y) / 2;		//����΢Ц��ʼ������
							startPoint_smile.x = mouthPoint_l.x;			//����΢Ц��ʼ������
							endPoint_smile.y = startPoint_smile.y;			//����΢Ц����������

							iTemp2 = (mouthPoint_r.x - mouthPoint_l.x) / 4;	//ȡ�첿��ȵ�1/4
							//Ц�ݾ��β�����ʼ��
							rect_smile.top = startPoint_smile.y - iTemp2;
							rect_smile.bottom = startPoint_smile.y + iTemp2;
							rect_smile.left = mouthPoint_l.x;
							rect_smile.right = mouthPoint_r.x;
							//*************************************************************************************************
							if (SmileDegree <= 45)		//���΢Ц�̶�С��45������΢Ц����������
								endPoint_smile.x = mouthPoint_r.x;	
							else
								endPoint_smile.x = mouthPoint_l.x;
						}

						if (rSmileDegree <= 20)		//���΢Ц�̶�û�дﵽ��ֵ
							rRect_smile.top = -1;
						else
						{
							rStartPoint_smile.y = (rMouthPoint_l.y + rMouthPoint_r.y) / 2;		//����΢Ц��ʼ������
							rStartPoint_smile.x = rMouthPoint_l.x;			//����΢Ц��ʼ������
							rEndPoint_smile.y = rStartPoint_smile.y;			//����΢Ц����������

							iTemp2 = (rMouthPoint_r.x - rMouthPoint_l.x) / 4;	//ȡ�첿��ȵ�1/4
							//Ц�ݾ��β�����ʼ��
							rRect_smile.top = rStartPoint_smile.y - iTemp2;
							rRect_smile.bottom = rStartPoint_smile.y + iTemp2;
							rRect_smile.left = rMouthPoint_l.x;
							rRect_smile.right = rMouthPoint_r.x;
							//*************************************************************************************************
							if (rSmileDegree <= 45)		//���΢Ц�̶�С��45������΢Ц����������
								rEndPoint_smile.x = rMouthPoint_r.x;
							else
								rEndPoint_smile.x = rMouthPoint_l.x;
						}
					}
				}
			}

			// ����ʶ����
			strTemp1 = "";
			//ʹ���沿����ʶ����Ϊ����ʶ��������������
			FunctionReturnCode = OKAO_AG_SetPointFromHandle(AgeDetect, FacePartsDetectResult);
			rFunctionReturnCode = OKAO_AG_SetPointFromHandle(rAgeDetect, rFacePartsDetectResult);
			if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)		//����������ɹ�
			{
				FunctionReturnCode = OKAO_AG_Estimate(AgeDetect, pGray, PictureWidth, PictureHeight, AgeDetectResult);	//����ʶ��
				rFunctionReturnCode = OKAO_AG_Estimate(rAgeDetect, rPGray, PictureWidth, PictureHeight, rAgeDetectResult);	//����ʶ��

				if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)	//ʶ������ɹ�
				{
					FunctionReturnCode = OKAO_AG_GetResult(AgeDetectResult, &Age, &Confidence);//��ȡ����ʶ�����Լ����Ŷ�
					rFunctionReturnCode = OKAO_AG_GetResult(rAgeDetectResult, &rAge, &rConfidence);//��ȡ����ʶ�����Լ����Ŷ�

					if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)	//��ȡ����ɹ�
					{
						Age -= 2;	
						Age = (Age + rAge - 4) / 2;
						//�������������ַ���
						if (Age < 12)strTemp1 = "��ͯ";
						else if (Age < 20)strTemp1 = "����";
						else if (Age < 34)strTemp1 = "����";
						else if (Age < 50)strTemp1 = "����";
						else strTemp1 = "����";
					}
				}
			}
			sAge = strTemp1;

			//�Ա�ʶ����
			cGender = 0;
			//ʹ���沿����ʶ����Ϊ�Ա�ʶ��������������
			FunctionReturnCode = OKAO_GN_SetPointFromHandle(GenderDetect, FacePartsDetectResult);
			rFunctionReturnCode = OKAO_GN_SetPointFromHandle(rGenderDetect, rFacePartsDetectResult);
			if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)		//����������ɹ�
			{
				FunctionReturnCode = OKAO_GN_Estimate(GenderDetect, pGray, PictureWidth, PictureHeight, GenderDetectResult);//�Ա�ʶ��
				rFunctionReturnCode = OKAO_GN_Estimate(rGenderDetect, rPGray, PictureWidth, PictureHeight, rGenderDetectResult);//�Ա�ʶ��

				if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)	//ʶ���Ա�ɹ�
				{
					FunctionReturnCode = OKAO_GN_GetResult(GenderDetectResult, &Gender, &Confidence);//��ȡ�Ա�ʶ�����Լ����Ŷ�
					rFunctionReturnCode = OKAO_GN_GetResult(rGenderDetectResult, &rGender, &rConfidence);//��ȡ�Ա�ʶ�����Լ����Ŷ�

					if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)	//��ȡ����ɹ�
					{
						if (Gender == GN_ESTIMATION_MALE&&rGender==GN_ESTIMATION_MALE)
							cGender = 1;
						else if (Gender == rGender)
							cGender = 2;
						else
							continue;
					}
				}
			}
			//�����������ʾװ��
			pointer->cimg.CopyOf(pointer->frame);
			pointer->cimg.DrawToHDC(hDC, &rect);

			pointer->cimg.CopyOf(pointer->rightFrame);
			pointer->cimg.DrawToHDC(rHDC, &rRect);

			//�������λ��ƣ���Ů�ֱ��ò�ͬ��ɫ��
			if (cGender == 1)
			{
				pDC->SelectObject(&pen_male); //�У���ɫ
				rPDC->SelectObject(&pen_male); //�У���ɫ
			}
			else if (cGender == 2)
			{
				pDC->SelectObject(&pen_female); //Ů����ɫ
				rPDC->SelectObject(&pen_female); //Ů����ɫ
			}
			else
			{
				pDC->SelectObject(&pen_noGender);  //���Ա𣬻�ɫ
				rPDC->SelectObject(&pen_noGender);  //���Ա𣬻�ɫ
			}

			//���Ʋ���
			pDC->MoveTo(rect_face.left, rect_face.top);
			pDC->LineTo(rect_face.right, rect_face.top);
			pDC->LineTo(rect_face.right, rect_face.bottom);
			pDC->LineTo(rect_face.left, rect_face.bottom);
			pDC->LineTo(rect_face.left, rect_face.top);

			rPDC->MoveTo(rRect_face.left, rRect_face.top);
			rPDC->LineTo(rRect_face.right, rRect_face.top);
			rPDC->LineTo(rRect_face.right, rRect_face.bottom);
			rPDC->LineTo(rRect_face.left, rRect_face.bottom);
			rPDC->LineTo(rRect_face.left, rRect_face.top);

			//���۾��λ��ƣ���ͬ����״̬ʹ�ò�ͬ��ɫ
			if (state_lEye == 0)
			{
				pDC->SelectObject(&pen_female);	//�պϣ���ɫ
				rPDC->SelectObject(&pen_female);	//�պϣ���ɫ
			}
			else if (state_lEye == 1)
			{
				pDC->SelectObject(&pen_noGender);	//�뿪����ɫ
				rPDC->SelectObject(&pen_noGender);	//�뿪����ɫ
			}
			else
			{
				pDC->SelectObject(&pen_open);		//��������ɫ
				rPDC->SelectObject(&pen_open);		//��������ɫ
			}
			
			//���Ʋ���
			pDC->MoveTo(rect_lEye.left, rect_lEye.top);
			pDC->LineTo(rect_lEye.right, rect_lEye.top);
			pDC->LineTo(rect_lEye.right, rect_lEye.bottom);
			pDC->LineTo(rect_lEye.left, rect_lEye.bottom);
			pDC->LineTo(rect_lEye.left, rect_lEye.top);

			rPDC->MoveTo(rRect_lEye.left, rRect_lEye.top);
			rPDC->LineTo(rRect_lEye.right, rRect_lEye.top);
			rPDC->LineTo(rRect_lEye.right, rRect_lEye.bottom);
			rPDC->LineTo(rRect_lEye.left, rRect_lEye.bottom);
			rPDC->LineTo(rRect_lEye.left, rRect_lEye.top);

			//��������ۿ���״̬��һ��,ʹ����һ����ɫ���ƣ�����ʹ��ͬһ����ɫ
			if (state_rEye != state_lEye)
			{
				if (state_rEye == 0)
				{
					pDC->SelectObject(&pen_female);
					rPDC->SelectObject(&pen_female);
				}
				else if (state_rEye == 1)
				{
					pDC->SelectObject(&pen_noGender);
					rPDC->SelectObject(&pen_noGender);
				}
				else
				{
					pDC->SelectObject(&pen_open);
					rPDC->SelectObject(&pen_open);
				}
			}

			//���Ʋ���
			pDC->MoveTo(rect_rEye.left, rect_rEye.top);
			pDC->LineTo(rect_rEye.right, rect_rEye.top);
			pDC->LineTo(rect_rEye.right, rect_rEye.bottom);
			pDC->LineTo(rect_rEye.left, rect_rEye.bottom);
			pDC->LineTo(rect_rEye.left, rect_rEye.top);

			rPDC->MoveTo(rRect_rEye.left, rRect_rEye.top);
			rPDC->LineTo(rRect_rEye.right, rRect_rEye.top);
			rPDC->LineTo(rRect_rEye.right, rRect_rEye.bottom);
			rPDC->LineTo(rRect_rEye.left, rRect_rEye.bottom);
			rPDC->LineTo(rRect_rEye.left, rRect_rEye.top);

			//�����Ц�ݱ���⵽
			if (rect_smile.top > -1)
			{
				pDC->SelectObject(&pen_female);
				pDC->MoveTo(startPoint_smile);
				pDC->Arc(&rect_smile, startPoint_smile, endPoint_smile);//��Ц�ݾ��ο��ڻ����߱�ʶЦ��
			}

			if (rRect_smile.top > -1)
			{
				rPDC->SelectObject(&pen_female);
				rPDC->MoveTo(rStartPoint_smile);
				rPDC->Arc(&rRect_smile, rStartPoint_smile, rEndPoint_smile);//��Ц�ݾ��ο��ڻ����߱�ʶЦ��
			}

			//�򴰿���ʾ����д���ַ���
			pDC->TextOut(rect_face.left, rect_face.top, sAge);	//����
			pDC->TextOut(rect_face.left, rect_face.bottom - 20, sUser);	//��ǰ�û�����
			pDC->TextOut(rect_face.right, rect_face.bottom, " ");	//��


			rPDC->TextOut(rRect_face.left, rRect_face.top, sAge);	//����
			rPDC->TextOut(rRect_face.left, rRect_face.bottom - 20, sUser);	//��ǰ�û�����
			rPDC->TextOut(rRect_face.right, rRect_face.bottom, " ");	//��
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//***********************************************************************************************************************************
	//Work Start
			
			//����ʶ����
			//ʹ���沿����ʶ����Ϊ����ʶ��������������
			FunctionReturnCode = OKAO_GB_SetPointFromHandle(EyeGazeBlinkDetect, FacePartsDetectResult);
			rFunctionReturnCode = OKAO_GB_SetPointFromHandle(rEyeGazeBlinkDetect, rFacePartsDetectResult);
			strTemp1 = "";
			if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)	//����������ɹ�
			{
				//ִ�����߲ⶨ
				FunctionReturnCode = OKAO_GB_Estimate(EyeGazeBlinkDetect, pGray, PictureWidth, PictureHeight, EyeGazeBlinkDetectResult);
				rFunctionReturnCode = OKAO_GB_Estimate(rEyeGazeBlinkDetect, rPGray, PictureWidth, PictureHeight, rEyeGazeBlinkDetectResult);

				if (FunctionReturnCode == OKAO_NORMAL&&FunctionReturnCode == OKAO_NORMAL)	//�ⶨ�ɹ�
				{
					//��ȡ���߲ⶨ���
					FunctionReturnCode = OKAO_GB_GetGazeDirection(EyeGazeBlinkDetectResult, &pnGazeLeftRight, &pnGazeUpDown);   
					rFunctionReturnCode = OKAO_GB_GetGazeDirection(rEyeGazeBlinkDetectResult, &rPnGazeLeftRight, &rPnGazeUpDown);

					if (FunctionReturnCode == OKAO_NORMAL&&rFunctionReturnCode == OKAO_NORMAL)	//��ȡ����ɹ�
					{
						//���Խ׶�
						/*pointer->framecnt++;*/
						HorizontalGaze = pnGazeLeftRight; //��ȡ����ˮƽ�н�ֵ
						VerticalGaze = pnGazeUpDown; //��ȡ���ߴ��н�ֵ

						rHorizontalGaze = rPnGazeLeftRight; //��ȡ����ˮƽ�н�ֵ
						rVerticalGaze = rPnGazeUpDown; //��ȡ���ߴ��н�ֵ
						//�궨
						char c;
						//if (pointer->framecnt < 500)
						//{
						//	ofstream fout, rfout;
						//	fout.open("d:\\Ans.txt", ios::app);
						//	rfout.open("d:\\rAns.txt", ios::app);
						//	fout << int(HorizontalGaze) << endl;
						//	rfout << int(rHorizontalGaze) << endl;
						//	fout.close();
						//	rfout.close();
						//}
						//��LoopTimeֵ����100��ʱ��Ž�����������
						if (!ReadyToEstimation)
						{
							//�ƶ���λ����
							if (FrameCount%MAX5 == 0)
							{
								if (WindowPositionFlag == -1)
								{
									cvMoveWindow("exp", 0, 0);  //��exp�����ƶ������Ͻ�
									WindowPositionFlag = 0;
								}
								else if (WindowPositionFlag == 0)
								{

									cvMoveWindow("exp", int(A - 117), int(B - 87));  //��exp�����ƶ������½�
									WindowPositionFlag = 1;
								}
							}
							//�ƶ����

						//	//****�����Ż�У׼����
							if (FrameCount == 0)
							{
								c = cvWaitKey(0);	//�ȴ�����һ����������ֹͣ
							}
							if (c == 32)	//�����¿ո��
							{
								if (FrameCount < MAX5)  //��ȡ�ۼ�ֱ��82��ˮƽ�н�ֵ����ֱ�н�ֵ����
								{
									horizontal[FrameCount] = int(HorizontalGaze);
									vertical[FrameCount] = int(VerticalGaze);

									rHorizontal[FrameCount] = int(rHorizontalGaze);
									rVertical[FrameCount] = int(rVerticalGaze);

									FrameCount++;
								}

								if (FrameCount == MAX5)	//���ۼƵõ��㹻��������Ժ�
								{
									//��ռ�����
									FrameCount = 0;
									c = 0;

									//ѡ������
									//��horizontal�����������
									for (int i = 0; i < MAX5 - 1; i++)
									{
										int k = i;
										for (int j = i + 1; j < MAX5; j++)
										{
											if (horizontal[j] < horizontal[k])
												k = j;
										}
										if (k != i)
										{
											int temp = horizontal[k];
											horizontal[k] = horizontal[i];
											horizontal[i] = temp;
										}
									}

									for (int i = 0; i < MAX5 - 1; i++)
									{
										int k = i;
										for (int j = i + 1; j < MAX5; j++)
										{
											if (rHorizontal[j] < rHorizontal[k])
												k = j;
										}
										if (k != i)
										{
											int temp = rHorizontal[k];
											rHorizontal[k] = rHorizontal[i];
											rHorizontal[i] = temp;
										}
									}

									//��vertical�����������
									for (int i = 0; i < MAX5 - 1; i++)
									{
										int	k = i;
										for (int j = i + 1; j < MAX5; j++)
										{
											if (vertical[j] < vertical[k])
												k = j;
										}
										if (k != i)
										{
											int temp = vertical[k];
											vertical[k] = vertical[i];
											vertical[i] = temp;
										}
									}

									for (int i = 0; i < MAX5 - 1; i++)
									{
										int	k = i;
										for (int j = i + 1; j < MAX5; j++)
										{
											if (rVertical[j] < rVertical[k])
												k = j;
										}
										if (k != i)
										{
											int temp = rVertical[k];
											rVertical[k] = rVertical[i];
											rVertical[i] = temp;
										}
									}

									double AverageHorizontal = 0, AverageVertical = 0;
									double rAverageHorizontal = 0, rAverageVertical = 0;

									for (int i = MAX5_MIN; i < MAX5 - MAX5_MIN; i++) //�ӵ�23��Ԫ�ؿ�ʼȡֵ��һ��ȡ40������ƽ��ֵ
									{	
										//ȡֵȥ������ͷ�ģ�Ҳ���ǽϴ�ͽ�С�ģ���������
										AverageHorizontal = AverageHorizontal + horizontal[i];
										AverageVertical = AverageVertical + vertical[i];

										rAverageHorizontal = rAverageHorizontal + rHorizontal[i];
										rAverageVertical = rAverageVertical + rVertical[i];
									}

									AverageHorizontal = AverageHorizontal / (MAX5-2*MAX5_MIN);
									AverageVertical = AverageVertical / (MAX5 - 2 * MAX5_MIN);

									rAverageHorizontal = rAverageHorizontal / (MAX5 - 2 * MAX5_MIN);
									rAverageVertical = rAverageVertical / (MAX5 - 2 * MAX5_MIN);

									//ͨ���Թ۲����¡�����λ�����ߵĲⶨ��ȷ����Ұ��Χ
									//��ǰWindowPositionFlag-1��ֵΪ�������߲ⶨ��������Ӧ�Ĵ���λ��
									if (WindowPositionFlag == 0)
									{
										ScreenLocation[WindowPositionFlag].x = float(AverageHorizontal);
										ScreenLocation[WindowPositionFlag].y = float(AverageVertical);

										rScreenLocation[WindowPositionFlag].x = float(rAverageHorizontal);
										rScreenLocation[WindowPositionFlag].y = float(rAverageVertical);
									}
									if (WindowPositionFlag == 1)
									{
										ScreenLocation[WindowPositionFlag].x = float(AverageHorizontal);
										ScreenLocation[WindowPositionFlag].y = float(AverageVertical);

										rScreenLocation[WindowPositionFlag].x = float(rAverageHorizontal);
										rScreenLocation[WindowPositionFlag].y = float(rAverageVertical);
										cvDestroyWindow("exp");

										X0 = A*tan(ScreenLocation[0].x*PI / 180.0) / (tan(ScreenLocation[0].x*PI / 180.0) - tan(ScreenLocation[1].x*PI / 180.0));
										Y0 = B*tan(ScreenLocation[0].y*PI / 180.0) / (tan(ScreenLocation[0].y*PI / 180.0) - tan(ScreenLocation[1].y*PI / 180.0));

										rX0 = A*tan(rScreenLocation[0].x*PI / 180.0) / (tan(rScreenLocation[0].x*PI / 180.0) - tan(rScreenLocation[1].x*PI / 180.0));
										rY0 = B*tan(rScreenLocation[0].y*PI / 180.0) / (tan(rScreenLocation[0].y*PI / 180.0) - tan(rScreenLocation[1].y*PI / 180.0));


//										distance = A / 2 / tan((fabs(ScreenLocation[0].x) + fabs(ScreenLocation[1].x)) / 360 * PI);
//										rdistance = A / 2 / tan((fabs(rScreenLocation[0].x) + fabs(rScreenLocation[1].x)) / 360 * PI);
										/*distance = distance / 2 + (B / 2 / tan(fabs(fabs(ScreenLocation[1].y) - fabs(ScreenLocation[0].y))) +
											B / 2 / tan(fabs(fabs(rScreenLocation[1].y) - fabs(rScreenLocation[0].y))))/2;*/
										ReadyToEstimation = true;
										::ShowWindow(AfxGetMainWnd()->m_hWnd,SW_SHOWMINIMIZED);
										
									}
								}
							}
						}
						else
						{
							GazeHangleData.data[GazeHangleData.insert] = INT32(HorizontalGaze); //ˮƽ���߼н�ֵ�����鱣�����µ�30�����ݣ�GazeHangleData.insert������һ������Ӧ���λ��
							GazeHangleData.insert = (GazeHangleData.insert + 1) % MAX2;
							TotalHAngleOf30fps += GazeHangleData.data[(GazeHangleData.insert - 1 + MAX2) % MAX2];//�ۻ�����ˮƽ����

							rGazeHangleData.data[rGazeHangleData.insert] = INT32(rHorizontalGaze); //ˮƽ���߼н�ֵ�����鱣�����µ�30�����ݣ�GazeHangleData.insert������һ������Ӧ���λ��
							rGazeHangleData.insert = (rGazeHangleData.insert + 1) % MAX2;
							rTotalHAngleOf30fps += rGazeHangleData.data[(rGazeHangleData.insert - 1 + MAX2) % MAX2];//�ۻ�����ˮƽ����


							GazeVangleData.data[GazeVangleData.insert] = INT32(VerticalGaze); //��ֱ���߼н�ֵ��ͬ��
							GazeVangleData.insert = (GazeVangleData.insert + 1) % MAX2;
							TotalVAngleOf30fps = TotalVAngleOf30fps + GazeVangleData.data[(GazeVangleData.insert - 1 + MAX2) % MAX2];

							rGazeVangleData.data[rGazeVangleData.insert] = INT32(rVerticalGaze); //��ֱ���߼н�ֵ��ͬ��
							rGazeVangleData.insert = (rGazeVangleData.insert + 1) % MAX2;
							rTotalVAngleOf30fps += rGazeVangleData.data[(rGazeVangleData.insert - 1 + MAX2) % MAX2];


							if ((GazeHangleData.insert == GazeHangleData.erase))  //ÿ���۵�30�����ݣ��������²���
							{

								HorizontalGaze = TotalHAngleOf30fps / MAX2;//����ˮƽ����ƽ��ֵ
								VerticalGaze = TotalVAngleOf30fps / MAX2;//���㴹ֱ����ƽ��ֵ

								rHorizontalGaze = rTotalHAngleOf30fps / MAX2;//����ˮƽ����ƽ��ֵ
								rVerticalGaze = rTotalVAngleOf30fps / MAX2;//���㴹ֱ����ƽ��ֵ

								double hor, ver;  //����
								double rHor, rVer;

								//�����߼н�ת������
								//hor = XCoordinateCalculation(HorizontalGaze, ScreenLocation[0].x,ScreenLocation[1].x, distance);
								//ver = YCoordinateCalculation(VerticalGaze, ScreenLocation[0].y, distance);

								hor = X0*(1.0 - (tan(HorizontalGaze*PI / 180.0) / tan(ScreenLocation[0].x*PI / 180.0)));
								ver = Y0*(1.0 - (tan(VerticalGaze*PI / 180.0) / tan(ScreenLocation[0].y*PI / 180.0)));

								//rHor = XCoordinateCalculation(rHorizontalGaze, rScreenLocation[0].x,rScreenLocation[1].x, rdistance);
								//rVer = YCoordinateCalculation(rVerticalGaze, rScreenLocation[0].y, rdistance);

								rHor = rX0*(1.0 - (tan(rHorizontalGaze*PI / 180.0) / tan(rScreenLocation[0].x*PI / 180.0)));
								rVer = rY0*(1.0 - (tan(rVerticalGaze*PI / 180.0) / tan(rScreenLocation[0].y*PI / 180.0)));

								GazeXData.data[GazeXData.insert] = INT32((rHor+hor)/2); //�������µ�30��ˮƽ����ֵ
								GazeXData.insert = (GazeXData.insert + 1) % MAX1;
								TotalXOf30fps = TotalXOf30fps + GazeXData.data[(GazeXData.insert - 1 + MAX1) % MAX1];  //���㵽��ǰ�ε��ܺ�

								GazeYData.data[GazeYData.insert] = INT32((rVer+ver)/2); //�������µ�30�δ�ֱ����ֵ
								GazeYData.insert = (GazeYData.insert + 1) % MAX1;
								TotalYOf30fps = TotalYOf30fps + GazeYData.data[(GazeYData.insert - 1 + MAX1) % MAX1];

								char* buf = new char[255];
								HWND hwndPointNow = NULL;
								hwndPointNow = ::GetForegroundWindow();  //����ǰ̨���ڵľ��
								::GetClassName(hwndPointNow, buf, 255);
								CString str1;
								str1.Format("%s", buf);
								pointer->SetDlgItemText(IDC_GAZELR, str1);
								//���ݵ����ڵ������ÿ����һ���������ݵ��ռ�
								//��Ҫ����30�����߽Ƕ����ݵ��ռ�
								//�����Ƿ��б�Ҫ�Ľ�

								if ((GazeXData.insert == GazeXData.erase))	//���۵�30��������������
								{
									HorizontalCoordinate = TotalXOf30fps / MAX1;  //ˮƽ����
									VerticalCoordinate = TotalYOf30fps / MAX1;	 //��ֱ����
									
									//��������ƶ������
									if (strcmp(buf, "OpusApp") == 0 || strcmp(buf, "classFoxitReader") == 0 || strcmp(buf, "XLMAIN") == 0 || strcmp(buf, "WordPadClass") == 0 || strcmp(buf, "KK_Frame") == 0)
									{
										//������������
										if (OldHorizontalCoordinate > 1200 && OldVerticalCoordinate<100)
										{
											if (SmileDegree>15)//�������
											{
												stay++;
												//������ƶ���ָ��λ��
												SetCursorPos(int(OldHorizontalCoordinate), int(OldVerticalCoordinate));
												if (MouseControlCode == 30)
												{
													//ִ�������������
													mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
												}
												if (MouseControlCode == 60)
												{
													//ִ�����˫������
													mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
													mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
													MouseControlCode = 0;
												}
												MouseControlCode++;
											}
											else//λ�Ʋ���
											{
												MouseControlCode = 0;
												//���������;����������300�������ڣ�ά��ԭλ�ã������ƶ���������
												if ((sqrt(pow(OldHorizontalCoordinate - HorizontalCoordinate) + pow(OldVerticalCoordinate - VerticalCoordinate))) < 300)
													SetCursorPos(int(OldHorizontalCoordinate), int(OldVerticalCoordinate));
												else
													SetCursorPos(int(HorizontalCoordinate), int(VerticalCoordinate));
												OldHorizontalCoordinate = HorizontalCoordinate;  ////����ֵ�����ֵ
												OldVerticalCoordinate = VerticalCoordinate;
											}

										}
										else
										{
											OldHorizontalCoordinate = HorizontalCoordinate;  //����ֵ�����ֵ
											OldVerticalCoordinate = VerticalCoordinate;
										}

										if (strcmp(buf, "KK_Frame") == 0)//���KK_Frame���

											//��ͼ������л�ͼƬ���Ŵ���СͼƬ����ʵ��

										{
											if (OldHorizontalCoordinate > 1000 && OldVerticalCoordinate<100)
											{
											}
											else if (OldHorizontalCoordinate>1150)//ˮƽ�������ұ�
											{
												if (stay == 0)//��ά������״̬
												{
													//�ϳ�һ�λ����¼�
													keybd_event(39, 0, 0, 0);	//ģ�ⰴ�����Ҽ�
													keybd_event(39, 0, KEYEVENTF_KEYUP, 0);	//ģ���ɿ����Ҽ�
													stay = 1;	//ά������״̬
												}

											}
											else if (OldHorizontalCoordinate < 150)//ˮƽ���������
											{
												if (stay == 0)
												{
													keybd_event(37, 0, 0, 0);	//ģ�ⰴ�������
													keybd_event(37, 0, KEYEVENTF_KEYUP, 0);//ģ���ɿ������
													stay = 1;	//ά������״̬
												}
											}
											else if (OldVerticalCoordinate < 20)//��ֱ�������ϱ�
											{
												if (stay == 0)
												{
													keybd_event(38, 0, 0, 0);	//ģ�ⰴ�����ϼ�
													keybd_event(38, 0, KEYEVENTF_KEYUP, 0);//ģ���ɿ����ϼ�
													stay = 1;	//ά������״̬
												}
											}
											else if (OldVerticalCoordinate > B - 20)//��ֱ�������±�
											{
												if (stay == 0)
												{
													keybd_event(40, 0, 0, 0);//ģ�ⰴ�����¼�
													keybd_event(40, 0, KEYEVENTF_KEYUP, 0);//ģ���ɿ����¼�
													stay = 1;	//ά������״̬
												}

											}
											else
												stay = 0;//��ά������״̬
										}


										else
											//���ֶ���ģ��

										{
											if (OldHorizontalCoordinate > 1000 && OldVerticalCoordinate < 100)
											{
											}
											else if ((OldVerticalCoordinate < 100) && (OldHorizontalCoordinate < 300))
											{
												//ģ�⻬����Զ�����ٻ���
												mouse_event(MOUSEEVENTF_WHEEL, 0, 0, 20, 0);
											}
											else if ((OldVerticalCoordinate<100) && (OldHorizontalCoordinate>300) && (OldHorizontalCoordinate<1000))
											{
												//ģ�⻬����Զ�����ٻ���
												mouse_event(MOUSEEVENTF_WHEEL, 0, 0, 15, 0);
											}
											else if ((OldVerticalCoordinate>700) && (OldHorizontalCoordinate<300))
											{
												//ģ�⻬������������ƶ�
												mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -20, 0);
											}
											else if ((OldVerticalCoordinate>700) && (OldHorizontalCoordinate > 300) && (OldHorizontalCoordinate < 1000))
											{
												//ģ�⻬������������ƶ�
												mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -15, 0);
											}

										}
									}


									else if (strcmp(buf, "CWmpControlCntr") == 0 || strcmp(buf, "WMPTransition") == 0)
									{
										//	������������
										if (OldHorizontalCoordinate > 1200 && OldVerticalCoordinate<100)
										{
											if (SmileDegree>15)//΢Цģ�������
											{
												stay++;
												SetCursorPos(int(OldHorizontalCoordinate), int(OldVerticalCoordinate));
												if (MouseControlCode == 30)
												{
													mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
												}
												if (MouseControlCode == 60)
												{
													mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
													mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
													MouseControlCode = 0;
												}
												MouseControlCode++;
											}

											else//λ�Ʋ���
											{
												MouseControlCode = 0;
												if ((sqrt(pow(OldHorizontalCoordinate - HorizontalCoordinate) + pow(OldVerticalCoordinate - VerticalCoordinate))) < 300)
													SetCursorPos(int(OldHorizontalCoordinate), int(OldVerticalCoordinate));
												else
													SetCursorPos(int(HorizontalCoordinate), int(VerticalCoordinate));
												OldHorizontalCoordinate = HorizontalCoordinate;  ////����ֵ�����ֵ
												OldVerticalCoordinate = VerticalCoordinate;
											}

										}
										else
										{
											OldHorizontalCoordinate = HorizontalCoordinate;  //��ֵ
											OldVerticalCoordinate = VerticalCoordinate;
										}
									}

									else   //��������������
									{
										if (SmileDegree > 15)//΢Цģ�������
										{
											stay++;
											SetCursorPos(int(OldHorizontalCoordinate), int(OldVerticalCoordinate));
											if (MouseControlCode == 30)
											{
												mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
											}
											if (MouseControlCode == 60)
											{
												mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
												mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
												MouseControlCode = 0;
											}
											MouseControlCode++;
										}

										else//λ�Ʋ���
										{
											MouseControlCode = 0;
											if ((sqrt(pow(OldHorizontalCoordinate - HorizontalCoordinate) + pow(OldVerticalCoordinate - VerticalCoordinate))) < 300)
												SetCursorPos(int(OldHorizontalCoordinate), int(OldVerticalCoordinate));
											else
												SetCursorPos(int(HorizontalCoordinate), int(VerticalCoordinate));
											OldHorizontalCoordinate = HorizontalCoordinate;  ////����ֵ�����ֵ
											OldVerticalCoordinate = VerticalCoordinate;
										}

									}

									//ɾ����ɵ���������
									TotalXOf30fps = TotalXOf30fps - GazeXData.data[GazeXData.erase];
									GazeXData.erase = (GazeXData.erase + 1) % MAX1;
									TotalYOf30fps = TotalYOf30fps - GazeYData.data[GazeYData.erase];
									GazeYData.erase = (GazeYData.erase + 1) % MAX1;
								}

								//ɾ����ɵļн�����
								TotalHAngleOf30fps = TotalHAngleOf30fps - GazeHangleData.data[GazeHangleData.erase];
								GazeHangleData.erase = (GazeHangleData.erase + 1) % MAX2;
								TotalVAngleOf30fps = TotalVAngleOf30fps - GazeVangleData.data[GazeVangleData.erase];
								GazeVangleData.erase = (GazeVangleData.erase + 1) % MAX2;

								rTotalHAngleOf30fps = rTotalHAngleOf30fps - rGazeHangleData.data[rGazeHangleData.erase];
								rGazeHangleData.erase = (rGazeHangleData.erase + 1) % MAX2;
								rTotalVAngleOf30fps = rTotalVAngleOf30fps - rGazeVangleData.data[rGazeVangleData.erase];
								rGazeVangleData.erase = (rGazeVangleData.erase + 1) % MAX2;
							}
						}
						CString  str2, str3;
						//str1.Format("%s", ReadyToEstimation ? "Ready" : "Preparing");
						//str1.Format("%.2f/%.2f/%d", A, B, distance);
						str2.Format("%.2f", HorizontalCoordinate);
						str3.Format("%.2f", VerticalCoordinate);
						/*pointer->SetDlgItemText(IDC_GAZELR, str1);*/
						pointer->SetDlgItemText(IDC_GAZEUD, str2);
						pointer->SetDlgItemText(IDC_EDIT1, str3);
						//��LoopTimeֵ����100��ʱ��Ž�����������

						//��LoopTimeֵ��TotalXOf30fps0��TotalYOf30fps0ֵ���õ��Ի����ı�
					}
				}

			}

			//Work End
//******************************************************************************************************
//******************************************************************************************************
//******************************************************************************************************
		}

	}

	//����ͷ�رպ��˳�ѭ����ִ��ʶ�������ٲ���

	if (AgeDetect != NULL)
		OKAO_AG_DeleteHandle(AgeDetect);	//��������ʶ����

	if (rAgeDetect != NULL)
		OKAO_AG_DeleteHandle(rAgeDetect);	//��������ʶ����

	if (AgeDetectResult != NULL)
		OKAO_AG_DeleteResultHandle(AgeDetectResult);	//��������ʶ����������

	if (rAgeDetectResult != NULL)
		OKAO_AG_DeleteResultHandle(rAgeDetectResult);	//��������ʶ����������

	if (GenderDetect != NULL)
		OKAO_GN_DeleteHandle(GenderDetect);  	//�����Ա�ʶ����

	if (rGenderDetect != NULL)
		OKAO_GN_DeleteHandle(rGenderDetect);  	//�����Ա�ʶ����

	if (GenderDetectResult != NULL)
		OKAO_GN_DeleteResultHandle(GenderDetectResult);  	//�����Ա�ʶ����ʶ����

	if (rGenderDetectResult != NULL)
		OKAO_GN_DeleteResultHandle(rGenderDetectResult);  	//�����Ա�ʶ����ʶ����

	if (EyeGazeBlinkDetectResult != NULL)
		OKAO_GB_DeleteResultHandle(EyeGazeBlinkDetectResult);  	//��������ʶ����������

	if (rEyeGazeBlinkDetectResult != NULL)
		OKAO_GB_DeleteResultHandle(rEyeGazeBlinkDetectResult);  	//��������ʶ����������

	if (EyeGazeBlinkDetect != NULL)
		OKAO_GB_DeleteHandle(EyeGazeBlinkDetect);  	//��������ʶ����

	if (rEyeGazeBlinkDetect != NULL)
		OKAO_GB_DeleteHandle(rEyeGazeBlinkDetect);  	//��������ʶ����

	if (EyeOpenDetect != NULL)
		OKAO_DeleteEyeOpen(EyeOpenDetect);  	//��������ʶ����

	if (rEyeOpenDetect != NULL)
		OKAO_DeleteEyeOpen(rEyeOpenDetect);  	//��������ʶ����

	if (SmileDetect != NULL)
		OKAO_SM_DeleteHandle(SmileDetect);  	//����΢Цʶ����

	if (rSmileDetect != NULL)
		OKAO_SM_DeleteHandle(rSmileDetect);  	//����΢Цʶ����

	if (SmileDetectResult != NULL)
		OKAO_SM_DeleteResultHandle(SmileDetectResult);  	//����΢Цʶ����������

	if (rSmileDetectResult != NULL)
		OKAO_SM_DeleteResultHandle(rSmileDetectResult);  	//����΢Цʶ����������

	if (FaceFeatureAlbum != NULL)
		OKAO_FR_DeleteAlbumHandle(FaceFeatureAlbum);  	//�����沿����ʶ����������

	if (rFaceFeatureAlbum != NULL)
		OKAO_FR_DeleteAlbumHandle(rFaceFeatureAlbum);  	//�����沿����ʶ����������

	if (FaceFeature != NULL)
		OKAO_FR_DeleteFeatureHandle(FaceFeature);  	//�����沿����ʶ����

	if (rFaceFeature != NULL)
		OKAO_FR_DeleteFeatureHandle(rFaceFeature);  	//�����沿����ʶ����

	if (FacePartsDetect != NULL)
		OKAO_PT_DeleteHandle(FacePartsDetect);  	//�����沿����ʶ����

	if (rFacePartsDetect != NULL)
		OKAO_PT_DeleteHandle(rFacePartsDetect);  	//�����沿����ʶ����

	if (FacePartsDetectResult != NULL)
		OKAO_PT_DeleteResultHandle(FacePartsDetectResult);  	//�����沿���ʶ����������

	if (rFacePartsDetectResult != NULL)
		OKAO_PT_DeleteResultHandle(rFacePartsDetectResult);  	//�����沿���ʶ����������

	if (FaceDetect != NULL)
		OKAO_DeleteDetection(FaceDetect);  	//�����沿ʶ����

	if (rFaceDetect != NULL)
		OKAO_DeleteDetection(rFaceDetect);  	//�����沿ʶ����

	if (FaceDetectResult != NULL)
		OKAO_DeleteDtResult(FaceDetectResult);  	//�����沿ʶ����������

	if (rFaceDetectResult != NULL)
		OKAO_DeleteDtResult(rFaceDetectResult);  	//�����沿ʶ����������

	//�������л��ʶ���
	pen_male.DeleteObject();
	pen_female.DeleteObject();
	pen_noGender.DeleteObject();
	pen_open.DeleteObject();
	//�ͷ�Ϊ���沶���ͼ��������ڴ�
	free(pGray);
	free(rPGray);
	return 0;
}

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ends here
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Click Set button

void fatiguedetectDlg::OnBnClickedSet()
{
	if (flag_CamState == 0)return;
	//isCamOpen = FALSE;
	AfxMessageBox("Please look at the video for about 5 seconds");
	flag_CamState = 2;
}

//Click Pause/Continue button

void fatiguedetectDlg::OnBnClickedPause()
{
	if (flag_CamState == 0)
		return;
	if (isCamOpen)
	{
		isCamOpen = FALSE;
		GetDlgItem(IDC_PAUSE)->SetWindowText(_T("Continue"));
		ServMainThread->SuspendThread();
	}
	else
	{
		isCamOpen = TRUE;
		GetDlgItem(IDC_PAUSE)->SetWindowText(_T("Pause"));
		ServMainThread->ResumeThread();
	}
}

//Click Exit button

void fatiguedetectDlg::OnBnClickedExit()
{
	isCamOpen = FALSE;
//	if (flag_CamState > 0&&ServMainThread != NULL)
//		TerminateThread(ServMainThread->m_hThread, 0);
	::CloseWindow(AfxGetMainWnd()->m_hWnd);//window minimize
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

//Respond to WM_CLOSE message

void fatiguedetectDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	cvDestroyAllWindows();
	if (flag_CamState > 0&&capture)
		cvReleaseCapture(&capture);
	CDialog::OnClose();
}

//Click Adduser button

void fatiguedetectDlg::OnBnClickedAdduser()
{
	if (flag_CamState == 0)
		return;

	if (GetDlgItem(IDC_ADDUSER)->IsWindowEnabled())//�����û�
	{
		if (nRegisteredUserNum == nMaxUserNum)
		{
			AfxMessageBox("The number of registered user have reached the upper limit.");
			return;
		}
		nCurDataNum = 0;   
		GetDlgItem(IDC_ADDUSER)->SetWindowText(_T("OK"));
		GetDlgItem(IDC_REGISTER_FEATURE)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADDUSER)->EnableWindow(FALSE);
	}
	else//�û�����ע�����
	{
		flag_faceRecognitionState = 1;
		CString str1;
		GetDlgItemText(IDC_ALL_USER, str1);
		str1.Trim();//�Ƴ��ַ�������Ŀհ��ַ�������Ԥ�����ַ�
		if (str1.IsEmpty())//δ�����û���
		{
			MessageBox("Please input the user name.");
			flag_faceRecognitionState = 0;
			return;
		}
		else//�ж������û����Ƿ��ظ�
		{
			for (int i = 0; i < nRegisteredUserNum; i++)
			{
				if (pStrUserList[i].Compare(str1) == 0)
				{
					MessageBox("It is a existent user name,please input another one.");
					flag_faceRecognitionState = 0;
					return;
				}
			}
		}
		pStrUserList[nRegisteredUserNum] = str1;
		combo_allUser.AddString(_T(str1));
		nRegisteredUserNum++;
		GetDlgItem(IDC_ADDUSER)->SetWindowText(_T("Add user"));
		GetDlgItem(IDC_ADDUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_REGISTER_FEATURE)->EnableWindow(FALSE);
		GetDlgItem(IDC_FEATURE_NUM)->SetWindowText(_T(" "));
		SetDlgItemText(IDC_ALL_USER, "");
	}
}

//Click RegisterFeature button

void fatiguedetectDlg::OnBnClickedRegisterFeature()
{
	if (nCurDataNum < nMaxDataNum)
		flag_faceRecognitionState = 2;//��������������¼
}

//Click ManualSet button

void fatiguedetectDlg::OnBnClickedManualSet()
{
	CvVideoWriter* videoWriter;
	IplImage *pFrame;
	CvCapture *pCapture;
	CWnd *wnd;
	CvvImage img;
	CRect rect;
	CDC *pDC;                  //*************************************************************************??
	HDC hDC;                   //*************************************************************************??

	//����������Ƶ�ߴ�
	pCapture = cvCaptureFromCAM(0);//����0������ͷ
	cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);  
	wnd = GetDlgItem(IDC_PICTURE);
	wnd->GetClientRect(&rect);
	pDC = wnd->GetDC();
	hDC = pDC->GetSafeHdc();
	videoWriter = cvCreateVideoWriter("test2.avi", CV_FOURCC('X', 'V', 'I', 'D'), 30, cvSize(320, 240), 1);

	int i = 0;
	do
	{
		pFrame = cvQueryFrame(pCapture);   //ͨ������ͷ����ͼ��
		cvWriteFrame(videoWriter, pFrame);    //��ͼ��д����Ƶ�ļ�
		img.CopyOf(pFrame);               
		img.DrawToHDC(hDC, &rect);         
	} while (++i < 900);
	cvReleaseVideoWriter(&videoWriter);
	MessageBox("Video Recording Complete");
	return;
}

//Click Print button

void fatiguedetectDlg::OnBnClickedPrint()
{
	char c[4];
	string str1;
	itoa(imageNo, c, 10);
	str1 = "picture\\";
	str1 += c;
	str1 += ".jpg";
	cvSaveImage(str1.c_str(), frame);
}

//Click Button1            //********************************************************************************??

void fatiguedetectDlg::OnBnClickedButton1()
{
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));//��ʼ��si���ڴ���е�ֵ�����memset������
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi;
	if (!CreateProcess(TEXT("D://OPENCV//ŷķ��//player//Release//player.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		cout << "Create Fail!" << endl;
		exit(1);
	}
}

//Click Button2            //********************************************************************************??

void fatiguedetectDlg::OnBnClickedButton2()
{
	ShellExecute(NULL, "explore", "D:\\OPENCV\\ŷķ��\\PHOTO", NULL, NULL, SW_SHOWNORMAL);
}

//Click Button3            //********************************************************************************??

void fatiguedetectDlg::OnBnClickedButton3()
{
	ShellExecute(NULL, "explore", "D:\\OPENCV\\ŷķ��\\BOOK", NULL, NULL, SW_SHOWNORMAL);
}

//Click Button6            //********************************************************************************??

void fatiguedetectDlg::OnBnClickedButton6()
{
	cvMoveWindow("exp", int(A / 2 - 50), int(B / 2 - 50));  //���Ͻ�
	cvDestroyWindow("exp");
	WindowPositionFlag = 0;
	FrameCount = 0;
#ifdef LOAD_OMRON
	ServMainThread = AfxBeginThread(ComputeThreadProc, (LPVOID)this, THREAD_PRIORITY_NORMAL);
#endif
}


//UNUSED

void fatiguedetectDlg::OnStnClickedPicture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//UNUSED

void fatiguedetectDlg::OnEnChangeGazelr()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//UNUSED

void fatiguedetectDlg::OnEnChangeGazeud()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//UNUSED

void fatiguedetectDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void fatiguedetectDlg::OnEnChangeLeye1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void fatiguedetectDlg::OnEnChangeReye1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void fatiguedetectDlg::OnEnChangeManualSet()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void fatiguedetectDlg::OnEnChangeReye2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void fatiguedetectDlg::OnCbnSelchangeAllUser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void fatiguedetectDlg::OnStnClickedPicture2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
