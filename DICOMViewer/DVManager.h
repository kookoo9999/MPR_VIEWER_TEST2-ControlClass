#pragma once

#include <math.h>
#include <vector>
#include <array>

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRendererCollection.h>
#include <vtkCornerAnnotation.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>

#include "DicomLoader.h"
#include "ControlManager.h"

class DVManager
{
	//////////////////////////////////////////////////////////////////////////
	/// Singleton 패턴을 이용한 전역 단일 매니저 생성
	//////////////////////////////////////////////////////////////////////////
private:
	DVManager();
	virtual ~DVManager();

	static void Destroy() { delete m_Instance; m_Instance = NULL; }

public:
	/// 전역 매니저 객체
	static DVManager* Mgr();
	
public:
	/// View Type
	static const int NUM_VIEW = 4;
	enum ViewType {VIEW_AXIAL, VIEW_CORONAL, VIEW_SAGITTAL, VIEW_3D  };

protected:
	/// Vtk Render Windows
	vtkSP<vtkRenderWindow> m_vtkWindow[NUM_VIEW];

	/// DICOM Loader
	vtkSP<DicomLoader> m_DicomLoader;

	/// 정보 표시
	vtkSP<vtkCornerAnnotation> m_Annotation[NUM_VIEW];

	static DVManager* m_Instance;
	

public:
	
	//Orient Marker Widget
	vtkSP<vtkOrientationMarkerWidget> m_orientMarker;

	// Vtk Render Windows
	vtkSP<vtkRenderWindow> GetVtkWindow( int viewType );

	// Vtk Window 초기화 (OnInitDialog)
	void InitVtkWindow( int viewType, void* hWnd );

	// Vtk Window 크기 조절 (OnSize)
	void ResizeVtkWindow( int viewType, int width, int height );

	// DICOM Loader 객체
	vtkSP<DicomLoader> GetDicomLoader();

	// View 타입에 따른 VTK 렌더러
	vtkSmartPointer<vtkRenderer> GetRenderer( int viewType );

	// DICOM Group 선택 시, 화면 업데이트 및 초기화
	void OnSelectDicomGroup( vtkSP<DicomGroup> group );

	// Volume 그리기 초기화
	void ClearVolumeDisplay();

	// Volume 그리기 업데이트
	void UpdateVolumeDisplay();

	// 스크롤바를 통한 슬라이스 인덱스 변경
	void ScrollSliceIndex( int viewType, int pos );

	// Volume 렌더링 모드 변경
	void ChangeVolumeRenderMode( int renderMode );

	// 정보 표시 업데이트
	void UpdateAnnotation();

	// DICOM 슬라이스 정보 표시
	void UpdateSliceAnnotation( int viewType );

	

public:
	bool bCheck = FALSE;

	// Rotation 설정 값
	double m_fRx = 0.0;
	double m_fRy = 0.0;
	double m_fRz = 0.0;

	// Skin, Bone Threshold 설정 값
	double m_fThresholdSkin = 72;
	double m_fThresholdBone = 452;

public:

	double fTemp = 0.0;
	int nTemp = 0;

	// 데이터 Outline
	vtkSP<vtkActor> m_pActorOutline;

	// Sagittal, Coronal, Axial View Plane Actor
	std::vector<vtkSP<vtkImageActor>> m_pActorSCAPlane;
	
	vtkSP<vtkActor> m_pActorBone;

	vtkSP<vtkActor> m_pActorSkin;

	//vtkSP < std::vector<vtkSP<vtkImageActor>>> m_pActorSCAPlnae;

	// Sagittal, Coronal, Axial Plane Position
	int m_nSagittalPos, m_nCoronalPos, m_nAxialPos;

	// Sagittal, Coronal, Axial Plane Maximum Position
	int m_nSagittalMax, m_nCoronalMax, m_nAxialMax;

	// Sagittal, Coronal, Axial Plane Visibility 설정 값
	bool m_bShowPlane=FALSE;

	// Bone Visibility 설정 값
	bool m_bShowBone = FALSE;

	// Skin Visibility 설정 값
	bool m_bShowSkin = FALSE;

	bool m_testCheck = TRUE;
	bool m_tBoneCheck = FALSE;
	bool m_tSkinCheck = FALSE;

	vtkSP<vtkImageData> m_pAlignedData;

	// Plane 을 켜고 끈다.
	void SetSCAViewOnOff(bool bFlag) { m_bShowPlane = bFlag; }
	bool GetSCAViewOnOff() const { return m_bShowPlane; }

	// Plane 의 위치를 조절한다.
	void SetSagittalPos(int value) { m_nSagittalPos = value; }
	int GetSagittalPos() const { return m_nSagittalPos; }
	void SetCoronalPos(int value) { m_nCoronalPos = value; }
	int GetCoronalPos() const { return m_nCoronalPos; }
	void SetAxialPos(int value) { m_nAxialPos = value; }
	int GetAxialPos() const { return m_nAxialPos; }

	//Show Plane
	void ShowPlnae();

	//Show Bone 
	void ShowBone();

	//Show Skin
	void ShowSkin();

	//3d View off
	void Update3DView();

	//Volume Rotate
	void RotateVolume();

	//show outline
	void ShowOutline();

	//via ControlManager class

	void ShowLineTest();

	void ShowBoneTest();

	void ShowSkinTest();

	void ShowPlnaeTest();

	void ShowPlnateTest2();

	//get mouse pos
	void Mouse();
	// 영상 데이터 저장 변수
	vtkSP<vtkImageData> m_pDICOMImage;

	// Thresholding, Rotate, Plane View Control 결과를 출력하는 클래스
	CControlManager* m_pControlManager;
};

