#pragma once
#pragma once

namespace nsK2EngineLow {
	/// <summary>
	/// �����_���[�̊��N���X�B
	/// </summary>
	class IRenderer : public Noncopyable {
	public:
		//���f���̕`��
		virtual void OnRenderModel(RenderContext& rc)
		{

		}

		//�V���h�E�}�b�v�ւ̕`�揈��
		virtual void OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix)
		{

		}

		//2D�`��
		virtual void OnRender2D(RenderContext& rc)
		{

		}
	};

}

