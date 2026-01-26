#include "stdafx.h"
#include "PoseMenu.h"
#include "Inventory/Inventory.h"
#include "SaveManager.h"

namespace
{
	constexpr int MAX_ITEM_SLOT_NUM = 20;
}


ItemPosePanel::ItemPosePanel() {
	m_image = m_canvas.CreateUI<UIImage>();
	m_frameImage = m_canvas.CreateUI<UIImage>();
	m_itemSlotList.clear();
}

ItemPosePanel::~ItemPosePanel() {
	m_itemSlotList.clear();
}

bool ItemPosePanel::Start() {
	return true;
}

void ItemPosePanel::Init(const PosePanelInitData* initData) {
	if (initData == nullptr) {
		/** 設定されていなければ仮画像で初期化 */
		Init(&defaultItemPosePanel);
		return;
	}

	/** アイテムパネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
	m_canvas.m_transform.m_localPosition = Vector3(180.0f, 0.0f, 0.0f);

	//リストが空の場合のみ、初期化として20個の空スロットを作成
	if (m_itemSlotList.empty()) {
		m_cachedSlotTypes.clear();	//キャッシュもクリア

		for (int i = 0; i < MAX_ITEM_SLOT_NUM; ++i) {
			// スロットを作成
			auto newSlot = std::make_shared <ItemSlot>();

			// 最初はすべて空アイテムを入れておく
			auto emptyItem = std::make_shared<EmptyRecoveryItem>();
			emptyItem->Init();
			newSlot->SetItem(emptyItem);

			// 座標計算
			float posX = (i % 5) * 105.0f;
			float posY = (i / 5) * -105.0f + 105.0f;

			// 座標設定
			newSlot->GetItemCanvas()->m_transform.m_localPosition = Vector3(posX, posY, 0.0f);

			// リストに登録
			m_itemSlotList.push_back(newSlot);

			//今は空状態を表示していると記録
			m_cachedSlotTypes.push_back(enItemType_Empty);
		}
	}
}

void ItemPosePanel::Update() {
	m_image->Update();
	m_canvas.Update();

	//InventoryデータとUI表示の同期処理
	// TODO: Inventoryクラスのインスタンスを取得はありなのか
	// GetInstance()とFindGOの違い
	Inventory* inventory = Inventory::GetInstance();
	if (inventory) {
		for (int i = 0; i < MAX_ITEM_SLOT_NUM; ++i) {
			//インベントリの実際のデータを確認
			ItemInfo* info = inventory->GetItemInfo(i);
			if (!info) continue;

			// データ上のタイプと現在表示中のタイプを比較
			EnItemType currentDataType = info->m_type;
			EnItemType currentUiType = m_cachedSlotTypes[i];

			// 違っていたら、画像を差し替える
			if (currentDataType != currentUiType) {
				// 新しい種類のアイテムUIを作成
				auto newItem = MakeSharedItem(currentDataType);
				newItem->Init();

				// スロットの中身を入れ替え
				m_itemSlotList[i]->SetItem(newItem);

				// アイテム生成時にTransform初期化の可能性があるため、座標再設定
				float posX = (i % 5) * 105.0f;
				float posY = (i / 5) * -105.0f + 105.0f;
				m_itemSlotList[i]->GetItemCanvas()->m_transform.m_localPosition = Vector3(posX, posY, 0.0f);

				//キャッシュを更新
				m_cachedSlotTypes[i] = currentDataType;
			}
		}
	}

	//各スロットの更新
	for (auto& slot : m_itemSlotList) {
		slot->Update();
	}
}

void ItemPosePanel::Render(RenderContext& rc) {
	m_image->Render(rc);

	for (auto& slot : m_itemSlotList) {
		slot->Render(rc);
	}

	if (m_frameImage) {
		m_frameImage->Render(rc);
	}
}

ItemPosePanel::RefItemBasePtr ItemPosePanel::MakeSharedItem(const EnItemType type)
{
	switch (type)
	{
		case enItemType_Onigiri:
		{
			return std::make_shared<SmallRecoveryItem>();
		}
		case enItemType_Bento:
		{
			return std::make_shared<StandardRecoveryItem>();
		}
		case enItemType_Sushi:
		{
			return std::make_shared<GreatRecoveryItem>();
		}
		case enItemType_Empty:
		{
			return std::make_shared<EmptyRecoveryItem>();
		}
	}
	//デフォルトは空を返す
	return std::make_shared<EmptyRecoveryItem>();

	//  K2_ASSERT(false, "種類に対する処理が追加されていません");
	//  return nullptr;
}




/**************************************************/


RecoveryItem::RecoveryItem() {
	m_backgroundImage = m_canvas->CreateUI<UIImage>();
	m_image = m_canvas->CreateUI<UIImage>();
}

RecoveryItem::~RecoveryItem() {}

void RecoveryItem::Update() {
	m_canvas->Update();
}

void RecoveryItem::Render(RenderContext& rc) {
	m_canvas->Update();
	m_canvas->Render(rc);
	if (m_backgroundImage) {
		m_backgroundImage->Render(rc);
	}

	if (m_image) {
		m_image->Render(rc);
	}
}

void RecoveryItem::Init(const ItemIconInitData* initData) {
	/** 背景画像は常に共通のデータで初期化 */
	m_backgroundImage->Init(
		iconBackGroundInitData.textFilePath, iconBackGroundInitData.frameSize_x, iconBackGroundInitData.frameSize_y
	);

	if (initData == nullptr) {
		Init(&iconBackGroundInitData);
		return;
	}

	if (initData != nullptr) {
		m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
	}
}

void RecoveryItem::Use(Player* p)
{
}




/**************************************************/


SmallRecoveryItem::SmallRecoveryItem() {
	m_healAmount = 20;
}

SmallRecoveryItem::~SmallRecoveryItem() {
}

void SmallRecoveryItem::Update() {
	m_canvas->Update();
}

void SmallRecoveryItem::Init(const ItemIconInitData* initData) {
	if (initData == nullptr) {
		Init(&smallRecoveryItemInitData);
		return;
	}
	RecoveryItem::Init(initData);
}




/**************************************************/


StandardRecoveryItem::StandardRecoveryItem() {
	m_healAmount = 60;
}

StandardRecoveryItem::~StandardRecoveryItem() {
}

void StandardRecoveryItem::Update() {
	m_canvas->Update();
}

void StandardRecoveryItem::Init(const ItemIconInitData* initData) {
	if (initData == nullptr) {
		Init(&standardRecoveryItemInitData);
		return;
	}
	RecoveryItem::Init(initData);
}




/**************************************************/


GreatRecoveryItem::GreatRecoveryItem() {
	m_healAmount = 120;
}

GreatRecoveryItem::~GreatRecoveryItem() {
}


void GreatRecoveryItem::Update() {
	m_canvas->Update();
}

void GreatRecoveryItem::Init(const ItemIconInitData* initData) {
	if (initData == nullptr) {
		Init(&GreatRecoveryItemInitData);
		return;
	}
	RecoveryItem::Init(initData);
}




/**************************************************/


EmptyRecoveryItem::EmptyRecoveryItem() {
}

EmptyRecoveryItem::~EmptyRecoveryItem() {
}

void EmptyRecoveryItem::Update() {
	m_canvas->Update();
}

void EmptyRecoveryItem::Init(const ItemIconInitData* initData) {
	if (initData == nullptr) {
		Init(&EmptyRecoveryItemInitData);
		return;
	}
	RecoveryItem::Init(initData);
}




/**************************************************/


bool ItemSlot::Start() {
	if (m_itemBase) {
		m_itemBase->Init();
	}
	return true;
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
	//  if (g_pad[0]->IsTrigger(enButtonDown)) {
	//  	m_currentCursolIndex++;
	//  	if (m_currentCursolIndex > m_maxCursolIndex) {
	//  		m_currentCursolIndex = 0;
	//  	}
	//  }
	//  if (g_pad[0]->IsTrigger(enButtonUp)) {
	//  	m_currentCursolIndex--;
	//  	if (m_currentCursolIndex < 0) {
	//  		m_currentCursolIndex = m_maxCursolIndex;
	//  	}
	//  }

	if (m_currentCursolIndex >= 0
		&& m_currentCursolIndex <= m_maxCursolIndex) {
		const auto& pos = CURSOR_POSITIONS[m_currentCursolIndex];
		m_canvas.m_transform.m_localPosition.y = pos.y;
	}

	if (g_pad[0]->IsPress(enButtonA)) {
		m_pushTime = 0.5f;
	}

	if (m_pushTime > 0.0f) {
		m_pushTime -= g_gameTime->GetFrameDeltaTime();
		PushAlphaCursol();
		m_isPressed = true;

		if (m_pushTime <= 0.0f) {
			m_pushTime = 0.0f;
		}
	}
	else {
		SelectAlphaCursol();
		m_isPressed = false;
	}

	if (m_owner) {
		m_owner->SetIsCursolPressed(m_isPressed);
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


namespace {
	/* 基準(左上)となる座標 */
	const float GRID_START_X = 0.0f;
	const float GRID_START_Y = 105.0f;
	/* アイコン同士の間隔 */
	const float GRID_STEP = 105.0f;
	/* グリッドの構成(5列 x 4行) */
	const int	NUM_COLS = 5;
	const int	NUM_ROWS = 4;
}

IconCursol::IconCursol() {
	m_image = m_canvas.CreateUI<UIImage>();
	
}

IconCursol::~IconCursol() {
}

bool IconCursol::Start() {
	return true;
}

void IconCursol::Update() {
	/** TODO:Updateメソッドに要素が多すぎる責任が複数あるので分けたい所存 */

	/** 横移動量 */
	int dx = 0;
	/** 縦移動量 */
	int dy = 0;

	/** アイテムシーンに入ったらカーソル移動を有効にしたい */
	//  if (!m_owner->GetEnableContentCursol()) {
	//  	return;
	//  }

	/* Jキー */
	if (g_pad[0]->IsPress(enButtonLB2)) {
		/**
		 * Aボタン押下中に上下移動
		 * RB1:7キー/LB1:Bキー
		 */
		if (g_pad[0]->IsTrigger(enButtonRB1)) dy = 1;	/* 下へ */
		if (g_pad[0]->IsTrigger(enButtonLB1))dy = -1;	/* 上へ */
	} else {
		/* Aボタンなし: 左右移動 */
		if (g_pad[0]->IsTrigger(enButtonRB1)) dx = 1;	/* 右へ */
		if (g_pad[0]->IsTrigger(enButtonLB1)) dx = -1;	/* 左へ */
	}

	/* 移動があった場合のみインデックスを更新 */
	if (dx != 0 || dy != 0) {
		/* 現在の1次元インデックスを2次元(列・行)に分解 */
		int col = m_currentCursolIndex % NUM_COLS;
		int row = m_currentCursolIndex / NUM_COLS;

		/* 横移動 */
		col = (col + dx + NUM_COLS) % NUM_COLS;
		/* 縦移動 */
		row = (row + dy + NUM_ROWS) % NUM_ROWS;
		/* 2次元から1次元インデックスへ戻す */
		m_currentCursolIndex = row * NUM_COLS + col;
	}

	/* 最新のインデックスから「現在の列と行」を再計算 */
	int currentCol = m_currentCursolIndex % NUM_COLS;
	int currentRow = m_currentCursolIndex / NUM_COLS;

	/* 座標 = 開始地点　+ (INDEX * 間隔)　下に行くほど値が小さくなるのでマイナス */
	float posX = GRID_START_X + (static_cast<float>(currentCol) * GRID_STEP);
	float posY = GRID_START_Y - (static_cast<float>(currentRow) * GRID_STEP);

	
	m_canvas.m_transform.m_localPosition = Vector3(posX, posY, 0.0f);
	m_canvas.Update();
}

void IconCursol::Render(RenderContext&rc) {
	if (m_owner->IsVisible()) {
		m_canvas.Render(rc);
	}
	
}

void IconCursol::Init(const PosePanelInitData* initData) {
	if (initData == nullptr) {
		Init(&defaultIconCursol);
		return;
	}
	m_image->Init(
		initData->textFilePath, initData->frameSize_x, initData->frameSize_y
	);
	m_canvas.m_transform.m_localPosition = Vector3(0.0f, 100.0f, 0.0f);
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
	
	float alpha = m_canvas.GetColor().w;

	bool isPressed = false;

	if (m_owner) {
		isPressed = m_owner->IsCursolPressed();
	}

	/** デバッグテスト */
	if(isPressed)
	{
	//if (g_pad[0]->IsTrigger(enButtonRB2)) {
		alpha -= 50.0f * g_gameTime->GetFrameDeltaTime();

		if (alpha <= 0.0f) {
			alpha = 0.0f;
		}
	}
	else {
		alpha += 50.0f * g_gameTime->GetFrameDeltaTime();

		if (alpha >= 1.0f) {
			alpha = 1.0f;
		}
		
	}
	m_canvas.SetColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
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


NameListBasePosePanel::NameListBasePosePanel() {
	m_image = m_canvas.CreateUI<UIImage>();
}

NameListBasePosePanel::~NameListBasePosePanel() {
}

bool NameListBasePosePanel::Start() {
	return true;
}

void NameListBasePosePanel::Update() {
	float alpha = m_canvas.GetColor().w;

	bool isPressed = false;

	if (m_owner) {
		isPressed = m_owner->IsCursolPressed();
	}

	if (isPressed) {
		alpha -= 50.0f * g_gameTime->GetFrameDeltaTime();
		if (alpha < 0.0f) {
			alpha = 0.0f;
		}
	}
	else {
		alpha += 50.0f * g_gameTime->GetFrameDeltaTime();
		if (alpha > 1.0f) {
			alpha = 1.0f;
		}
	}

	m_canvas.SetColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_canvas.Update();
}

void NameListBasePosePanel::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void NameListBasePosePanel::Init(const PosePanelInitData* initData) {
	if (initData == nullptr) {
		/** 設定されていなければ、画像で初期化 */
		Init(&defaultNameListBase);
		return;
	}
	/** 名前リストパネルの下敷き初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
}

/**************************************************/


PoseMenu::PoseMenu()
{
	m_image = m_canvas.CreateUI<UIImage>();
	/** スロット生成 */
	m_itemSlot = std::make_shared<ItemSlot>();

	/** パネルを生成 */
	auto itemPanel = std::make_shared<ItemPosePanel>();
	itemPanel->SetOwner(this);
	/** パネルにスロットを教える */
	itemPanel->SetItemSlot(m_itemSlot);
	m_posePanelList.push_back(itemPanel);

	auto manualControlPanel = std::make_shared<ManualControlPosePanel>();
	manualControlPanel->SetOwner(this);
	m_posePanelList.push_back(manualControlPanel);
	//  m_posePanelList.push_back(
	//  	std::make_shared<ManualControlPosePanel>()
	//  );

	auto settingPanel = std::make_shared<SettingPosePanel>();
	settingPanel->SetOwner(this);
	m_posePanelList.push_back(settingPanel);

	//  m_posePanelList.push_back(
	//  	std::make_shared<SettingPosePanel>()
	//  );

	

	//  m_posePanelList.push_back(
	//  	std::make_shared<CursolPosePanel>()
	//  );
	
	auto nameListBase = std::make_shared<NameListBasePosePanel>();
	nameListBase->SetOwner(this);
	m_posePanelList.push_back(nameListBase);


	auto cursolPanel = std::make_shared<CursolPosePanel>();
	cursolPanel->SetOwner(this);
	m_posePanelList.push_back(cursolPanel);

	auto nameListPanel = std::make_shared<NameListPosePanel>();
	nameListPanel->SetOwner(this);
	m_posePanelList.push_back(nameListPanel);
	

	
	//  m_posePanelList.push_back(
	//  	std::make_shared<NameListPosePanel>()
	//  );

	m_iconCursol = std::make_shared<IconCursol>();
	m_iconCursol->SetOwner(this);
	m_posePanelList.push_back(m_iconCursol);
	//何も以上なかったら消す
	//  m_posePanelList.push_back(
	//  	std::make_shared<IconCursol>()
	//  );

	//SaveManager::GetInstance().Load();
}

PoseMenu::~PoseMenu() {
}

bool PoseMenu::Start() {
	return true;
}

void PoseMenu::Update() {
	m_canvas.Update();
	for (const auto& p : m_posePanelList) {
		p->Update();
	}

	if (m_isContentfCursolEnable && m_iconCursol) {
		m_iconCursol->Update();
	}

	//  /** メインパネルの切り替え(上下) */
	//  if (g_pad[0]->IsTrigger(enButtonDown)) {
	//  	m_currentListIndex++;
	//  	if (m_currentListIndex > m_maxListIndex) {
	//  		m_currentListIndex = 0;
	//  	}
	//  }
	//  if (g_pad[0]->IsTrigger(enButtonUp)) {
	//  	m_currentListIndex--;
	//  	if (m_currentListIndex < 0) {
	//  		m_currentListIndex = m_maxListIndex;
	//  	}
	//  }

	/** アイテムの使用時の処理 */
	// アイテム一覧が表示されているとき(0番目と仮定)
	if (m_currentListIndex == 0) {
		//デバッグテスト：フラグを立てる
		m_isVisible = true;

		// Aボタンで使用
		if (g_pad[0]->IsTrigger(enButtonB)) {
			// カーソルが存在するなら
			if (m_iconCursol) {
				// カーソルの現在の位置を取得
				int targetIndex = m_iconCursol->GetCurrentIndex();

				// Inventoryから削除
				Inventory* inventory = Inventory::GetInstance();
				if (inventory) {
					inventory->RemoveItem(targetIndex);
				}
			}
		}
	}
	else {
		m_isVisible = false;
	}
	
}

void PoseMenu::Render(RenderContext& rc) {
	/** ポーズ画面、画面上に出てるので非表示にしています。*/
	m_canvas.Render(rc);

	for (const auto& p : m_posePanelList) {
		/** 常に描画されるもの/現在の選択パネルを描画 */
		if (p->ShouldAlwayRender()
			|| m_posePanelList[m_currentListIndex] == p) {
			p->Render(rc);
		}
	}

	//  /** 最後にスロット(アイテム画像)を一番上に描画 */
	//  if (m_itemSlot) {
	//  	m_itemSlot->Render(rc);
	//  }
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
