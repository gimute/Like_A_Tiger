#include "stdafx.h"
#include "PoseMenu.h"
#include "Inventory/Inventory.h"
#include "SaveManager.h"
#include "Actor/Player/Player.h"

namespace
{
	/** アイテムスロットの数 */
	constexpr int MAX_ITEM_SLOT_NUM = 20;

	constexpr const char* ITEMS_RANDOM_PATH = "Assets/modelData/items/itemsRandom.tkm";
	constexpr const char* USER_MANUAL_ITEM_PATH = "Assets/spriteData/UserManual/list_item.DDS";

	const float MIN_SCALE = 0.0000001f;
	const float MAX_SCALE = 1.0f;
	
	static constexpr float COOLDOWN_DURATION = 5.0f;
}

ItemPosePanel::ItemPosePanel() {
	m_image = m_canvas.CreateUI<UIImage>();
	m_frameImage = m_canvas.CreateUI<UIImage>();
	m_itemSlotList.clear();
	m_userManualImage = m_canvas.CreateUI<UIImage>();
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
	/** 開くようスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_openScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}

	/** 閉じる用アニメーション */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
	/** 閉じる用スケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_closeScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** スロット用スケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_scaleitemAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseInOut, timeList, scaleList);
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

	m_userManualImage->Init(USER_MANUAL_ITEM_PATH, 909.0f, 59.0f);
	m_userManualImage->m_transform.m_localPosition = Vector3(200.0f, -420.0f, 0.0f);
	m_userManualImage->m_transform.m_localScale = Vector3(0.9f, 0.9f, 0.0f);
}

void ItemPosePanel::Update() {
	m_image->Update();
	m_canvas.Update();

	if (m_colorAnimation) {
		m_colorAnimation->Update();
	}
	if (m_closeColorAnimation) {
		m_closeColorAnimation->Update();
	}
	if (m_openScaleAnimation) {
		m_openScaleAnimation->Update();
	}
	if (m_closeScaleAnimation) {
		m_closeScaleAnimation->Update();
	}
	if (m_scaleitemAnimation) { m_scaleitemAnimation->Update(); }

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
		slot->Update();
	}
}

void ItemPosePanel::Render(RenderContext& rc) {
	m_image->Render(rc);
	for (auto& slot : m_itemSlotList) {
		slot->Render(rc);
		if (m_isUserManualVisible) {
			m_userManualImage->Render(rc);
		}
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
}

void ItemPosePanel::OnCursolIndexChanged(int newIndex) {
	/** 前のスロットのアニメーションを停止 */
	if (m_previousCursorIndex >= 0
		&& m_previousCursorIndex < static_cast<int>(m_itemSlotList.size())) 
	{
		auto& prevSlot = m_itemSlotList[m_previousCursorIndex];
		if (prevSlot && prevSlot->GetItem()) {
			prevSlot->GetItem()->StopScaleItemAnimation();
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
				newSlot->GetItem()->PlayScaleItemAnimation();
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
	if (m_selectedItemScaleAnimation) {
		m_selectedItemScaleAnimation->Update();
	}

	/** スロット用スケールアニメーション */
	if (m_scaleSlotAnimation) {
		m_scaleSlotAnimation->Update();
	}
	/** スロット用縮小スケールアニメーション */
	if(m_scaleSlotSmallAnimation){
		m_scaleSlotSmallAnimation->Update();
	}
}

void RecoveryItem::Render(RenderContext& rc) {
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
	/** アイテム選択時のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.1f,0.1f };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f)};
		m_selectedItemScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseInOut, timeList, scaleList);
	}
	/** スロット用スケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_scaleSlotAnimation = std::make_unique<ScaleUIAnimation>(m_canvas, false, EasingType::EaseInOut, timeList, scaleList);
	}
	/** スロット用縮小スケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_scaleSlotSmallAnimation = std::make_unique<ScaleUIAnimation>(m_canvas, false, EasingType::EaseInOut, timeList, scaleList);
	}
}

void RecoveryItem::Use(Player* p)
{
}




/**************************************************/


SmallRecoveryItem::SmallRecoveryItem() {
	m_healAmount = 20.0f;
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

	/** アイテム選択時のスケールアニメーション */
	if (m_selectedItemScaleAnimation) {
		m_selectedItemScaleAnimation->Update();
	}

	/** スロット用スケールアニメーション */
	if (m_scaleSlotAnimation) {
		m_scaleSlotAnimation->Update();
	}
	/** スロット用縮小スケールアニメーション */
	if (m_scaleSlotSmallAnimation) {
		m_scaleSlotSmallAnimation->Update();
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
	m_healAmount = 40.0f;
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

	/** アイテム選択時のスケールアニメーション */
	if (m_selectedItemScaleAnimation) {
		m_selectedItemScaleAnimation->Update();
	}
	/** スロット用スケールアニメーション */
	if (m_scaleSlotAnimation) {
		m_scaleSlotAnimation->Update();
	}
	/** スロット用縮小スケールアニメーション */
	if (m_scaleSlotSmallAnimation) {
		m_scaleSlotSmallAnimation->Update();
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
	m_healAmount = 60.0f;
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

	/** アイテム選択時のスケールアニメーション */
	if (m_selectedItemScaleAnimation) {
		m_selectedItemScaleAnimation->Update();
	}
	/** スロット用スケールアニメーション */
	if (m_scaleSlotAnimation) {
		m_scaleSlotAnimation->Update();
	}
	/** スロット用縮小スケールアニメーション */
	if (m_scaleSlotSmallAnimation) {
		m_scaleSlotSmallAnimation->Update();
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

	/** スロット用スケールアニメーション */
	if (m_scaleSlotAnimation) {
		m_scaleSlotAnimation->Update();
	}
	/** スロット用縮小スケールアニメーション */
	if (m_scaleSlotSmallAnimation) {
		m_scaleSlotSmallAnimation->Update();
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
namespace {

	constexpr const char* USER_MANUAL_PATH = "Assets/spriteData/UserManual/list_userManual.DDS";
	constexpr const char* BUTTON_L1_PATH = "Assets/spriteData/PoseMenu/Controller/push_L1.DDS";
	constexpr const char* BUTTON_R1_PATH = "Assets/spriteData/PoseMenu/Controller/push_R1.DDS";
	constexpr const char* BUTTON_CIRCLE_PATH = "Assets/spriteData/PoseMenu/Controller/push_R_Stick.DDS";
	constexpr const char* CONTROLLER_BASE_PATH = "Assets/spriteData/PoseMenu/Controller/ControllerBase.DDS";
	constexpr const char* CONTROLLER_BUTTON_PATH = "Assets/spriteData/PoseMenu/Controller/ControllerButton.DDS";
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
	if (m_openImageColorAnimation) {
		m_openImageColorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}
	if (m_openScaleAnimation) {
		m_openScaleAnimation->Update();
	}
	if (m_closeScaleAnimation) {
		m_closeScaleAnimation->Update();
	}
	if (m_buttonL1_OpenColorAnimation) {
		m_buttonL1_OpenColorAnimation->Update();
	}
	if (m_buttonR1_OpenColorAnimation) {
		m_buttonR1_OpenColorAnimation->Update();
	}
	if (m_openControllerColorAnimation) {
		m_openControllerColorAnimation->Update();
	}
	if (m_closeControllerColorAnimation) {
		m_closeControllerColorAnimation->Update();
	}
	if (m_openControllerScaleAnimation) {
		m_openControllerScaleAnimation->Update();
	}
	if (m_closeControllerScaleAnimation) {
		m_closeControllerScaleAnimation->Update();
	}

	/** ボタンのアニメーションを更新 */
	for (auto& anim : m_buttonAnimations) {
		if (anim) {
			anim->Update();
		}
	}
	//  if (m_buttonL1_CloseColorAnimation) {
	//  	m_buttonL1_CloseColorAnimation->Update();
	//  }
	
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
	m_canvas.m_transform.m_localScale = Vector3(1.3f, 1.3f, 1.3f);
	//** 操作説明パネルの初期化 */
	m_image->Init(initData->textFilePath, initData->frameSize_x, initData->frameSize_y);
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 開く用カラーイージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_openImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 開く用スケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_openScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 閉じる用カラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
	/** 閉じる用スケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_closeScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}

	m_userManual = m_canvas.CreateUI<UIImage>();
	m_userManual->Init(USER_MANUAL_PATH, 350.0f, 90.0f);
	m_userManual->m_transform.m_localPosition = Vector3(400.0f, -325.0f, 0.0f);
	m_userManual->m_transform.m_localScale = Vector3(0.4f, 0.4f, 0.0f);

	m_buttonL1 = m_canvas.CreateUI<UIImage>();
	m_buttonL1->Init(BUTTON_L1_PATH, 60.0f, 34.0f);
	m_buttonL1->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_buttonL1->m_transform.m_localPosition = Vector3(-83.0f, 91.5f, 0.0f);
	m_buttonImages[static_cast<int>(ButtonType::L1)] = m_buttonL1;

	m_buttonR1 = m_canvas.CreateUI<UIImage>();
	m_buttonR1->Init(BUTTON_R1_PATH, 55.0f, 30.0f);
	m_buttonR1->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_buttonR1->m_transform.m_localPosition = Vector3(80.0f, 92.0f, 0.0f);
	m_buttonImages[static_cast<int>(ButtonType::R1)] = m_buttonR1;
	//
	m_buttonRStick = m_canvas.CreateUI<UIImage>();
	m_buttonRStick->Init(BUTTON_CIRCLE_PATH, 22.0f, 22.0f);
	m_buttonRStick->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_buttonRStick->m_transform.m_localPosition = Vector3(39.0f, -33.0f, 0.0f);
	m_buttonImages[static_cast<int>(ButtonType::RStick)] = m_buttonRStick;
	//
	m_buttonLStick = m_canvas.CreateUI<UIImage>();
	m_buttonLStick->Init(BUTTON_CIRCLE_PATH, 22.5f, 22.5f);
	m_buttonLStick->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_buttonLStick->m_transform.m_localPosition = Vector3(-36.0f, -33.0f, 0.0f);
	m_buttonImages[static_cast<int>(ButtonType::LStick)] = m_buttonLStick;
	//
	m_buttonA = m_canvas.CreateUI<UIImage>();
	m_buttonA->Init(BUTTON_CIRCLE_PATH, 22.5f, 22.5f);
	m_buttonA->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_buttonA->m_transform.m_localPosition = Vector3(75.0f, -10.0f, 0.0f);
	m_buttonImages[static_cast<int>(ButtonType::A)] = m_buttonA;
	//
	m_buttonB = m_canvas.CreateUI<UIImage>();
	m_buttonB->Init(BUTTON_CIRCLE_PATH, 22.5f, 22.5f);
	m_buttonB->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_buttonB->m_transform.m_localPosition = Vector3(95.0f, 10.0f, 0.0f);
	m_buttonImages[static_cast<int>(ButtonType::B)] = m_buttonB;
	//
	m_buttonX = m_canvas.CreateUI<UIImage>();
	m_buttonX->Init(BUTTON_CIRCLE_PATH, 22.5f, 22.5f);
	m_buttonX->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_buttonX->m_transform.m_localPosition = Vector3(55.0f, 10.0f, 0.0f);
	m_buttonImages[static_cast<int>(ButtonType::X)] = m_buttonX;
	//
	m_buttonY = m_canvas.CreateUI<UIImage>();
	m_buttonY->Init(BUTTON_CIRCLE_PATH, 22.5f, 22.5f);
	m_buttonY->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	m_buttonY->m_transform.m_localPosition = Vector3(75.0f, 25.0f, 0.0f);
	m_buttonImages[static_cast<int>(ButtonType::Y)] = m_buttonY;

	m_controllerButton = m_canvas.CreateUI<UIImage>();
	m_controllerButton->Init(CONTROLLER_BUTTON_PATH, 640.0f, 400.0f);
	m_controllerButton->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	/** 開く用カラーイージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_openControllerColorAnimation = std::make_unique<ColorUIAnimation>(m_controllerButton, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 開く用スケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_openControllerScaleAnimation = std::make_unique<ScaleUIAnimation>(m_controllerButton, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 閉じる用カラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeControllerColorAnimation = std::make_unique<ColorUIAnimation>(m_controllerButton, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
	/** 閉じる用スケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_closeControllerScaleAnimation = std::make_unique<ScaleUIAnimation>(m_controllerButton, false, EasingType::EaseIn, timeList, scaleList);
	}

	//アニメーションを配列に登録
	for (int i = 0; i < static_cast<int>(ButtonType::Count); ++i) {
		if (m_buttonImages[i]) {
			std::vector<float> timeList = { 0.5f,0.5f };
			std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };

			m_buttonAnimations[i] = std::make_unique<ColorUIAnimation>(
				m_buttonImages[i], true, EasingType::EaseInOut, timeList, colorList
			);
		}
	}
}



/**************************************************/


//  namespace {
//  	constexpr const char* PUSH_L1_PATH = "Assets/spriteData/PoseMenu/Controller/push_L1.DDS";
//  	constexpr const char* PUSH_R1_PATH = "Assets/spriteData/PoseMenu/Controller/push_R1.DDS";
//  	constexpr const char* PUSH_L_STICK_PATH = "Assets/spriteData/PoseMenu/Controller/push_L_Stick.DDS";
//  	constexpr const char* PUSH_R_STICK_PATH = "Assets/spriteData/PoseMenu/Controller/push_R_Stick.DDS";
//  	constexpr const char* PUSH_A_PATH = "Assets/spriteData/PoseMenu/Controller/push_A.DDS";
//  	constexpr const char* PUSH_B_PATH = "Assets/spriteData/PoseMenu/Controller/push_B.DDS";
//  	constexpr const char* PUSH_X_PATH = "Assets/spriteData/PoseMenu/Controller/push_X.DDS";
//  	constexpr const char* PUSH_Y_PATH = "Assets/spriteData/PoseMenu/Controller/push_Y.DDS";
//  }
//  
//  ManualControlButtonUI::ManualControlButtonUI() 
//  {
//  }
//  
//  ManualControlButtonUI::~ManualControlButtonUI()
//  {
//  }
//  
//  bool ManualControlButtonUI::Start() 
//  {
//  	m_pushUI_L1 = std::make_unique<UIImage>();
//  	m_pushUI_L1->Init(PUSH_L1_PATH, 60.0f, 30.0f);
//  	m_pushUI_L1->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
//  
//  	//m_pushUI_R1
//  	//m_pushUI_RStick
//  	//m_pushUI_LStick
//  	//m_pushUI_A
//  	//m_pushUI_B
//  	//m_pushUI_X
//  	//m_pushUI_Y
//  
//  	return true;
//  }
//  
//  void ManualControlButtonUI::Update() 
//  {
//  	if (m_pushUI_L1) {
//  		m_pushUI_L1->Update();
//  	}
//  }
//  
//  void ManualControlButtonUI::Render(RenderContext& rc) 
//  {
//  	m_pushUI_L1->Render(rc);
//  }




/**************************************************/
namespace {
	constexpr const char* SETTING_USER_MANUAL = "Assets/spriteData/UserManual/list_setting.DDS";
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
	if (m_openImageColorAnimation) {
		m_openImageColorAnimation->Update();
	}
	if (m_closeImageColorAnimation) {
		m_closeImageColorAnimation->Update();
	}
	if (m_openScaleAnimation) {
		m_openScaleAnimation->Update();
	}
	if (m_closeScaleAnimation) {
		m_closeScaleAnimation->Update();
	}
	m_canvas.Update();
}

void SettingPosePanel::Render(RenderContext& rc) {
	m_image->Render(rc);
	if (m_isUserManualVisible) {
		m_userManualImage->Render(rc);
	}
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
	/** 開く用のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_openScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 閉じる時のカラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
	/** 閉じる用のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_closeScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}

	m_userManualImage = m_canvas.CreateUI<UIImage>();
	//m_userManualImage = std::make_unique<UIImage>();
	m_userManualImage->Init(SETTING_USER_MANUAL, 850.0f, 90.0f);
	m_userManualImage->m_transform.m_localPosition = Vector3(375.0f, -420.0f, 0.0f);
	m_userManualImage->m_transform.m_localScale = Vector3(0.55f, 0.55f, 0.0f);

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
	if (m_owner->IsSettingInSelectActive()
		||m_owner->IsActivedVolumeAdjustment()
		||m_owner->IsInSelectActive()) {
		m_canvas.m_transform.UpdateTransform();
		m_canvas.Update();
		m_image->Update();

		if (m_imageColorAnimation) {
			m_imageColorAnimation->Update();
		}
		if (m_closeImageColorAnimation) {
			m_closeImageColorAnimation->Update();
		}
		if (m_openScaleAnimation) {
			m_openScaleAnimation->Update();
		}
		if (m_closeScaleAnimation) {
			m_closeScaleAnimation->Update();
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
	//  {
	//  	std::vector<float> timeList = { 1.0f };
	//  	std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
	//  	m_imageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, timeList, colorList);
	//  }
	//  /** 開く用のスケールアニメーション */
	//  {
	//  	std::vector<float> timeList = { 0.25f,0.25f, };
	//  	std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
	//  	m_openScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	//  }
	/** 閉じる用のカラーイージング */
	//  {
	//  	std::vector<float> closeTimeList = { 0.01f };
	//  	std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
	//  	m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	//  }
	//  /** 閉じる用のアニメーション */
	//  {
	//  	std::vector<float> timeList = { 0.25f,0.25f, };
	//  	std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
	//  	m_closeScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	//  }
	//m_canvas.m_transform.m_localPosition = Vector3(-3.0f, -50.0f, 0.0f);
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
	/** TODO:Updateメソッドに要素が多すぎる責任が複数あるので分けたい */

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
	if (m_openScaleAnimation) {
		m_openScaleAnimation->Update();
	}
	if (m_closeScaleAnimation) {
		m_closeScaleAnimation->Update();
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
	/**  開く時のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_openScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 閉じる用のカラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeImageColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
	/** 閉じる用のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_closeScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
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
	if (m_openScaleAnimation) {
		m_openScaleAnimation->Update();
	}
	if (m_closeScaleAnimation) {
		m_closeScaleAnimation->Update();
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
	/** 開くときのスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_openScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 閉じる用のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_closeScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
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
	if (m_openScaleAnimation) {
		m_openScaleAnimation->Update();
	}
	if (m_closeScaleAnimation) {
		m_closeScaleAnimation->Update();
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
	/** 開く用のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_openScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 閉じる用の色イージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeColorAnimation = std::make_unique<ColorUIAnimation>(m_image, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
	/** 閉じる用のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_closeScaleAnimation = std::make_unique<ScaleUIAnimation>(m_image, false, EasingType::EaseIn, timeList, scaleList);
	}
}




/**************************************************/


namespace {
	constexpr const char* INSELECT_USER_MANUAL_PATH = "Assets/spriteData/UserManual/list_check.DDS";
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
	/** 開いた時のアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_openScaleAnimation = std::make_unique<ScaleUIAnimation>(m_canvas, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 閉じた時のアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_openScaleAnimation = std::make_unique<ScaleUIAnimation>(m_canvas, false, EasingType::EaseIn, timeList, scaleList);
	}

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

	m_userManualUI = m_canvas->CreateUI<UIImage>();
	m_userManualUI->Init(INSELECT_USER_MANUAL_PATH, 850.0f, 90.0f);
	m_userManualUI->m_transform.m_localPosition = Vector3(555.0f, -420.0f,0.0f);
	m_userManualUI->m_transform.m_localScale = Vector3(0.55f, 0.55f, 0.0f);

	return true;
}

void InSelect::Update() 
{
	//アクティブでなければ更新しない
	if (!m_isActived) return;

	m_cursolColorAnimaiton->Update();
	if (m_openScaleAnimation) { m_openScaleAnimation->Update(); }
	if (m_closeScaleAnimation) { m_closeScaleAnimation->Update(); }

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
	/** アクティブでなければ描画しない */
	if (!m_isActived) return;
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

	//	/** 開った時のアニメーション */
	//	std::vector<float> openTimeList = { 1.0f };
	//	std::vector<Vector4> openColorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
	//	auto openAnim = std::make_unique<ColorUIAnimation>(baseUI, false, EasingType::EaseIn, openTimeList, openColorList);
	//	
		/** 閉じた時のアニメーション */
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		auto closeAnim = std::make_unique<ColorUIAnimation>(baseUI, false, EasingType::EaseIn, closeTimeList, closeColorList);

		m_baseUIList.push_back(baseUI);
		//m_openBaseUIColorAnimationList.push_back(std::move(openAnim));
		m_closeBaseUIColorAnimationList.push_back(std::move(closeAnim));
	}

	m_cursolUI = m_canvas->CreateUI<UIImage>();
	m_cursolUI->Init(SETTING_CURSOL_PATH, 300.0f, 50.0f);
	m_cursolUI->m_transform.m_localPosition = Vector3(TEXT_TO_MENU_POSITION.x, TEXT_TO_MENU_POSITION.y, 0.0f);
	/** 色のイージング */
	{
		std::vector<float> timeList = { 0.5f,0.3f };
		std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_cursolUIColorAnimation = std::make_unique<ColorUIAnimation>(m_cursolUI, true, EasingType::EaseIn, timeList, colorList);
		m_cursolUIColorAnimation->Play();
	}
	//  /** 開った時のアニメーション */
	//  {
	//  	std::vector<float> timeList = { 1.0f };
	//  	std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
	//  	m_openCursolUIColorAnimation = std::make_unique<ColorUIAnimation>(m_cursolUI, false, EasingType::EaseIn, timeList, colorList);
	//  }
	/** 閉じた時のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeCursolUIColorAnimation = std::make_unique<ColorUIAnimation>(m_cursolUI, false, EasingType::EaseIn, timeList, colorList);
	}

	m_textToTitleUI = m_canvas->CreateUI<UIImage>();
	m_textToTitleUI->Init(RETURN_TO_TITLE_PATH, 150.0f, 25.0f);
	m_textToTitleUI->m_transform.m_localPosition = Vector3(TEXT_TO_TITLE_POSITION.x, TEXT_TO_TITLE_POSITION.y, 0.0f);
	///** 開った時のアニメーション */
	//{
	//	std::vector<float> timeList = { 1.0f };
	//	std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
	//	m_openTextToTitleUIColorAnimation = std::make_unique<ColorUIAnimation>(m_textToTitleUI, false, EasingType::EaseIn, timeList, colorList);
	//}
	/** 閉じる時のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4::White, Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_closeTextToTitleUIColorAnimation = std::make_unique<ColorUIAnimation>(m_textToTitleUI, false, EasingType::EaseIn, timeList, colorList);
	}

	m_textVolumeUI = m_canvas->CreateUI<UIImage>();
	m_textVolumeUI->Init(VOLUME_PATH, 100.0f, 25.0f);
	m_textVolumeUI->m_transform.m_localPosition = Vector3(TEXT_TO_TITLE_POSITION.x, TEXT_VOLUME_POSITION.y, 0.0f);
	//  /** 開った時のアニメーション */
	//  {
	//  	std::vector<float> timeList = { 1.0f };
	//  	std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
	//  	m_openTextVolumeUIColorAnimation = std::make_unique<ColorUIAnimation>(m_textVolumeUI, false, EasingType::EaseIn, timeList, colorList);
	//  }
	/** 閉じた時のアニメーション */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f), };
		m_closeTextVolumeUIColorAnimation = std::make_unique<ColorUIAnimation>(m_textVolumeUI, false, EasingType::EaseIn, timeList, colorList);
	}
	//  /** 開いた時の拡大アニメーション */
	//  {
	//  	std::vector<float> timeList = { 0.25f,0.25f };
	//  	std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
	//  	m_openScaleCanvasAnimation = std::make_unique<ScaleUIAnimation>(m_canvas, false, EasingType::EaseIn, timeList, scaleList);
	//  }
	/** 閉じた時の縮小アニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_closeScaleCanvasAnimation = std::make_unique<ScaleUIAnimation>(m_canvas, false, EasingType::EaseIn, timeList, scaleList);
	}
	return true;
}

void Setting::Update() {
	if (!m_isActived) return;

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
	/** 開く時のアニメーション更新 */
	if (m_openCursolUIColorAnimation) {
		m_openCursolUIColorAnimation->Update();
	}
	if (m_openTextToTitleUIColorAnimation) {
		m_openTextToTitleUIColorAnimation->Update();
	}
	if (m_openTextVolumeUIColorAnimation) {
		m_openTextVolumeUIColorAnimation->Update();
	}

	if (m_cursolUIColorAnimation) {
		m_cursolUIColorAnimation->Update();
	}
	//  if (m_openScaleCanvasAnimation) {
	//  	m_openScaleCanvasAnimation->Update();
	//  }
	
	/** 閉じる時のアニメーション更新 */
	if (m_closeCursolUIColorAnimation) {
		m_closeCursolUIColorAnimation->Update();
	}
	if (m_closeTextToTitleUIColorAnimation) {
		m_closeTextToTitleUIColorAnimation->Update();
	}
	if (m_closeTextVolumeUIColorAnimation) {
		m_closeTextVolumeUIColorAnimation->Update();
	}
	if (m_closeScaleCanvasAnimation) {
		m_closeScaleCanvasAnimation->Update();
	}


	if (m_owner->IsPoseActive()) {
		if (g_pad[0]->IsTrigger(enButtonLB1)) {
			m_currentSettingCursolIndex--;
			if (m_currentSettingCursolIndex < 0) {
				m_currentSettingCursolIndex = 0;
			}
		}

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
				m_isVisible = true;
			}
			else if (m_currentSettingCursolIndex == 1) {
				m_result = SelectResult::ToTitle;
				m_isVisible = true;
			}
			else {
				m_result = SelectResult::None;
			}
		}
	}
	m_canvas->Update();
}

void Setting::Render(RenderContext& rc) {
	if (!m_isActived) return;
	m_canvas->Render(rc);
}

void Setting::SetActived(bool isActived){
	m_isActived = isActived;
	if (isActived) {
		m_result = SelectResult::None;
		m_currentSettingCursolIndex = 0;
		m_cursolUI->m_transform.m_localPosition = Vector3(
			settingCursolPositionYData[0].positionX,
			settingCursolPositionYData[0].positionY,
			0.0f
		);
		PlayOpenAnimation();
	}
	//else {
	//	PlayCloseAnimation();
	//}
	
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

	m_cursolUI = m_canvas->CreateUI<UIImage>();
	m_cursolUI->Init(INSELECT_CURSOL_PATH, 300.0f, 50.0f);
	m_cursolUI->m_transform.m_localPosition = Vector3(0.0f, -20.0f, 0.0f);
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

	m_userManualUI = m_canvas->CreateUI<UIImage>();
	m_userManualUI->Init(INSELECT_USER_MANUAL_PATH, 850.0f, 90.0f);
	m_userManualUI->m_transform.m_localPosition = Vector3(555.0f, -420.0f, 0.0f);
	m_userManualUI->m_transform.m_localScale = Vector3(0.55f, 0.55f, 0.0f);

	return true;
}

void SettingInSelect::Update() {
	if (!m_isActived) return; 

	m_cursolUIColorAnimation->Update();

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
	if (!m_isActived) return;
	m_canvas->Render(rc);
}


/**************************************************/
namespace {
	constexpr const char* VOLUME_ADJUSTMENT_USER_MANUAL_PATH = "Assets/spriteData/UserManual/list_volume.DDS";

	/** 音量のノブ座標Xの数値 */
	const float VOLUME_POS_X_0 = -133.0f;
	const float VOLUME_POS_X_1 = -87.0f;
	const float VOLUME_POS_X_2 = -45.0f;
	const float VOLUME_POS_X_3 = -5.0f;
	const float VOLUME_POS_X_4 = 40.0f;
	const float VOLUME_POS_X_5 = 82.0f;
	const float VOLUME_POS_X_6 = 122.0f;
	const float VOLUME_POS_X_7 = 164.0f;
	const float VOLUME_POS_X_8 = 207.0f;
	const float VOLUME_POS_X_9 = 247.0f;
	const float VOLUME_POS_X_10 = 289.0f;


	/** 音量の増減による座標Xの数値*/
	const float VOLUME_AMOUNT_POS_X_0 = 0.0f;
	const float VOLUME_AMOUNT_POS_X_1 = -103.0f;
	const float VOLUME_AMOUNT_POS_X_2 = -87.0f;
	const float VOLUME_AMOUNT_POS_X_3 = -65.0f;
	const float VOLUME_AMOUNT_POS_X_4 = -45.0f;
	const float VOLUME_AMOUNT_POS_X_5 = -21.0f;
	const float VOLUME_AMOUNT_POS_X_6 = 0.0f;
	const float VOLUME_AMOUNT_POS_X_7 = 21.0f;
	const float VOLUME_AMOUNT_POS_X_8 = 42.0f;
	const float VOLUME_AMOUNT_POS_X_9 = 63.0f;
	const float VOLUME_AMOUNT_POS_X_10 = 80.0f;

	/** 音量の拡大率 */
	const float VOLUME_AMOUNT_SCALE_X_0 = 0.0f;
	const float VOLUME_AMOUNT_SCALE_X_1 = 1.1f;
	const float VOLUME_AMOUNT_SCALE_X_2 = 1.9f;
	const float VOLUME_AMOUNT_SCALE_X_3 = 2.5f;
	const float VOLUME_AMOUNT_SCALE_X_4 = 3.3f;
	const float VOLUME_AMOUNT_SCALE_X_5 = 4.4f;
	const float VOLUME_AMOUNT_SCALE_X_6 = 5.2f;
	const float VOLUME_AMOUNT_SCALE_X_7 = 6.0f;
	const float VOLUME_AMOUNT_SCALE_X_8 = 6.8f;
	const float VOLUME_AMOUNT_SCALE_X_9 = 7.6f;
	const float VOLUME_AMOUNT_SCALE_X_10 = 8.1f;

	/** カーソルの座標 */
	const float TEXT_BGM_POS_Y = 78.0f;
	const float TEXT_SE_POS_Y = -116.0f;

	/** 音量の１増減 */
	const float VOLUME_STEP = 0.1f;
}


/**  */
const VolumeAdjustment::PosXData m_volumePosXData[] = {
	{VOLUME_POS_X_0},
	{VOLUME_POS_X_1},
	{VOLUME_POS_X_2},
	{VOLUME_POS_X_3},
	{VOLUME_POS_X_4},
	{VOLUME_POS_X_5},
	{VOLUME_POS_X_6},
	{VOLUME_POS_X_7},
	{VOLUME_POS_X_8},
	{VOLUME_POS_X_9},
	{VOLUME_POS_X_10},
};

const VolumeAdjustment::PosXData m_volumeAmountPosXData[] = {
	{VOLUME_AMOUNT_POS_X_0},
	{VOLUME_AMOUNT_POS_X_1},
	{VOLUME_AMOUNT_POS_X_2},
	{VOLUME_AMOUNT_POS_X_3},
	{VOLUME_AMOUNT_POS_X_4},
	{VOLUME_AMOUNT_POS_X_5},
	{VOLUME_AMOUNT_POS_X_6},
	{VOLUME_AMOUNT_POS_X_7},
	{VOLUME_AMOUNT_POS_X_8},
	{VOLUME_AMOUNT_POS_X_9},
	{VOLUME_AMOUNT_POS_X_10},
};

const VolumeAdjustment::PosXData m_volumeAmountScaleXData[] = {
	{VOLUME_AMOUNT_SCALE_X_0},
	{VOLUME_AMOUNT_SCALE_X_1},
	{VOLUME_AMOUNT_SCALE_X_2},
	{VOLUME_AMOUNT_SCALE_X_3},
	{VOLUME_AMOUNT_SCALE_X_4},
	{VOLUME_AMOUNT_SCALE_X_5},
	{VOLUME_AMOUNT_SCALE_X_6},
	{VOLUME_AMOUNT_SCALE_X_7},
	{VOLUME_AMOUNT_SCALE_X_8},
	{VOLUME_AMOUNT_SCALE_X_9},
	{VOLUME_AMOUNT_SCALE_X_10},
};

const VolumeAdjustment::PosXData m_textCursolPosYData[] = {
	{TEXT_BGM_POS_Y},
	{TEXT_SE_POS_Y},
};


VolumeAdjustment::VolumeAdjustment() 
{
}

VolumeAdjustment::~VolumeAdjustment()
{
}

bool VolumeAdjustment::Start()
{
	const auto& bgmKnobPos = m_volumePosXData[m_currentVolumeIndex];
	const auto& bgmAmountPos = m_volumeAmountPosXData[m_currentVolumeIndex];
	const auto& bgmAmountScale = m_volumeAmountScaleXData[m_currentVolumeIndex];

	const auto& seKnobPos = m_volumePosXData[m_currentSEVolumeIndex];
	const auto& seAmountPos = m_volumeAmountPosXData[m_currentSEVolumeIndex];
	const auto& seAmoutScale = m_volumeAmountScaleXData[m_currentSEVolumeIndex];

	const auto& cursolPos = m_textCursolPosYData[m_currentResultIndex];

	/** UIキャンバス */
	m_canvas = std::make_unique<UICanvas>();
	m_canvas->m_transform.m_localPosition = Vector3(180.0f, 0.0f, 0.0f);
	m_canvas->m_transform.m_localScale = Vector3(0.7f, 0.7f, 0.7f);
	/**  */
	m_baseUI = m_canvas->CreateUI<UIImage>();
	m_baseUI->Init("Assets/spriteData/PoseMenu/VolumeAdjustment/volume_base.DDS", 942, 585);
	m_baseUI->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);

	m_bgmAmountUI = m_canvas->CreateUI<UIImage>();
	m_bgmAmountUI->Init("Assets/spriteData/PoseMenu/VolumeAdjustment/volume_amount.DDS", 52, 16);
	m_bgmAmountUI->m_transform.m_localPosition = Vector3(bgmAmountPos.m_positionX, 83.0f, 0.0f);
	m_bgmAmountUI->m_transform.m_localScale = Vector3(bgmAmountScale.m_positionX, 1.3f, 1.0f);

	
	m_seAmountUI = m_canvas->CreateUI<UIImage>();
	m_seAmountUI->Init("Assets/spriteData/PoseMenu/VolumeAdjustment/volume_amount.DDS", 52, 16);
	m_seAmountUI->m_transform.m_localPosition = Vector3(seAmountPos.m_positionX, -121.0f, 0.0f);
	m_seAmountUI->m_transform.m_localScale = Vector3(seAmoutScale.m_positionX, 1.3f, 1.0f);


	m_listCursolUI = m_canvas->CreateUI<UIImage>();
	m_listCursolUI->Init("Assets/spriteData/PoseMenu/VolumeAdjustment/volume_listCursol.DDS", 233, 50);
	m_listCursolUI->m_transform.m_localPosition = Vector3(-320.0f, cursolPos.m_positionX, 0.0f);
	{
		std::vector<float> timeList = { 0.3f,0.5f };
		std::vector<Vector4> colorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_listCursolUIColorAnimation = std::make_unique<ColorUIAnimation>(m_listCursolUI, true, EasingType::EaseInOut, timeList, colorList);
		m_listCursolUIColorAnimation->Play();
	}

	m_frameBarUI = m_canvas->CreateUI<UIImage>();
	m_frameBarUI->Init("Assets/spriteData/PoseMenu/VolumeAdjustment/volume_frontBar.DDS", 650, 269);
	m_frameBarUI->m_transform.m_localPosition = Vector3(-32.0f,-20.0f, 0.0f);

	/** ０めもり⇒ -135,0f  1めもり⇒50の間隔*/
	m_bgmKnobUI = m_canvas->CreateUI<UIImage>();
	m_bgmKnobUI->Init("Assets/spriteData/PoseMenu/VolumeAdjustment/volume_knob.DDS", 33, 37);
	m_bgmKnobUI->m_transform.m_localPosition = Vector3(bgmKnobPos.m_positionX, 83.0f, 0.0f);

	m_seKnobUI = m_canvas->CreateUI<UIImage>();
	m_seKnobUI->Init("Assets/spriteData/PoseMenu/VolumeAdjustment/volume_knob.DDS", 33, 37);
	m_seKnobUI->m_transform.m_localPosition = Vector3(seKnobPos.m_positionX, -121.0f, 0.0f);

	m_userManualUI = m_canvas->CreateUI<UIImage>();
	m_userManualUI->Init(VOLUME_ADJUSTMENT_USER_MANUAL_PATH, 797.0f, 59.0f);
	m_userManualUI->m_transform.m_localPosition = Vector3(465.0f, -600.0f, 0.0f);
	m_userManualUI->m_transform.m_localScale = Vector3(1.0f, 1.0f, 0.0f);

	return true;
}

void VolumeAdjustment::Update()
{
	if (!m_isActived) { return; }

	if (m_owner->IsPoseActive()) {
		
		if (g_pad[0]->IsTrigger(enButtonDown)) {
			m_currentResultIndex++;
			if (m_currentResultIndex > 1) {
				m_currentResultIndex = 1;
			}
		}
		if (g_pad[0]->IsTrigger(enButtonUp)) {
			m_currentResultIndex--;
			if (m_currentResultIndex < 0) {
				m_currentResultIndex = 0;
			}
		}

		const auto& cursolPos = m_textCursolPosYData[m_currentResultIndex];
		if (m_currentResultIndex == 0) {
			m_volumeResult = enNextType_BGM;
			m_listCursolUI->m_transform.m_localPosition = Vector3(-320.0f, cursolPos.m_positionX, 0.0f);
		}
		else if (m_currentResultIndex == 1) {
			m_volumeResult = enNextType_SE;
			m_listCursolUI->m_transform.m_localPosition = Vector3(-320.0f, cursolPos.m_positionX, 0.0f);
		}

		/** この部分を引数ありの関数にまとめられそう！ */
		if (m_volumeResult == enNextType_BGM) {
			if (g_pad[0]->IsTrigger(enButtonRB1)) {
				m_currentVolumeIndex++;
				m_bgmAmount += VOLUME_STEP;
				
				if (m_currentVolumeIndex > 10) {
					m_currentVolumeIndex = 10;
					m_bgmAmount = 1.0f;
				}
			}
			if (g_pad[0]->IsTrigger(enButtonLB1)) {
				m_currentVolumeIndex--;
				m_bgmAmount -= VOLUME_STEP;
				if (m_currentVolumeIndex < 0) {
					m_currentVolumeIndex = 0;
					m_bgmAmount = 0.0f;
				}
			}

			if (m_currentVolumeIndex >= 0
				&& m_currentVolumeIndex < 11) {
				const auto& knobPos = m_volumePosXData[m_currentVolumeIndex];
				const auto& amountPos = m_volumeAmountPosXData[m_currentVolumeIndex];
				const auto& amountScale = m_volumeAmountScaleXData[m_currentVolumeIndex];
				m_bgmKnobUI->m_transform.m_localPosition = Vector3(knobPos.m_positionX, 83.0f, 0.0f);
				m_bgmAmountUI->m_transform.m_localPosition = Vector3(amountPos.m_positionX, 83.0f, 0.0f);
				m_bgmAmountUI->m_transform.m_localScale = Vector3(amountScale.m_positionX, 1.3f, 1.0f);
			}
		}
		
		if (m_volumeResult == enNextType_SE) {
			if (g_pad[0]->IsTrigger(enButtonRB1)) {
				m_currentSEVolumeIndex++;
				m_seAmount += VOLUME_STEP;
				if (m_currentSEVolumeIndex > 10) {
					m_currentSEVolumeIndex = 10;
					m_seAmount = 1.0f;
				}
			}
			if (g_pad[0]->IsTrigger(enButtonLB1)) {
				m_currentSEVolumeIndex--;
				m_seAmount -= VOLUME_STEP;
				if (m_currentSEVolumeIndex < 0) {
					m_currentSEVolumeIndex = 0;
					m_seAmount = 0.0f;
				}
			}

			if (m_currentSEVolumeIndex >= 0
				&& m_currentSEVolumeIndex < 11) {
				const auto& knobPos = m_volumePosXData[m_currentSEVolumeIndex];
				const auto& amountPos = m_volumeAmountPosXData[m_currentSEVolumeIndex];
				const auto& amoutScale = m_volumeAmountScaleXData[m_currentSEVolumeIndex];
				m_seKnobUI->m_transform.m_localPosition = Vector3(knobPos.m_positionX, -121.0f, 0.0f);
				m_seAmountUI->m_transform.m_localPosition = Vector3(amountPos.m_positionX, -121.0f, 0.0f);
				m_seAmountUI->m_transform.m_localScale = Vector3(amoutScale.m_positionX, 1.3f, 1.0f);
			}
		}

		if (g_pad[0]->IsTrigger(enButtonA)) {
			/** 非表示にしてSettingに戻る */
			//m_isActived = false;
			m_volumeResult = enNextType_None;
		}
	}	

	m_listCursolUIColorAnimation->Update();
	m_canvas->Update();
}

void VolumeAdjustment::Render(RenderContext& rc)
{
	if (m_isActived) {
		m_canvas->Render(rc);
	}
}

/**************************************************/
namespace {
	constexpr const char* INGAME_USER_MANUAL_PATH = "Assets/spriteData/UserManual/inGameManual.DDS";
}

PoseMenu::PoseMenu()
{
	m_canvas = std::make_shared<UICanvas>();
	m_canvas->CreateUI<UICanvas>();
	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);
	
	m_image = m_canvas->CreateUI<UIImage>();
	m_image->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));

	m_userManualUI = std::make_shared<UIImage>();
	m_userManualUI->Init(INGAME_USER_MANUAL_PATH, 279.0f, 273.0f);
	m_userManualUI->m_transform.m_localPosition = Vector3(650.0f, -310.0f, 0.0f);
	m_userManualUI->m_transform.m_localScale = Vector3(1.0f, 1.0f, 0.0f);
	
	/** スロット生成 */
	m_itemSlot = std::make_shared<ItemSlot>();

	/** パネルを生成 */
	auto itemPanel = std::make_shared<ItemPosePanel>();
	itemPanel->SetOwner(this);
	/** パネルにスロットを教える */
	itemPanel->SetItemSlot(m_itemSlot);
	m_posePanelList.push_back(itemPanel);
	/** 操作説明パネル作成 */
	m_manualControlPanel = std::make_shared<ManualControlPosePanel>();
	m_manualControlPanel->SetOwner(this);
	m_posePanelList.push_back(m_manualControlPanel);
	/** 設定パネル作成 */
	auto settingPanel = std::make_shared<SettingPosePanel>();
	settingPanel->SetOwner(this);
	m_posePanelList.push_back(settingPanel);
	

	/** 名前リストベース作成 */
	m_nameListBase = std::make_shared<NameListBasePosePanel>();
	m_nameListBase->SetOwner(this);
	m_posePanelList.push_back(m_nameListBase);
	/** カーソルパネル作成 */
	m_cursolPanel = std::make_shared<CursolPosePanel>();
	m_cursolPanel->SetOwner(this);
	m_posePanelList.push_back(m_cursolPanel);
	/** 名前リスト作成 */
	auto nameListPanel = std::make_shared<NameListPosePanel>();
	nameListPanel->SetOwner(this);
	m_posePanelList.push_back(nameListPanel);
	/** アイコンカーソル作成 */
	m_iconCursol = std::make_shared<IconCursol>();
	m_iconCursol->SetOwner(this);
	m_posePanelList.push_back(m_iconCursol);
	
	/** 設定パネル作成 */
	m_setting = NewGO<Setting>(0, "setting");
	m_setting->SetOwner(this);
	/** 選択UI */
	m_inSelect = NewGO<InSelect>(0, "inselect");
	m_inSelect->SetOwner(this);

	/** 設定内の選択UI */
	m_settingInSelect = NewGO<SettingInSelect>(0, "settingInSelect");
	m_settingInSelect->SetOwner(this);
	/** 音量調整UI作成 */
	m_volumeAdjusutment = NewGO<VolumeAdjustment>(0, "volumeadjustment");
	m_volumeAdjusutment->SetOwner(this);

	//m_item3DModel = NewGO<Item3DModel>(0,"item3dmodel");

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
	if (m_settingInSelect) {
		DeleteGO(m_settingInSelect);
		m_settingInSelect = nullptr;
	}

	if (m_volumeAdjusutment) {
		DeleteGO(m_volumeAdjusutment);
		m_volumeAdjusutment = nullptr;
	}
	
}

bool PoseMenu::Start() {
	return true;
}

void PoseMenu::Update() {
	m_canvas->Update();

	if (m_userManualUI) {
		m_userManualUI->Update();
	}

	// アイテムリストパネルのユーザーマニュアル画像の表示制御
	if (!m_posePanelList.empty()) {
		auto itemPanel = std::dynamic_pointer_cast<ItemPosePanel>(m_posePanelList[0]);
		if (itemPanel) {
			itemPanel->SetUserManualVisible(IsPoseActive() && m_currentListIndex == 0);
			// ポーズメニューが開いていて、アイテムリストが選択されていて、かつInSelectが非表示のときだけ表示
			bool showManual = IsPoseActive() && m_currentListIndex == 0 && !(m_inSelect && m_inSelect->IsActived());
			itemPanel->SetUserManualVisible(showManual);
		}

		// 設定パネルのユーザーマニュアル画像の表示制御
		auto settingPanel = std::dynamic_pointer_cast<SettingPosePanel>(m_posePanelList[2]);
		if (settingPanel) {
			bool showSettingManual =
				IsPoseActive()
				&& m_currentListIndex == 2
				&& !(m_settingInSelect && m_settingInSelect->IsActived())
				&& !(m_volumeAdjusutment && m_volumeAdjusutment->IsActived());
			settingPanel->SetUserManualVisible(showSettingManual);
		}
	}

	/** 名前リストベースのスケールアニメーション */
	if (m_waitingForOpenColor
		&& m_nameListBase) {
		if (m_nameListBase->IsOpenScaleCompleted()) {
			if (m_cursolPanel) {
				m_cursolPanel->SetAlphaVisible();
			}
			m_waitingForOpenColor = false;
		}
	}

	/** アニメーション更新 */
	if (m_canvasColorOpenAnimation
		&&!m_canvasColorOpenAnimation->IsCompleted()) {
		m_canvasColorOpenAnimation->Update();
	}
	if (m_canvasColorCloseAnimation
		&&!m_canvasColorCloseAnimation->IsCompleted()) {
		m_canvasColorCloseAnimation->Update();
	}
	if (m_canvasScaleOpenAnimation
		&& !m_canvasScaleOpenAnimation->IsCompleted()) {
		m_canvasScaleOpenAnimation->Update();
	}
	if (m_canvasScaleCloseAnimation
		&& !m_canvasScaleCloseAnimation->IsCompleted()) {
		m_canvasScaleCloseAnimation->Update();
	}

	/** 確認UIが表示中の場合 */
	if (IsPoseActive()) {
		if (m_inSelect
			&& m_inSelect ->IsActived()) {
			auto result = m_inSelect->GetResult();

			if (result == InSelect::SelectResult::Yes) {
				//アイテムを使用
				ProcessInSelectResult();
				Inventory::GetInstance()->RemoveItem(m_selectedItemIndex);
			}
			else if (result == InSelect::SelectResult::No) {
				if (m_inSelect) {
					m_inSelect->SetActived(false);
				}
				m_selectedItemIndex = -1;
				m_inSelectInputConsumed = true;
			}
		}
	}

	if (IsPoseActive()) {
		if (m_setting && m_setting->IsActived()) {
			auto result = m_setting->GetResult();

			/** タイトルに戻るUIに遷移 */
			if (result == Setting::SelectResult::ToTitle) {
				if (m_settingInSelect) {
					m_settingInSelect->SetActived(true);
				}
				m_setting->SetActived(false);
			}
			/** 音量調整に遷移 */
			else if (result == Setting::SelectResult::Volume) {
				/** 表示状態にする */
				if (m_volumeAdjusutment) {
					m_volumeAdjusutment->SetActived(true);
				}
				m_setting->SetActived(false);
			}
		}
	}

	if (IsPoseActive()) {
		/** SettingInSelectの結果を処理 */
		if (m_settingInSelect
			&& m_settingInSelect->IsActived()) {
			auto result = m_settingInSelect->GetResult();
			if (result == SettingInSelect::EnNextType::enNextType_Yes) {
				m_isRequestReturnToTitle = true;
				m_settingInSelect->SetActived(false);
			}
			else if (result == SettingInSelect::EnNextType::enNextType_No) {
				/** キャンセル Settingに戻る */
				m_settingInSelect->SetActived(false);
				if (m_setting) {
					m_setting->SetActived(true);
				}
			}
		}

		if (m_volumeAdjusutment
			&& m_volumeAdjusutment->IsActived()) {
			auto result = m_volumeAdjusutment->GetResult();
			if (result == VolumeAdjustment::EnNextType::enNextType_None) {
				/** キャンセル Settingに戻る */
				m_volumeAdjusutment->SetActived(false);
				if (m_setting) {
					m_setting->SetActived(true);
				}
			}
		}


		/**  */
		UpdateCallbackButton(enButtonLB1, ManualControlPosePanel::ButtonType::L1);

		UpdateCallbackButton(enButtonRB1, ManualControlPosePanel::ButtonType::R1);

		UpdateCallbackButton(enButtonA, ManualControlPosePanel::ButtonType::A);

		UpdateCallbackButton(enButtonB, ManualControlPosePanel::ButtonType::B);

		UpdateCallbackButton(enButtonX, ManualControlPosePanel::ButtonType::X);

		UpdateCallbackButton(enButtonY, ManualControlPosePanel::ButtonType::Y);

		UpdateCallbackStick(StickType::L, ManualControlPosePanel::ButtonType::LStick);

		UpdateCallbackStick(StickType::R, ManualControlPosePanel::ButtonType::RStick);


		//UpdateCallbackButton()
		/*if (g_pad[0]->IsPress(enButtonLB1)) {
			m_manualControlPanel->
			PlayPushButton(ManualControlPosePanel::ButtonType::L1);
		}
		else {
			m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::L1);
		}*/

		// if (g_pad[0]->IsPress(enButtonRB1)) {
		// 	m_manualControlPanel->PlayPushButton(ManualControlPosePanel::ButtonType::R1);
		// }
		// else {
		// 	m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::R1);
		// }
		/** TODO:Rスティック入力検知したら */
	}
	else {
		m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::L1);
		m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::R1);
		m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::A);
		m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::B);
		m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::X);
		m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::Y);
		m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::RStick);
		m_manualControlPanel->PlayReleseButton(ManualControlPosePanel::ButtonType::LStick);
	}
	

	/** 通常のパネル更新 */
	for (const auto& p : m_posePanelList) {
		p->Update();
	}

	if (m_isContentfCursolEnable && m_iconCursol) {
		m_iconCursol->Update();
	}

	/**
	 * ここから修正
	 */
	if (IsPoseActive()) {
		if (m_currentListIndex == 2
			&& m_previousListIndex != 2) {
			bool volumeNotActive = (m_volumeAdjusutment == nullptr) || !m_volumeAdjusutment->IsActived();
			bool settingInSelectNotActive = (m_settingInSelect == nullptr) || !m_settingInSelect->IsActived();
			bool inSelectNotActive = (m_inSelect == nullptr) || !m_inSelect->IsActived();

			if (m_setting != nullptr
				&& !m_setting->IsActived()
				&& settingInSelectNotActive
				&& volumeNotActive
				&& inSelectNotActive) {
				m_setting->SetActived(true);
			}
		}
		else if (m_currentListIndex != 2
			&& m_previousListIndex == 2){
			/** 設定リスト以外に移動したとき */
			if (m_setting && m_setting->IsActived()) {
				m_setting->SetActived(false);
			}
		}
		/** 前回のインデックスを更新 */
		m_previousListIndex = m_currentListIndex;
	}

	if (IsPoseActive()) {
	/** アイテムの使用時の処理 */
	// アイテム一覧が表示されているとき(0番目と仮定)
		if (m_currentListIndex == 0) {
			//デバッグテスト：フラグを立てる
			m_isVisible = true;
			
			// Bボタンで使用
			if (g_pad[0]->IsTrigger(enButtonB)
				&& !(m_inSelect&& m_inSelect->IsActived())
				&& !m_inSelectInputConsumed) {
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
							m_inSelect->SetActived(true);
							// m_inSelect = NewGO<InSelect>(0, "inselect");
							// m_inSelect->SetOwner(this);
						}
					}
				}
			}
		}
		else {
			m_isVisible = false;
		}
	}
	m_inSelectInputConsumed = false;
	
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

	/** 最前に描画 */
	if (m_setting) {
		m_setting->Render(rc);
	}
	if (m_settingInSelect) {
		m_settingInSelect->Render(rc);
	}
	if (m_volumeAdjusutment) {
		m_volumeAdjusutment->Render(rc);
	}
	if (m_inSelect) {
		m_inSelect ->Render(rc);
	}

	// ポーズメニューが閉じているときだけ inGameManual.DDS を描画
	if (!IsPoseActive() && m_userManualUI) {
		m_userManualUI->Render(rc);
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
	//m_image->m_transform.m_localScale = Vector3(0.0f, 0.0f, 0.0f);
	/** 開く用のカラーイージング */
	{
		std::vector<float> timeList = { 1.0f };
		std::vector<Vector4> colorList = { Vector4(1.0f,1.0f,1.0f,0.0f),Vector4::White };
		m_canvasColorOpenAnimation = std::make_unique<ColorUIAnimation>(m_canvas, false, EasingType::EaseIn, timeList, colorList);
	}
	/** 開く時のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f, };
		std::vector<Vector2> scaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_canvasScaleOpenAnimation = std::make_unique<ScaleUIAnimation>(m_canvas, false, EasingType::EaseIn, timeList, scaleList);
	}
	/** 閉じる用のカラーイージング */
	{
		std::vector<float> closeTimeList = { 1.0f };
		std::vector<Vector4> closeColorList = { Vector4::White,Vector4(1.0f,1.0f,1.0f,0.0f) };
		m_canvasColorCloseAnimation = std::make_unique<ColorUIAnimation>(m_canvas, false, EasingType::EaseIn, closeTimeList, closeColorList);
	}
	/** 閉じる時のスケールアニメーション */
	{
		std::vector<float> timeList = { 0.25f,0.25f };
		std::vector<Vector2> closeScaleList = { Vector2(1.0f,1.0f),Vector2(1.15f,1.15f),Vector2(1.0f,1.0f) };
		m_canvasScaleCloseAnimation = std::make_unique<ScaleUIAnimation>(m_canvas, false, EasingType::EaseIn, timeList, closeScaleList);
	}

	for (const auto& p : m_posePanelList) {
		p->Init();
	}
}

void PoseMenu::Open()
{
	if (m_isActived) return;
	m_currentListIndex = 0;
	m_previousListIndex = -1;
	m_cursolPanel->ResetCursolIndex();

	//  if (m_setting) {
	//  	m_setting->PlayOpenAnimation();
	//  }
	if (m_canvasColorOpenAnimation) {
		m_canvasColorOpenAnimation->Play();
	}
	if (m_canvasScaleOpenAnimation) {
		m_canvasScaleOpenAnimation->Play();
	}
	for (const auto& p : m_posePanelList) {
		p->PlayOpenAnimation();
	}
	//  if (m_setting) {
	//  	m_setting->SetActived(true);
	//  }

	m_waitingForOpenColor = true;
	m_isActived = true;
}

void PoseMenu::Close() 
{
	if (!m_isActived) return;

	if (m_cursolPanel) {
		m_cursolPanel->SetAlphaInvisible();
	}

	if (m_setting) {
		m_setting->SetActived(false);
	}

	if (m_settingInSelect) {
		m_settingInSelect->SetActived(false);
	}

	if (m_inSelect) {
		m_inSelect->SetActived(false);
		//m_selectedItemIndex = -1;
	}

	if (m_volumeAdjusutment) {
		m_volumeAdjusutment->SetActived(false);
	}

	if (m_canvasColorCloseAnimation) {
		m_canvasColorCloseAnimation->Play();
	}
	if (m_canvasScaleCloseAnimation) {
		m_canvasScaleCloseAnimation->Play();
	}

	for (const auto& p : m_posePanelList) {
		p->PlayCloseAnimation();
	}
	m_waitingForCloseColor = true;
	m_isActived = false;
}

/** 第一引数：押すボタンのタイプ/第二引数:ボタンUIのタイプ*/
void PoseMenu::UpdateCallbackButton(const EnButton inputButtonType, const ManualControlPosePanel::ButtonType uiButtonType)
{
	if (g_pad[0]->IsPress(inputButtonType)) {
		m_manualControlPanel->PlayPushButton(uiButtonType);
	}
	else {
		m_manualControlPanel->PlayReleseButton(uiButtonType);
	}
}

void PoseMenu::UpdateCallbackStick(const StickType stickType, const ManualControlPosePanel::ButtonType uiButtonType)
{
	bool isPush = false;
	if (stickType == StickType::L) {
		if ((fabs(g_pad[0]->GetLStickXF()) >= FLT_EPSILON)
			|| (fabs(g_pad[0]->GetLStickYF() >= FLT_EPSILON)))
		{
			isPush = true;
		}
	}
	else if (stickType == StickType::R)
	{
		if ((fabs(g_pad[0]->GetRStickXF()) >= FLT_EPSILON)
			|| (fabs(g_pad[0]->GetRStickYF() >= FLT_EPSILON)))
		{
			isPush = true;
		}
	}

	if (isPush) {
		m_manualControlPanel->PlayPushButton(uiButtonType);
	}
	else {
		m_manualControlPanel->PlayReleseButton(uiButtonType);
	}
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

void PoseMenu::ProcessInSelectResult()
{
	if (!m_inSelect) {
		return;
	}

	auto result = m_inSelect->GetResult();

	if (result == InSelect::SelectResult::None) {
		return;
	}

	if (result == InSelect::SelectResult::Yes) {
		UseSelectedItem();
		Inventory::GetInstance()->RemoveItem(m_selectedItemIndex);
	}

	/** Yes/No どちらでも確認UI削除 */
	m_inSelect->SetActived(false);
	m_selectedItemIndex = -1;
	m_inSelectInputConsumed = true;
}

void PoseMenu::UseSelectedItem()
{
	//Player* player = FindGO<Player>("player");
	if (!m_player) { return;}

	auto itemPanel = std::dynamic_pointer_cast<ItemPosePanel>(m_posePanelList[0]);
	if (!itemPanel) { return; }

	auto slot = itemPanel->GetItemSlot(m_selectedItemIndex);
	if (!slot
		|| !slot->GetItem()) { return; }

	auto recoveryItem = std::dynamic_pointer_cast<RecoveryItem>(slot->GetItem());
	if (!recoveryItem) { return; }

	float healAmount = recoveryItem->GetHealAmount();
	if (healAmount > 0.0f) {
		m_player->HealPlayerHP(healAmount);
	}
}
