#pragma once

namespace nsK2EngineLow {
	
	static const int MAX_DIRECTIONAL_LIGHT = 4;
	static const int MAX_POINT_LIGHT = 32;
	static const int MAX_SPOT_LIGHT = 32;
	////�f�B���N�V�������C�g�̍\����
	//struct DirectionLight
	//{
	//	Vector3 direction;
	//	float pad0;
	//	//int castShadow = true;
	//	Vector3 color;
	//	float pad1;
	//};

	////�|�C���g���C�g�̍\����
	//struct SPointLight
	//{
	////private: 
	//	Vector3 position;		//���W
	//	int isUse = false;					//�g�p���t���O
	//	Vector3 color;			//���C�g�̃J���[
	//	float pad2;
	//	float range = 0.0f;	//�����p�����[�^�BX�ɉe���͈́AY�ɂ͉e�����ɗݏ悷��p�����[�^
	//	float pad3;
	//};

	////�X�|�b�g���C�g�̍\����
	//struct SSpotLight
	//{
	////private:
	//	Vector3 position = g_vec3Zero;	//���W
	//	int isUse = false;				//�g�p���t���O
	//	Vector3 color = g_vec3One;		//���C�g�̃J���[
	//	float range;					//�e���͈�
	//	Vector3 direction = g_vec3Down; //�ˏo����
	//	float angle;					//�ˏo�p�x
	//	Vector3 pow = { 1.0f,1.0f,0.0f };//�e�����ɗݏ悷��p�����[�^
	//									 //x�������ɂ��e�����ɏ�Z����p�����[�^
	//									 //y���p�x�ɂ��e�����ɏ�Z����p�����[�^
	//	float pad4;

	//
	//};

	//���C�g�̍\����
	struct Light
	{
		//�f�B���N�V�������C�g�p�̃����o�ϐ�
		Vector3 dirDirection; //���C�g�̕���
		float pad1;
		Vector3 color;		  //���C�g�̃J���[
		float pad2;

		Vector3 lightPos;
		float padlight;
		
		//DirectionLight directionalLight[MAX_DIRECTIONAL_LIGHT];	//�f�B���N�V�������C�g�̔z��
		//SPointLight pointLights[MAX_POINT_LIGHT];				//�|�C���g���C�g�̔z��[
		//DirectionLight directionalLight;
		//PointLight pointLight;
		
		//�|�C���g���C�g�p�̃����o�ϐ�
		Vector3 ptPosition;
		float pad3;
		Vector3 ptColor;
		float ptRange;

		//�X�|�b�g���C�g�p�̃����o�ϐ�
		Vector3 spPosition;	//�ʒu
		float pad4;
		Vector3 spColor;	//���C�g�̃J���[
		float spRange;		//�e���͈�
		Vector3 spDirection;//�ˏo����
		float spAngle;		//�ˏo�p�x


		Vector3 eyePos;		//���_�̈ʒu
		float specPow;		//�X�y�L�����̍i��
		Vector3 ambientLight;	//����
		float pad5;

		//�n�ʐF�ƓV���F�A�n�ʂ̖@����ǉ�
		Vector3 groundColor;	//�n�ʐF
		float pad7;
		Vector3 skyColor;		//�V���F
		float pad8;
		Vector3 groundNormal;	//�n�ʂ̖@��
		float pad9;

		Matrix mLVP;
	};

	//�V�[�����C�g�N���X
	class SceneLight : public Noncopyable
	{
	public:
		SceneLight();

		//������
		void Init();
		//�f�B���N�V�������C�g
		void InitDirectionLight();
		//�|�C���g���C�g
		void InitPointLight();
		//�X�|�b�g���C�g
		void InitSpotLight();
		//����
		void InitAmbientLight();
		//�������C�g
		void InitHemisphereLight();
		//�f�B���N�V�������C�g����
		void DirRot();
		//�V�[�����C�g���擾
		Light& GetSceneLight()
		{
			return m_light;
		}
		

		//�X�V����
		void Update();

		

		Light& GetLightData()
		{
			return m_light;
		}

	private:
		Light m_light;	//�V�[�����C�g
		Camera m_lightCamera;
		//Shadow m_shadow;
		//ModelInitData bgModelInitData;	//�e���󂯂�w�i��������
		//DirectionLight m_dirLight; //�f�B���N�V�������C�g
		//SPointLight m_pointLight; //�|�C���g���C�g
	};
}


