#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()
	{
	}

	ModelRender::~ModelRender()
	{
	}

	void ModelRender::Init(
		const char* tkmFilePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAcxis,
		bool isShadowCaster,
		bool isShadowReceiver)
	{
		//�X�P���g����������
		InitSkeleton(tkmFilePath);
		//�A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationClips, enModelUpAcxis);
		//���f���̏�����
		ModelInitData initData;
		Camera lightCamera;

		initData.m_tkmFilePath = tkmFilePath;
		//�V���h�E���V�[�o�[�i�e�����Ƃ���郂�f���j�p�̃V�F�[�_�[���w�肷��
		initData.m_fxFilePath = "Assets/shader/model.fx";
		//initData.m_fxFilePath = "Assets/shader/shadowReceiverModel.fx";

		lightCamera.SetAspectOnrFlag(true);
		lightCamera.SetViewAngle(Math::DegToRad(80.0f));
		lightCamera.SetPosition(-6000.0f, 500.0f, -500.0f);
		lightCamera.SetTarget(0, 0, 0);
		lightCamera.Update();

		m_light.m_mt = lightCamera.GetViewProjectionMatrix();
		initData.m_expandConstantBuffer = &g_sceneLight->GetLightData();

		initData.m_expandConstantBufferSize = sizeof(g_sceneLight->GetLightData());
	
		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		initData.m_vsEntryPointFunc = "VSMain";
	
		//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";
		
		if (isShadowCaster == true)
		{
			InitShadowModel(tkmFilePath, enModelUpAcxis);
		}

		if (isShadowReceiver == true)
		{
			initData.m_psEntryPointFunc = "PSShadowReceverMain";

			initData.m_expandShaderResoruceView[0] =
				&(g_renderingEngine->GetShadowMap().GetRenderTargetTexture());
		}
		else
		{
			initData.m_psEntryPointFunc = "PSNormalMain";
		}
		if (animationClips != nullptr) {
			initData.m_skeleton = &m_skeleton;
		}

		m_model.Init(initData);
	}

	void ModelRender::InitShadowModel(const char* tkmFilePath, EnModelUpAxis modelUpAxis)
	{
		ModelInitData shadowInitData;
		shadowInitData.m_tkmFilePath = tkmFilePath;
		//shadowInitData.m_modelUpAxis = modelUpAxis;
		shadowInitData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
		shadowInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

		//�A�j���[�V����������Ȃ�X�P���g�����w�肷��
		if (m_animationClips != nullptr)
		{
			shadowInitData.m_skeleton = &m_skeleton;
		}
	
		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		shadowInitData.m_vsEntryPointFunc = "VSMain";
		
		//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
		shadowInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		m_shadowModel.Init(shadowInitData);
	}

	void ModelRender::InitSkeleton(const char* tkmFilePath)
	{
		//�X�P���g���̃f�[�^��ǂݍ��݁B
		std::string skeletonFilePath = tkmFilePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips,EnModelUpAxis EnModelUpAxis)
	{
		//�A�j���[�V�����̐ݒ�
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr)
		{
			m_animation.Init(
				m_skeleton,
				m_animationClips,
				numAnimationClips
			);
		}
	}

	/*void ModelRender::InitComputeAnimationVertexBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis)
	{
		StructuredBuffer* worldMatrxiArraySB = nullptr;
		if (m_isEnableInstancingDraw) {
			worldMatrxiArraySB = &m_worldMatrixArraySB;
		}

		m_computeAnimationVertexBuffer.Init(
			tkmFilePath,
			m_skeleton.GetNumBones(),
			m_skeleton.GetBoneMatricesTopAddress(),
			enModelUpAxis,
			m_maxInstance,
			worldMatrxiArraySB
		);
	}*/

	void ModelRender::Update()
	{
		//���f���̃��[���h�s��X�V
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		m_skeleton.Update(m_model.GetWorldMatrix());
		//�A�j���[�V������i�߂�
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	//void ModelRender::UpdateInstancingData(int instanceNo, const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	//{
	//	K2_ASSERT(instanceNo < m_maxInstance, "�C���X�^���X�ԍ����s���ł��B");
	//	if (!m_isEnableInstancingDraw) {
	//		return;
	//	}
	//	Matrix worldMatrix;
	//	if (m_translucentModel.IsInited()) {
	//		// ���������f����ZPrepass���f�������������Ă��Ȃ��̂ŁA��������g���B
	//		worldMatrix = m_translucentModel.CalcWorldMatrix(pos, rot, scale);
	//	}
	//	else {
	//		worldMatrix = m_zprepassModel.CalcWorldMatrix(pos, rot, scale);
	//	}
	//	// �C���X�^���X�ԍ�����s��̃C���f�b�N�X���擾����B
	//	int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];
	//	// �C���X�^���V���O�`����s���B
	//	m_worldMatrixArray[matrixArrayIndex] = worldMatrix;
	//	if (m_numInstance == 0) {
	//		//�C���X�^���X����0�̏ꍇ�̂݃A�j���[�V�����֌W�̍X�V���s���B
	//		// �X�P���g�����X�V�B
	//		// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
	//		// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
	//		// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
	//		m_skeleton.Update(g_matIdentity);
	//		//�A�j���[�V������i�߂�B
	//		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	//	}
	//	m_numInstance++;
	//}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddRenderObject(this);
	}

	void ModelRender::OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix)
	{
		m_shadowModel.Draw(
			rc,
			g_matIdentity,
			lvpMatrix,
			1
		);
	}

	void ModelRender::OnRenderModel(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}