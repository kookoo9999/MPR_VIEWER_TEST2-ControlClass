#pragma once

#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageReslice.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageProperty.h>
#include <vtkMarchingCubes.h>
#include <vtkStripper.h>
#include <vtkPolyDataMapper.h>

class VolumeData :
	public vtkObject
{
public:
	/// vtkSmartPointer�� ���� ���� �� ����
	vtkTypeMacro( VolumeData, vtkObject );
	static VolumeData *New() { return new VolumeData; };

protected:
	// New() �Լ� �̿��� ���� ����
	VolumeData();
	virtual ~VolumeData();
	
public:
	// Volume Slice Ÿ��
	enum SliceType { AXIAL, CORONAL, SAGITTAL };

	// �̸� ���ǵ� Volume ������ ���� �Լ� �� �÷� �Լ�
	enum RenderingPreset { MIP, BONE, SKIN, NONE };

protected:
	// VTK 3���� �̹��� (Volume) ������
	vtkSP<vtkImageData> m_ImageData;

	// Volume �������� ȸ���̵� ����
	vtkSP<vtkMatrix4x4> m_Orientation;

	// Volume ������ ��ü
	vtkSP<vtkVolume> m_VolumeRendering;
	
	// Volume ������ ���� �Լ�
	vtkSP<vtkPiecewiseFunction> m_OpacityFunc;

	// Volume ������ �÷� �Լ�
	vtkSP<vtkColorTransferFunction> m_ColorFunc;

	//skin Extractor
	vtkSP<vtkActor> m_pActorSKin;

	// ���� Volume ������ ���
	int m_CurrentPresetMode;

public:

	
	
	// Slice ������ ��ü (Axial, Coronal, Sagittal �����̽�)
	vtkSP<vtkImageActor> m_SliceActor[3];

	// Slice �������� ���� Volume �ܸ� 
	vtkSP<vtkImageReslice> m_VolumeSlice[3];

	// Slice ������ �Ӽ�
	vtkSP<vtkImageProperty> m_SliceProperty;

	// Slice �ε���
	int m_SliceIndex[3];

public:	
	// VTK Volume ������ ��������/�����ϱ�
	vtkSP<vtkImageData> GetImageData() const { return m_ImageData; }
	void SetImageData( vtkSP<vtkImageData> val ) { m_ImageData = val; }
	
	// Volume �������� ȸ���̵� ���� ��������/�����ϱ�
	vtkSP<vtkMatrix4x4> GetOrientation() const { return m_Orientation; }
	void SetOrientation( vtkSP<vtkMatrix4x4> val ) { m_Orientation = val; }

	

	// Volume ������ ��ü
	vtkSP<vtkVolume> GetVolumeRendering() const { return m_VolumeRendering; }

	// 3D Volume ������ �غ�
	void ReadyForVolumeRendering();
	
	// ���� Volume ������ ���
	int GetCurrentPresetMode() const { return m_CurrentPresetMode; }

	// Volume ������ ��� ����
	void SetCurrentPresetMode( int val );

	
	
	// Slice ������ ��ü
	vtkSP<vtkImageActor> GetSliceActor( int sliceType );

	// Slice ������ �غ�
	void ReadyForSliceRendering();

	

	// Slice Ÿ�԰� �ε����� ���� Reslice ��� ���
	vtkSP<vtkMatrix4x4> GetResliceMatrix( int sliceType, int sliceIdx );
	
	// ���� Slice �ε���
	int GetSliceIndex( int sliceType );

	// Slice �ε��� ����
	void SetSliceIndex( int sliceType, int sliceIndex );
};
