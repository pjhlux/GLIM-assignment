
// AssignmentDlg.h: 헤더 파일
//

#pragma once

#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


// CAssignmentDlg 대화 상자
class CAssignmentDlg : public CDialogEx
{
// 생성입니다.
public:
	CAssignmentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASSIGNMENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
private:
	CImage m_image;
	CImage m_image_init;

	CFont fontLeft;
	CFont fontMid;
	CFont fontButton;

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRangeRadio(UINT uID);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedButtonAction();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnKillfocusEditX1();
	afx_msg void OnEnKillfocusEditY1();
	afx_msg void OnEnKillfocusEditX2();
	afx_msg void OnEnKillfocusEditY2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CEdit m_Edit_x1;
	CEdit m_Edit_y1;
	CEdit m_Edit_x2;
	CEdit m_Edit_y2;
	CStatic m_static_range;						// 입력 범위를 표시하는 dlg static text

	static const int IMAGE_WIDTH = 640;	// Image 크기
	static const int IMAGE_HEIGHT = 480;
	static const int MIN_RADIUS = 10;		// 원의 최소 반지름
	static const int MAX_RADIUS = 80;
	static const int INIT_X = 30;					// Dlg 위에서 Image의 시작 위치
	static const int INIT_Y = 120;
	static const int MOVING_COUNT = 9;				// 원을 몇 번 이동시킬 건지
	static const int TOTAL_MOVEMENT_TIME = 1400;	// 원 이동에 걸리는 시간 (ms)
	static const int DARK_PIXEL_THRESHOLD = 0x7F;	// 원의 어두운 픽셀을 감지하기 위한 임계값
		
	static unsigned int nImageNumber;	// Image를 저장할 때 증가하는 파일명의 꼬리번호
	int nRadioCheck = 1;						// 이미지의 확장자를 결정하는 Radio를 확인하는 수단. 1:JPG   2:BMP
	int nGray = 0x10;							// 원 색깔 지정
	int nRadius = 0;							// 원 반지름,	Draw 없이 Action 할 때 검사하므로 0으로 초기화 필요.


	void CreateImage();
	void SaveImage();
	void OpenImage();
	
	void SetRadiusRandom();
	bool DrawCircle(int i, int j, int nRadius, int nGray);
	void UpdateDisplay();
	void MoveRect();
	void DrawXCenter(const CString& cstrPath);
	POINT DrawXCenter_MakeTextPos(int nCenterX, int nCenterY, int nWidth, int nHeight, int nCount);
	
	BOOL ValidImgPos(int x, int y);
	bool IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	int GetEdit2Int(CEdit& m_edit);
	void ValidateEditValue(CEdit& editCtrl, int nMin, int nMax);
	bool IsEmptyEditControl(CEdit&);
	
};
