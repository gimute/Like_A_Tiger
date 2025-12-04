#include "stdafx.h"
#include "PoseMenu.h"

ItemPosePanel::ItemPosePanel(){
	m_image = m_canvas.CreateUI<UIImage>();
}

ItemPosePanel::~ItemPosePanel(){
}

bool ItemPosePanel::Start() {
	return true;
}


void ItemPosePanel::Update() {
	m_image->Update();
}


void ItemPosePanel::Render(RenderContext& rc) {
	m_image->Render(rc);
}

void ItemPosePanel::Init(const PosePanelInitData* initData){
	if (initData == nullptr) {
		//** 設定されていなければ仮画像で初期化 */
		Init(&defaultItemPosePanel);
		return;
	}
	//** アイテムパネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
}




ManualControlPosePanel::ManualControlPosePanel() {
	m_image = m_canvas.CreateUI<UIImage>();
}

ManualControlPosePanel::~ManualControlPosePanel() {
}


bool ManualControlPosePanel::Start() {
	return true;
}


void ManualControlPosePanel::Update() {
	m_canvas.Update();
}


void ManualControlPosePanel::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void ManualControlPosePanel::Init(const PosePanelInitData* initData) {
	if (initData == nullptr) {
		//** 設定されていなければ仮画像で初期化 */
		Init(&defaultManualControlPosePanel);
		return;
	}
	//** 操作説明パネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
}




SettingPosePanel::SettingPosePanel() {
	m_image = m_canvas.CreateUI<UIImage>();
}

SettingPosePanel::~SettingPosePanel() {
}

bool SettingPosePanel::Start() {
	return true;
}

void SettingPosePanel::Update() {
	m_canvas.Update();
}

void SettingPosePanel::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void SettingPosePanel::Init(const PosePanelInitData* initData) {
	if (initData == nullptr) {
		//** 設定されていなければ仮画像で初期化 */
		Init(&defaultSettingPosePanel);
		return;
	}
	/** 設定用パネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
}




CursolPosePanel::CursolPosePanel() {
	m_image = m_canvas.CreateUI<UIImage>();
}

CursolPosePanel::~CursolPosePanel() {
}

bool CursolPosePanel::Start() {
	return true;
}

void CursolPosePanel::Update() {

	if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_currentCursolIndex++;
		if (m_currentCursolIndex > m_maxCursolIndex){
			m_currentCursolIndex = 0;
		}
	}
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_currentCursolIndex--;
		if (m_currentCursolIndex < 0) {
			m_currentCursolIndex = m_maxCursolIndex;
		}
	}

	if (m_currentCursolIndex == 1) {
		//操作説明用の座標変更
		m_canvas.m_transform.m_localPosition.y = 90.0f;
	}
	else if(m_currentCursolIndex == 2) {
		//設定用の座標変更
		m_canvas.m_transform.m_localPosition.y = 15.0f;
	}
	else {
		//アイテム用の座標変更
		m_canvas.m_transform.m_localPosition = Vector3(-407.0f, 160.0f, 0.0f);
	}
	
	m_canvas.m_transform.UpdateTransform();
	m_canvas.Update();
}

void CursolPosePanel::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void CursolPosePanel::Init(const PosePanelInitData* initData) {
	if (initData == nullptr) {
		//** 設定されていなければ仮画像で初期化 */
		Init(&defaultCursolPosePanel);
		return;
	}
	//** カーソルパネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);

}




NameListPosePanel::NameListPosePanel() {
	m_image = m_canvas.CreateUI<UIImage>();
}

NameListPosePanel::~NameListPosePanel() {
}

bool NameListPosePanel::Start() {
	return true;
}

void NameListPosePanel::Update() {
	m_canvas.Update();
}

void NameListPosePanel::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void NameListPosePanel::Init(const PosePanelInitData* initData) {
	if (initData == nullptr) {
		//** 設定されていなければ仮画像で初期化 */
		Init(&defaultNameListPosePanel);
		return;
	}
	/** 名前リストパネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
}


PoseMenu::PoseMenu()
{
	m_image = m_canvas.CreateUI<UIImage>();
	std::vector<std::unique_ptr<PosePanelBase>> panel;

	m_posePanelList.push_back(
		std::move(std::make_unique<ItemPosePanel>())
	);
	m_posePanelList.push_back(
		std::move(std::make_unique<ManualControlPosePanel>())
	);
	m_posePanelList.push_back(
		std::move(std::make_unique<SettingPosePanel>())
	);
	m_posePanelList.push_back(
		std::move(std::make_unique<CursolPosePanel>())
	);
	m_posePanelList.push_back(
		std::move(std::make_unique<NameListPosePanel>())
	);
}


PoseMenu::~PoseMenu(){
}


bool PoseMenu::Start() {
	return true;
}


void PoseMenu::Update() {
	m_canvas.Update();
	for (const auto& p : m_posePanelList) {
		p->Update();
	}
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_currentListIndex++;
		if (m_currentListIndex > m_maxListIndex) {
			m_currentListIndex = 0;
		}
	}
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_currentListIndex--;
		if (m_currentListIndex < 0) {
			m_currentListIndex = m_maxListIndex;
		}
	}
}


void PoseMenu::Render(RenderContext& rc) {

	/**
	 * ポーズ画面、画面上に出てるので
	 * 非表示にしています。
	 */
	//m_canvas.Render(rc);
	//
	//for (const auto& p : m_posePanelList) {
	//
	//	if (m_posePanelList[m_currentListIndex] == p) {
	//		p->Render(rc);
	//	}
	//	if (p == m_posePanelList[3] || p == m_posePanelList[4]) {
	//		p->Render(rc);
	//	}
	//}
}

void PoseMenu::Init(const PoseMenuInitData* initData)
{
	if (initData == nullptr) {
		//** 設定されていなければ仮画像で初期化 */
		Init(&defaultPoseMenu);
		return;
	}
	
	//ポーズメニュー背景の初期化
	m_image->Init(initData->frameFilePath, initData->frameSize_x, initData->frameSize_y);

	for (const auto& p : m_posePanelList) {
		p->Init();
	}
}
