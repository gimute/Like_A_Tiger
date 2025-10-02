#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow {

		void Bloom::InitRenderTarget(RenderTarget& renderTarget)
		{
			luminanceRenderTarget.Create(
				renderTarget.GetWidth(),
				renderTarget.GetHeight(),
				1,
				1,
				renderTarget.GetColorBufferFormat(),
				DXGI_FORMAT_D32_FLOAT
			);
		}


		void Bloom::InitLumi(RenderTarget& mainRenderTarget)
		{
			//�P�x���o�p�̃V�F�[�_�[
			luminanceSpriteInitData.m_fxFilePath = "Assets/Shader/postEffect.fx";
			//���_�V�F�[�_�[�̃t�@�C���p�X���w�肷��
			luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
			//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
			luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
			//�X�v���C�g�̕��ƍ����� luminnceRenderTarget �Ɠ���
			luminanceSpriteInitData.m_width = luminanceRenderTarget.GetWidth();
			luminanceSpriteInitData.m_height = luminanceRenderTarget.GetHeight();
			//�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@
			luminanceSpriteInitData.m_textures[0]
				= &mainRenderTarget.GetRenderTargetTexture();
			//�������ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
			luminanceSpriteInitData.m_colorBufferFormat[0]
				= DXGI_FORMAT_R32G32B32A32_FLOAT;
			//�쐬�����������������ƂɃX�v���C�g������������
			luminanceSprite.Init(luminanceSpriteInitData);
		}


		void Bloom::InitGaussBlur()
		{
			//gaussianBlur[0]�͋P�x�e�N�X�`���ɃK�E�X�A���u���[��������
			gaussianBlur[0].Init(&luminanceRenderTarget.GetRenderTargetTexture());
			//gaussianBlur[1]�� gaussianBlur[0]�̃e�N�X�`���ɃK�E�X�A���u���[��������
			gaussianBlur[1].Init(&gaussianBlur[0].GetBokeTexture());
			//gaussianBlur[2]�� gaussianBlur[1]�̃e�N�X�`���ɃK�E�X�A���u���[��������
			gaussianBlur[2].Init(&gaussianBlur[1].GetBokeTexture());
			//gaussianBlur[3]�� gaussianBlur[2]�̃e�N�X�`���ɃK�E�X�A���u���[��������
			gaussianBlur[3].Init(&gaussianBlur[2].GetBokeTexture());
		}

		void Bloom::InitBoke(RenderTarget& mainRenderTarget)
		{
			//�{�P�e�N�X�`����4���w��
			finalSpriteInitData.m_textures[0] = &gaussianBlur[0].GetBokeTexture();
			finalSpriteInitData.m_textures[1] = &gaussianBlur[1].GetBokeTexture();
			finalSpriteInitData.m_textures[2] = &gaussianBlur[2].GetBokeTexture();
			finalSpriteInitData.m_textures[3] = &gaussianBlur[3].GetBokeTexture();

			//�𑜓x�� mainRenderTarget�̕��ƍ���
			finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
			finalSpriteInitData.m_height = mainRenderTarget.GetHeight();

			//�ڂ������摜���A�ʏ��2D�Ƃ��ă��C�������_�����O�^�[�Q�b�g�̕`�悷��̂�
			//2D�p�̃V�F�[�_�[���g�p����
			//finalSpriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";

			//�{�P�摜����������K�v������̂ŁA2D�p�̃V�F�[�_�[�ł͂Ȃ��A
			//��p�V�F�[�_�[���w��
			finalSpriteInitData.m_fxFilePath = "Assets/Shader/postEffect.fx";
			finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

			//�������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z����
			finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;

			//�J���[�o�b�t�@�[�̃t�H�[�}�b�g�͗�ɂ���āA32�r�b�g���������_�o�b�t�@�[
			finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

			//�������������ɉ��Z�����p�̃X�v���C�g��������
			finalSprite.Init(finalSpriteInitData);

			//�e�N�X�`���� mainRenderTarget �̃J���[�o�b�t�@�[
			//spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
			//spriteInitData.m_width = 1280;
			//spriteInitData.m_height = 720;

			//���m�N���p�̃V�F�[�_�[���w�肷��
			//spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";

			//�������I�u�W�F�N�g���g���āA�X�v���C�g��������
			//copyToFrameBufferSprite.Init(spriteInitData);
		}

		void Bloom::RenderLumi(RenderContext& rc)
		{
			//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
			rc.WaitUntilToPossibleSetRenderTarget(luminanceRenderTarget);
			//�����_�����O�^�[�Q�b�g��ݒ�
			rc.SetRenderTargetAndViewport(luminanceRenderTarget);
			//�����_�����O�^�[�Q�b�g���N���A
			rc.ClearRenderTargetView(luminanceRenderTarget);
			//�P�x���o���s��
			luminanceSprite.Draw(rc);

			//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
			rc.WaitUntilFinishDrawingToRenderTarget(luminanceRenderTarget);
		}

		void Bloom::RenderGauss(RenderContext& rc)
		{
			gaussianBlur[0].ExecuteOnGPU(rc, 10);
			gaussianBlur[1].ExecuteOnGPU(rc, 10);
			gaussianBlur[2].ExecuteOnGPU(rc, 10);
			gaussianBlur[3].ExecuteOnGPU(rc, 10);
		}

		//void Bloom::RenderBoke(RenderContext& rc, RenderTarget& mainRenderTarget)
		//{
		//	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		//	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//	//�����_�����O�^�[�Q�b�g��ݒ�
		//	rc.SetRenderTargetAndViewport(mainRenderTarget);
		//	//�ŏI����
		//	finalSprite.Draw(rc);
		//	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		//	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
		//}


}
