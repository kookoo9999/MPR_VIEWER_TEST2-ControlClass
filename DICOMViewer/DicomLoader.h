#pragma once

#include <vector>
#include <string>

#include <vtkObject.h>
#include <vtkSmartPointer.h>

#include "DicomGroup.h"
#include "VolumeData.h"

class DicomLoader :
	public vtkObject
{
public:
	vtkTypeMacro( DicomLoader, vtkObject );
	static DicomLoader *New() { return new DicomLoader; };

protected:
	DicomLoader();
	virtual ~DicomLoader();

protected:
	/// DICOM �׷� ���
	std::vector<vtkSmartPointer<DicomGroup>> m_GroupList;

	/// ���� ���õ� DICOM Group
	vtkSP<DicomGroup> m_CurrentGroup;

	/// ���� ���õ� Volume ������
	vtkSP<VolumeData> m_VolumeData;

public:
	/// DICOM ���丮 ����
	void OpenDicomDirectory( const char* dirPath );

	/// DICOM ���� �߰�
	void AddDicomFile( const char *filePath );

	/// DICOM �׷� ����
	int GetNumberOfGroups() { return (int)m_GroupList.size(); }

	/// DICOM �׷�
	vtkSP<DicomGroup> GetDicomGroup( int idx );

	/// ���� ���õ� DICOM Group
	vtkSP<DicomGroup> GetCurrentGroup() const { return m_CurrentGroup; }

	/// Volume ������
	vtkSP<VolumeData> GetVolumeData() const { return m_VolumeData; }

	/// DICOM �׷쿡�� Volume ������ �ε�
	void LoadVolumeData( vtkSP<DicomGroup> dicomGroup );
};
