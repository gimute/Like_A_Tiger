#include "stdafx.h"
#include "PoseMenu.h"

PoseMenu::PoseMenu()
{
	m_image = m_canvas.CreateUI<UIImage>();
}


PoseMenu::~PoseMenu()
{
}


bool PoseMenu::Start() {
	return true;
}


void PoseMenu::Update() {
	m_canvas.Update();

}


void PoseMenu::Render(RenderContext& rc) {
	/**
	 * ポーズ画面、画面上に出てるので
	 * 非表示にしています。
	 */
	// m_canvas.Render(rc);
}

void PoseMenu::Init(const PoseMenuInitData* initData)
{
	if (initData == nullptr) {
		//設定されていなければ仮画像で初期化
		Init(&defaultPoseMenu);
		return;
	}

	m_image->Init(initData->frameFilePath, initData->frameSize_x, initData->frameSize_y);
}
