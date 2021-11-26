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

// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "ChildView.h"
#include "DicomGroupView.h"

class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CChildView    m_wndView;
	CDicomGroupView m_DicomGroupView;

public:
	/// �⺻ View ������
	CChildView* GetWndView() { return &m_wndView; }

	

	
	enum State{stop=0,working};
	bool m_bThreadWorking = stop;
	CWinThread *m_pThread = NULL;
	

	//2. ������ ���۽�ų �Լ��� ����

	static UINT Thread_MouseTracking(LPVOID _mothod);

	

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	
	afx_msg void OnViewDicomGroup();
	afx_msg void OnUpdateViewDicomGroup( CCmdUI *pCmdUI );
	afx_msg void OnOpenDicomFolder();
	afx_msg void OnComboVolumeRenderMode();
//	afx_msg void OnViewSca();
//	afx_msg void OnUpdateViewSca(CCmdUI *pCmdUI);
	afx_msg void OnView3dOnOff();
	afx_msg void OnUpdateView3dOnOff(CCmdUI *pCmdUI);
//	afx_msg void OnViewSca();
//	afx_msg void OnUpdateViewSca(CCmdUI *pCmdUI);
//	afx_msg void OnSliderRx();
//	afx_msg void OnSliderRy();
//	afx_msg void OnSliderRz();
	afx_msg void OnViewOutline();
	afx_msg void OnUpdateViewOutline(CCmdUI *pCmdUI);
	afx_msg void OnCheckBone();
	afx_msg void OnUpdateCheckBone(CCmdUI *pCmdUI);
	afx_msg void OnSliderBone();
	afx_msg void OnCheckSkin();
	afx_msg void OnUpdateCheckSkin(CCmdUI *pCmdUI);
	afx_msg void OnSliderSkin();
	afx_msg void OnUpdateSliderBone(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSliderSkin(CCmdUI *pCmdUI);
	afx_msg void OnSliderrx();
	afx_msg void OnSliderry();
	afx_msg void OnSliderrz();
	

	/*afx_msg void OnCheckThread();*/
	afx_msg void OnCheckThread();
};


