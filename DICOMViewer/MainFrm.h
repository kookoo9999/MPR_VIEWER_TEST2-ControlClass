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

// MainFrm.h : CMainFrame 클래스의 인터페이스
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

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CChildView    m_wndView;
	CDicomGroupView m_DicomGroupView;
	
	

public:
	// 기본 View 윈도우
	CChildView* GetWndView() { return &m_wndView; }
	
private:
	enum State{stop=0,working};
	bool m_bThreadWorking = working;
	CWinThread *m_pThread = NULL;
	
	void TestFunc();
	//2. 스레드 동작시킬 함수를 생성
public:
	bool setThreadWorking();
	static UINT Thread_MouseTracking(LPVOID _mothod);
	
	

// 생성된 메시지 맵 함수
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
	//afx_msg void OnCheckThread();
protected:

	//Write Corrdinate at CMFCRibbonEdit*
	afx_msg LRESULT OnUserMousepos(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
};


