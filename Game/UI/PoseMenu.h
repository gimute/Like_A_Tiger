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

protected:
	std::shared_ptr<UICanvas> m_canvas;
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
	int GetHealAmout() {
		return m_healAmount;
	}


protected:
	/** 回復量を保持する変数 */
	int m_healAmount = 0;

private:
	std::shared_ptr<UIImage> m_image;
	std::shared_ptr<UIImage> m_backgroundImage;

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
	//std::vector<ItemBase> m_itemList;
	/** アイテムの数量 */
	int m_quantity = 0;
	/** 画面上の表示座標 */
	Vector3 m_position;
	/**  */
	int m_currentSlotIndex = 0;

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

private:
	ItemPosePanel::RefItemBasePtr MakeSharedItem(const EnItemType type);

private:
	std::shared_ptr<UIImage> m_image;		
	std::vector<RefItemBasePtr> m_itemList;	//マスターアイテムリスト
	UILayout m_uiLayout;

	std::vector<RefItemSlotPtr> m_itemSlotList;	//スロットのリスト
	RefItemSlotPtr m_itemSlot = nullptr;
	

	int m_currentItemIndex = 0;

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
	/** 押されたら */
	bool IsPressed() { return m_isPressed; }
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

	

private:
	std::shared_ptr<UIImage> m_image;
	Vector3 pos = Vector3::Zero;
	int m_currentCursolIndex = 0;
	/** カーソル位置の静的データ配列 */
	static const CursolPosYData CURSOR_ICON_POSITIONS[];
};




/**************************************************/


/** 名前リストパネルクラス */
class NameListPosePanel : public PosePanelBase {
private:
	//  /** ポーズメニュー内の各パネルを設定する画像のデータ */
	//  struct NamePanelInitData {
	//  	const char* textFilePath;	//フレーム画像のファイルパス
	//  	int frameSize_x;			//フレーム画像の横のサイズ
	//  	int frameSize_y;			//フレーム画像の縦のサイズ
	//  };

	/** 名前リストパネル用データ */
	const PosePanelInitData defaultNameListPosePanel{
		"Assets/spriteData/PoseMenu/poseMenu_nameList.DDS",
		1280,
		720,
	};

	//  const NamePanelInitData defaultNameListBase{
	//  	"Assets/spriteData/PoseMenu/poseMenu_nameListBase.DDS",
	//  	1280,
	//  	720,
	//  };

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
	void AddItemInfo(ItemIconInformation* info){
		m_itemPanelInfoList.push_back(std::shared_ptr<ItemIconInformation>(info));
	}
	std::vector<std::shared_ptr<ItemIconInformation>>& GetItemPanelInfoList()
	{
		return m_itemPanelInfoList;
	}

	bool IsVisible() const { return m_isVisible; }

	void SetAlpha(float alpha) {
		m_alpha = alpha;

		m_canvas.SetColor(1.0f, 1.0f, 1.0f, m_alpha);
		// for (auto& p : m_posePanelList) {
		// 	
		// }


		//  if (m_image) m_image->SetColor(1.0f, 1.0f, 1.0f, alpha);
		//  
		//  for (auto& panel : m_posePanelList)
		//  {
		//  
		//  }
	}

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

private:
	UICanvas m_canvas;
	std::shared_ptr<UIImage> m_image = nullptr;
	std::vector<RefPosePanelBasePtr> m_posePanelList;
	std::shared_ptr<ItemSlot> m_itemSlot = nullptr;
	int m_currentListIndex = 0;
	const int m_maxListIndex = 2;

	/** アイテムパネルで使用する情報 */
	std::vector<std::shared_ptr<ItemIconInformation>> m_itemPanelInfoList;
	/** カーソルを操作・参照するためにメンバ変数として保持 */
	std::shared_ptr<IconCursol> m_iconCursol = nullptr;

	bool m_isVisible = false;
	bool m_isContentfCursolEnable = false;
	bool m_isCursolPressed = false;

	float m_alpha = 1.0f;
};