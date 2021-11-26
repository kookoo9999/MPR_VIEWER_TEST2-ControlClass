#include "stdafx.h"
#include "DVManager.h"

#include "DICOMViewer.h"
#include "MainFrm.h"
#include "ChildView.h"

//to detect memory leak
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DVManager* DVManager::m_Instance = NULL;

DVManager::DVManager()
	: m_pControlManager(nullptr)
{
	vtkObject::GlobalWarningDisplayOff();
}




DVManager::~DVManager()
{
	SAFE_DELETE(m_pControlManager);
}

DVManager* DVManager::Mgr()
{
	if( m_Instance == NULL ) {
		m_Instance = new DVManager();
		atexit( Destroy );
	}
	return m_Instance;
}


vtkSP<vtkRenderWindow> DVManager::GetVtkWindow( int viewType )
{
	// viewType ���� �˻�
	if( viewType < 0 || viewType >= NUM_VIEW ) return NULL;

	// viewType �� vtkRenderWindow ��ȯ
	return m_vtkWindow[viewType];
}

void DVManager::InitVtkWindow( int viewType, void* hWnd )
{
	// viewType ���� �˻�
	if( viewType < 0 || viewType >= NUM_VIEW ) return;
	
	// vtk Render Window ����
	if( m_vtkWindow[viewType] == NULL ) {
		// Interactor ����
		C_VTK(vtkRenderWindowInteractor,interactor);
		//vtkSP<vtkRenderWindowInteractor> interactor = vtkSP<vtkRenderWindowInteractor>::New();

		// Renderer ����
		C_VTK(vtkRenderer, renderer);
		
		renderer->SetBackground( 0.1, 0.1, 0.1 );					// ������ ���

		// 3D View ����
		if( viewType == VIEW_3D ) {

			

			// Trackball Camera ���ͷ��� ��Ÿ�� ����
			interactor->SetInteractorStyle( vtkSP<vtkInteractorStyleTrackballCamera>::New() );

			vtkSP<vtkCamera> camera = renderer->GetActiveCamera();
			camera->ParallelProjectionOn();				// Parallel Projection ���
			camera->SetPosition( 0.0, -1.0, 0.0 );		// ī�޶� ��ġ
			camera->SetViewUp( 0.0, 0.0, 1.0 );			// ī�޶� Up ����
		}
		// 2D View ����
		else {
			// Image ���ͷ��� ��Ÿ�� ����
			interactor->SetInteractorStyle( vtkSP<vtkInteractorStyleImage>::New() );

			vtkSP<vtkCamera> camera = renderer->GetActiveCamera();
			camera->ParallelProjectionOn();				// Parallel Projection ���
			camera->SetPosition( 0.0, 0.0, -1.0 );		// ī�޶� ��ġ
			camera->SetViewUp( 0.0, -1.0, 0.0 );			// ī�޶� Up ����
		}


		
		// RenderWindow ���� �� Dialog �ڵ�, Interactor, Renderer ����
		m_vtkWindow[viewType] = vtkSP<vtkRenderWindow>::New();
		m_vtkWindow[viewType]->SetParentId( hWnd );
		m_vtkWindow[viewType]->SetInteractor( interactor );
		m_vtkWindow[viewType]->AddRenderer( renderer );
		m_vtkWindow[viewType]->Render();

		// 3D Render View�� Axes Actor �߰�
		if (m_vtkWindow[VIEW_3D]) {
			C_VTK(vtkAxesActor, axesActor);
			m_orientMarker = vtkSP<vtkOrientationMarkerWidget>::New();
			m_orientMarker->SetOrientationMarker(axesActor);
			m_orientMarker->SetCurrentRenderer(renderer);
			m_orientMarker->SetInteractor(interactor);
			m_orientMarker->SetViewport(0.8, 0.0, 1.0, 0.2);
			m_orientMarker->SetEnabled(1);
			m_orientMarker->InteractiveOff();
		}
	}
}

void DVManager::ResizeVtkWindow( int viewType, int width, int height )
{
	// viewType ���� �˻� �� vtkRenderWindow �˻�
	if( viewType < 0 || viewType >= NUM_VIEW ) return;
	if( m_vtkWindow[viewType] == NULL ) return;

	// �ش� vtkRenderWindow ũ�� ����
	m_vtkWindow[viewType]->SetSize( width, height );
}

vtkSP<DicomLoader> DVManager::GetDicomLoader()
{
	// DicomLoader ��ü�� null �̸� ����
	if( m_DicomLoader == NULL ) {
		m_DicomLoader = vtkSP<DicomLoader>::New();
	}

	// DicomLoader ��ü ��ȯ
	return m_DicomLoader;
}

vtkSP<vtkRenderer> DVManager::GetRenderer( int viewType )
{
	// View Ÿ�� �˻�
	if( viewType < 0 || viewType >= NUM_VIEW ) return NULL;
	// vtkRenderWindow ���� ���� �˻�
	if( m_vtkWindow[viewType] == NULL ) return NULL;

	// �ش��ϴ� View Ÿ���� vtkRenderWindow���� ù ��° Renderer ��ȯ
	return m_vtkWindow[viewType]->GetRenderers()->GetFirstRenderer();
}

void DVManager::OnSelectDicomGroup( vtkSP<DicomGroup> group )
{
	// ������ �ʱ�ȭ
	ClearVolumeDisplay();

	// ���õ� DICOM �׷쿡�� Volume ������ �ε�
	GetDicomLoader()->LoadVolumeData( group );
	
	// ���� ǥ�� ������Ʈ
	UpdateAnnotation();

	// Volume ������ ������ ������Ʈ
	UpdateVolumeDisplay();

	//outline
	//if(bCheck)	ShowOutline();
	
	// Plnae ������ ������Ʈ
	//UpdatePlnae();

	// �⺻ View ������ ���
	CChildView* mainView = ((CMainFrame*)AfxGetMainWnd())->GetWndView();
	if( mainView == NULL ) return;

	// 2D View ��ũ�� �� ������Ʈ
	for( int viewType = VIEW_AXIAL; viewType <= VIEW_SAGITTAL; viewType++ ) {
		mainView->GetDlgVtkView( viewType )->UpdateScrollBar();
	}
	// �ε�� Volume ������ �˻�
	vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if (volumeData == NULL) return;

	m_pDICOMImage = volumeData->GetImageData();
	
	// Thresholding, Rotate, Plane View Control ����� ����ϴ� Ŭ���� ��ü �ʱ�ȭ
	m_pControlManager = new CControlManager(m_pDICOMImage, GetRenderer(VIEW_3D));


}

void DVManager::ClearVolumeDisplay()
{
	// �ε�� Volume ������ �˻�
	vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;

	// 3D �信 ���� ������ ����
	GetRenderer( VIEW_3D )->RemoveViewProp( volumeData->GetVolumeRendering() );
	GetRenderer(VIEW_3D)->RemoveAllObservers();
	GetRenderer(VIEW_3D)->RemoveAllViewProps();
	// �����̽� �信 �� �����̽� Actor ����
	for( int viewType = VIEW_AXIAL; viewType <= VIEW_SAGITTAL; viewType++ ) {
		GetRenderer( viewType )->RemoveActor( volumeData->GetSliceActor( viewType ) );
	}
}

void DVManager::UpdateVolumeDisplay()
{
	// �ε�� Volume ������ �˻�
	vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;

	// 3D �信 ���� ������ �߰�
	GetRenderer( VIEW_3D )->AddVolume( volumeData->GetVolumeRendering() );
	//GetRenderer(VIEW_3D)->ComputeVisiblePropBounds(renderer_bounds);
	GetRenderer( VIEW_3D )->ResetCamera();	// ī�޶� �缳��
	m_vtkWindow[VIEW_3D]->Render();			// ȭ�� ����

	// �����̽� �信 �� �����̽� Actor �߰�
	for( int viewType = VIEW_AXIAL; viewType <= VIEW_SAGITTAL; viewType++ ) {
		GetRenderer( viewType )->AddActor( volumeData->GetSliceActor( viewType ) );
		GetRenderer( viewType )->ResetCamera();	// ī�޶� �缳��
		m_vtkWindow[viewType]->Render();			// ȭ�� ����		
	}
}



void DVManager::ScrollSliceIndex( int viewType, int pos )
{
	// ���� �ε�� Volume ������
	vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;

	// Volume �̹����� �ε��� ����
	volumeData->SetSliceIndex( viewType, pos );

	//Control Class�� Plane Position Update
	if(viewType==VIEW_AXIAL)	m_pControlManager->SetAxialPos(pos);
	if (viewType == VIEW_CORONAL) m_pControlManager->SetCoronalPos(pos);
	if (viewType == VIEW_SAGITTAL)m_pControlManager->SetSagittalPos(pos);

	//���泻�� Update �� Render Update
	m_pControlManager->Update();
	GetVtkWindow(VIEW_3D)->Render();

	// ���� ǥ�� ������Ʈ
	UpdateAnnotation();

	// ȭ�� ������Ʈ
	m_vtkWindow[viewType]->Render();
}

void DVManager::ChangeVolumeRenderMode( int renderMode )
{
	// ���� �ε�� Volume ������ �˻�
	vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;
	
	// Volume �������� ��� ����
	volumeData->SetCurrentPresetMode( renderMode );

	// ȭ�� ������Ʈ
	m_vtkWindow[VIEW_3D]->Render();
}

void DVManager::UpdateAnnotation()
{
	// ���� ǥ�� ��ü ����
	for( int viewType = 0; viewType < NUM_VIEW; viewType++ ) {
		if( m_Annotation[viewType] == NULL ) {
			m_Annotation[viewType] = vtkSP<vtkCornerAnnotation>::New();
			m_Annotation[viewType]->SetMaximumFontSize( 20 );

			GetRenderer( viewType )->AddViewProp( m_Annotation[viewType] );
		}

		// 2D �����̽� ���� ǥ��
		UpdateSliceAnnotation( viewType );
	}
	
	// 3D ���� ǥ��
	m_Annotation[VIEW_3D]->SetText( 2, "3D" );
}

void DVManager::UpdateSliceAnnotation( int viewType )
{
	// 2D �����̽� View Ÿ�� �˻�
	if( viewType != VIEW_AXIAL &&
		viewType != VIEW_CORONAL &&
		viewType != VIEW_SAGITTAL ) return;

	// Volume ������ �˻�
	vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;
	
	// Volume �̹����� �ε��� ���� 
	int ext[6];
	volumeData->GetImageData()->GetExtent( ext );

	// ���� �� ���� : �����̽� �̸�/�ε���
	std::string leftTopText;
	switch( viewType ) {
	case VIEW_AXIAL:
		leftTopText = "Axial\n" + 
			std::to_string( volumeData->GetSliceIndex( viewType ) ) + 
			" / " + std::to_string( ext[5] );
		//m_nAxialPos = volumeData->GetSliceIndex(viewType);
		break;
	case VIEW_CORONAL:
		leftTopText = "Coronal\n" + 
			std::to_string( volumeData->GetSliceIndex( viewType ) ) + 
			" / " + std::to_string( ext[3] );
		//m_nCoronalPos = volumeData->GetSliceIndex(viewType);
		break;
	case VIEW_SAGITTAL:
		leftTopText = "Sagittal\n" + 
			std::to_string( volumeData->GetSliceIndex( viewType ) ) + 
			" / " + std::to_string( ext[1] );
		//m_nSagittalPos = volumeData->GetSliceIndex(viewType);
		break;
	}

	// �׷� ������ �˻�
	vtkSP<DicomGroup> group = GetDicomLoader()->GetCurrentGroup();
	if( group == NULL ) return;

	// ������ �� ���� : ȯ������
	std::string rightTopText = group->GetPatientName() + "\n" 
		+ group->GetPatientBirthDate() + "\n"
		+ group->GetPatientSex() + "\n"
		+ group->GetPatientAge() + "\n"
		+ group->GetPatientWeight() + "\n";

	// ���� �� �ڳ� (�ε��� 2) ���� ǥ��
	m_Annotation[viewType]->SetText( 2, leftTopText.c_str() );
	// ������ �� �ڳ� (�ε��� 3) ���� ǥ��
	m_Annotation[viewType]->SetText( 3, rightTopText.c_str() );
}

void DVManager::RotateVolume()
{
	// �ε�� Volume ������ �˻�
	vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
	if (volumeData == NULL) return;

	auto m_origin=volumeData->GetOrientation();
	


	

}

void DVManager::ShowOutline()
{
	GetRenderer(VIEW_3D)->RemoveActor(m_pActorOutline);
	m_pActorOutline = nullptr;
	bCheck = !bCheck;
	
	if (bCheck)
	{
		// Set the colors.	
		C_VTK(vtkNamedColors, colors);

		// Volume ������ �˻�
		vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
		if (volumeData == NULL) return;

		{
			C_VTK(vtkOutlineFilter, outlineData);
			outlineData->SetInputData(volumeData->GetImageData());

			C_VTK(vtkPolyDataMapper, mapOutline);
			mapOutline->SetInputConnection(outlineData->GetOutputPort());

			m_pActorOutline = vtkSP<vtkActor>::New();
			m_pActorOutline->SetMapper(mapOutline);

			m_pActorOutline->GetProperty()->SetColor(colors->GetColor3d("Silver").GetData());

			GetRenderer(VIEW_3D)->AddActor(m_pActorOutline);
			

		}
	}
	
	GetVtkWindow(VIEW_3D)->Render();
}

void DVManager::ShowLineTest()
{
	auto pCont = m_pControlManager;
	if (nullptr == pCont) return;

	pCont->SetLineOnOff(!pCont->GetLineOnOff());
	pCont->Update();
	GetVtkWindow(VIEW_3D)->Render();
}

void DVManager::ShowBoneTest()
{
	auto pCont = m_pControlManager;
	if (nullptr == pCont) return;

	//pCont->SetBoneOnOff(!pCont->GetBoneOnOff());
	pCont->Update();
	GetVtkWindow(VIEW_3D)->Render();
}

void DVManager::ShowSkinTest()
{
	auto pCont = m_pControlManager;
	if (nullptr == pCont) return;

	pCont->SetSkinOnOff(!pCont->GetSkinOnOff());
	pCont->Update();
	GetVtkWindow(VIEW_3D)->Render();
}

#include <vtkSphereSource.h>

void DVManager::ShowPlnaeTest()
{
	auto pRenWin = GetVtkWindow(VIEW_3D);	
	//auto pRen = pRenWin->GetRenderers()->GetFirstRenderer();

	auto pCont = m_pControlManager;
	if (nullptr == pCont) return;

	pCont->SetSCAViewOnOff(!pCont->GetSCAViewOnOff());
	pCont->Update();
	
	/*
		
	C_VTK(vtkSphereSource, src);
	src->SetRadius(10.0);
	src->SetCenter(100, 200, 300);

	C_VTK(vtkPolyDataMapper, mapper);
	mapper->SetInputConnection(src->GetOutputPort());

	C_VTK(vtkActor, actor);
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(1, 0, 0);

	pRen->AddActor(actor);

	pRen->ResetCamera();
	*/
	   
	pRenWin->Render();	
}

void DVManager::ShowPlnateTest2()
{
	for (auto& s : m_pActorSCAPlane)
	{
		GetRenderer(VIEW_3D)->RemoveActor(s);
		s = nullptr;
	}
	m_pActorSCAPlane.clear();

	if (m_bShowPlane)
	{
		// Volume ������ �˻�
		vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
		if (volumeData == NULL) return;

		auto spacing = volumeData->GetImageData()->GetSpacing();

		auto intDim = volumeData->GetImageData()->GetDimensions();

		m_nAxialMax = intDim[2];
		m_nCoronalMax = intDim[1];
		m_nSagittalMax = intDim[0];

		// ������ 1/2 ��ġ�� Postion �� �ʱ�ȭ�Ѵ�.
		m_nSagittalPos = m_nSagittalMax / 2;
		m_nCoronalPos = m_nCoronalMax / 2;
		m_nAxialPos = m_nAxialMax / 2;

		volumeData->GetSliceActor(VIEW_SAGITTAL);
		{
			// Start by creating a black/white lookup table.
			C_VTK(vtkLookupTable, bwLut);
			bwLut->SetTableRange(0, 2000);
			bwLut->SetSaturationRange(0, 0);
			bwLut->SetHueRange(0, 0);
			bwLut->SetValueRange(0, 1);
			bwLut->Build(); //effective built

			C_VTK(vtkImageMapToColors, sagittalColors);
			sagittalColors->SetInputData(volumeData->GetImageData());
			sagittalColors->SetLookupTable(bwLut);
			sagittalColors->Update();

			volumeData->GetSliceActor(VIEW_SAGITTAL)->GetMapper()->SetInputConnection(sagittalColors->GetOutputPort());
			volumeData->GetSliceActor(VIEW_SAGITTAL)->SetDisplayExtent(m_nSagittalPos, m_nSagittalPos, 0, m_nCoronalMax, 0, m_nAxialMax);
			volumeData->GetSliceActor(VIEW_SAGITTAL)->SetDisplayExtent(m_nSagittalPos, m_nSagittalPos, 0, m_nCoronalMax, 0, m_nAxialMax);



			m_pActorSCAPlane.push_back(volumeData->GetSliceActor(VIEW_SAGITTAL));
		}
		
		

		volumeData->GetSliceActor(VIEW_AXIAL);
		{
			// Now create a lookup table that consists of the full hue circle (from HSV).
			C_VTK(vtkLookupTable, hueLut);
			hueLut->SetTableRange(0, 2000);
			hueLut->SetHueRange(0, 0);
			hueLut->SetSaturationRange(0, 0);
			hueLut->SetValueRange(0, 1);
			hueLut->Build(); //effective built

			C_VTK(vtkImageMapToColors, axialColors);
			axialColors->SetInputData(volumeData->GetImageData());
			axialColors->SetLookupTable(hueLut);
			axialColors->Update();

			volumeData->GetSliceActor(VIEW_AXIAL)->GetMapper()->SetInputConnection(axialColors->GetOutputPort());
			volumeData->GetSliceActor(VIEW_AXIAL)->SetDisplayExtent(0, m_nSagittalMax, 0, m_nCoronalMax, m_nAxialPos, m_nAxialPos);
			volumeData->GetSliceActor(VIEW_AXIAL)->ForceOpaqueOn();

			m_pActorSCAPlane.push_back(volumeData->GetSliceActor(VIEW_AXIAL));
		}
		
		
		

		volumeData->GetSliceActor(VIEW_CORONAL);
		{
			// Finally, create a lookup table with a single hue but having a range in the saturation of the hue.
			C_VTK(vtkLookupTable, satLut);
			satLut->SetTableRange(0, 2000);
			satLut->SetHueRange(0, 0);
			satLut->SetSaturationRange(0, 0);
			satLut->SetValueRange(0, 1);
			satLut->Build(); //effective built

			C_VTK(vtkImageMapToColors, coronalColors);
			coronalColors->SetInputData(volumeData->GetImageData());
			coronalColors->SetLookupTable(satLut);
			coronalColors->Update();

			volumeData->GetSliceActor(VIEW_CORONAL)->GetMapper()->SetInputConnection(coronalColors->GetOutputPort());
			volumeData->GetSliceActor(VIEW_CORONAL)->SetDisplayExtent(0, m_nSagittalMax, m_nCoronalPos, m_nCoronalPos, 0, m_nAxialMax);
			volumeData->GetSliceActor(VIEW_CORONAL)->ForceOpaqueOn();



			m_pActorSCAPlane.push_back(volumeData->GetSliceActor(VIEW_CORONAL));
		}
		for (auto& s : m_pActorSCAPlane)
		{
			GetRenderer(VIEW_3D)->AddActor(s);

		}
		
	}	

	auto pWin = GetVtkWindow(VIEW_3D);

	pWin->Render();

}

void DVManager::Mouse()
{
	CPoint cp;
	::GetCursorPos(&cp);
	
	
}

	


void DVManager::ShowPlnae()
{	

	for (auto& s : m_pActorSCAPlane)
	{
		GetRenderer(VIEW_3D)->RemoveActor(s);
		s = nullptr;
	}	
	m_pActorSCAPlane.clear();
	
	if (m_bShowPlane)
	{
		// Volume ������ �˻�
		vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
		if (volumeData == NULL) return;

		auto spacing = volumeData->GetImageData()->GetSpacing();

		auto intDim = volumeData->GetImageData()->GetDimensions();

		m_nAxialMax = intDim[2];
		m_nCoronalMax = intDim[1];
		m_nSagittalMax = intDim[0];

		// ������ 1/2 ��ġ�� Postion �� �ʱ�ȭ�Ѵ�.
		m_nSagittalPos = m_nSagittalMax / 2;
		m_nCoronalPos = m_nCoronalMax / 2;
		m_nAxialPos = m_nAxialMax / 2;
		
		C_VTK(vtkImageActor, sagittal);
		{
			// Start by creating a black/white lookup table.
			C_VTK(vtkLookupTable, bwLut);
			bwLut->SetTableRange(0, 2000);
			bwLut->SetSaturationRange(0, 0);
			bwLut->SetHueRange(0, 0);
			bwLut->SetValueRange(0, 1);
			bwLut->Build(); //effective built

			C_VTK(vtkImageMapToColors, sagittalColors);
			sagittalColors->SetInputData(volumeData->GetImageData());
			sagittalColors->SetLookupTable(bwLut);
			sagittalColors->Update();

			sagittal->GetMapper()->SetInputConnection(sagittalColors->GetOutputPort());
			sagittal->SetDisplayExtent(m_nSagittalPos, m_nSagittalPos, 0, m_nCoronalMax, 0, m_nAxialMax);
			sagittal->ForceOpaqueOn();

			m_pActorSCAPlane.push_back(sagittal);
		}

		C_VTK(vtkImageActor, axial);
		{
			// Now create a lookup table that consists of the full hue circle (from HSV).
			C_VTK(vtkLookupTable, hueLut);
			hueLut->SetTableRange(0, 2000);
			hueLut->SetHueRange(0, 0);
			hueLut->SetSaturationRange(0, 0);
			hueLut->SetValueRange(0, 1);
			hueLut->Build(); //effective built

			C_VTK(vtkImageMapToColors, axialColors);
			axialColors->SetInputData(volumeData->GetImageData());
			axialColors->SetLookupTable(hueLut);
			axialColors->Update();

			axial->GetMapper()->SetInputConnection(axialColors->GetOutputPort());
			axial->SetDisplayExtent(0, m_nSagittalMax, 0, m_nCoronalMax, m_nAxialPos, m_nAxialPos);
			axial->ForceOpaqueOn();

			m_pActorSCAPlane.push_back(axial);
		}

		C_VTK(vtkImageActor, coronal);
		{
			// Finally, create a lookup table with a single hue but having a range in the saturation of the hue.
			C_VTK(vtkLookupTable, satLut);
			satLut->SetTableRange(0, 2000);
			satLut->SetHueRange(0, 0);
			satLut->SetSaturationRange(0, 0);
			satLut->SetValueRange(0, 1);
			satLut->Build(); //effective built

			C_VTK(vtkImageMapToColors, coronalColors);
			coronalColors->SetInputData(volumeData->GetImageData());
			coronalColors->SetLookupTable(satLut);
			coronalColors->Update();

			coronal->GetMapper()->SetInputConnection(coronalColors->GetOutputPort());
			coronal->SetDisplayExtent(0, m_nSagittalMax, m_nCoronalPos, m_nCoronalPos, 0, m_nAxialMax);
			coronal->ForceOpaqueOn();

			m_pActorSCAPlane.push_back(coronal);
		}

		for (auto& s : m_pActorSCAPlane)
		{
			GetRenderer(VIEW_3D)->AddActor(s);
			
		}
	
	}
	
	auto pWin = GetVtkWindow(VIEW_3D);

	pWin->Render();
}

void DVManager::ShowBone()
{
	GetRenderer(VIEW_3D)->RemoveActor(m_pActorBone);
	m_pActorBone = nullptr;

	

	//m_bShowBone = !m_bShowBone;
	// Set the colors.	
	C_VTK(vtkNamedColors, colors);

	

	if (m_bShowBone)
	{
		// Volume ������ �˻�
		vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
		if (volumeData == NULL) return;

		
		C_VTK(vtkMarchingCubes, boneExtractor);
		boneExtractor->SetInputData(volumeData->GetImageData());
		boneExtractor->SetValue(0, m_fThresholdBone);

		C_VTK(vtkStripper, boneStripper);
		boneStripper->SetInputConnection(boneExtractor->GetOutputPort());

		C_VTK(vtkPolyDataMapper, boneMapper);
		boneMapper->SetInputConnection(boneStripper->GetOutputPort());
		boneMapper->ScalarVisibilityOff();

		m_pActorBone = vtkSP<vtkActor>::New();
		m_pActorBone->SetMapper(boneMapper);
		m_pActorBone->GetProperty()->SetDiffuseColor(colors->GetColor3d("Ivory").GetData());

		GetRenderer(VIEW_3D)->AddActor(m_pActorBone);
	}

	GetVtkWindow(VIEW_3D)->Render();

}

void DVManager::ShowSkin()
{
	GetRenderer(VIEW_3D)->RemoveActor(m_pActorSkin);
	m_pActorSkin = nullptr;

	
	C_VTK(vtkNamedColors, colors);

	std::array<unsigned char, 4> skinColor{ { 255, 125, 64 } };
	colors->SetColor("SkinColor", skinColor.data());


	if (m_bShowSkin)
	{
		// Volume ������ �˻�
		vtkSP<VolumeData> volumeData = GetDicomLoader()->GetVolumeData();
		if (volumeData == NULL) return;

		C_VTK(vtkMarchingCubes, skinExtractor);
		skinExtractor->SetInputData(volumeData->GetImageData());
		skinExtractor->SetValue(0, m_fThresholdSkin);

		C_VTK(vtkStripper, skinStripper);
		skinStripper->SetInputConnection(skinExtractor->GetOutputPort());

		C_VTK(vtkPolyDataMapper, skinMapper);
		skinMapper->SetInputConnection(skinStripper->GetOutputPort());
		skinMapper->ScalarVisibilityOff();

		m_pActorSkin = vtkSP<vtkActor>::New();
		m_pActorSkin->SetMapper(skinMapper);
		m_pActorSkin->GetProperty()->SetDiffuseColor(colors->GetColor3d("SkinColor").GetData());
		m_pActorSkin->GetProperty()->SetSpecular(.3);
		m_pActorSkin->GetProperty()->SetSpecularPower(20);
		m_pActorSkin->GetProperty()->SetOpacity(.5);

		GetRenderer(VIEW_3D)->AddActor(m_pActorSkin);
	}
	GetVtkWindow(VIEW_3D)->Render();
}
