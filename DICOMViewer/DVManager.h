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
	/// Singleton ������ �̿��� ���� ���� �Ŵ��� ����
	//////////////////////////////////////////////////////////////////////////
private:
	DVManager();
	virtual ~DVManager();

	static void Destroy() { delete m_Instance; m_Instance = NULL; }

public:
	/// ���� �Ŵ��� ��ü
	static DVManager* Mgr();
	
public:
	/// View Type
	static const int NUM_VIEW = 4;
	enum ViewType { VIEW_AXIAL, VIEW_CORONAL, VIEW_SAGITTAL, VIEW_3D };

protected:
	/// Vtk Render Windows
	vtkSmartPointer<vtkRenderWindow> m_vtkWindow[NUM_VIEW];

	/// DICOM Loader
	vtkSmartPointer<DicomLoader> m_DicomLoader;

	/// ���� ǥ��
	vtkSmartPointer<vtkCornerAnnotation> m_Annotation[NUM_VIEW];

	static DVManager* m_Instance;
	

public:
	
	//Orient Marker Widget
	vtkSmartPointer<vtkOrientationMarkerWidget> m_orientMarker;

	// Vtk Render Windows
	vtkSmartPointer<vtkRenderWindow> GetVtkWindow( int viewType );

	// Vtk Window �ʱ�ȭ (OnInitDialog)
	void InitVtkWindow( int viewType, void* hWnd );

	// Vtk Window ũ�� ���� (OnSize)
	void ResizeVtkWindow( int viewType, int width, int height );

	// DICOM Loader ��ü
	vtkSmartPointer<DicomLoader> GetDicomLoader();

	// View Ÿ�Կ� ���� VTK ������
	vtkSmartPointer<vtkRenderer> GetRenderer( int viewType );

	// DICOM Group ���� ��, ȭ�� ������Ʈ �� �ʱ�ȭ
	void OnSelectDicomGroup( vtkSmartPointer<DicomGroup> group );

	// Volume �׸��� �ʱ�ȭ
	void ClearVolumeDisplay();

	// Volume �׸��� ������Ʈ
	void UpdateVolumeDisplay();

	// ��ũ�ѹٸ� ���� �����̽� �ε��� ����
	void ScrollSliceIndex( int viewType, int pos );

	// Volume ������ ��� ����
	void ChangeVolumeRenderMode( int renderMode );

	// ���� ǥ�� ������Ʈ
	void UpdateAnnotation();

	// DICOM �����̽� ���� ǥ��
	void UpdateSliceAnnotation( int viewType );

	

public:
	bool bCheck = FALSE;

	// Rotation ���� ��
	double m_fRx = 0.0;
	double m_fRy = 0.0;
	double m_fRz = 0.0;

	// Skin, Bone Threshold ���� ��
	double m_fThresholdSkin = 72;
	double m_fThresholdBone = 452;

public:

	double fTemp = 0.0;
	int nTemp = 0;

	// ������ Outline
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

	// Sagittal, Coronal, Axial Plane Visibility ���� ��
	bool m_bShowPlane=FALSE;

	// Bone Visibility ���� ��
	bool m_bShowBone = FALSE;

	// Skin Visibility ���� ��
	bool m_bShowSkin = FALSE;

	bool m_testCheck = FALSE;
	bool m_tBoneCheck = FALSE;
	bool m_tSkinCheck = FALSE;

	vtkSP<vtkImageData> m_pAlignedData;

	// Plane �� �Ѱ� ����.
	void SetSCAViewOnOff(bool bFlag) { m_bShowPlane = bFlag; }
	bool GetSCAViewOnOff() const { return m_bShowPlane; }

	// Plane �� ��ġ�� �����Ѵ�.
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

	void ShowLineTest();

	void ShowBoneTest();

	void ShowSkinTest();

	void ShowPlnaeTest();
	// ���� ������ ���� ����
	vtkSP<vtkImageData> m_pDICOMImage;

	// Thresholding, Rotate, Plane View Control ����� ����ϴ� Ŭ����
	CControlManager* m_pControlManager;
};

