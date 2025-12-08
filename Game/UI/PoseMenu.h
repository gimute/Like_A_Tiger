#pragma once
#include "Inventory/Types.h"


class PoseMenu;


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

protected:
	UICanvas m_canvas;

	PoseMenu* m_owner;
};




/**************************************************/


class Player;
/** アイテムベースクラス */
class ItemBase {
	//using RefRecoveryItemPtr = std::shared_ptr<RecoveryItem>();

public:
	struct ItemIconInitData {
		const char* textFilePath;	//フレーム画像のファイルパス
		int frameSize_x;			//フレーム画像の横のサイズ
		int frameSize_y;			//フレーム画像の縦のサイズ
	};

	//  struct ItemIconBackGroundData {
	//  	const char* textFilePath;
	//  	int frameSize_x;
	//  	int frameSize_y;
	//  };

public:
	ItemBase() {}
	virtual~ItemBase() {}

	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

	virtual void Init(const ItemIconInitData* initData = nullptr) = 0;
	virtual void Use(Player* p) = 0;

protected:
	UICanvas m_canvas;

private:
	//std::vector<RefRecoveryItemPtr> m_recoveryItemList;
};




/**************************************************/


/** 回復アイテムクラス */
class RecoveryItem : public ItemBase {
private:
	const ItemIconInitData iconBackGroundInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_base.DDS",
		150,
		150
	};

public:
	RecoveryItem();
	~RecoveryItem();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const ItemIconInitData* initData = nullptr) override;
	void Use(Player* p) override;

	/** 回復量を取得する処理 */
	int GetHealAmout() {
		return m_healAmount;
	}
protected:
	/** 回復量を保持する変数 */
	int m_healAmount = 0;

private:
	std::shared_ptr<UIImage> m_image;

};




/**************************************************/


/** 小回復アイテムクラス */
class SmallRecoveryItem : public RecoveryItem {
private:
	const ItemIconInitData smallRecoveryItemInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_small.DDS",
		250,
		250,
	};

public:
	SmallRecoveryItem();
	~SmallRecoveryItem();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const ItemIconInitData* initData = nullptr) override;

private:
	std::shared_ptr<UIImage> m_image;
};




/**************************************************/


/** 中回復アイテムクラス */
class StandardRecoveryItem : public RecoveryItem {
private:
	const ItemIconInitData standardRecoveryItemInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_standard.DDS",
		150,
		150,
	};

public:
	StandardRecoveryItem();
	~StandardRecoveryItem();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const ItemIconInitData* initData = nullptr) override;

private:
	std::shared_ptr<UIImage> m_image;
};




/**************************************************/


/** 大回復アイテムクラス */
class GreatRecoveryItem : public RecoveryItem {
private:
	const ItemIconInitData GreatRecoveryItemInitData{
		"Assets/spriteData/Item/RecoveryItem/itemRecovery_great.DDS",
		100,
		100,
	};

public:
	GreatRecoveryItem();
	~GreatRecoveryItem();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const ItemIconInitData* initData = nullptr) override;

private:
	std::shared_ptr<UIImage> m_image;
};




/**************************************************/


/** アイテムパネルクラス */
class ItemPosePanel : public PosePanelBase {
	using RefItemBasePtr = std::shared_ptr<ItemBase>;
	//using RefItemSlotPtr = std::shared_ptr<ItemSlot>;

private:
	/** アイテムパネル用のデータ */
	const PosePanelInitData defaultItemPosePanel{
		"Assets/spriteData/PoseMenu/poseMenu_item.DDS",
		640,
		360,
	};

public:
	ItemPosePanel();
	~ItemPosePanel();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const PosePanelInitData* initData = nullptr) override;


private:
	std::shared_ptr<UIImage> m_image;
	//std::vector<RefItemSlotPtr> m_itemSlotList;
	std::vector<RefItemBasePtr> m_itemList;
};




/**************************************************/


/** アイテムスロットクラス */
class ItemSlot {
	using RefItemBasePtr = std::shared_ptr<ItemBase>;
public:
	ItemSlot();
	~ItemSlot();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	//std::vector<ItemBase> m_itemList;
	/** アイテムの数量 */
	int m_quantity = 0;
	/** 画面上の表示座標 */
	Vector3 m_position;
	/**  */
	int m_currentSlotIndex = 0;

	//std::vector<RefItemBasePtr> m_itemList;
	RefItemBasePtr m_itemBase = nullptr;
};




/**************************************************/


/** 操作説明パネルクラス */
class ManualControlPosePanel : public PosePanelBase {
private:
	/** 操作説明パネル用のデータ */
	const PosePanelInitData defaultManualControlPosePanel{
		"Assets/spriteData/PoseMenu/poseMenu_manualControl.DDS",
		640,
		360,
	};

public:
	ManualControlPosePanel();
	~ManualControlPosePanel();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const PosePanelInitData* initData = nullptr) override;

private:
	std::shared_ptr<UIImage> m_image;
	
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

private:
	std::shared_ptr<UIImage> m_image;
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
	/** 選択中の点滅処理 */
	void SelectAlphaCursol();
	/** 選択した時の点滅処理 */
	void PushAlphaCursol();

private:
	std::shared_ptr<UIImage> m_image;
	int m_currentCursolIndex = 0;
	const int m_maxCursolIndex = 2;
	float m_alphaCursol = 1.0f;
	float m_speedAlpha = -0.02f;
	float m_speedPushAlpha = -0.4f;
	float m_pushTime = 0.0f;
	/** カーソル位置の静的データ配列 */
	static const CursolPosYData CURSOR_POSITIONS[];
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

private:
	std::shared_ptr<UIImage> m_image;
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

	friend class ItemPosePanel;

public:
	struct PoseMenuInitData {
		const char* frameFilePath;		//フレーム画像のファイルパス
		int frameSize_x;				//フレーム画像の横のサイズ
		int frameSize_y;				//フレーム画像の縦のサイズ
	};

private:
	/** ポーズメニューの背景用のデータ */
	const PoseMenuInitData defaultPoseMenu = {
		"Assets/spriteData/PoseMenu/poseMenu_base.DDS",
		1280,
		720,
	};

public:
	PoseMenu();
	~PoseMenu();
	
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Init(const PoseMenuInitData* initData = nullptr);

public:
	void AddItemInfo(ItemIconInformation* info)
	{
		m_itemPanelInfoList.push_back(std::unique_ptr<ItemIconInformation>(info));
	}

private:
	UICanvas m_canvas;

	std::shared_ptr<UIImage> m_image = nullptr;

	std::vector<RefPosePanelBasePtr> m_posePanelList;

	std::shared_ptr<ItemSlot> m_itemSlot = nullptr;

	int m_currentListIndex = 0;
	const int m_maxListIndex = 2;

	/** アイテムパネルで使用する情報 */
	std::vector<std::shared_ptr<ItemIconInformation>> m_itemPanelInfoList;
};