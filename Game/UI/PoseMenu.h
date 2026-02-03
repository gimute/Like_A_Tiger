#pragma once
#include "Inventory/Types.h"
#include "Actor/Actor.h"

class PoseMenu;
class IconCursol;
/**
 * スロットを増やしたい
 * 継承して空っぽ用のクラスを作りたい
 */



/** ポーズパネルの基底クラス */
class PosePanelBase : public Noncopyable{
public:
	/** ポーズメニュー内の各パネルを設定する画像のデータ */
	struct PosePanelInitData {
		const char* textFilePath;	//フレーム画像のファイルパス
		int frameSize_x;			//フレーム画像の横のサイズ
		int frameSize_y;			//フレーム画像の縦のサイズ
	};

public:
	PosePanelBase(){}
	virtual ~PosePanelBase() {}

	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;
	virtual void Init(const PosePanelInitData* initData = nullptr) = 0;
	/** 常に描画されるべきパネルかを返す */ 
	virtual bool ShouldAlwayRender() const { return false;}

	void SetOwner(PoseMenu* owner) { m_owner = owner; }

	virtual void PlayOpenAnimation() = 0;
	virtual void PlayCloseAnimation() = 0;

protected:
	UICanvas m_canvas;

	PoseMenu* m_owner = nullptr;
};




/**************************************************/


//  /**
//   * アイテムの3Dモデル表示を管理するクラス
//   */
//  class Item3DModel : public Actor {
//  public:
//  	Item3DModel();
//  	~Item3DModel();
//  
//  	bool Start();
//  
//  	void Update();
//  
//  	void Render(RenderContext& rc);
//  
//  	/** 座標設定 */
//  	void SetPosition(Vector3& pos)
//  	{
//  		m_transform.m_position = pos;
//  	}
//  
//  	/** 回転率設定 */
//  	void SetRotation(Quaternion& rot)
//  	{
//  		m_transform.m_rotation = rot;
//  	}
//  
//  	/** 拡大率設定 */
//  	void SetScale(Vector3& scale)
//  	{
//  		m_transform.m_scale = scale;
//  	}
//  
//  private:
//  	Transform m_transform;
//  
//  private:
//  	/** 静的オブジェクトを付与 */
//  	PhysicsStaticObject physicsStaticObject;
//  	ModelRender m_modelRender;
//  	bool isVisible = true;
//  };




/**************************************************/


class Player;
/** アイテムベースクラス */
class ItemBase {
public:
	struct ItemIconInitData {
		const char* textFilePath;	//フレーム画像のファイルパス
		int frameSize_x;			//フレーム画像の横のサイズ
		int frameSize_y;			//フレーム画像の縦のサイズ
	};

public:
	ItemBase()
	{
		m_canvas = std::make_shared<UICanvas>();
		//m_itemModel = std::make_shared<Item3DModel>();
		//m_itemModel->Start();
		//m_itemModel->Update();
	}
	virtual~ItemBase() {}

	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

	virtual void Init(const ItemIconInitData* initData = nullptr) = 0;
	virtual void Use(Player* p) = 0;

	std::shared_ptr<UICanvas> GetCanvas() { return m_canvas; }
	//std::shared_ptr<Item3DModel> GetItemModel() { return m_itemModel; }

	virtual void PlayOpenAnimation() = 0;
	virtual void PlayCloseAnimation() = 0;

	/** スケールアニメーションを再生 */
	virtual void PlayScaleAnimation(){}
	/** スケールアニメーションを停止してリセット */
	virtual void StopScaleAnimation() {}
	/** 選択状態を設定 */
	void SetSelected(bool selected) { m_isSelected = selected; }

	bool IsSelected() const { return m_isSelected; }

protected:
	std::shared_ptr<UICanvas> m_canvas;

	bool m_isSelected = false;
	//std::shared_ptr<Item3DModel> m_itemModel;
};




/**************************************************/


/** 回復アイテムクラス */
class RecoveryItem : public ItemBase {
private:
	const ItemIconInitData iconBackGroundInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_base.DDS",
		100,
		100
	};

public:
	RecoveryItem();
	virtual ~RecoveryItem() override;

	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const ItemIconInitData* initData = nullptr) override;
	void Use(Player* p) override;

	std::shared_ptr<UIImage> GetImage() const { return m_image; }
	/** 回復量を取得する処理 */
	virtual float GetHealAmount() = 0;

	//   int GetHealAmout() {
	//   	return m_healAmount;
	//   }

	virtual void PlayOpenAnimation()  {
		if (m_backGroundColorAnimation) {
			m_backGroundColorAnimation->Play();
		}
		if (m_imageColorAnimation) {
			m_imageColorAnimation->Play();
		}
		m_pauseedFullAlpha = false;
	}

	virtual void PlayCloseAnimation() {
		if (m_closeBackGoundColorAnimation) {
			m_closeBackGoundColorAnimation->Play();
		}
		if (m_closeImageColorAnimation) {
			m_closeImageColorAnimation->Play();
		}
	}

	void PlayScaleAnimation() override {
		if (m_scaleAnimation) {
			m_scaleAnimation->Play();
		}
	}

	void StopScaleAnimation() override {
		if (m_scaleAnimation) {
			m_scaleAnimation->Stop();
		}
		/** スケールを元に戻す */
		if (m_image) {
			m_image->m_transform.m_localScale = Vector3::One;
		}
	}
protected:
	/** 回復量を保持する変数 */
	float m_healAmount = 0.0f;
	/** 開く用カラーアニメーション */
	std::unique_ptr<ColorUIAnimation> m_backGroundColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_imageColorAnimation = nullptr;
	/** 閉じる用カラーアニメーション */
	std::unique_ptr<ColorUIAnimation> m_closeBackGoundColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeImageColorAnimation = nullptr;
	/** スケールアニメーション */
	std::unique_ptr<ScaleUIAnimation> m_scaleAnimation = nullptr;

	std::shared_ptr<UIImage> m_image;
	std::shared_ptr<UIImage> m_backgroundImage;
	/** アルファ1,0で一時停止中かどうか */
	bool m_pauseedFullAlpha = true;

private:
	Vector3 pos = Vector3(0.0f, 100.0f, 0.0);
	
};




/**************************************************/


/** 小回復アイテムクラス */
class SmallRecoveryItem : public RecoveryItem {
private:
	const ItemIconInitData smallRecoveryItemInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_small.DDS",
		90,
		60,
	};

public:
	SmallRecoveryItem();
	virtual ~SmallRecoveryItem() override;

	void Update() override;

	void Init(const ItemIconInitData* initData = nullptr) override;

	float GetHealAmount() override {
		return m_healAmount;
	}
};




/**************************************************/


/** 中回復アイテムクラス */
class StandardRecoveryItem : public RecoveryItem {
private:
	const ItemIconInitData standardRecoveryItemInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_standard.DDS",
		90,
		60,
	};

public:
	StandardRecoveryItem();
	virtual ~StandardRecoveryItem() override;
;
	void Update() override;

	void Init(const ItemIconInitData* initData = nullptr) override;

	float GetHealAmount() override {
		return m_healAmount;
	}
};




/**************************************************/


/** 大回復アイテムクラス */
class GreatRecoveryItem : public RecoveryItem {
private:
	const ItemIconInitData GreatRecoveryItemInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_great.DDS",
		90,
		60,
	};

public:
	GreatRecoveryItem();
	virtual ~GreatRecoveryItem() override;
;
	void Update() override;

	void Init(const ItemIconInitData* initData = nullptr) override;

	float GetHealAmount() override {
		return m_healAmount;
	}
};




/**************************************************/


/** 空っぽアイテムクラス */
class EmptyRecoveryItem : public RecoveryItem {
private:
	const ItemIconInitData EmptyRecoveryItemInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_great.DDS",
		0,
		0,
	};

public:
	EmptyRecoveryItem();
	virtual ~EmptyRecoveryItem();
;
	void Update() override;

	void Init(const ItemIconInitData* initData = nullptr) override;

	void PlayOpenAnimation() override {
		if (m_backGroundColorAnimation){
			m_backGroundColorAnimation->Play();
		}
	}

	void PlayCloseAnimation() override {
		if (m_closeBackGoundColorAnimation) {
			m_closeBackGoundColorAnimation->Play();
		}
	}
	float GetHealAmount() override {
		return 0;
	}

};


/**************************************************/


/** アイテムスロットクラス */
class ItemSlot {
	using RefItemBasePtr = std::shared_ptr<ItemBase>;
public:
	ItemSlot() : m_itemBase(nullptr) {
	}
	virtual ~ItemSlot() {}

	bool Start();
	void Update() {
		if (m_itemBase) {
			m_itemBase->Update();
		}
	}
	void Render(RenderContext& rc) {
		if (m_itemBase) {
			m_itemBase->Render(rc);
		}
	}

public:
	/** アイテムをセットし、表示切替 */
	void SetItem(RefItemBasePtr item) {
		m_itemBase = item;
	}

	/** 現在所持しているアイテムを取得 */
	RefItemBasePtr GetItem() const {
		return m_itemBase;
	}

	std::shared_ptr<UIImage> GetItemImage() const {
		if (!m_itemBase) return nullptr;

		auto recovery = std::dynamic_pointer_cast<RecoveryItem>(m_itemBase);
		return recovery ? recovery->GetImage() : nullptr;
	}

	std::shared_ptr<UICanvas> GetItemCanvas()
	{
		if (m_itemBase) {
			return m_itemBase->GetCanvas();
		}
		K2_ASSERT(false, "アイテムを作ってください");
		return nullptr;
	}

	void SetPosition(Vector3 pos) {
		m_position = pos;
	}
private:
	/** アイテムの数量 */
	int m_quantity = 0;
	/** 画面上の表示座標 */
	Vector3 m_position;

	RefItemBasePtr m_itemBase = nullptr;	//中身のアイテム
};




/**************************************************/


/** アイテムパネルクラス(アイテムの管理・選択) */
class ItemPosePanel : public PosePanelBase {
	using RefItemBasePtr = std::shared_ptr<ItemBase>;
	using RefItemSlotPtr = std::shared_ptr<ItemSlot>;
	

private:
	/** アイテムパネル用のデータ */
	const PosePanelInitData defaultItemPosePanel{
		"Assets/spriteData/PoseMenu/poseMenu_item.DDS",640,360,
	};

	/** フレーム用のデータ */
	const PosePanelInitData iconFrameInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_frame.DDS",
		100,
		100,
	};

public:
	ItemPosePanel();
	virtual ~ItemPosePanel() override;

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const PosePanelInitData* initData = nullptr) override;

	void SetItemSlot(RefItemSlotPtr slot) { m_itemSlot = slot; }

	RefItemSlotPtr GetItemSlot(int index) const {
		if (index >= 0
			&& index < static_cast<int>(m_itemSlotList.size())) {
			return m_itemSlotList[index];
		}
		return nullptr;
	}

	void PlayOpenAnimation() override {
		if (m_colorAnimation) {
			m_colorAnimation->Play();
		}
		// 各スロットのアイテムのアニメーションも開始
		for (auto& slot : m_itemSlotList) {
			if (slot && slot->GetItem()) {
				slot->GetItem()->PlayOpenAnimation();
			}
		}
	}

	void PlayCloseAnimation() override {
		if (m_closeColorAnimation) {
			m_closeColorAnimation->Play();
		}
		//各スロットのアイテムのアニメーションも開始
		for (auto& slot : m_itemSlotList) {
			if (slot && slot->GetItem()) {
				slot->GetItem()->PlayCloseAnimation();
			}
		}
	}

	/** カーソル位置が変更されたときに呼ぶ */
	void OnCursolIndexChanged(int newIndex);


private:
	ItemPosePanel::RefItemBasePtr MakeSharedItem(const EnItemType type);

private:
	std::shared_ptr<UIImage> m_image;
	std::unique_ptr<ColorUIAnimation> m_colorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeColorAnimation = nullptr;
	std::vector<RefItemBasePtr> m_itemList;	//マスターアイテムリスト
	UILayout m_uiLayout;

	std::vector<RefItemSlotPtr> m_itemSlotList;	//スロットのリスト
	RefItemSlotPtr m_itemSlot = nullptr;
	

	int m_currentItemIndex = 0;
	int m_previousCursorIndex = -1;

	int m_testIndex = 0;


	std::vector<std::shared_ptr<ItemIconInformation>> m_itemInfoList;

	std::shared_ptr<UIImage> m_frameImage;

	// 現在のスロットに表示しているアイテムタイプを記録しておく配列
	// データが変わった時だけ画像を更新
	std::vector<EnItemType> m_cachedSlotTypes;
};




/**************************************************/


/** 操作説明パネルクラス */
class ManualControlPosePanel : public PosePanelBase {
private:
	/** 操作説明パネル用のデータ */
	const PosePanelInitData defaultManualControlPosePanel{
		"Assets/spriteData/PoseMenu/Controller.DDS",
		640,
		400,
	};

public:
	ManualControlPosePanel();
	~ManualControlPosePanel();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const PosePanelInitData* initData = nullptr) override;

	void PlayOpenAnimation() override {
		if (m_openImageColorAnimation) {
			m_openImageColorAnimation->Play();
		}
	}
	void PlayCloseAnimation() override {
		if (m_closeImageColorAnimation) {
			m_closeImageColorAnimation->Play();
		}
	}

private:
	std::shared_ptr<UIImage> m_image;
	std::unique_ptr<ColorUIAnimation> m_openImageColorAnimation;
	std::unique_ptr<ColorUIAnimation> m_closeImageColorAnimation;
};




/**************************************************/


/** 設定パネルクラス */
class SettingPosePanel : public PosePanelBase {
private:
	/** 設定パネル用のデータ */
	const PosePanelInitData defaultSettingPosePanel{
		"Assets/spriteData/PoseMenu/poseMenu_setting.DDS",
		640,
		360,
	};

public:
	SettingPosePanel();
	~SettingPosePanel();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const PosePanelInitData* initData = nullptr) override;


	void PlayOpenAnimation() override {
		if (m_openImageColorAnimation) {
			m_openImageColorAnimation->Play();
		}
	}

	void PlayCloseAnimation() override {
		if (m_closeImageColorAnimation) {
			m_closeImageColorAnimation->Play();
		}
	}

private:
	std::shared_ptr<UIImage> m_image;

	std::unique_ptr<ColorUIAnimation> m_openImageColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeImageColorAnimation = nullptr;
};




/**************************************************/


/** カーソルパネルクラス */
class CursolPosePanel :public PosePanelBase {
private:
	/** カーソルパネル用のデータ */
	const PosePanelInitData defaultCursolPosePanel{
		"Assets/spriteData/PoseMenu/poseMenu_cursor.DDS",
		292,
		140,
	};

public:
	struct CursolPosYData{
		float y;
	};

public:
	CursolPosePanel();
	~CursolPosePanel();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const PosePanelInitData* initData = nullptr) override;

	/** 常に描画 */
	bool ShouldAlwayRender() const override { return true; }
	/** 押されたら */
	bool IsPressed() { return m_isPressed; }
	/** 選択中の点滅処理 */
	void SelectAlphaCursol();
	/** 選択した時の点滅処理 */
	void PushAlphaCursol();

	void PlayOpenAnimation() override {
		if (m_imageColorAnimation){
			m_imageColorAnimation->Play();
		}
	}

	void PlayCloseAnimation() override {
		if (m_closeImageColorAnimation) {
			m_closeImageColorAnimation->Play();
		}
	}

	/** カーソル位置をリセット */
	void ResetCursolIndex() {
		m_currentCursolIndex = 0;
		m_canvas.m_transform.m_localPosition.y = CURSOR_POSITIONS[0].y;
	}

private:
	std::shared_ptr<UIImage> m_image;
	std::unique_ptr<ColorUIAnimation> m_imageColorAnimation;
	std::unique_ptr<ColorUIAnimation> m_closeImageColorAnimation;

	int m_currentCursolIndex = 0;
	const int m_maxCursolIndex = 2;
	float m_alphaCursol = 1.0f;
	float m_speedAlpha = -0.02f;
	float m_speedPushAlpha = -0.4f;
	float m_pushTime = 0.0f;
	/** カーソル位置の静的データ配列 */
	static const CursolPosYData CURSOR_POSITIONS[];

	bool m_isPressed = false;
};




/**************************************************/


/** アイコンカーソルクラス */
class IconCursol :public PosePanelBase {
private:
	const PosePanelInitData defaultIconCursol{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_frame.DDS",
		100,
		100,
	};

public:
	struct CursolPosYData {
		Vector3 m_pos;
	};

public:
	IconCursol();
	~IconCursol();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const PosePanelInitData* initData = nullptr)override;
	//** 常に描画 *
	bool ShouldAlwayRender() const override { return true; }
	
	// 現在選択しているカーソルの番号を取得
	int GetCurrentIndex() const { return m_currentCursolIndex; }

	void SetOwner(PoseMenu* owner) { m_owner = owner; }

	void PlayOpenAnimation() override {
		if (m_imageColorAnimation) {
			m_imageColorAnimation->Play();
		}
	}

	void PlayCloseAnimation() override {
		if (m_closeImageColorAnimation) {
			m_closeImageColorAnimation->Play();
		}
	}

private:
	std::shared_ptr<UIImage> m_image;
	std::unique_ptr<ColorUIAnimation> m_imageColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeImageColorAnimation = nullptr;

	Vector3 pos = Vector3::Zero;
	int m_currentCursolIndex = 0;
	/** カーソル位置の静的データ配列 */
	static const CursolPosYData CURSOR_ICON_POSITIONS[];

	PoseMenu* m_owner = nullptr;
};




/**************************************************/


/** 名前リストパネルクラス */
class NameListPosePanel : public PosePanelBase {
private:
	/** 名前リストパネル用データ */
	const PosePanelInitData defaultNameListPosePanel{
		"Assets/spriteData/PoseMenu/poseMenu_nameList.DDS",
		1280,
		720,
	};

public:
	NameListPosePanel();
	~NameListPosePanel();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Init(const PosePanelInitData* initData = nullptr);
	//** 常に描画 *
	bool ShouldAlwayRender() const override { return true; }

	void PlayOpenAnimation() override {
		if (m_colorAnimation) {
			m_colorAnimation->Play();
		}
	}

	void PlayCloseAnimation() override {
		if (m_closeImageColorAnimation) {
			m_closeImageColorAnimation->Play();
		}
	}

private:
	std::shared_ptr<UIImage> m_image;
	std::unique_ptr<ColorUIAnimation> m_colorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeImageColorAnimation = nullptr;
};




/**************************************************/


class NameListBasePosePanel : public PosePanelBase {
private:
	const PosePanelInitData defaultNameListBase{
		"Assets/spriteData/PoseMenu/poseMenu_nameListBase.DDS",
		1280,
		720,
	};

public:
	NameListBasePosePanel();
	~NameListBasePosePanel();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const PosePanelInitData* initData = nullptr) override;

	/** 常に描画 */
	bool ShouldAlwayRender() const override { return true; }

	void PlayOpenAnimation() override {
		if (m_colorAnimation) {
			m_colorAnimation->Play();
		}
	}

	void PlayCloseAnimation() override {
		if (m_closeColorAnimation) {
			m_closeColorAnimation->Play();
		}
	}

private:
	std::shared_ptr<UIImage> m_image;
	std::unique_ptr<ColorUIAnimation> m_colorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeColorAnimation = nullptr;
};




/**************************************************/


class InSelect : public IGameObject
{
public:
	/** 選択結果 */
	enum SelectResult {
		None,	// まだ選択中
		Yes,
		No,
	};

private:
	SelectResult m_result = SelectResult::None;

public:
	SelectResult GetResult() const { return m_result; }
	/** Aボタンで確定 */
	//void ConfirmSelection();

public:
	struct CursolPositionYData
	{
		float positionY;
	};

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_cursolUI;

	std::unique_ptr<PositionUIAnimation> m_cursolPositionAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_cursolColorAnimaiton = nullptr;

	int m_currentCursolIndex = 0;

	PoseMenu* m_owner = nullptr;

public:
	InSelect();
	~InSelect();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void SetOwner(PoseMenu* owner) { m_owner = owner; }
};




/**************************************************/
class SettingInSelect;


class Setting : public IGameObject {
public:
	enum SelectResult {
		None,
		Volume,
		ToTitle
	};

private:
	SelectResult m_result = SelectResult::None;
public:
	SelectResult GetResult() {
		return m_result;
	}

	bool IsVisible() {
		return m_isVisible;
	}

public:
	struct CursolPositionData {
		float positionX;
		float positionY;
	};

private:
	SettingInSelect* m_settingInSelect = nullptr;

	std::shared_ptr<UICanvas> m_canvas;

	/** baseUI */
	std::shared_ptr<UIImage> m_baseUI;
	/** cursolUI */
	std::shared_ptr<UIImage> m_cursolUI;
	/** textToTitleUI */
	std::shared_ptr<UIImage> m_textToTitleUI;
	/** textVolumeUI */
	std::shared_ptr<UIImage> m_textVolumeUI;

	std::vector<std::shared_ptr<UIImage>> m_baseUIList;


	/** baseUI */
	std::unique_ptr<ColorUIAnimation> m_openBaseUIColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeBaseUIColorAnimation = nullptr;

	std::vector<std::unique_ptr<ColorUIAnimation>> m_openBaseUIColorAnimationList;
	std::vector<std::unique_ptr<ColorUIAnimation>> m_closeBaseUIColorAnimationList;
	

	/** cursolUI */
	std::unique_ptr<PositionUIAnimation> m_cursolUIPositionAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_cursolUIColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_openCursolUIColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeCursolUIColorAnimation = nullptr;
	
	/** textToTitleUI */
	std::unique_ptr<ColorUIAnimation> m_openTextToTitleUIColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeTextToTitleUIColorAnimation = nullptr;

	/** textVolumeUI */
	std::unique_ptr<ColorUIAnimation> m_openTextVolumeUIColorAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_closeTextVolumeUIColorAnimation = nullptr;
	

	int m_currentSettingCursolIndex = 0;

	bool m_isVisible = false;

	PoseMenu* m_owner = nullptr;

public:
	Setting();
	~Setting();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	SettingInSelect* GetSetting() const { return m_settingInSelect; }

	void SetOwner(PoseMenu* owner) { m_owner = owner; }

	void PlayOpenAnimation() {
		for (auto& anim : m_openBaseUIColorAnimationList) {
			if (anim) {
				anim->Play();
			}
		}

		if (m_openBaseUIColorAnimation) {
			m_openBaseUIColorAnimation->Play();
		}
		if (m_openCursolUIColorAnimation) {
			m_openCursolUIColorAnimation->Play();
		}
		if (m_openTextToTitleUIColorAnimation) {
			m_openTextToTitleUIColorAnimation->Play();
		}
		if (m_openTextVolumeUIColorAnimation) {
			m_openTextVolumeUIColorAnimation->Play();
		}
	}
	void PlayCloseAnimation() {
		for (auto& anim : m_closeBaseUIColorAnimationList) {
			if (anim) {
				anim->Play();
			}
		}

		if (m_closeBaseUIColorAnimation) {
			m_closeBaseUIColorAnimation->Play();
		}
		if (m_closeCursolUIColorAnimation) {
			m_closeCursolUIColorAnimation->Play();
		}
		if (m_closeTextToTitleUIColorAnimation) {
			m_closeTextToTitleUIColorAnimation->Play();
		}
		if (m_closeTextVolumeUIColorAnimation) {
			m_closeTextVolumeUIColorAnimation->Play();
		}
	}
};




/**************************************************/


class SettingInSelect : public IGameObject {
public:
	/** TODO:
	 *  InSelectクラスでも実装中なので一括したクラスを作りたい
	 */
	enum EnNextType {
		enNextType_None,
		enNextType_Yes,
		enNextType_No,
	};

	EnNextType m_result = EnNextType::enNextType_None;

public:
	struct CursolPositionYData {
		float m_positionY;
	};

public:
	SettingInSelect();
	~SettingInSelect();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	EnNextType GetResult() const { return m_result; }
	void SetOwner(PoseMenu* owner) { m_owner = owner; }

	bool IsYes() const { return m_currentCursolIndex == enNextType_Yes; }
	bool IsNo() const { return m_currentCursolIndex == enNextType_No; }

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_cursolUI;

	std::unique_ptr<ColorUIAnimation> m_cursolUIColorAnimation = nullptr;

	int m_currentCursolIndex = 0;

	PoseMenu* m_owner = nullptr;
};




/**************************************************/

/**
 * ポーズメニュークラス
 * 大きいキャンバスのイメージ
 */
class PoseMenu : public IGameObject
{
	/** 長い型名に、短くて分かりやすいあだ名を付けた */
	using RefPosePanelBasePtr = std::shared_ptr<PosePanelBase>;

public:
	struct PoseMenuInitData {
		const char* frameFilePath;		//フレーム画像のファイルパス
		int frameSize_x;				//フレーム画像の横のサイズ
		int frameSize_y;				//フレーム画像の縦のサイズ
	};

private:
	/** ポーズメニューの背景用のデータ */
	const PoseMenuInitData defaultPoseMenu = {
		"Assets/spriteData/PoseMenu/poseMenu_base.DDS",1280,720,
	};

public:
	PoseMenu();
	virtual ~PoseMenu() override;
	
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Init(const PoseMenuInitData* initData = nullptr);

public:
	/** ポーズメニューを開く */
	void Open();
	/** ポーズメニュを閉じる */
	void Close();
	/** ポーズメニューがアクティブか */
	bool IsPoseActive() const { return m_isActived; }
	/** タイトルへの遷移がリクエストされたか */
	bool IsRequestReturnToTitle() const { return m_isRequestReturnToTitle; }

private:
	/** タイトル遷移リクエストを設定 */
	bool SetRequestReturnToTitle(bool request) { m_isRequestReturnToTitle = request; }

public:
	void AddItemInfo(ItemIconInformation* info){
		m_itemPanelInfoList.push_back(std::shared_ptr<ItemIconInformation>(info));
	}
	std::vector<std::shared_ptr<ItemIconInformation>>& GetItemPanelInfoList()
	{
		return m_itemPanelInfoList;
	}

	bool IsVisible() const { return m_isVisible; }

	

	Vector4 GetAlpha() const {
		return Vector4(1.0f, 1.0f, 1.0f, m_alpha);
	}

	/** タブ切り替え */
	void SetCurrentListIndex(int index) {
		if (index >= 0 && index <= m_maxListIndex) {
			m_currentListIndex = index;
		}
	}

	int GetCurrentListIndex() {
		return m_currentListIndex;
	}

	/** カーソルの操作の有効/無効を切り替え */
	void SetEnableContentCursol(bool isEnable) {
		m_isContentfCursolEnable = isEnable;
	}

	bool GetEnableContentCursol() {
		return m_isContentfCursolEnable;
	}

	bool IsCursolPressed(){
		return m_isCursolPressed;
	}

	void SetIsCursolPressed(bool cursolPressed)
	{
		m_isCursolPressed = cursolPressed;	
	}

	/** アイコンカーソルの位置変更を通知 */
	void OnIconCursorIndexChanged(int newIndex);

private:
	/** アイテム使用確認UIの結果を処理 */
	void ProcessInSelectResult();
	/** 選択されたアイテムを使用してプレイヤーに反映 */
	void UseSelectedItem();

private:
	std::shared_ptr<UICanvas> m_canvas = nullptr;
	std::unique_ptr<ColorUIAnimation> m_canvasColorOpenAnimation = nullptr;
	std::unique_ptr<ColorUIAnimation> m_canvasColorCloseAnimation = nullptr;

	std::shared_ptr<UIImage> m_image = nullptr;
	std::vector<RefPosePanelBasePtr> m_posePanelList;
	std::shared_ptr<ItemSlot> m_itemSlot = nullptr;
	std::shared_ptr<CursolPosePanel> m_cursolPanel = nullptr;

	int m_currentListIndex = 0;
	const int m_maxListIndex = 2;

	/** アイテムパネルで使用する情報 */
	std::vector<std::shared_ptr<ItemIconInformation>> m_itemPanelInfoList;
	/** カーソルを操作・参照するためにメンバ変数として保持 */
	std::shared_ptr<IconCursol> m_iconCursol = nullptr;
	std::shared_ptr<CursolPosePanel> m_cursol = nullptr;

	bool m_isVisible = false;
	bool m_isContentfCursolEnable = false;
	bool m_isCursolPressed = false;

	float m_alpha = 1.0f;

	bool m_isActived = false;

	/** タイトル遷移リクエスト */
	bool m_isRequestReturnToTitle = false;

private:
	/** アイテム使用確認UI */
	InSelect* m_inSelect = nullptr;
	/** 選択中のアイテムインデックス */
	int m_selectedItemIndex = -1;
	/** 設定用 */
	Setting* m_setting = nullptr;
	/** 設定内の使用確認UI */
	SettingInSelect* m_settingInSelect = nullptr;

public:
	/** 確認UIが表示中か */
	bool IsInSelectActive() const { return m_inSelect != nullptr; }
	/** 選択結果を取得 */
	int GetSelectedItemIndex() const { return m_selectedItemIndex; }
	/** 設定UIがアクティブか */
	bool IsSettingActive() const { return m_setting != nullptr; }
	/** 設定内の使用確認UIがアクティブか */
	bool IsSettingInSelectActive() const { return m_settingInSelect != nullptr; }
};