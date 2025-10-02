#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

namespace nsK2EngineLow {
	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
	{
		SpriteInitData initData;
		//DDS�t�@�C���i�摜�f�[�^�j�̃t�@�C���p�X���w�肷��
		initData.m_ddsFilePath[0] = filePath;
		//Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
		initData.m_fxFilePath = "Assets/Shader/sprite.fx";
		//�X�v���C�g�̕��ƍ������w�肷��
		initData.m_width = static_cast<UINT>(w);
		initData.m_height = static_cast<UINT>(h);
		initData.m_alphaBlendMode = alphaBlendMode;

		//initData.m_vsEntryPointFunc = "VSMain";

		//initData.m_psEntryPoinFunc = "PSMain";

		//Sprite�������I�u�W�F�N�g���g�p���āASprite������������
		m_sprite.Init(initData);
	}

	/*void SpriteRender::SetTexture(const std::string& filePath)
	{
		if (m_texture.InitFromMemory(filePath.c_str())) {

		}
	}*/

	void SpriteRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddSpriteRenderObject(this);
	}
}
