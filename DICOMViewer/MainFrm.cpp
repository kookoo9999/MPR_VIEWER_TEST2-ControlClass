// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "DICOMViewer.h"

#include "MainFrm.h"
#include "DVManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_USER_MOUSEPOS WM_USER + 101

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND( ID_VIEW_DICOM_GROUP, &CMainFrame::OnViewDicomGroup )
	ON_UPDATE_COMMAND_UI( ID_VIEW_DICOM_GROUP, &CMainFrame::OnUpdateViewDicomGroup )
	ON_COMMAND( ID_OPEN_DICOM_FOLDER, &CMainFrame::OnOpenDicomFolder )
	ON_COMMAND( ID_COMBO_VOLUME_RENDER_MODE, &CMainFrame::OnComboVolumeRenderMode )
//	ON_COMMAND(ID_VIEW_SCA, &CMainFrame::OnViewSca)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_SCA, &CMainFrame::OnUpdateViewSca)
	ON_COMMAND(ID_VIEW_3D_ON_OFF, &CMainFrame::OnView3dOnOff)
	ON_UPDATE_COMMAND_UI(ID_VIEW_3D_ON_OFF, &CMainFrame::OnUpdateView3dOnOff)
//	ON_COMMAND(ID_VIEW_SCA, &CMainFrame::OnViewSca)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_SCA, &CMainFrame::OnUpdateViewSca)
//ON_COMMAND(ID_SLIDER_RX, &CMainFrame::OnSliderRx)
//ON_COMMAND(ID_SLIDER_RY, &CMainFrame::OnSliderRy)
//ON_COMMAND(ID_SLIDER_RZ, &CMainFrame::OnSliderRz)
ON_COMMAND(ID_VIEW_OUTLINE, &CMainFrame::OnViewOutline)
ON_UPDATE_COMMAND_UI(ID_VIEW_OUTLINE, &CMainFrame::OnUpdateViewOutline)
ON_COMMAND(ID_CHECK_BONE, &CMainFrame::OnCheckBone)
ON_UPDATE_COMMAND_UI(ID_CHECK_BONE, &CMainFrame::OnUpdateCheckBone)
ON_COMMAND(ID_SLIDER_BONE, &CMainFrame::OnSliderBone)
ON_COMMAND(ID_CHECK_SKIN, &CMainFrame::OnCheckSkin)
ON_UPDATE_COMMAND_UI(ID_CHECK_SKIN, &CMainFrame::OnUpdateCheckSkin)
ON_COMMAND(ID_SLIDER_SKIN, &CMainFrame::OnSliderSkin)
ON_UPDATE_COMMAND_UI(ID_SLIDER_BONE, &CMainFrame::OnUpdateSliderBone)
ON_UPDATE_COMMAND_UI(ID_SLIDER_SKIN, &CMainFrame::OnUpdateSliderSkin)
ON_COMMAND(ID_SLIDERRX, &CMainFrame::OnSliderrx)
ON_COMMAND(ID_SLIDERRY, &CMainFrame::OnSliderry)
ON_COMMAND(ID_SLIDERRZ, &CMainFrame::OnSliderrz)
//ON_COMMAND(ID_CHECK_Thread, &CMainFrame::OnCheckThread)

ON_MESSAGE(WM_USER_MOUSEPOS, &CMainFrame::OnUserMousepos)
ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
	
}

void CMainFrame::TestFunc()
{
	AfxMessageBox(_T("Hello, this is test"));
}

bool CMainFrame::setThreadWorking()
{
	m_bThreadWorking = stop;
	return true;
}

UINT CMainFrame::Thread_MouseTracking(LPVOID _mothod)
{
	CMainFrame* pDlg = (CMainFrame*)_mothod;	
	
	//HWND view = ::GetDlgItem(AfxGetMainWnd()->m_hWnd, IDD_VTK_VIEW);
	HWND hWnd = AfxGetMainWnd()->m_hWnd;

	//CMFCRibbonEdit* pEdit = new CMFCRibbonEdit(ID_EDIT_mPos, 72, _T("X: Y: Z:"), 13);
	/*CMFCRibbonSlider* edit = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		m_wndRibbonBar.FindByID(ID_EDIT_mPos));*/
	//ST_Thread st = *(ST_Thread*)_mothod;

//	CMFCRibbonEdit* edit = st.m_rEd;		
	//CString str;
	CPoint m_pos;
	//pDlg->m_bThreadWorking = !pDlg->m_bThreadWorking;


	while (pDlg->m_bThreadWorking)
	{
		Sleep(10);		
		::GetCursorPos(&m_pos);		
		::ScreenToClient(hWnd, &m_pos);			
		pDlg->PostMessage(WM_USER_MOUSEPOS, (WPARAM)m_pos.x, (WPARAM)m_pos.y);

	}
	
	

	return 0;
}

//LRESULT CMainFrame::OnSetMousePos(WPARAM wParam, LPARAM lParam)
//{
//	CString strMousePos;
//	CMFCRibbonEdit* edit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_mPos));
//
//	strMousePos.Format(_T("X : %d, Y : %d"), (int)wParam, (int)lParam);	
//	edit->SetEditText(strMousePos);
//
//	return 0;
//}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// 보관된 값에 따라 비주얼 관리자 및 스타일을 설정합니다.
	OnApplicationLook(theApp.m_nAppLook);

	// DICOM Group View 생성
	if( !m_DicomGroupView.Create( 
		_T( "Patient List" ),		// 도킹 창 이름
		this,							// 부모 윈도우 = 메인 프레임
		CRect( 0, 0, 200, 200 ),		// 초기 윈도우 크기
		TRUE,							// 제목 창 표시 여부
		ID_VIEW_DICOM_GROUP,	// 도킹 창 ID
		WS_CHILD | WS_VISIBLE |	// 도킹 창 스타일
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
		CBRS_LEFT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0("도킹 창을 만들지 못했습니다.\n");
		return -1;
	}
	// DICOM Group 도킹창이 부모창의 모든 부분에 도킹이 가능하도록 설정
	m_DicomGroupView.EnableDocking( CBRS_ALIGN_ANY );
	// DICOM Group 창 도킹
	DockPane( &m_DicomGroupView );

	m_pThread = NULL;	
	m_pThread = ::AfxBeginThread(Thread_MouseTracking, this);

	if (m_pThread == NULL)
	{
		AfxMessageBox(_T("Error"));
		return FALSE;
	}
	

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}



void CMainFrame::OnViewDicomGroup()
{
	// DICOM Group 도킹창을 표시/숨김 (현재상태와 반대로)
	m_DicomGroupView.ShowPane( !m_DicomGroupView.IsPaneVisible(), FALSE, FALSE );
}


void CMainFrame::OnUpdateViewDicomGroup( CCmdUI *pCmdUI )
{
	// DICOM Group 도킹창이 보이는 상태이면 체크박스 체크
	pCmdUI->SetCheck( m_DicomGroupView.IsPaneVisible() );
}


void CMainFrame::OnOpenDicomFolder()
{
	CFolderPickerDialog folderDlg( _T( "" ), 0, NULL, 0 );


	if( IDOK == folderDlg.DoModal() ) {
		// DICOM 파일이 포함된 폴더 경로
		CString path = folderDlg.GetPathName();

		// 폴더내의 DICOM(*.dcm)파일 읽기
		DVManager::Mgr()->GetDicomLoader()
			->OpenDicomDirectory( CT2CA( LPCWSTR( path ) ) );

		// DICOM 그룹 트리 업데이트
		m_DicomGroupView.UpdateDicomTree();
	}
}


void CMainFrame::OnComboVolumeRenderMode()
{
	// 리본 메뉴의 콤보 박스 컨트롤 가져오기
	CMFCRibbonComboBox *volumeModeComboBox = 
		DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_VOLUME_RENDER_MODE));
	
	// 콤보박스에서 선택된 인덱스
	int selectedIdx = volumeModeComboBox->GetCurSel();

	// Volume 렌더링 모드 변경
	DVManager::Mgr()->ChangeVolumeRenderMode( selectedIdx );
}


//void CMainFrame::OnViewSca()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	//DVManager::Mgr()->UpdatePlnae();
//	DVManager::Mgr()->bCheck = !DVManager::Mgr()->bCheck;
//}


//void CMainFrame::OnUpdateViewSca(CCmdUI *pCmdUI)
//{
//	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
//	 pCmdUI->SetCheck(DVManager::Mgr()->bCheck);
//}


void CMainFrame::OnView3dOnOff()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//DVManager::Mgr()->bCheck = !DVManager::Mgr()->bCheck;

	//Show Plane via Control Class 
	
	DVManager::Mgr()->m_bShowPlane = !DVManager::Mgr()->m_bShowPlane;
	DVManager::Mgr()->m_pControlManager->GetSCAViewOnOff();
	DVManager::Mgr()->ShowPlnaeTest();
	

	//DVManager::Mgr()->m_bShowPlane = !DVManager::Mgr()->m_bShowPlane;
	//DVManager::Mgr()->ShowPlnateTest2();

	//DVManager::Mgr()->ShowPlnae();

	//DVManager::Mgr()->m_bShowPlane = !DVManager::Mgr()->m_bShowPlane;
	
}

void CMainFrame::OnUpdateView3dOnOff(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//pCmdUI->SetCheck(DVManager::Mgr()->bCheck);
	pCmdUI->SetCheck(DVManager::Mgr()->m_bShowPlane);
}

void CMainFrame::OnViewOutline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	//DVManager::Mgr()->m_pControlManager->SetLineOnOff(DVManager::Mgr()->bCheck);

	DVManager::Mgr()->m_testCheck = !DVManager::Mgr()->m_testCheck;
	DVManager::Mgr()->ShowLineTest();

	//DVManager::Mgr()->ShowOutline();
	//DVManager::Mgr()->bCheck = !DVManager::Mgr()->bCheck;
}


void CMainFrame::OnUpdateViewOutline(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//pCmdUI->SetCheck(DVManager::Mgr()->bCheck);
	
	pCmdUI->SetCheck(DVManager::Mgr()->m_testCheck);
	//pCmdUI->SetCheck(DVManager::Mgr()->m_pControlManager->GetLineOnOff());
}


void CMainFrame::OnCheckBone()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDER_BONE));
	pSlider->SetPos(DVManager::Mgr()->m_pControlManager->GetBoneThreshold());
	pSlider->Redraw();
	
	DVManager::Mgr()->m_bShowBone = !DVManager::Mgr()->m_bShowBone;
	DVManager::Mgr()->m_pControlManager->SetBoneOnOff(!DVManager::Mgr()->m_pControlManager->GetBoneOnOff());
	//DVManager::Mgr()->ShowBone();
	DVManager::Mgr()->ShowBoneTest();
	
}


void CMainFrame::OnUpdateCheckBone(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(DVManager::Mgr()->m_bShowBone);
}

void CMainFrame::OnCheckSkin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonSlider* spSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDER_SKIN));
	spSlider->SetPos(DVManager::Mgr()->m_fThresholdSkin);
	spSlider->Redraw();

	DVManager::Mgr()->m_bShowSkin = !DVManager::Mgr()->m_bShowSkin;
	DVManager::Mgr()->ShowSkin();

	
}


void CMainFrame::OnUpdateCheckSkin(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(DVManager::Mgr()->m_bShowSkin);
}


//void CMainFrame::OnViewSca()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	DVManager::Mgr()->bCheck = !DVManager::Mgr()->bCheck;
//}



//void CMainFrame::OnUpdateViewSca(CCmdUI *pCmdUI)
//{
//	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
//	pCmdUI->SetCheck(DVManager::Mgr()->bCheck);
//}


//void CMainFrame::OnSliderRx()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//
//	// 리본 슬라이더 포인터 획득
//	//CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
//	//	m_wndRibbonBar.FindByID(ID_SLIDER_RX));
//
//	
//	// 리본 슬라이더 위치 값 획득
//	//int pos = pSlider->GetPos();
//
//	
//
//}


//void CMainFrame::OnSliderRy()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//
//	// 리본 슬라이더 포인터 획득
//	//CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
//	//	m_wndRibbonBar.FindByID(ID_SLIDER_RY));
//
//	// 리본 슬라이더 위치 값 획득
//	//int pos = pSlider->GetPos();
//
//}


//void CMainFrame::OnSliderRz()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//
//	// 리본 슬라이더 포인터 획득
//	//CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
//	//	m_wndRibbonBar.FindByID(ID_SLIDER_RZ));
//
//	// 리본 슬라이더 위치 값 획득
//	//int pos = pSlider->GetPos();
//
//}





void CMainFrame::OnSliderBone()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.


	// 리본 슬라이더 포인터 획득
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDER_BONE));

	//CMFCRibbonLabel* pLabel = DYNAMIC_DOWNCAST(CMFCRibbonLabel, m_wndRibbonBar.FindByID(IDC_NUM_BONE));
	//pLabel->SetText(_T("ChangeText"));
	//pLabel->Redraw();

	
	// 리본 슬라이더 위치 값 획득
	int pos = pSlider->GetPos();

	//DVManager::Mgr()->m_fThresholdBone = pos;
	//DVManager::Mgr()->ShowBone();

	DVManager::Mgr()->m_pControlManager->SetBoneThreshold(pos);
	DVManager::Mgr()->ShowBoneTest();

	

	//DVManager::Mgr()->m_fThresholdBone += pos;
}





void CMainFrame::OnSliderSkin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 리본 슬라이더 포인터 획득
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDER_SKIN));

	//CMFCRibbonLabel* pLabel = DYNAMIC_DOWNCAST(CMFCRibbonLabel, m_wndRibbonBar.FindByID(IDC_NUM_BONE));
	//pLabel->SetText(_T("ChangeText"));
	//pLabel->Redraw();


	// 리본 슬라이더 위치 값 획득
	int pos = pSlider->GetPos();

	DVManager::Mgr()->m_fThresholdSkin = pos;
	DVManager::Mgr()->ShowSkin();
}


void CMainFrame::OnUpdateSliderBone(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	
}


void CMainFrame::OnUpdateSliderSkin(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void CMainFrame::OnSliderrx()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDERRX));

	
	
	// 리본 슬라이더 위치 값 획득
	int pos = pSlider->GetPos();
	//pSlider->SetPos(DVManager::Mgr()->m_pControlManager->GetRotationRX());

	
		//DVManager::Mgr()->fTemp = DVManager::Mgr()->m_pControlManager->GetRotationRX() + pos;
	DVManager::Mgr()->m_pControlManager->SetRotationRX(pos);
	DVManager::Mgr()->m_pControlManager->RotateUpdate();
	DVManager::Mgr()->m_pControlManager->Update();
	DVManager::Mgr()->GetVtkWindow(3)->Render();
	
}


void CMainFrame::OnSliderry()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDERRY));

	// 리본 슬라이더 위치 값 획득
	int pos = pSlider->GetPos();
	//pSlider->SetPos(DVManager::Mgr()->m_pControlManager->GetRotationRX());


		//DVManager::Mgr()->fTemp = DVManager::Mgr()->m_pControlManager->GetRotationRX() + pos;
	DVManager::Mgr()->m_pControlManager->SetRotationRY(pos);
	DVManager::Mgr()->m_pControlManager->RotateUpdate();
	DVManager::Mgr()->m_pControlManager->Update();
	DVManager::Mgr()->GetVtkWindow(3)->Render();
}


void CMainFrame::OnSliderrz()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDERRZ));

	// 리본 슬라이더 위치 값 획득
	int pos = pSlider->GetPos();
	//pSlider->SetPos(DVManager::Mgr()->m_pControlManager->GetRotationRX());


		//DVManager::Mgr()->fTemp = DVManager::Mgr()->m_pControlManager->GetRotationRX() + pos;
	DVManager::Mgr()->m_pControlManager->SetRotationRZ(pos);
	DVManager::Mgr()->m_pControlManager->RotateUpdate();
	DVManager::Mgr()->m_pControlManager->Update();
	DVManager::Mgr()->GetVtkWindow(3)->Render();
}

//void CMainFrame::OnCheckThread()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	
//	/*m_rEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_mPos));
//	
//	m_rEdit->SetEditText(_T("test"));
//
//	ST_Thread st;*/
//
//	/*CMFCRibbonEdit* m_rEdit1 = new CMFCRibbonEdit(ID_EDIT_mPos, 72, _T("test!"), 13);
//	m_rEdit->SetEditText(_T("hi"));*/
//	m_pThread = NULL;
//	//m_pThread = ::AfxBeginThread(Thread_MouseTracking,&arg1,THREAD_PRIORITY_NORMAL,0,0);
//	m_pThread = ::AfxBeginThread(Thread_MouseTracking,this);
//		
//
//	if (m_pThread == NULL)
//	{
//		AfxMessageBox(_T("Error"));
//	}
//	//CloseHandle(m_pThread);
//      
//}


afx_msg LRESULT CMainFrame::OnUserMousepos(WPARAM wParam, LPARAM lParam)
{

	CString strMousePos;
	CMFCRibbonEdit* edit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_mPos));

	//// Client 크기
	//CRect rect;
	//GetClientRect(rect);

	//// 메인 View에 포함된 Dialog 배치 (4분할)

	//LONG xPos[3];
	//xPos[0] = rect.left;
	//xPos[1] = rect.left + rect.Width() / 2;
	//xPos[2] = rect.right;

	//LONG yPos[4];
	//yPos[0] = rect.top;
	//yPos[1] = rect.top + rect.Height() / 3;
	//yPos[2] = rect.top + 2 * (rect.Height() / 3);
	//yPos[3] = rect.bottom;

	//CRect subRect[4];
	//subRect[0] = CRect(xPos[0], yPos[0], xPos[1], yPos[1]); // Axial 위치
	//subRect[1] = CRect(xPos[0], yPos[1], xPos[1], yPos[2]); // Coronal 위치
	//subRect[2] = CRect(xPos[0], yPos[2], xPos[1], yPos[3]); // Sagittal 위치
	//subRect[3] = CRect(xPos[1], yPos[0], xPos[2], yPos[3]); // 3D View 위치

	//if ((wParam > xPos[0] && xPos[1] > wParam) && (lParam > yPos[0] && lParam < yPos[1])) 
	//{
	//	strMousePos.Format(_T("Axial X : %d , Y : %d , depth : %d"), (int)wParam, (int)lParam, (int)NULL);
	//	edit->SetEditText(strMousePos);
	//}
	//else if ((wParam > xPos[0] && xPos[1] > wParam) && (lParam > yPos[1] && lParam < yPos[2]))
	//{
	//	strMousePos.Format(_T("Coronal X : %d , Y : %d , depth : %d"), (int)wParam, (int)lParam, (int)NULL);
	//	edit->SetEditText(strMousePos);
	//}
	//else if ((wParam > xPos[0] && xPos[1] > wParam) && (lParam > yPos[2] && lParam < yPos[3]))
	//{
	//	strMousePos.Format(_T("Sagittal X : %d , Y : %d , depth : %d"), (int)wParam, (int)lParam, (int)NULL);
	//	edit->SetEditText(strMousePos);
	//}
	//else if ((wParam > xPos[1] && xPos[2] > wParam) && (lParam > yPos[0] && yPos[3]>lParam))
	//{
	//	strMousePos.Format(_T("3D View X : %d , Y : %d , depth : %d"), (int)wParam, (int)lParam, (int)NULL);
	//	edit->SetEditText(strMousePos);
	//}
	
	strMousePos.Format(_T("X : %d , Y : %d , depth : %d"), (int)wParam, (int)lParam ,(int)NULL);
	edit->SetEditText(strMousePos);

	

	return 0;
}


void CMainFrame::OnDestroy()
{
	CFrameWndEx::OnDestroy();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pThread != NULL)
	{
		WaitForSingleObject(m_pThread->m_hThread, 100);		
		m_bThreadWorking = stop;
		m_pThread = NULL;
		
	}
}
