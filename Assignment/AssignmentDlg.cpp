
//	AssignmentDlg.cpp: 구현 파일
//
//
//	x86 으로 빌드했습니다.
//	
//	2024년 10월
//	㈜그림 SW직군 입사 과제 1
//	박준홍	pjhnauri@naver.com


/*
	Microsoft Visual Studio Community 2022
	버전 17.8.0
	VisualStudio.17.Release / 17.8.0 + 34309.116
	Microsoft.NET Framework
	버전 4.8.09032
	설치된 버전 : Community
	Visual C++ 2022 
	Microsoft Visual C++ 2022
*/


#include "pch.h"
//#include "framework.h"
#include "Assignment.h"
#include "AssignmentDlg.h"
//#include "afxdialogex.h"
#include <Shlwapi.h> // PathRemoveFileSpec 사용을 위해 필요
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// dlg 위의 그림 시작점

unsigned int CAssignmentDlg::nImageNumber;	// 이미지 저장 꼬리번호

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAssignmentDlg 대화 상자



CAssignmentDlg::CAssignmentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSIGNMENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAssignmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_X1, m_Edit_x1);
	DDX_Control(pDX, IDC_EDIT_Y1, m_Edit_y1);
	DDX_Control(pDX, IDC_EDIT_X2, m_Edit_x2);
	DDX_Control(pDX, IDC_EDIT_Y2, m_Edit_y2);
	DDX_Control(pDX, IDC_STATIC_RANGE, m_static_range);
}

BEGIN_MESSAGE_MAP(CAssignmentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DRAW, &CAssignmentDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON_ACTION, &CAssignmentDlg::OnBnClickedButtonAction)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CAssignmentDlg::OnBnClickedButtonOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_EN_KILLFOCUS(IDC_EDIT_X1, &CAssignmentDlg::OnEnKillfocusEditX1)
	ON_EN_KILLFOCUS(IDC_EDIT_Y1, &CAssignmentDlg::OnEnKillfocusEditY1)
	ON_EN_KILLFOCUS(IDC_EDIT_X2, &CAssignmentDlg::OnEnKillfocusEditX2)
	ON_EN_KILLFOCUS(IDC_EDIT_Y2, &CAssignmentDlg::OnEnKillfocusEditY2)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_SAVE_JPG, IDC_RADIO_SAVE_BMP, &CAssignmentDlg::OnRangeRadio)
END_MESSAGE_MAP()


// CAssignmentDlg 메시지 처리기

BOOL CAssignmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	cout << "\tDebug 모드입니다. \n\tRelease로 빌드하면 Console 창이 생기지 않습니다.\n";

	nImageNumber = 1;		// 이미지 저장 꼬리번호 초기화


	((CButton*)GetDlgItem(IDC_RADIO_SAVE_JPG))->SetCheck(TRUE);			// Radio 버튼 : JPG로 저장을 기본값으로 체크

	// 미리 하얗게 칠하기
	m_image_init.Create(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
	HDC hDC = m_image_init.GetDC();

	// 배경을 하얀색으로 채우기
	CDC* pDC = CDC::FromHandle(hDC);
	pDC->FillSolidRect(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, RGB(255, 255, 255));  // 하얀색으로 채우기

	// 이미지의 DC 해제
	m_image_init.ReleaseDC();

	// 좌표 입력 범위 Static_Text 문구 설정
	CString str;
	str.Format(_T("x: %d~%d   y: %d~%d"), 0, IMAGE_WIDTH , 0, IMAGE_HEIGHT );
	m_static_range.SetWindowTextW(str);


	fontLeft.CreatePointFont(80, _T("굴림"));
	GetDlgItem(IDC_STATIC1)->SetFont(&fontLeft);
	GetDlgItem(IDC_STATIC2)->SetFont(&fontLeft);
	GetDlgItem(IDC_STATIC3)->SetFont(&fontLeft);
	GetDlgItem(IDC_STATIC4)->SetFont(&fontLeft);
	GetDlgItem(IDC_STATIC5)->SetFont(&fontLeft);

	fontMid.CreatePointFont(90, _T("굴림"));
	GetDlgItem(IDC_STATIC5)->SetFont(&fontMid);
	GetDlgItem(IDC_STATIC6)->SetFont(&fontMid);

	fontButton.CreatePointFont(110, _T("맑은 고딕"));
	GetDlgItem(IDC_BUTTON_DRAW)->SetFont(&fontButton);
	GetDlgItem(IDC_BUTTON_ACTION)->SetFont(&fontButton);
	GetDlgItem(IDC_BUTTON_OPEN)->SetFont(&fontButton);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAssignmentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CAssignmentDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	if (IsIconic())
	{


		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 이미지 작업 영역을 하얗게 칠하기
		if (m_image_init)
		{
			m_image_init.BitBlt(dc.m_hDC,  INIT_X, INIT_Y);
		}

		// x1,y1  x2,y2   입력 구역을 분리하는 직선 그리기
		CPoint start(5, 50);  // 시작점 (x1, y1)
		CPoint end(210, 50);  // 끝점 (x2, y2)
		
		CPen pen(PS_SOLID, 1, RGB(120, 50, 50));	// 펜 설정 (선의 색상, 스타일, 두께)
		CPen* pOldPen = dc.SelectObject(&pen);
		
		dc.MoveTo(start);	// 직선 그리기
		dc.LineTo(end);

		dc.SelectObject(pOldPen);	// 이전 펜 복원

		CDialogEx::OnPaint();
	}
}

HCURSOR CAssignmentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CAssignmentDlg::PreTranslateMessage(MSG* pMsg)
{
	// Enter / ESC 로 안 꺼지게!
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		GetDlgItem(IDC_STATIC_RANGE)->SetFocus();		// Edit에 값 입력 후, 빈 곳 클릭시 Edit 포커스 해제
		return true;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}







// 버튼 : Draw
// CImage를 생성하고 원을 그린다.
void CAssignmentDlg::OnBnClickedButtonDraw()
{
	if (m_image != NULL) {
		m_image.Destroy();
	}

	// 그레이 스케일의 이미지 생성 및 초기화(흰색)
	CreateImage();
	
	// 무작위 반지름 지정
	SetRadiusRandom();	

	// 원 그리기
	DrawCircle(GetEdit2Int(m_Edit_x1), GetEdit2Int(m_Edit_y1), nRadius, nGray);
}

// 버튼 : Action
// Draw한 원을 목적지로 이동시키면서 bmp로 지정한 경로에 저장한다.
void CAssignmentDlg::OnBnClickedButtonAction()
{
	if (m_image != NULL) {
		m_image.Destroy();
	}

	// 그레이 스케일의 이미지 생성 및 초기화(흰색)
	CreateImage();

	// 무작위 반지름 지정
	if (nRadius == 0) {
		SetRadiusRandom();
	}

	if (!IsEmptyEditControl(m_Edit_x1) && !IsEmptyEditControl(m_Edit_y1) && \
		!IsEmptyEditControl(m_Edit_x2) && !IsEmptyEditControl(m_Edit_x2))	
	{
		MoveRect();
	}
	
}

// 버튼 : Open
// 탐색기를 열어서 bmp를 선택하면 화면에 출력하고 원의 중심위치와 좌표를 표시한다.
void CAssignmentDlg::OnBnClickedButtonOpen()
{
	OpenImage();
}


// 함수 : 그레이 스케일의 이미지 생성 및 초기화(흰색)
void CAssignmentDlg::CreateImage()
{
	int nBpp = 8;

	m_image.Create(IMAGE_WIDTH, -IMAGE_HEIGHT, nBpp);
	if (nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++) {
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_image.SetColorTable(0, 256, rgb);
	}

	unsigned char* fm = (unsigned char*)m_image.GetBits();

	// image 영역 흰색 칠
	memset(fm, 0xff, IMAGE_WIDTH * IMAGE_HEIGHT);

}

// 함수 : 원을 그린다
bool CAssignmentDlg::DrawCircle(int x, int y, int nRadius, int nGray)
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	int nPitch = m_image.GetPitch();
	bool isPerfectCircle = true;

	for (int j = y - nRadius; j <= y + nRadius ; j++) {
		for (int i = x - nRadius; i <= x + nRadius ; i++) 
		{
			if (IsInCircle(i, j, x, y, nRadius))
			{
				if (ValidImgPos(i, j)) {
					fm[j * nPitch + i] = nGray;
				}
				else {
					isPerfectCircle = false;
				}
			}
		}
	}

	UpdateDisplay();

	return isPerfectCircle;
}

// 함수 : 생성된 원을 수회 이동시키며 저장함수를 호출한다.
void CAssignmentDlg::MoveRect()
{
	const int x1 = GetEdit2Int(m_Edit_x1);
	const int y1 = GetEdit2Int(m_Edit_y1);
	const int x2 = GetEdit2Int(m_Edit_x2);
	const int y2 = GetEdit2Int(m_Edit_y2);
	int nX = x1;
	int nY = y1;
	int nDX = x2 - x1;
	int nDY = y2 - y1;
	const int nMoving = MOVING_COUNT; // +1 회, 원을 그림
	

	for (int i = 0; i <= nMoving; i++)
	{
		bool isPerfectCircle = true;

		// 이동 전 영역만 지우기
		if (i != 0) {
			DrawCircle(nX, nY, nRadius, 0xff);
		}

		nX = x1 + (nDX * i) / nMoving; 
		nY = y1 + (nDY * i) / nMoving;

		// 원이 완벽하게 그려진 것만 저장하기
		if (( DrawCircle(nX, nY, nRadius, nGray) && isPerfectCircle) == false) {
			isPerfectCircle = false;
		}

		if (isPerfectCircle) {
			SaveImage();
		}

		if (i != nMoving) {
			Sleep(int(TOTAL_MOVEMENT_TIME / nMoving));
		}
	}
}

// 함수 : 실행파일 경로에 있는 "image"폴더에 bmp를 저장한다.
//			폴더가 없으면 생성한다.
void CAssignmentDlg::SaveImage()
{
	CString fileName, ext;
	if (nRadioCheck == 2) {
		ext = _T("bmp");
	}
	else {
		ext = _T("jpg");
	}
	fileName.Format(_T("image%04d."), nImageNumber++);
	fileName += ext;

	TCHAR exePath[MAX_PATH] = { 0 };						// 실행 파일 경로 저장
	
	GetModuleFileName(NULL, exePath, MAX_PATH);		// 실행 파일 경로 얻음
	
	PathRemoveFileSpec(exePath);								// 실행 파일 경로만 남기기

	CString fullPath = exePath;
	fullPath += _T("\\image\\");

	// 폴더 존재 확인, 없으면 생성
	if (!PathFileExists(fullPath)) 
	{
		if (!CreateDirectory(fullPath, NULL)) 
		{
			AfxMessageBox(_T("폴더 생성 실패!"));
			return;
		}
	}
	fullPath += fileName;

	m_image.Save(fullPath);
}

// 함수 : 화면을 갱신한다.
void CAssignmentDlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, INIT_X, INIT_Y);
}

// 함수 : 좌표가 생성된 CImage 안에 있는지 검사한다.
BOOL CAssignmentDlg::ValidImgPos(int x, int y)
{
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	CRect rect(0, 0, nWidth, nHeight);
	
	return rect.PtInRect(CPoint(x, y));
}

// 함수 : 좌표가 생성된 원에 해당하는지 검사한다.
bool CAssignmentDlg::IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	long dX = i - nCenterX;
	long dY = j - nCenterY;
	long dDist = dX * dX + dY * dY;

	if (dDist <= nRadius * nRadius) {
		bRet = true;
	}

	return bRet;
}

// 함수 : x1 y1 x2 y2 가 입력된 Edit Control 로부터 값을 얻는다.
int CAssignmentDlg::GetEdit2Int(CEdit& m_edit)
{
	CString buf;
	m_edit.GetWindowTextW(buf);
	return _ttoi(buf.GetBuffer());
}

// 함수 : 원의 반지름을 일정한 범위 내의 무작위로 지정한다.
void CAssignmentDlg::SetRadiusRandom()
{
	std::random_device rd;  // 시드로 사용할 난수 생성기
	std::mt19937 gen(rd());  // 메르센 트위스터 엔진
	std::uniform_real_distribution<> dis(MIN_RADIUS, MAX_RADIUS);  // 균등 분포 생성

	nRadius = (int)dis(gen);
}

// 함수 : 파일탐색기로 이미지파일을 선택하고, 화면에 출력한다.
void CAssignmentDlg::OpenImage()
{
	if (m_image != NULL) {
		m_image.Destroy();
	}

	// 실행 파일 경로 저장
	TCHAR exePath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, exePath, MAX_PATH);
	PathRemoveFileSpec(exePath);

	// 파일 탐색기 대화 상자 설정 (BMP, JPG, PNG 필터)
	CString defaultPath = exePath;  // 실행 파일 경로로 설정
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("Image Files (*.bmp; *.jpg;)|*.bmp; *.jpg;|"));

	// 실행파일 경로안에 image 폴더가 있는지 검사하고 없으면 실행파일 경로로 지정
	defaultPath += _T("\\image");
	if (PathFileExistsW(defaultPath) == false) {
		defaultPath = exePath;
	}

	// 경로 설정
	fileDlg.m_ofn.lpstrInitialDir = defaultPath;

	// 탐색기 열기
	if (fileDlg.DoModal() == IDOK)
	{
		CString filePath = fileDlg.GetPathName();  // 선택한 경로 얻기

		HRESULT hResult = m_image.Load(filePath);

		if (SUCCEEDED(hResult)) {
			UpdateDisplay();	// 이미지 출력
			DrawXCenter(filePath);
		}
		else {
			AfxMessageBox(_T("Image 열기 실패!"));
		}
	}

}

// 함수 : OpenImage() 함수로 출력된 화면에서 원의 중심을 찾아서 X를 그리고 좌표를 표시한다.
void CAssignmentDlg::DrawXCenter(const CString &cstrPath)
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();

	CRect rect(0, 0, nWidth, nHeight);
	int nSumX = 0;
	int nSumY = 0;
	unsigned long nCount = 0;

	// 파일 확장자에 따라 처리 방식 결정. bmp or jpg
	CString ext = PathFindExtension(cstrPath);			// 확장자 얻기
	ext.MakeLower();

	
	if (ext == _T(".bmp"))			// BMP 이미지인 경우
	{
		for (int j = rect.top; j < rect.bottom; j++) {
			for (int i = rect.left; i < rect.right; i++)
			{
				if (fm[j * nPitch + i] < DARK_PIXEL_THRESHOLD)
				{
					nSumX += i;
					nSumY += j;
					nCount++;
				}
			}
		}
	}
	else if (ext == _T(".jpg"))		// JPG 이미지인 경우 
	{
		for (int j = 0; j < nHeight; j++) 
		{
			for (int i = 0; i < nWidth; i++) 
			{
				// RGB 3채널에서 밝기(Grayscale)에 해당하는 값 계산
				int blue =	fm[j * nPitch + i * 3];			// Blue
				int green =	fm[j * nPitch + i * 3 + 1];	// Green
				int red =		fm[j * nPitch + i * 3 + 2];	// Red

				int gray = (red + green + blue) / 3;		// 그레이스케일 (밝기)

				if (gray < DARK_PIXEL_THRESHOLD + 20)	// 압축 손실 고려 ( +#)
				{
					nSumX += i;
					nSumY += j;
					nCount++;
				}
			}
		}
	}

	if (nCount == 0) {
		return;
	}
	double dCenterX = (double)nSumX / nCount;
	double dCenterY = (double)nSumY / nCount;
	int nCenterX = (int)round(dCenterX);
	int nCenterY = (int)round(dCenterY);

	CString cstrCoord;
	cstrCoord.Format(_T(" %3d, %3d "), nCenterX, nCenterY);

	HDC hDC = m_image.GetDC();
	if (hDC)
	{
		CDC memDC;
		memDC.Attach(hDC);

		// X 표시 그리기 (X 크기)
		int size = 4;				// X 크기
		int penThick = 2;		// 선과 글자의 두께 (pixel)

		CPen pen(PS_SOLID, penThick, RGB(255, 255, 255));
		CPen* pOldPen = memDC.SelectObject(&pen);		// 기존 펜 저장, 새 펜 선택

		// X 그리기
		memDC.MoveTo(nCenterX - size, nCenterY - size);
		memDC.LineTo(nCenterX + size, nCenterY + size);
		memDC.MoveTo(nCenterX + size, nCenterY - size);
		memDC.LineTo(nCenterX - size, nCenterY + size);

		// 좌표 텍스트 그리기
		memDC.SetTextColor(0x00);
		POINT TextPos = DrawXCenter_MakeTextPos(nCenterX, nCenterY, nWidth, nHeight, nCount);
		memDC.TextOut(TextPos.x, TextPos.y, cstrCoord);		// 좌표 표시
		memDC.Detach();
	}
	m_image.ReleaseDC();

	UpdateDisplay();
}

// 함수 : DrawXCenter() 에서 좌표Text의 출력 위치를 결정
POINT CAssignmentDlg::DrawXCenter_MakeTextPos(int nCenterX, int nCenterY, int nWidth, int nHeight, int nCount)
{
	// 좌표 Static_Text가 Image 영역을 벗어나지 않게 출력 위치를 처리
	const double PI = 3.141593;
	int Radius = int(sqrt(nCount / PI));

	int nTextDx;
	int nTextDy;
	int degree = 0;
	int nTextWidth = 0;

	// [좌표 텍스트 출력 위치]를 [원의 중심]과 [이미지의 중심] 사이에 두기
	if (nCenterX <= nWidth / 2 && nCenterY <= nHeight / 2) {	// 2사분면
		degree = 60;
		nTextWidth = 5;
		nTextDx = int((Radius + nTextWidth) * cos(degree * PI / 180));		// 원의 중심으로부터 좌표 텍스트가 얼마나 떨어져야 하는지; X
		nTextDy = int((Radius + nTextWidth) * sin(degree * PI / 180));		// 원의 중심으로부터 좌표 텍스트가 얼마나 떨어져야 하는지; Y
	}
	else if (nCenterX >= nWidth / 2 && nCenterY <= nHeight / 2) {	// 1사분면
		degree = 30;
		nTextWidth = 50;
		nTextDx = int(-(Radius + nTextWidth) * cos(degree * PI / 180));
		nTextDy = int((Radius + nTextWidth) * sin(degree * PI / 180));
	}
	else if (nCenterX <= nWidth / 2 && nCenterY >= nHeight / 2) {	// 3사분면
		degree = 60;
		nTextWidth = 15;
		nTextDx = int((Radius + nTextWidth) * cos(degree * PI / 180));
		nTextDy = int(-(Radius + nTextWidth) * sin(degree * PI / 180));
	}
	else if (nCenterX >= nWidth / 2 && nCenterY >= nHeight / 2) {	// 4사분면
		degree = 30;
		nTextWidth = 60;
		nTextDx = int(-(Radius + nTextWidth) * cos(degree * PI / 180));
		nTextDy = int(-(Radius + nTextWidth) * sin(degree * PI / 180));
	}
	else {
		nTextDx = (nCenterX <= nWidth / 2) ?	30 : -80;
		nTextDy = (nCenterY <= nHeight / 2) ? 30 : -30;
	}

	POINT P = { nCenterX + nTextDx, nCenterY + nTextDy };

	return P;
}



// 이벤트 함수 : Image 영역에서 마우스 좌클릭을 하면 x1 y1 Edit에 커서의 상대위치가 입력된다.
void CAssignmentDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_STATIC_RANGE)->SetFocus();	// Edit에 입력하고 빈 곳 클릭 시, Edit에서 포커스를 해제하기 위해서


	POINT P;
	GetCursorPos(&P);		// 전체 창에서 커서 위치
	ScreenToClient(&P);	// 클라이언트 커서 위치로 변환

	CString str;
	int nPointX = P.x - INIT_X;	// 이미지 영역 위에서의 커서의 상대좌표
	int nPointY = P.y - INIT_Y;
	
	if (nPointX >= 0 && nPointX <= IMAGE_WIDTH && \
		nPointY >= 0 && nPointY <= IMAGE_HEIGHT)
	{
		str.Format(_T("%d"), nPointX);
		m_Edit_x1.SetWindowTextW(str);

		str.Format(_T("%d"), nPointY);
		m_Edit_y1.SetWindowTextW(str);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

// 이벤트 함수 : Image 영역에서 마우스 우클릭을 하면 x2 y2 Edit에 커서의 상대위치가 입력된다.
void CAssignmentDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	POINT P;
	GetCursorPos(&P);		// 전체 창에서 커서 위치
	ScreenToClient(&P);	// 클라이언트 커서 위치로 변환

	CString str;
	int nPointX = P.x - INIT_X;	// 이미지 영역 위에서의 커서의 상대좌표
	int nPointY = P.y - INIT_Y;

	if (nPointX >= 0 && nPointX <= IMAGE_WIDTH && \
		nPointY >= 0 && nPointY <= IMAGE_HEIGHT)
	{
		str.Format(_T("%d"), nPointX);
		m_Edit_x2.SetWindowTextW(str);

		str.Format(_T("%d"), nPointY);
		m_Edit_y2.SetWindowTextW(str);
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}

// 이벤트 함수 : Edit에서 포커스가 떠날때, 유효한 값인지 확인
void CAssignmentDlg::OnEnKillfocusEditX1()
{
	ValidateEditValue(m_Edit_x1, 0, IMAGE_WIDTH);
}
void CAssignmentDlg::OnEnKillfocusEditY1()
{
	ValidateEditValue(m_Edit_y1, 0, IMAGE_HEIGHT);
}
void CAssignmentDlg::OnEnKillfocusEditX2()
{
	ValidateEditValue(m_Edit_x2, 0, IMAGE_WIDTH);
}
void CAssignmentDlg::OnEnKillfocusEditY2()
{
	ValidateEditValue(m_Edit_y2, 0, IMAGE_HEIGHT);
}

// 함수 : 입력된 Edit의 값이 지정된 영역 안에 있는지 검사한다.
void CAssignmentDlg::ValidateEditValue(CEdit& editCtrl, int nMin, int nMax)
{
	// 입력중인지 체크
	static bool bIsUpdating = false;
	if (bIsUpdating) 
		return;  // 입력 중이면 리턴

	CString strValue;
	editCtrl.GetWindowText(strValue);

	int nValue = _ttoi(strValue);

	if (nValue < nMin || nValue > nMax)
	{
		bIsUpdating = true;
		CString defaultStr;
		if (nValue < nMin) {
			defaultStr.Format(_T("%d"), nMin);		// 범위를 벗어나면 기본값으로 초기화 (nMin으로 초기화)
		}
		else 	if (nValue > nMax) {
			defaultStr.Format(_T("%d"), nMax);	// 범위를 벗어나면 기본값으로 초기화 (nMax으로 초기화)
		}
		editCtrl.SetWindowText(defaultStr);
		bIsUpdating = false;
	}
}

// 함수 : CEdit 입력칸이 비어있는지 확인.	true : empty
bool CAssignmentDlg::IsEmptyEditControl(CEdit& edit)
{
	CString str;
	edit.GetWindowTextW(str);
	if (str.IsEmpty())
		return true;
	else
		return false;
}

// 저장 확장자를 결정하는 Radio 그룹 관리
void CAssignmentDlg::OnRangeRadio(UINT uID)
{
	switch (uID)
	{
		case IDC_RADIO_SAVE_JPG:
			nRadioCheck = 1;
			break;
		case IDC_RADIO_SAVE_BMP:
			nRadioCheck = 2;
			break;
	}
}