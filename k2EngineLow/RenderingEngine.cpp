#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {
	RenderingEngine::RenderingEngine()
	{
	}

	RenderingEngine::~RenderingEngine()
	{
	}

	//�o�^����
	void RenderingEngine::Init()
	{
		//�t���[���o�b�t�@�[�̉����A�������擾
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		float clearColor[4] = { 0.5f,0.5f,0.5f,1.0f };

		//���C�������_�����O�^�[�Q�b�g
		m_mainRenderingTarget.Create(
			frameBuffer_w,	//�e�N�X�`���̕�
			frameBuffer_h,	//�e�N�X�`���̍���
			1,											//Mipmap���x��
			1,											//�e�N�X�`���z��̃T�C�Y
			DXGI_FORMAT_R32G32B32A32_FLOAT,				//�J���[�o�b�t�@�̃t�H�[�}�b�g
			DXGI_FORMAT_D32_FLOAT,						//�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g
			clearColor
		);

		//�t���[���o�b�t�@�[�Ƀe�N�X�`����\��t���邽�߂̃X�v���C�g��������
		//�������I�u�W�F�N�g���쐬
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_mainRenderingTarget.GetRenderTargetTexture();
		spriteInitData.m_width = frameBuffer_w;
		spriteInitData.m_height = frameBuffer_h;
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		m_copyToFrameBufferSprite.Init(spriteInitData);

		m_bloom.InitRenderTarget(m_mainRenderingTarget);

		//2D(�t�H���g��X�v���C�g)�p�̏�����
		Init2DSprite();

		//�u���[���̏�����
		InitBloom();

		//�V���h�E�̂��߂̏�����
		InitShadowMap();
	}

	void RenderingEngine::InitShadowMap()
	{
		m_shadow.Init();
	}

	void RenderingEngine::InitBloom()
	{
		m_bloom.InitLumi(m_mainRenderingTarget);
		m_bloom.InitGaussBlur();
		m_bloom.InitBoke(m_mainRenderingTarget);
	}

	void RenderingEngine::Init2DSprite()
	{
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
		//2D�p�̃^�[�Q�b�g�̏�����
		m_2DRenderTarget.Create(
			m_mainRenderingTarget.GetWidth(),
			m_mainRenderingTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//�ŏI�����p�̃X�v���C�g������������
		SpriteInitData spriteInitData;
		//�e�N�X�`����2D�����_�[�^�[�Q�b�g
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		//�𑜓x��mainRenderTarget�̕��ƍ���
		spriteInitData.m_width = m_mainRenderingTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderingTarget.GetHeight();
		//2D�p�V�F�[�_�[���g�p����
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		//�㏑��
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderingTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		//�e�N�X�`���̓��C�������_�[�^�[�Q�b�g
		spriteInitData.m_textures[0] = &m_mainRenderingTarget.GetRenderTargetTexture();

		//�𑜓x��2D�����_�[�^�[�Q�b�g�̕��ƍ���
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

		m_mainSprite.Init(spriteInitData);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//�e�̕`��
		m_shadow.Render(rc, m_renderObjects);
		//PreRender2D(rc);
		//�����_�����O�^�[�Q�b�g�����C�������_�����O�^�[�Q�b�g�ɕύX
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderingTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
		//�����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_mainRenderingTarget);

		//���f���̕`��
		ModelDraw(rc);
		//�G�t�F�N�g�̕`��
		EffectEngine::GetInstance()->Draw();
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);

		//�摜�ƕ����̕`��
		SpriteFontDraw(rc);

		//�u���[��
		//�P�x���o
		m_bloom.RenderLumi(rc);
		//�{�P�摜�𐶐�
		m_bloom.RenderGauss(rc);
		//�{�P�摜�����Z����
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderingTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
		//�ŏI����
		m_bloom.FinalSpriteDraw(rc);
		//�����_�����O�^�[�Q�b�g�̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);

		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[
		CopyMainRenderTargetToFrameBuffer(rc);

		Render2DSprite(rc);
		//�o�^����Ă���`��I�u�W�F�N�g���N���A
		m_renderObjects.clear();
	}

	void RenderingEngine::ModelDraw(RenderContext& rc)
	{
		for (auto& renderObj : m_renderObjects)
		{
			renderObj->OnRenderModel(rc);
		}
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		//�t���[���o�b�t�@���Z�b�g
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
		//bloom.GetCopyToFrameBuffer().Draw(rc);
		m_copyToFrameBufferSprite.Draw(rc);
	}

	void RenderingEngine::SpriteFontDraw(RenderContext& rc)
	{
		//2D�p�̃^�[�Q�b�g���g����悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);
		//�^�[�Q�b�g�Z�b�g
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);
		//�^�[�Q�b�g�̃N���A
		rc.ClearRenderTargetView(m_2DRenderTarget);

		m_mainSprite.Draw(rc);

		m_2DSprite.Draw(rc);

		//�`�悳���܂ő҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);
		//�^�[�Q�b�g�����C���ɖ߂�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderingTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
		//���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);
	}

	//2D�`�揈��
	void RenderingEngine::Render2DSprite(RenderContext& rc)
	{
		for (auto& renderObj : m_renderObjects)
		{
			renderObj->OnRender2D(rc);
		}
	}
}