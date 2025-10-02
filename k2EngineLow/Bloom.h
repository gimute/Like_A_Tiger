#pragma once
#include "graphics/GaussianBlur.h"


namespace nsK2EngineLow {
	/// <summary>
	/// �u���[���B
	/// </summary>
	class Bloom : public Noncopyable
	{
	public:
		
		
		/// <summary>
		/// �|�X�g�G�t�F�N�g�����s�������ʂƂȂ�e�N�X�`�����擾
		/// </summary>
		/// <returns></returns>
		Texture& GetResultTexture()
		{
			return luminanceRenderTarget.GetRenderTargetTexture();
		}

		RenderTarget& GetRenderTarget()
		{
			return luminanceRenderTarget;
		}

		Sprite& GetCopyToFrameBuffer()
		{
			return copyToFrameBufferSprite;
		}
		

		/// <summary>
		/// �P�x���o�p�̃X�v���C�g��������
		/// </summary>
		void InitLumi(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �P�x���o�p�̃����_�����O�^�[�Q�b�g
		/// </summary>
		/// <param name="rc"></param>
		void InitRenderTarget(RenderTarget& rc);
		/// <summary>
		/// �K�E�V�A���u���[��������
		/// </summary>
		void InitGaussBlur();
		/// <summary>
		/// �{�P�摜�����Z��������X�v���C�g��������
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void InitBoke(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �P�x���o�p�̃X�v���C�g��`��
		/// </summary>
		/// <param name="rc"></param>
		void RenderLumi(RenderContext& rc);
		/// <summary>
		/// �K�E�V�A���u���[�����s
		/// </summary>
		/// <param name="rc"></param>
		void RenderGauss(RenderContext& rc);
		/// <summary>
		/// �{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void RenderBoke(RenderContext& rc, RenderTarget& mainRenderTarget);

		void FinalSpriteDraw(RenderContext& rc)
		{
			finalSprite.Draw(rc);
		}

	private:
		//RenderTarget* mainRenderTarget;

		RenderTarget luminanceRenderTarget;	//�P�x���o�p�̃����_�����O�^�[�Q�b�g
		SpriteInitData luminanceSpriteInitData; //�P�x���o�p�̃X�v���C�g��������
		Sprite luminanceSprite;				

		GaussianBlur gaussianBlur[4];			//�K�E�V�A���u���[

		SpriteInitData finalSpriteInitData; //�{�P�摜�����Z��������X�v���C�g��������
		Sprite finalSprite;					//���Z�����p�̃X�v���C�g��������
		//SpriteInitData spriteInitData;		//�X�v���C�g�̏������I�u�W�F�N�g
		Sprite copyToFrameBufferSprite;		//�t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g��������
	};
}


