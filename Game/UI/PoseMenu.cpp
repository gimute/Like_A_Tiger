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

	m_canvas.m_transform.m_localPosition = Vector3(180.0f, 0.0f, 0.0f);
	/** アイテムパネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 開く用カラーイージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_colorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じる用アニメーション */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
	

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

			//座標設定
			newSlot->GetItemCanvas()->m_transform.m_localPosition = Vector3(posX, posY, 0.0f);

			// リストに登録
			m_itemSlotList.push_back(newSlot);

			//今は空状態を表示していると記録
			m_cachedSlotTypes.push_back(enItemType_Empty);
		}
	}
}

void ItemPosePanel::Update() {
	/*Vector4 color = m_canvas.GetColor();
	color.w = m_parentAlpha;
	m_canvas.SetColor(color);*/

	m_image->Update();
	m_canvas.Update();

	if (m_colorAnimation) {
		m_colorAnimation->Update();
	}
	if (m_closeColorAnimation) {
		m_closeColorAnimation->Update();
	}

	//InventoryデータとUI表示の同期処理
	// TODO: Inventoryクラスのインスタンスを取得はありなのか
	// GetInstance()とFindGOの違い
	Inventory* inventory = Inventory::GetInstance();
	if (inventory && !m_cachedSlotTypes.empty()) {
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

				if (m_owner->IsPoseActive()) {
					/** 新しいアイテムの開くアニメーションして表示 */
					newItem->PlayOpenAnimation();
				}
				

				//キャッシュを更新
				m_cachedSlotTypes[i] = currentDataType;
			}
		}
	}

	//各スロットの更新
	for (auto& slot : m_itemSlotList) {
		//slot->GetItemCanvas()->SetColor(Vector4(1.0f, 1.0f, 1.0f, m_parentAlpha));
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

void ItemPosePanel::OnCursolIndexChanged(int newIndex) {
	/** 前のスロットのアニメーションを停止 */
	if (m_previousCursorIndex >= 0
		&& m_previousCursorIndex < static_cast<int>(m_itemSlotList.size())) 
	{
		auto& prevSlot = m_itemSlotList[m_previousCursorIndex];
		if (prevSlot && prevSlot->GetItem()) {
			prevSlot->GetItem()->StopScaleAnimation();
			prevSlot->GetItem()->SetSelected(false);
		}
	}

	/** アイテムありの新しいスロットのアニメーションを開始 */
	if (newIndex >= 0
		&& newIndex <= static_cast<int>(m_itemSlotList.size())) 
	{
		auto& newSlot = m_itemSlotList[newIndex];
		if (newSlot
			&& newSlot->GetItem()) {
			/** キャッシュから空でないかチェック */
			if (newIndex < static_cast<int>(m_cachedSlotTypes.size())
				&& m_cachedSlotTypes[newIndex] != enItemType_Empty)
			{
				newSlot->GetItem()->SetSelected(true);
				newSlot->GetItem()->PlayScaleAnimation();
			}

		}
	}
	/** 前のインデックス更新 */
	m_previousCursorIndex = newIndex;
}



/**************************************************/


RecoveryItem::RecoveryItem() {
	m_backgroundImage = m_canvas->CreateUI<UIImage>();
	m_image = m_canvas->CreateUI<UIImage>();
}

RecoveryItem::~RecoveryItem() {}

void RecoveryItem::Update() {
	m_canvas->Update();
	
	/** 開くアニメーション */
	if (m_backGroundColorAnimation && !m_pauseedFullAlpha) {
		m_backGroundColorAnimation->Update();
	}
	if (m_imageColorAnimation) {
		m_imageColorAnimation->Update();
	}

	/** 閉じるアニメーション */
	if (m_backGroundColorAnimation) {
		m_backGroundColorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}

	/** スケールアニメーション */
	if (m_scaleAnimation) {
		m_scaleAnimation->Update();
	}
}

void RecoveryItem::Render(RenderContext& rc) {
	//m_canvas->Update();
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
	
	m_backgroundImage->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 背景：開くアニメーション（0→1） */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f), Vector4::White };
		m_backGroundColorAnimation = std::make_unique<ColorUIAnimation>(m_backgroundImage, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 背景：閉じるアニメーション（1→0） */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4::White, Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeBackGoundColorAnimation = std::make_unique<ColorUIAnimation>(m_backgroundImage, false, EasingType::EaseIn, timeList, colorList);
	}
	
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 画像：開くアニメーション（0→1） */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f), Vector4::White };
		m_imageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 画像：閉じるアニメーション（1→0） */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4::White, Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	}
	/** スケールアニメーション (1⇒1.5⇒1) */
	{
		std::vector<float> timeList = { 0.1f,0.1f };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f)};
		m_scaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseInOut, timeList, scaleList);
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

	/** 開くアニメーション */
	if (m_imageColorAnimation) {
		m_imageColorAnimation->Update();
	}
	if (m_backGroundColorAnimation) {
		m_backGroundColorAnimation->Update();
	}

	/** 閉じるアニメーション */
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}
	if (m_closeBackGoundColorAnimation) {
		m_closeBackGoundColorAnimation->Update();
	}

	/** 拡大のアニメーション */
	if (m_scaleAnimation) {
		m_scaleAnimation->Update();
	}
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

	/** 開くアニメーション */
	if (m_backGroundColorAnimation) {
		m_backGroundColorAnimation->Update();
	}
	if (m_imageColorAnimation) {
		m_imageColorAnimation->Update();
	}

	/** 閉じるアニメーション */
	if (m_closeBackGoundColorAnimation) {
		m_closeBackGoundColorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}

	/** 拡大のアニメーション */
	if (m_scaleAnimation) {
		m_scaleAnimation->Update();
	}
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

	/** 開くアニメーション */
	if (m_backGroundColorAnimation) {
		m_backGroundColorAnimation->Update();
	}
	if (m_imageColorAnimation) {
		m_imageColorAnimation->Update();
	}

	/** 閉じるアニメーション */
	if (m_closeBackGoundColorAnimation) {
		m_closeBackGoundColorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}

	/** 拡大のアニメーション */
	if (m_scaleAnimation) {
		m_scaleAnimation->Update();
	}
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

	if (m_backGroundColorAnimation) {
		m_backGroundColorAnimation->Update();
	}

	if (m_closeBackGoundColorAnimation) {
		m_closeBackGoundColorAnimation->Update();
	}
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
	if (m_openImageColorAnimation) {
		m_openImageColorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}
	
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
	m_canvas.m_transform.m_localPosition.x = 180.0f;
	//** 操作説明パネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 開く用カラーイージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_openImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じる用カラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
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
	if (m_openImageColorAnimation) {
		m_openImageColorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}
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
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 開く時のカラーイージング */
	{
		std::vector<float> openTimeList = { 1.0f };
		std::vector<Vector4> openColorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_openImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, openTimeList, openColorList);
	}
	/** 閉じる時のカラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}

	m_canvas.m_transform.m_localPosition.x = 180.0f;
}




/**************************************************/


namespace {
	const float ITEM_POS = 160.0f;
	const float MANUALCONTROL_POS = 85.0f;
	const float SETTING_POS = 10.0f;
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
	if (m_owner->IsSettingInSelectActive()) {
		m_canvas.m_transform.UpdateTransform();
		m_canvas.Update();
		m_image->Update();

		if (m_imageColorAnimation) {
			m_imageColorAnimation->Update();
		}
		if (m_closeImageColorAnimation) {
			m_closeImageColorAnimation->Update();
		}
		return;
	}

	/** カーソル移動処理 */
	bool indexChanged = false;

	/** メニューがアクティブのみ動かす */
	if (m_owner->IsPoseActive()) {
		if (g_pad[0]->IsTrigger(enButtonDown)) {
			m_currentCursolIndex++;
			if (m_currentCursolIndex > m_maxCursolIndex) {
				m_currentCursolIndex = 0;
			}
			indexChanged = true;
		}
		if (g_pad[0]->IsTrigger(enButtonUp)) {
			m_currentCursolIndex--;
			if (m_currentCursolIndex < 0) {
				m_currentCursolIndex = m_maxCursolIndex;
			}
			indexChanged = true;
		}

		if (m_currentCursolIndex >= 0
			&& m_currentCursolIndex <= m_maxCursolIndex) {
			const auto& pos = CURSOR_POSITIONS[m_currentCursolIndex];
			m_canvas.m_transform.m_localPosition.y = pos.y;
		}

		/** カーソル位置が変更されたらPoseMenuのリストインデックスも更新 */
		if (indexChanged && m_owner) {
			m_owner->SetCurrentListIndex(m_currentCursolIndex);
		}
	}

	

	if (m_owner) {
		m_owner->SetIsCursolPressed(m_isPressed);
	}
	 
	m_canvas.m_transform.UpdateTransform();
	m_canvas.Update();
	m_image->Update();

	if (m_imageColorAnimation) {
		m_imageColorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}
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
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 開く用のカラーイージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_imageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じる用のカラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
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
	/** 前のインデックス */
	int previousIndex = m_currentCursolIndex;

	if (m_owner->IsPoseActive()) {
		/* Jキー */
		if (g_pad[0]->IsPress(enButtonLB2)) {
			/**
			 * Aボタン押下中に上下移動
			 * RB1:7キー/LB1:Bキー
			 */
			if (g_pad[0]->IsTrigger(enButtonRB1)) dy = 1;	/* 下へ */
			if (g_pad[0]->IsTrigger(enButtonLB1))dy = -1;	/* 上へ */
		}
		else {
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

		/** カーソル位置が変更されたらオーナーに通知 */
		if (previousIndex != m_currentCursolIndex
			&& m_owner)
		{
			m_owner->OnIconCursorIndexChanged(m_currentCursolIndex);
		}
	}
	

	/* 最新のインデックスから「現在の列と行」を再計算 */
	int currentCol = m_currentCursolIndex % NUM_COLS;
	int currentRow = m_currentCursolIndex / NUM_COLS;

	/* 座標 = 開始地点　+ (INDEX * 間隔)　下に行くほど値が小さくなるのでマイナス */
	float posX = GRID_START_X + (static_cast<float>(currentCol) * GRID_STEP);
	float posY = GRID_START_Y - (static_cast<float>(currentRow) * GRID_STEP);

	
	m_canvas.m_transform.m_localPosition = Vector3(posX, posY, 0.0f);
	m_canvas.Update();

	if (m_imageColorAnimation) {
		m_imageColorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}
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
	m_image->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	/** 開く用のカラーイージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_imageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じる用のカラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
	
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
	bool isPressed = false;

	if (m_owner) {
		isPressed = m_owner->IsCursolPressed();
	}
	m_canvas.Update();

	if (m_colorAnimation) {
		m_colorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}
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
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 開く用カラーイージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_colorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じる用カラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
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
	if (m_colorAnimation) {
		m_colorAnimation->Update();
	}
	if (m_closeColorAnimation) {
		m_closeColorAnimation->Update();
	}
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
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 開く用の色イージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_colorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じる用の色イージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
}




/**************************************************/


namespace {
	constexpr const char* INSELECT_TEXT_NO_PATH = "Assets/spriteData/PoseMenuInSelect/No.DDS";
	constexpr const char* INSELECT_TEXT_YES_PATH = "Assets/spriteData/PoseMenuInSelect/Yes.DDS";
	constexpr const char* INSELECT_CURSOL_PATH = "Assets/spriteData/PoseMenuInSelect/Cursol.DDS";
	constexpr const char* INSELECT_BASE_PATH = "Assets/spriteData/PoseMenuInSelect/Base.DDS";
	constexpr const char* INSELECT_TEXT_USEITEM_PATH = "Assets/spriteData/PoseMenuInSelect/UseItem.DDS";

	static const Vector2 TEXT_YES_POSITION = Vector2(0.0f, -10.0f);
	static const Vector2 TEXT_NO_POSITION = Vector2(0.0f, -70.0f);
}

const InSelect::CursolPositionYData InSelectCursolPositionYData[] = {
	{TEXT_YES_POSITION.y},
	{TEXT_NO_POSITION.y},
};

InSelect::InSelect() 
{
}

InSelect::~InSelect() 
{
}

bool InSelect::Start() 
{
	m_canvas = std::make_shared<UICanvas>();
	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);

	auto baseUI = m_canvas->CreateUI<UIImage>();
	baseUI->Init(INSELECT_BASE_PATH, 1000.0f, 300.0f);

	m_cursolUI = m_canvas->CreateUI<UIImage>();
	m_cursolUI->Init(INSELECT_CURSOL_PATH, 300.0f, 50.0f);
	m_cursolUI->m_transform.m_localPosition = Vector3(0.0f, -10.0f, 0.0f);
	// カーソルの色イージング
	{
		std::vector<float> timeList = { 0.8f,0.8f };
		std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.3f),Vector4::White };
		m_cursolColorAnimaiton = std::make_unique<ColorUIAnimation>(m_cursolUI, true, EasingType::EaseIn, timeList, colorList);
	}

	auto textUseItemUI = m_canvas->CreateUI<UIImage>();
	textUseItemUI->Init(INSELECT_TEXT_USEITEM_PATH, 250.0f, 25.0f);
	textUseItemUI->m_transform.m_localPosition = Vector3(0.0f, 80.0f, 0.0f);

	auto textYesUI = m_canvas->CreateUI<UIImage>();
	textYesUI->Init(INSELECT_TEXT_YES_PATH, 40.0f, 20.0f);
	textYesUI->m_transform.m_localPosition = Vector3(0.0f,-10.0f,0.0f);

	auto textNoUI = m_canvas->CreateUI<UIImage>();
	textNoUI->Init(INSELECT_TEXT_NO_PATH, 60.0f, 20.0f);
	textNoUI->m_transform.m_localPosition = Vector3(0.0f, -70.0f, 0.0f);

	return true;
}

void InSelect::Update() 
{
	m_cursolColorAnimaiton->Update();

	if (m_owner->IsPoseActive()) {
		if (g_pad[0]->IsTrigger(enButtonUp)) {
			m_currentCursolIndex--;
			if (m_currentCursolIndex < 0) {
				m_currentCursolIndex = 0;
			}
		}

		if (g_pad[0]->IsTrigger(enButtonDown)) {
			m_currentCursolIndex++;
			if (m_currentCursolIndex > 1) {
				m_currentCursolIndex = 1;
			}
		}

		if (m_currentCursolIndex >= 0
			&& m_currentCursolIndex <= 1)
		{
			const auto& pos = InSelectCursolPositionYData[m_currentCursolIndex];
			m_cursolUI->m_transform.m_localPosition = Vector3(0.0f, pos.positionY, 0.0f);
		}

		/** Bボタンで確定 */
		if (g_pad[0]->IsTrigger(enButtonB)) {
			if (m_currentCursolIndex == 0) {
				m_result = SelectResult::Yes;
			}
			else {
				m_result = SelectResult::No;
			}
		}

		/** Aボタンでキャンセル */
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_result = SelectResult::No;
		}
	}
	
	m_canvas->Update();
}

void InSelect::Render(RenderContext& rc) 
{
	m_canvas->Render(rc);
}




/**************************************************/


namespace {
	constexpr const char* SETTING_BASE_PATH = "Assets/spriteData/PoseMenuInSelect/SettingBase.DDS";
	constexpr const char* SETTING_CURSOL_PATH = "Assets/spriteData/PoseMenuInSelect/SettingCursol.DDS";
	constexpr const char* RETURN_TO_TITLE_PATH = "Assets/spriteData/PoseMenuInSelect/ReturnToTitle.DDS";
	constexpr const char* RETURN_TO_MENU_PATH = "Assets/spriteData/PoseMenuInSelect/ReturnToMenu.DDS";
	constexpr const char* THINK_TO_CHECK_PATH = "Assets/spriteData/PoseMenuInSelect/ThinkToCheck.DDS";
	constexpr const char* VOLUME_PATH = "Assets/spriteData/PoseMenuInSelect/VolumeAdjustment.DDS";

	static const Vector2 TEXT_TO_MENU_POSITION = Vector2(0.0f, 50.0f);
	static const Vector2 TEXT_TO_TITLE_POSITION = Vector2(170.0f, 20.0f);
	static const Vector2 TEXT_VOLUME_POSITION = Vector2(170.0f, 100.0f);

}

const Setting::CursolPositionData settingCursolPositionYData[] = {
	{TEXT_VOLUME_POSITION.x,TEXT_VOLUME_POSITION.y},
	{TEXT_TO_TITLE_POSITION.x,TEXT_TO_TITLE_POSITION.y},
};

Setting::Setting()
{
}

Setting::~Setting() {
}

bool Setting::Start() {
	m_canvas = std::make_shared<UICanvas>();
	m_canvas->m_transform.m_localPosition = Vector3(0.0f, -50.0f, 0.0f);

	/** baseUIを複製作成 */
	const int BASE_UI_COUNT = 2;
	const float BASE_UI_SPACING = 80.0f;
	const float BASE_UI_START_Y = 100.0f;


	for (int i = 0; i < BASE_UI_COUNT; ++i) {
		auto baseUI = m_canvas->CreateUI<UIImage>();
		baseUI->Init(SETTING_BASE_PATH, 500.0f, 60.0f);
		/** 縦に並べる */
		float posY = BASE_UI_START_Y - (static_cast<float>(i) * BASE_UI_SPACING);
		baseUI->m_transform.m_localPosition = Vector3(170.0f, posY, 0.0f);


		/** 開いた時のアニメーション */
		std::vector<float> openTimeList = { 1.0f };
		std::vector<Vector4> openColorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		auto openAnim = std::make_unique<ColorUIAnimation>(baseUI, false, EasingType::EaseIn, openTimeList, openColorList);
		//m_openBaseUIColorAnimation = std::make_unique<ColorUIAnimation>(m_baseUI, false, EasingType::EaseIn, timeList, colorList);
		
		/** 閉じた時のアニメーション */
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		auto closeAnim = std::make_unique<ColorUIAnimation>(baseUI, false, EasingType::EaseIn, closeTimeList, closeColorList);
		//m_closeBaseUIColorAnimation = std::make_unique<ColorUIAnimation>(m_baseUI, false, EasingType::EaseIn, closeTimeList, closeColorList);

		m_baseUIList.push_back(baseUI);
		m_openBaseUIColorAnimationList.push_back(std::move(openAnim));
		m_closeBaseUIColorAnimationList.push_back(std::move(closeAnim));
	}

	//  m_baseUI = m_canvas->CreateUI<UIImage>();
	//  m_baseUI->Init(SETTING_BASE_PATH, 1000.0f, 300.0f);
	//   /** 開いた時のアニメーション */
	//   {
	//   	std::vector<float> timeList = { 1.0f };
	//   	std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
	//   	m_openBaseUIColorAnimation = std::make_unique<ColorUIAnimation>(m_baseUI, false, EasingType::EaseIn, timeList, colorList);
	//   }
	//   /** 閉じた時のアニメーション */
	//   {
	//   	std::vector<float> timeList = { 1.0f };
	//   	std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
	//   	m_closeBaseUIColorAnimation = std::make_unique<ColorUIAnimation>(m_baseUI, false, EasingType::EaseIn, timeList, colorList);
	//   }
	//  auto baseUI = m_canvas->CreateUI<UIImage>();
	//  baseUI->Init(INSELECT_BASE_PATH, 1000.0f, 300.0f);

	m_cursolUI = m_canvas->CreateUI<UIImage>();
	m_cursolUI->Init(SETTING_CURSOL_PATH, 300.0f, 50.0f);
	m_cursolUI->m_transform.m_localPosition = Vector3(TEXT_TO_MENU_POSITION.x, TEXT_TO_MENU_POSITION.y, 0.0f);
	/** 色のイージング */
	{
		std::vector<float> timeList = { 1.0f,1.0f };
		std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_cursolUIColorAnimation = std::make_unique<ColorUIAnimation>(m_cursolUI, true, EasingType::EaseIn, timeList, colorList);
		m_cursolUIColorAnimation->Play();
	}
	/** 開いた時のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_openCursolUIColorAnimation = std::make_unique<ColorUIAnimation>(m_cursolUI, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じた時のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeCursolUIColorAnimation = std::make_unique<ColorUIAnimation>(m_cursolUI, false, EasingType::EaseIn, timeList, colorList);
	}

	m_textToTitleUI = m_canvas->CreateUI<UIImage>();
	m_textToTitleUI->Init(RETURN_TO_TITLE_PATH, 150.0f, 25.0f);
	m_textToTitleUI->m_transform.m_localPosition = Vector3(TEXT_TO_TITLE_POSITION.x, TEXT_TO_TITLE_POSITION.y, 0.0f);
	/** 開いた時のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_openTextToTitleUIColorAnimation = std::make_unique<ColorUIAnimation>(m_textToTitleUI, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じる時のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4::White, Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeTextToTitleUIColorAnimation = std::make_unique<ColorUIAnimation>(m_textToTitleUI, false, EasingType::EaseIn, timeList, colorList);
	}

	m_textVolumeUI = m_canvas->CreateUI<UIImage>();
	m_textVolumeUI->Init(VOLUME_PATH, 100.0f, 25.0f);
	m_textVolumeUI->m_transform.m_localPosition = Vector3(TEXT_TO_TITLE_POSITION.x, TEXT_VOLUME_POSITION.y, 0.0f);
	/** 開いた時のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_openTextVolumeUIColorAnimation = std::make_unique<ColorUIAnimation>(m_textVolumeUI, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じた時のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f), };
		m_closeTextVolumeUIColorAnimation = std::make_unique<ColorUIAnimation>(m_textVolumeUI, false, EasingType::EaseIn, timeList, colorList);
	}
	return true;
}

void Setting::Update() {
	for (auto& anim : m_openBaseUIColorAnimationList) {
		if (anim) {
			anim->Update();
		}
	}

	for (auto& anim : m_closeBaseUIColorAnimationList) {
		if (anim) {
			anim->Update();
		}
	}

	//  if (m_cursolUIColorAnimation) {
	//  	m_cursolUIColorAnimation->Update();
	//  }
	
	/** 開いた時のアニメーション */
	//  if (m_openBaseUIColorAnimation) {
	//  	m_openBaseUIColorAnimation->Update();
	//  }
	if (m_openCursolUIColorAnimation) {
		m_openCursolUIColorAnimation->Update();
	}
	if (m_openTextToTitleUIColorAnimation) {
		m_openTextToTitleUIColorAnimation->Update();
	}
	if (m_openTextVolumeUIColorAnimation) {
		m_openTextVolumeUIColorAnimation->Update();
	}
	

	/** 閉じる時のアニメーション更新 */
	//  if (m_closeBaseUIColorAnimation) {
	//  	m_closeBaseUIColorAnimation->Update();
	//  }
	if (m_closeCursolUIColorAnimation) {
		m_closeCursolUIColorAnimation->Update();
	}
	if (m_closeTextToTitleUIColorAnimation) {
		m_closeTextToTitleUIColorAnimation->Update();
	}
	if (m_closeTextVolumeUIColorAnimation) {
		m_closeTextVolumeUIColorAnimation->Update();
	}


	if (m_owner->IsPoseActive()) {
		if (g_pad[0]->IsTrigger(enButtonLB1)) {
			m_currentSettingCursolIndex--;
			if (m_currentSettingCursolIndex < 0) {
				m_currentSettingCursolIndex = 0;
			}
		}
		/**  */
		if (g_pad[0]->IsTrigger(enButtonRB1)) {
			m_currentSettingCursolIndex++;
			if (m_currentSettingCursolIndex > 1) {
				m_currentSettingCursolIndex = 1;
			}
		}

		if (m_currentSettingCursolIndex >= 0
			&& m_currentSettingCursolIndex <= 1)
		{
			const auto& pos = settingCursolPositionYData[m_currentSettingCursolIndex];
			m_cursolUI->m_transform.m_localPosition = Vector3(pos.positionX, pos.positionY, 0.0f);
		}

		/** Bボタンで確定 */
		if (g_pad[0]->IsTrigger(enButtonB)) {
			if (m_currentSettingCursolIndex == 0) {
				m_result = SelectResult::Volume;
			}
			else if (m_currentSettingCursolIndex == 1) {
				m_result = SelectResult::ToTitle;
				m_isVisible = true;
			}
			//else if (m_currentSettingCursolIndex == 2) {
			//	m_result = SelectResult::ToTitle;
				/** TODO: Settingのインスタンスを消したい
				 *	フラグを上げて、PoseMenuクラス側で削除させるのもあり⇒それが最有力
				 */

				 /******************************************/

				 //m_settingInSelect = NewGO<SettingInSelect>(0, "settingSelect");
			 //}
			else {
				m_result = SelectResult::None;
			}
		}
	}
	

	m_canvas->Update();
}

void Setting::Render(RenderContext& rc) {
	m_canvas->Render(rc);
}




/**************************************************/


SettingInSelect::SettingInSelect(){
}

SettingInSelect::~SettingInSelect() {
}

bool SettingInSelect::Start() 
{
	m_canvas = std::make_shared<UICanvas>();
	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);

	auto baseUI = m_canvas->CreateUI<UIImage>();
	baseUI->Init(INSELECT_BASE_PATH, 1000.0f, 300.0f);
	//baseUI->m_transform.m_localPosition = Vector3(0.0f,)

	m_cursolUI = m_canvas->CreateUI<UIImage>();
	m_cursolUI->Init(INSELECT_CURSOL_PATH, 300.0f, 50.0f);
	m_cursolUI->m_transform.m_localPosition = Vector3(0.0f, -20.0f, 0.0f);
	//m_cursolUI->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	{
		std::vector<float> timeList = { 1.0f,1.0f };
		std::vector<Vector4> ColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_cursolUIColorAnimation = std::make_unique<ColorUIAnimation>(m_cursolUI, true, EasingType::EaseIn, timeList, ColorList);
		m_cursolUIColorAnimation->Play();
	}

	auto textThinkToCheckUI = m_canvas->CreateUI<UIImage>();
	textThinkToCheckUI->Init(THINK_TO_CHECK_PATH, 280.0f, 25.0f);
	textThinkToCheckUI->m_transform.m_localPosition = Vector3(0.0f, 80.0f, 0.0f);
	
	auto textYesUI = m_canvas->CreateUI<UIImage>();
	textYesUI->Init(INSELECT_TEXT_YES_PATH, 40.0f, 20.0f);
	textYesUI->m_transform.m_localPosition = Vector3(0.0f, -10.0f, 0.0f);
	
	auto textNoUI = m_canvas->CreateUI<UIImage>();
	textNoUI->Init(INSELECT_TEXT_NO_PATH, 60.0f, 20.0f);
	textNoUI->m_transform.m_localPosition = Vector3(0.0f, -70.0f, 0.0f);

	return true;
}

void SettingInSelect::Update() {
	//if (m_cursolUIColorAnimation) {
		m_cursolUIColorAnimation->Update();
	//}

	if (m_owner->IsPoseActive()) {
		if (g_pad[0]->IsTrigger(enButtonRB1)) {
			m_currentCursolIndex++;
			if (m_currentCursolIndex > 1) {
				m_currentCursolIndex = 1;
			}
		}
		if (g_pad[0]->IsTrigger(enButtonLB1)) {
			m_currentCursolIndex--;
			if (m_currentCursolIndex <= 0) {
				m_currentCursolIndex = 0;
			}
		}

		if (m_currentCursolIndex >= 0
			&& m_currentCursolIndex <= 1) {
			const auto& pos = InSelectCursolPositionYData[m_currentCursolIndex];
			m_cursolUI->m_transform.m_localPosition = Vector3(0.0f, pos.positionY, 0.0f);
		}

		/** Bボタンで決定 */
		if (g_pad[0]->IsTrigger(enButtonB)) {
			if (m_currentCursolIndex == 0) {
				m_result = EnNextType::enNextType_Yes;
				/** TODO: たいとるに戻る処理 */
				m_result = EnNextType::enNextType_Yes;
			}
			else if (m_currentCursolIndex == 1) {
				m_result = EnNextType::enNextType_No;
			}
		}


		/** Aボタンで戻る */
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_result = EnNextType::enNextType_No;
		}
	}

	

	m_canvas->Update();
}

void SettingInSelect::Render(RenderContext& rc) {
	m_canvas->Render(rc);
}




/**************************************************/


PoseMenu::PoseMenu()
{
	m_canvas = std::make_shared<UICanvas>();
	m_canvas->CreateUI<UICanvas>();
	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);
	//m_canvas->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	
	m_image = m_canvas->CreateUI<UIImage>();
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	
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

	auto settingPanel = std::make_shared<SettingPosePanel>();
	settingPanel->SetOwner(this);
	m_posePanelList.push_back(settingPanel);
	
	auto nameListBase = std::make_shared<NameListBasePosePanel>();
	nameListBase->SetOwner(this);
	m_posePanelList.push_back(nameListBase);


	m_cursolPanel = std::make_shared<CursolPosePanel>();
	m_cursolPanel->SetOwner(this);
	m_posePanelList.push_back(m_cursolPanel);

	auto nameListPanel = std::make_shared<NameListPosePanel>();
	nameListPanel->SetOwner(this);
	m_posePanelList.push_back(nameListPanel);

	m_iconCursol = std::make_shared<IconCursol>();
	m_iconCursol->SetOwner(this);
	m_posePanelList.push_back(m_iconCursol);
}

PoseMenu::~PoseMenu() {
	if (m_inSelect) {
		DeleteGO(m_inSelect);
		m_inSelect = nullptr;
	}
	if (m_setting) {
		DeleteGO(m_setting);
		m_setting = nullptr;
	}
}

bool PoseMenu::Start() {
	//Init();
	return true;
}

void PoseMenu::Update() {
	m_canvas->Update();

	/** アニメーション更新 */
	if (m_canvasColorOpenAnimation
		&&!m_canvasColorOpenAnimation->IsCompleted()) {
		m_canvasColorOpenAnimation->Update();
	}
	if (m_canvasColorCloseAnimation
		&&!m_canvasColorCloseAnimation->IsCompleted()) {
		m_canvasColorCloseAnimation->Update();
	}

	//  /** メニューが閉じているときはスキップ */
	//  if (!m_isActived) {
	//  	return;
	//  }

	/** 確認UIが表示中の場合 */
	if (m_inSelect) {
		// 結果をチェック
		auto result = m_inSelect->GetResult();

		if (result == InSelect::Yes) {
			// アイテムを使用
			Inventory* inventory = Inventory::GetInstance();
			if (inventory && m_selectedItemIndex >= 0) {
				inventory->RemoveItem(m_selectedItemIndex);
				//TODO:プレイヤーへの回復処理などを追加
			}

			// 確認UIを削除
			DeleteGO(m_inSelect);
			m_inSelect = nullptr;
			m_selectedItemIndex = -1;
		}
		else if (result == InSelect::SelectResult::No) {
			// キャンセル - 確認UIを削除
			DeleteGO(m_inSelect);
			m_inSelect = nullptr;
			m_selectedItemIndex = -1;
		}
		// 確認UI表示中は他の操作を受け付けない
		return;
	}

	if (IsPoseActive()) {
		if (m_setting) {
			auto result = m_setting->GetResult();

			//if (result == Setting::SelectResult::ToMenu) {
			//	/** メニューにもどる要らない説濃厚 */
			//}

			if (result == Setting::SelectResult::ToTitle) {
				if (m_settingInSelect == nullptr) {
					m_settingInSelect = NewGO<SettingInSelect>(0, "settingInSelect");
					m_settingInSelect->SetOwner(this);
				}
				DeleteGO(m_setting);
				m_setting = nullptr;
			}
		}
	}
	

	if (IsPoseActive()) {
		/** SettingInSelectの結果を処理 */
		if (m_settingInSelect) {
			auto result = m_settingInSelect->GetResult();
			if (result == SettingInSelect::EnNextType::enNextType_Yes) {
				m_isRequestReturnToTitle = true;
				DeleteGO(m_settingInSelect);
				m_settingInSelect = nullptr;
			}
			else if (result == SettingInSelect::EnNextType::enNextType_No) {
				/** キャンセル Settingに戻る */
				DeleteGO(m_settingInSelect);
				m_settingInSelect = nullptr;
				m_setting = NewGO<Setting>(0, "setting");
				m_setting->SetOwner(this);
			}
		}
	}
	

	/** 通常のパネル更新 */
	for (const auto& p : m_posePanelList) {
		p->Update();
	}

	if (m_isContentfCursolEnable && m_iconCursol) {
		m_iconCursol->Update();
	}

	if (IsPoseActive()) {
		if (m_currentListIndex == 2) {
			if (m_setting == nullptr
				&& m_settingInSelect == nullptr) {
				m_setting = NewGO<Setting>(0, "setting");
				m_setting->SetOwner(this);
			}
		}

		else {
			/** 設定リスト以外に移動したとき */
			if (m_setting) {
				DeleteGO(m_setting);
				m_setting = nullptr;
			}
		}
	}
	

	if (IsPoseActive()) {
		/** アイテムの使用時の処理 */
	// アイテム一覧が表示されているとき(0番目と仮定)
		if (m_currentListIndex == 0) {
			//デバッグテスト：フラグを立てる
			m_isVisible = true;

			// Bボタンで使用
			if (g_pad[0]->IsTrigger(enButtonB)) {
				// カーソルが存在するなら
				if (m_iconCursol) {
					// カーソルの現在の位置を取得
					int targetIndex = m_iconCursol->GetCurrentIndex();

					// Inventoryから削除
					Inventory* inventory = Inventory::GetInstance();

					if (inventory) {
						ItemInfo* info = inventory->GetItemInfo(targetIndex);
						//空アイテムでなければ確認UIを表示
						if (info && info->m_type != enItemType_Empty) {
							m_selectedItemIndex = targetIndex;
							m_inSelect = NewGO<InSelect>(0, "inselect");
							m_inSelect->SetOwner(this);
						}
					}
				}
			}
		}
		else {
			m_isVisible = false;
		}
	}
	
}

void PoseMenu::Render(RenderContext& rc) {
	/** ポーズ画面、画面上に出てるので非表示にしています。*/
	m_canvas->Render(rc);

	for (const auto& p : m_posePanelList) {
		/** 常に描画されるもの/現在の選択パネルを描画 */
		if (p->ShouldAlwayRender()
			|| m_posePanelList[m_currentListIndex] == p) {
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
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 開く用のカラーイージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_canvasColorOpenAnimation = std::make_unique<ColorUIAnimation>(m_canvas, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 閉じる用のカラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_canvasColorCloseAnimation = std::make_unique<ColorUIAnimation>(m_canvas, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}

	for (const auto& p : m_posePanelList) {
		p->Init();
	}

	//  m_canvasColorOpenAnimation->Play();
	//  for (const auto& p : m_posePanelList) {
	//  	p->PlayOpenAnimation();
	//  }
}

void PoseMenu::Open()
{
	
	if (m_isActived) return;
	m_currentListIndex = 0;
	m_cursolPanel->ResetCursolIndex();

	if (m_setting) {
		m_setting->PlayOpenAnimation();
	}
	if (m_canvasColorOpenAnimation) {
		m_canvasColorOpenAnimation->Play();
	}
	for (const auto& p : m_posePanelList) {
		p->PlayOpenAnimation();
	}
	m_isActived = true;
}

void PoseMenu::Close() 
{
	if (!m_isActived) return;

	//m_currentListIndex = 0;

	if (m_setting) {
		m_setting->PlayCloseAnimation();
		//DeleteGO(m_setting);
		//m_setting = nullptr;
	}

	if (m_settingInSelect) {
		DeleteGO(m_settingInSelect);
		m_settingInSelect = nullptr;
	}

	if (m_inSelect) {
		DeleteGO(m_inSelect);
		m_inSelect = nullptr;
		m_selectedItemIndex = -1;
	}

	

	if (m_canvasColorCloseAnimation) {
		m_canvasColorCloseAnimation->Play();
	}
	for (const auto& p : m_posePanelList) {
		p->PlayCloseAnimation();
	}

	

	m_isActived = false;

	//   /** リストインデックスを0にリセット */
	//   m_currentListIndex = 0;
	//   
	//   /** カーソル位置もリセット */
	//   if (m_cursolPanel) {
	//   	m_cursolPanel->ResetCursolIndex();
	//   }
}

void PoseMenu::OnIconCursorIndexChanged(int newIndex)
{
	/** ItemPosePanelに通知 */
	if (!m_posePanelList.empty()) {
		auto itemPanel = std::dynamic_pointer_cast<ItemPosePanel>(m_posePanelList[0]);
		if (itemPanel) {
			itemPanel->OnCursolIndexChanged(newIndex);
		}
	}
}
