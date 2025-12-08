#include "stdafx.h"
#include "PoseMenu.h"

ItemPosePanel::ItemPosePanel(){
	m_image = m_canvas.CreateUI<UIImage>();
	m_itemList.push_back(
		std::make_shared <SmallRecoveryItem>()
	);
	m_itemList.push_back(
		std::make_shared<StandardRecoveryItem>()
	);
	m_itemList.push_back(
		std::make_shared<GreatRecoveryItem>()
	);
}

ItemPosePanel::~ItemPosePanel(){
}

bool ItemPosePanel::Start() {
	return true;
}

void ItemPosePanel::Update() {
	m_image->Update();
	m_canvas.Update();
	
	for (const auto& a : m_itemList) {
		a->Update();
	}
}

void ItemPosePanel::Render(RenderContext& rc) {
	m_image->Render(rc);

	for (const auto& a : m_itemList) {
		a->Render(rc);
	}
}

void ItemPosePanel::Init(const PosePanelInitData* initData) {
	if (initData == nullptr) {
		//** 設定されていなければ仮画像で初期化 */
		Init(&defaultItemPosePanel);
		return;
	}
	//** アイテムパネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
	m_canvas.m_transform.m_localPosition.x = 180.0f;

	//for (const auto& a : m_itemList) {
	//	a->Init();
	//}


	auto itemList = m_owner->m_itemPanelInfoList;
	for (auto info : itemList) {
		for (int i = 0; i < info->m_num; ++i) {

			RefItemBasePtr newItem = nullptr;

			if (info->m_type == enItemType_Onigiri) {
				// おにぎりなど１個の情報
				newItem = std::make_shared<SmallRecoveryItem>();
				//item->Init();
			}
			else if (info->m_type == enItemType_Bento) {
				// タウリン
				newItem = std::make_shared<StandardRecoveryItem>();
			}
			else if (info->m_type == enItemType_Sushi) {
				// 肉まん
				newItem = std::make_shared< GreatRecoveryItem>();
			}

			if (newItem) {
				m_itemList.push_back(newItem);
				newItem->Init();
			}
		}
	}
}




/**************************************************/


RecoveryItem::RecoveryItem() {
	m_image = m_canvas.CreateUI<UIImage>();
}

RecoveryItem::~RecoveryItem() {
}

bool RecoveryItem::Start() {
	return true;
}

void RecoveryItem::Update() {
	m_canvas.Update();
}

void RecoveryItem::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void RecoveryItem::Init(const ItemIconInitData* initData)
{
	if (initData == nullptr) {
		Init(&iconBackGroundInitData);
		return;
	}
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
}

void RecoveryItem::Use(Player* p)
{
}




/**************************************************/


SmallRecoveryItem::SmallRecoveryItem() {
	m_image = m_canvas.CreateUI<UIImage>();
	m_healAmount = 20;
}

SmallRecoveryItem::~SmallRecoveryItem() {
}

bool SmallRecoveryItem::Start() {
	return true;
}

void SmallRecoveryItem::Update() {
	m_canvas.Update();
}

void SmallRecoveryItem::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void SmallRecoveryItem::Init(const ItemIconInitData* initData) {
	if (initData == nullptr) {
		Init(&smallRecoveryItemInitData);
		return;
	}
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
}




/**************************************************/


StandardRecoveryItem::StandardRecoveryItem() {
	m_image = m_canvas.CreateUI<UIImage>();
	m_healAmount = 60;
}

StandardRecoveryItem::~StandardRecoveryItem() {
}

bool StandardRecoveryItem::Start() {
	return true;
}

void StandardRecoveryItem::Update() {
	m_canvas.Update();
}

void StandardRecoveryItem::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void StandardRecoveryItem::Init(const ItemIconInitData* initData) {
	if (initData == nullptr) {
		Init(&standardRecoveryItemInitData);
		return;
	}
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
}




/**************************************************/


GreatRecoveryItem::GreatRecoveryItem() {
	m_image = m_canvas.CreateUI<UIImage>();
	m_healAmount = 120;
}

GreatRecoveryItem::~GreatRecoveryItem() {
}

bool GreatRecoveryItem::Start() {
	return true;
}

void GreatRecoveryItem::Update() {
	m_canvas.Update();
}

void GreatRecoveryItem::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void GreatRecoveryItem::Init(const ItemIconInitData* initData) {
	if (initData == nullptr) {
		Init(&GreatRecoveryItemInitData);
		return;
	}
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
}




/**************************************************/


ItemSlot::ItemSlot() {
	//  m_itemList.push_back(
	//  	//std::move(std::make_unique<ManualControlPosePanel>())
	//  	std::move(std::make_unique<SmallRecoveryItem>())
	//  );
	//  m_itemList.push_back(
	//  	std::move(std::make_unique<StandardRecoveryItem>())
	//  );
	//  m_itemList.push_back(
	//  	std::move(std::make_unique<GreatRecoveryItem>())
	//  );
}

ItemSlot::~ItemSlot() {
}

bool ItemSlot::Start() {
	//  for (const auto& a : m_itemList) {
	//  	a->Init();
	//  }
	if (m_itemBase) {
		m_itemBase->Init();
	}

	return true;
}

void ItemSlot::Update() {
	if (m_itemBase) {
		m_itemBase->Update();
	}
	//  for (const auto& a : m_itemList) {
	//  	a->Update();
	//  }
}

void ItemSlot::Render(RenderContext& rc) {
	if (m_itemBase) {
		m_itemBase->Render(rc);
	}

	//  for (const auto& a : m_itemList) {
	//  	a->Render(rc);
	//  }
}




/**************************************************/


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
	m_canvas.m_transform.m_localPosition.x = 180.0f;

}




/**************************************************/


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
	m_canvas.m_transform.m_localPosition.x = 180.0f;
}




/**************************************************/


namespace {
	const float ITEM_POS = 160.0f;
	const float MANUALCONTROL_POS = 90.0f;
	const float SETTING_POS = 15.0f;
}

const CursolPosePanel::CursolPosYData CursolPosePanel::CURSOR_POSITIONS[] = {
	{ITEM_POS},
	{MANUALCONTROL_POS},
	{SETTING_POS}
};

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

	if (m_currentCursolIndex >= 0
		&& m_currentCursolIndex <= m_maxCursolIndex) {
		const auto& pos = CURSOR_POSITIONS[m_currentCursolIndex];
		m_canvas.m_transform.m_localPosition.y = pos.y;
	}
	
	if (g_pad[0]->IsPress(enButtonA)) {
		m_pushTime = 0.5f;
	}
		
	if (m_pushTime > 0.0f){

		m_pushTime -= g_gameTime->GetFrameDeltaTime();
		PushAlphaCursol();

		if(m_pushTime <= 0.0f){
			m_pushTime = 0.0f;
		}	
	}
	else {
		SelectAlphaCursol();
	}

	m_image->SetColor(255.0f, 0.0f, 0.0f, m_alphaCursol);
	m_canvas.m_transform.UpdateTransform();
	m_canvas.Update();
	m_image->Update();
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
	m_canvas.m_transform.m_localPosition = Vector3(-407.0f, 160.0f, 0.0f);
}

void CursolPosePanel::SelectAlphaCursol()
{
	m_alphaCursol += m_speedAlpha;

	if (m_alphaCursol >= 0.85f) {
		m_alphaCursol = 0.85f;
		m_speedAlpha *= -1.0f;
	}
	else if (m_alphaCursol < 0.5f) {
		m_alphaCursol = 0.5f;
		m_speedAlpha *= -1.0f;
	}
}

void CursolPosePanel::PushAlphaCursol()
{
	m_alphaCursol += m_speedPushAlpha;

	if (m_alphaCursol >= 0.85f) {
		m_alphaCursol = 0.85f;
		m_speedPushAlpha *= -1.0f;
	}
	else if (m_alphaCursol < 0.3f) {
		m_alphaCursol = 0.3f;
		m_speedPushAlpha *= -1.0f;
	}
}




/**************************************************/


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




/**************************************************/


PoseMenu::PoseMenu()
{
	m_image = m_canvas.CreateUI<UIImage>();

	std::vector<std::unique_ptr<PosePanelBase>> panel;

	auto itemPosePanel = std::make_shared<ItemPosePanel>();
	itemPosePanel->SetOwner(this);
	m_posePanelList.push_back(
		//std::move(std::make_unique<ItemPosePanel>())
		itemPosePanel
	);
	m_posePanelList.push_back(
		//std::move(std::make_unique<ManualControlPosePanel>())
		std::make_shared<ManualControlPosePanel>()
	);
	m_posePanelList.push_back(
		//std::move(std::make_unique<SettingPosePanel>())
		std::make_shared<SettingPosePanel>()
	);
	m_posePanelList.push_back(
		//std::move(std::make_unique<CursolPosePanel>())
		std::make_shared<CursolPosePanel>()
	);
	m_posePanelList.push_back(
		//std::move(std::make_unique<NameListPosePanel>())
		std::make_shared<NameListPosePanel>()
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
	m_canvas.Render(rc);
	
	for (const auto& p : m_posePanelList) {
	
		/** 常に描画すべきパネルを描画 */
		if (p->ShouldAlwayRender()) {
			p->Render(rc);
		}
		/**
		 * 現在選択されているメインパネルを描画
		 * unique_ptrを比較するために、get()を使って生ポインタで比較
		 */
		else if (m_posePanelList[m_currentListIndex].get() == p.get()) {
			p->Render(rc);
		}
	}
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

	m_itemSlot = std::make_shared<ItemSlot>();
	m_itemSlot->Start();
}