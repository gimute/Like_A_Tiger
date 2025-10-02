#include "k2EngineLowPreCompile.h"
#include "k2EngineLow.h"
#include "SceneLight.h"

namespace nsK2EngineLow {
	SceneLight::SceneLight()
	{

	}

	void SceneLight::Init()
	{

		InitDirectionLight();
		//InitPointLight();
		//InitSpotLight();
		InitAmbientLight();
		InitHemisphereLight();

		//�e�`��p�̃��C�g�J�������쐬����
		Camera lightCamera;
		//�J�����̈ʒu��ݒ�A����̓��C�g�̈ʒu
		lightCamera.SetPosition(0, 600, 300);
		//�J�����̒����_��ݒ�A����̓��C�g���Ƃ炵�Ă���ꏊ
		lightCamera.SetTarget(0, 0, 0);
		//�������ݒ�A����̓��C�g���^���Ɍ����Ă���̂ŁAX��������ɂ��Ă���
		lightCamera.SetUp(0, 1, 0);
		//��p�����߂ɂ��Ă���
		//lightCamera.SetViewAngle(Math::DegToRad(80.0f));
		//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
		lightCamera.Update();

		m_light.mLVP = lightCamera.GetViewProjectionMatrix();



		//m_light.directionalLight[0].castShadow = true;


		////���ׂẴ|�C���g���C�g�𖢎g�p�ɂ���B
		//for (auto& pt : m_light.pointLights) {
		//	pt.UnUse();
		//	pt.SetAffectPowParam(1.0f);
		//}
		//m_light.numPointLight = 0;

		//
	}

	void SceneLight::Update()
	{
		m_light.mLVP = g_renderingEngine->GetLigCameraViewProjection();

		//DirRot();
	}

	void SceneLight::InitDirectionLight()
	{
		//���z��
		//�f�B���N�V�������C�g�̃f�[�^���쐬����
		//���C�g�͎΂ߏォ�瓖�����Ă���
		m_light.dirDirection.x = 1.0f;
		m_light.dirDirection.y = -1.0f;
		m_light.dirDirection.z = -1.0f;

		//���K������
		m_light.dirDirection.Normalize();

		//���C�g�̃J���[�͊D�F(0.6f),bloom(5.8f)
		//���C�g�����߂ɐݒ�
		m_light.color.x = 0.6f;
		m_light.color.y = 0.6f;
		m_light.color.z = 0.6f;

		//���_
		m_light.eyePos = g_camera3D->GetPosition();
		//�X�y�L�����̍i��
		m_light.specPow = 5.0f;
	}

	void SceneLight::InitPointLight()
	{
		//�|�C���g���C�g
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 50.0f;

		m_light.ptColor.x = 15.0f; //���Ƃ͂P�T
		m_light.ptColor.y = 0.0f;
		m_light.ptColor.z = 0.0f;


		m_light.ptRange = 100.0f;
	}

	void SceneLight::InitSpotLight()
	{
		//�X�|�b�g���C�g
		//�������W
		m_light.spPosition.x = 0.0f;
		m_light.spPosition.y = 50.0f;
		m_light.spPosition.z = 0.0f;
		//���C�g�̃J���[
		m_light.spColor.x = 10.0f;
		m_light.spColor.y = 10.0f;
		m_light.spColor.z = 10.0f;
		//��������
		m_light.spDirection.x = 1.0f;
		m_light.spDirection.y = -1.0f;
		m_light.spDirection.z = 1.0f;
		//�����f�[�^�𐳋K��
		m_light.spDirection.Normalize();
		//�ˏo�͈�
		m_light.spRange = 300.0f;
		//�ˏo�p�x
		m_light.spAngle = Math::DegToRad(25.0f);

	}

	void SceneLight::InitAmbientLight()
	{
		//����
		m_light.ambientLight.x = 0.4f;
		m_light.ambientLight.y = 0.4f;
		m_light.ambientLight.z = 0.4f;
	}

	void SceneLight::InitHemisphereLight()
	{
		//�n�ʐF
		m_light.groundColor.x = 0.7f;
		m_light.groundColor.y = 0.5f;
		m_light.groundColor.z = 0.3f;

		//�V���F
		m_light.skyColor.x = 0.15f;
		m_light.skyColor.y = 0.7f;
		m_light.skyColor.z = 0.95f;

		//�n�ʂ̖@����ݒ�
		m_light.groundNormal.x = 0.0f;
		m_light.groundNormal.y = 1.0f;
		m_light.groundNormal.z = 0.0f;
	}

	void SceneLight::DirRot()
	{
		//�f�B���N�V�������C�g����
		Quaternion qRotY;
		qRotY.SetRotation(g_vec3AxisY, g_pad[0]->GetLStickXF() * 0.02f);
		qRotY.Apply(m_light.dirDirection);
	}

	/*void SceneLight::Update()
	{
		m_light.numPointLight = 0;
		for (int i = 0; i < MAX_POINT_LIGHT; i++)
		{
			if (m_light.pointLights[i].GetUse())
			{
				m_light.numPointLight++;
			}
		}
	}*/

	/*SPointLight* SceneLight::NewPointLight()
	{
		if (m_light.numPointLight >= MAX_POINT_LIGHT)
		{
			return nullptr;
		}

		for (int i = 0; i < MAX_POINT_LIGHT; i++)
		{
			if (m_light.pointLights[i].GetUse() != true)
			{
				m_light.pointLights[i].Use();
				return &m_light.pointLights[i];
			}
		}
	}*/

	//void SceneLight::DeletePointLight(SPointLight* m_pointLight)
	//{
	//	//�|�C���^�ɃA�h���X�������Ă��Ȃ������牽�������ɕԂ�
	//	if (m_pointLight == nullptr)
	//	{
	//		return;
	//	}
	//	//������A�V�[�����C�g���Ǘ����Ă��Ȃ��|�C���g���C�g�ɑ΂��č폜���������s�����
	//	//�Ǘ����o�N��i���Ɏg�p���̃��C�g�̐��j�̂Ŗ{���ɍ폜���������Ă������m�F����B
	//	//
	//	//�����Ă����|�C���^�ɓ����Ă���A�h���X�ƁA�z��̊e�v�f�̃A�h���X���Ƃ炵���킹�āA
	//	//�A�h���X���������̂���������A�폜���������s����
	//	for (int i = 0; i < MAX_POINT_LIGHT; i++)
	//	{
	//		if (m_pointLight == &m_light.pointLights[i])
	//		{
	//			m_pointLight->UnUse();
	//			m_pointLight->SetColor(0.0f, 0.0f, 0.0f);
	//			m_pointLight->SetPosition(0.0f, 0.0f, 0.0f);
	//			m_pointLight->SetRange(0.0f);
	//			m_pointLight->SetAffectPowParam(1.0f);
	//		}
	//	}
	//}


}