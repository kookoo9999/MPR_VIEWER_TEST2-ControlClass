
#pragma once

#include <iostream>

#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkActor.h>
#include <vtkRenderer.h>

#include "stdafx.h"
#include "VolumeData.h"

using namespace std;

class vtkRenderer;

// Thresholding, Rotate, Plane View Control ����� ����ϴ� Ŭ����
// �Է� : DICOM ����, ��� Renderer
// ��� : �Ķ���� ���� �� Actor ������Ʈ (Ŭ���� ���ο���)
class CControlManager
{
public:

	// �Է� ����� Target Renderer �� �����޴´�.
	CControlManager(vtkImageData* pImage = nullptr, vtkRenderer* pRenderer = nullptr);

	//Outline Visibility�� ����
	void SetLineOnOff(bool bFlag) { m_bShowOutline = bFlag; }
	bool GetLineOnOff() const { return m_bShowOutline; }

	// Skin Visibility, Threshold �� ����
	void SetSkinOnOff(bool bFlag) { m_bShowSkin = bFlag; }
	bool GetSkinOnOff() const { return m_bShowSkin; }
	void SetSkinThreshold(double value) { m_fThresholdSkin = value; }
	double GetSkinThreshold() const { return m_fThresholdSkin; }

	// Bone Visibility, Threshold �� ����
	void SetBoneOnOff(bool bFlag) { m_bShowBone = bFlag; }
	bool GetBoneOnOff() const { return m_bShowBone; }
	void SetBoneThreshold(double value) { m_fThresholdBone = value; }
	double GetBoneThreshold() const { return m_fThresholdBone; }

	// �������� ȸ�� �Ķ���͸� ����
	void SetRotationRX(double value) { m_fRx = value; }
	double GetRotationRX() const { return m_fRx; }
	void SetRotationRY(double value) { m_fRy = value; }
	double GetRotationRY() const { return m_fRy; }
	void SetRotationRZ(double value) { m_fRz = value; }
	double GetRotationRZ() const { return m_fRz; }

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

	// ������ ȸ�� ������ �ݿ��Ѵ�.
	void RotateUpdate();

	// ������ �����Ͽ� �������� ����Ѵ�.
	void Update();
	
private:

	// Source Image
	vtkImageData* m_pImage;

	vtkSP<vtkImageData> m_pAlignedData;

	// Target Renderer
	vtkRenderer* m_pRenderer;

	// ������ Outline
	vtkSP<vtkActor> m_pActorOutline;

	// Skin, Bone Threshold ��� ������ Actor
	vtkSP<vtkActor> m_pActorSkin;
	vtkSP<vtkActor> m_pActorBone;

	//outline Visibility ���� ��
	bool m_bShowOutline;

	// Skin, Bone Visibility ���� ��
	bool m_bShowSkin;
	bool m_bShowBone;

	// Skin, Bone Threshold ���� ��
	double m_fThresholdSkin;
	double m_fThresholdBone;

	// Rotation ���� ��
	double m_fRx, m_fRy, m_fRz;

	// Sagittal, Coronal, Axial View Plane Actor
	std::vector<vtkSP<vtkImageActor>> m_pActorSCAPlane;

	// Sagittal, Coronal, Axial Plane Position
	int m_nSagittalPos, m_nCoronalPos, m_nAxialPos;

	// Sagittal, Coronal, Axial Plane Maximum Position
	int m_nSagittalMax, m_nCoronalMax, m_nAxialMax;

	// Sagittal, Coronal, Axial Plane Visibility ���� ��
	bool m_bShowPlane;

};
