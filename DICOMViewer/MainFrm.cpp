// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "DICOMViewer.h"

#include "MainFrm.h"
#include "DVManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
END_MESSAGE_MAP()

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// �������� Ŭ���̾�Ʈ ������ �����ϴ� �並 ����ϴ�.
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("�� â�� ������ ���߽��ϴ�.\n");
		return -1;
	}

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// Visual Studio 2005 ��Ÿ�� ��ŷ â ������ Ȱ��ȭ�մϴ�.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 ��Ÿ�� ��ŷ â �ڵ� ���� ������ Ȱ��ȭ�մϴ�.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// ������ ���� ���� ���־� ������ �� ��Ÿ���� �����մϴ�.
	OnApplicationLook(theApp.m_nAppLook);

	// DICOM Group View ����
	if( !m_DicomGroupView.Create( 
		_T( "DICOM Group" ),		// ��ŷ â �̸�
		this,							// �θ� ������ = ���� ������
		CRect( 0, 0, 200, 200 ),		// �ʱ� ������ ũ��
		TRUE,							// ���� â ǥ�� ����
		ID_VIEW_DICOM_GROUP,	// ��ŷ â ID
		WS_CHILD | WS_VISIBLE |	// ��ŷ â ��Ÿ��
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
		CBRS_LEFT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0("��ŷ â�� ������ ���߽��ϴ�.\n");
		return -1;
	}
	// DICOM Group ��ŷâ�� �θ�â�� ��� �κп� ��ŷ�� �����ϵ��� ����
	m_DicomGroupView.EnableDocking( CBRS_ALIGN_ANY );
	// DICOM Group â ��ŷ
	DockPane( &m_DicomGroupView );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame ����

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


// CMainFrame �޽��� ó����

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// �� â���� ��Ŀ���� �̵��մϴ�.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// �信�� ù° ũ���� �ش� ��ɿ� ��Ÿ������ �մϴ�.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// �׷��� ������ �⺻ ó���մϴ�.
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
	// DICOM Group ��ŷâ�� ǥ��/���� (������¿� �ݴ��)
	m_DicomGroupView.ShowPane( !m_DicomGroupView.IsPaneVisible(), FALSE, FALSE );
}


void CMainFrame::OnUpdateViewDicomGroup( CCmdUI *pCmdUI )
{
	// DICOM Group ��ŷâ�� ���̴� �����̸� üũ�ڽ� üũ
	pCmdUI->SetCheck( m_DicomGroupView.IsPaneVisible() );
}


void CMainFrame::OnOpenDicomFolder()
{
	CFolderPickerDialog folderDlg( _T( "" ), 0, NULL, 0 );

	if( IDOK == folderDlg.DoModal() ) {
		/// DICOM ������ ���Ե� ���� ���
		CString path = folderDlg.GetPathName();

		/// �������� DICOM(*.dcm)���� �б�
		DVManager::Mgr()->GetDicomLoader()
			->OpenDicomDirectory( CT2CA( LPCWSTR( path ) ) );

		/// DICOM �׷� Ʈ�� ������Ʈ
		m_DicomGroupView.UpdateDicomTree();
	}
}


void CMainFrame::OnComboVolumeRenderMode()
{
	// ���� �޴��� �޺� �ڽ� ��Ʈ�� ��������
	CMFCRibbonComboBox *volumeModeComboBox = 
		DYNAMIC_DOWNCAST(CMFCRibbonComboBox, 
		m_wndRibbonBar.FindByID(ID_COMBO_VOLUME_RENDER_MODE));

	// �޺��ڽ����� ���õ� �ε���
	int selectedIdx = volumeModeComboBox->GetCurSel();

	// Volume ������ ��� ����
	DVManager::Mgr()->ChangeVolumeRenderMode( selectedIdx );
}


//void CMainFrame::OnViewSca()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//	//DVManager::Mgr()->UpdatePlnae();
//	DVManager::Mgr()->bCheck = !DVManager::Mgr()->bCheck;
//}


//void CMainFrame::OnUpdateViewSca(CCmdUI *pCmdUI)
//{
//	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
//	 pCmdUI->SetCheck(DVManager::Mgr()->bCheck);
//}


void CMainFrame::OnView3dOnOff()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//DVManager::Mgr()->bCheck = !DVManager::Mgr()->bCheck;

	DVManager::Mgr()->m_bShowPlane = !DVManager::Mgr()->m_bShowPlane;
	DVManager::Mgr()->ShowPlnaeTest();

	//DVManager::Mgr()->m_bShowPlane = !DVManager::Mgr()->m_bShowPlane;
	
}

void CMainFrame::OnUpdateView3dOnOff(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	//pCmdUI->SetCheck(DVManager::Mgr()->bCheck);
	pCmdUI->SetCheck(DVManager::Mgr()->m_bShowPlane);
}

void CMainFrame::OnViewOutline()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	DVManager::Mgr()->ShowLineTest();
	DVManager::Mgr()->m_testCheck = !DVManager::Mgr()->m_testCheck;
	//DVManager::Mgr()->ShowOutline();
	//DVManager::Mgr()->bCheck = !DVManager::Mgr()->bCheck;
}


void CMainFrame::OnUpdateViewOutline(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	//pCmdUI->SetCheck(DVManager::Mgr()->bCheck);
	
	pCmdUI->SetCheck(DVManager::Mgr()->m_testCheck);
}


void CMainFrame::OnCheckBone()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDER_BONE));
	pSlider->SetPos(DVManager::Mgr()->m_pControlManager->GetBoneThreshold());
	pSlider->Redraw();
	
	DVManager::Mgr()->m_bShowBone = !DVManager::Mgr()->m_bShowBone;
	//DVManager::Mgr()->ShowBone();
	DVManager::Mgr()->ShowBoneTest();
	
}


void CMainFrame::OnUpdateCheckBone(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->SetCheck(DVManager::Mgr()->m_bShowBone);
}

void CMainFrame::OnCheckSkin()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonSlider* spSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDER_SKIN));
	spSlider->SetPos(DVManager::Mgr()->m_fThresholdSkin);
	spSlider->Redraw();

	DVManager::Mgr()->m_bShowSkin = !DVManager::Mgr()->m_bShowSkin;
	DVManager::Mgr()->ShowSkin();

	
}


void CMainFrame::OnUpdateCheckSkin(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->SetCheck(DVManager::Mgr()->m_bShowSkin);
}


//void CMainFrame::OnViewSca()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//	DVManager::Mgr()->bCheck = !DVManager::Mgr()->bCheck;
//}



//void CMainFrame::OnUpdateViewSca(CCmdUI *pCmdUI)
//{
//	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
//	pCmdUI->SetCheck(DVManager::Mgr()->bCheck);
//}


//void CMainFrame::OnSliderRx()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//
//	// ���� �����̴� ������ ȹ��
//	//CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
//	//	m_wndRibbonBar.FindByID(ID_SLIDER_RX));
//
//	
//	// ���� �����̴� ��ġ �� ȹ��
//	//int pos = pSlider->GetPos();
//
//	
//
//}


//void CMainFrame::OnSliderRy()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//
//	// ���� �����̴� ������ ȹ��
//	//CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
//	//	m_wndRibbonBar.FindByID(ID_SLIDER_RY));
//
//	// ���� �����̴� ��ġ �� ȹ��
//	//int pos = pSlider->GetPos();
//
//}


//void CMainFrame::OnSliderRz()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//
//	// ���� �����̴� ������ ȹ��
//	//CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
//	//	m_wndRibbonBar.FindByID(ID_SLIDER_RZ));
//
//	// ���� �����̴� ��ġ �� ȹ��
//	//int pos = pSlider->GetPos();
//
//}





void CMainFrame::OnSliderBone()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.


	// ���� �����̴� ������ ȹ��
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDER_BONE));

	//CMFCRibbonLabel* pLabel = DYNAMIC_DOWNCAST(CMFCRibbonLabel, m_wndRibbonBar.FindByID(IDC_NUM_BONE));
	//pLabel->SetText(_T("ChangeText"));
	//pLabel->Redraw();

	
	// ���� �����̴� ��ġ �� ȹ��
	int pos = pSlider->GetPos();

	//DVManager::Mgr()->m_fThresholdBone = pos;
	//DVManager::Mgr()->ShowBone();

	DVManager::Mgr()->m_pControlManager->SetBoneThreshold(pos);
	DVManager::Mgr()->ShowBoneTest();

	

	//DVManager::Mgr()->m_fThresholdBone += pos;
}





void CMainFrame::OnSliderSkin()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� �����̴� ������ ȹ��
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDER_SKIN));

	//CMFCRibbonLabel* pLabel = DYNAMIC_DOWNCAST(CMFCRibbonLabel, m_wndRibbonBar.FindByID(IDC_NUM_BONE));
	//pLabel->SetText(_T("ChangeText"));
	//pLabel->Redraw();


	// ���� �����̴� ��ġ �� ȹ��
	int pos = pSlider->GetPos();

	DVManager::Mgr()->m_fThresholdSkin = pos;
	DVManager::Mgr()->ShowSkin();
}


void CMainFrame::OnUpdateSliderBone(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	
}


void CMainFrame::OnUpdateSliderSkin(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
}


void CMainFrame::OnSliderrx()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDERRX));

	pSlider->SetPos(DVManager::Mgr()->fTemp + 180);
	
	// ���� �����̴� ��ġ �� ȹ��
	int pos = pSlider->GetPos();

	if (pos > 180)
	{
		DVManager::Mgr()->fTemp = DVManager::Mgr()->m_pControlManager->GetRotationRX() - pos;
		DVManager::Mgr()->m_pControlManager->SetRotationRX(DVManager::Mgr()->fTemp);
		DVManager::Mgr()->m_pControlManager->RotateUpdate();
		DVManager::Mgr()->m_pControlManager->Update();
	}
	else
	{
		DVManager::Mgr()->fTemp = DVManager::Mgr()->m_pControlManager->GetRotationRX() + pos;
		DVManager::Mgr()->m_pControlManager->SetRotationRX(DVManager::Mgr()->fTemp);
		DVManager::Mgr()->m_pControlManager->RotateUpdate();
		DVManager::Mgr()->m_pControlManager->Update();
	}
}


void CMainFrame::OnSliderry()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDERRY));

	pSlider->SetPos(DVManager::Mgr()->fTemp + 180);

	// ���� �����̴� ��ġ �� ȹ��
	int pos = pSlider->GetPos();

	if (pos > 180)
	{
		DVManager::Mgr()->fTemp = DVManager::Mgr()->m_pControlManager->GetRotationRX() - pos;
		DVManager::Mgr()->m_pControlManager->SetRotationRX(DVManager::Mgr()->fTemp);
		DVManager::Mgr()->m_pControlManager->RotateUpdate();
		DVManager::Mgr()->m_pControlManager->Update();
	}
	else
	{
		DVManager::Mgr()->fTemp = DVManager::Mgr()->m_pControlManager->GetRotationRX() + pos;
		DVManager::Mgr()->m_pControlManager->SetRotationRX(DVManager::Mgr()->fTemp);
		DVManager::Mgr()->m_pControlManager->RotateUpdate();
		DVManager::Mgr()->m_pControlManager->Update();
	}
}


void CMainFrame::OnSliderrz()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider,
		m_wndRibbonBar.FindByID(ID_SLIDERRZ));

	// ���� �����̴� ��ġ �� ȹ��
	int pos = pSlider->GetPos();
}
