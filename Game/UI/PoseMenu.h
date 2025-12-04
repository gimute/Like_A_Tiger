#pragma once

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

protected:
	UICanvas m_canvas;
};




/** アイテムパネルクラス */
class ItemPosePanel : public PosePanelBase {
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

};




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
	CursolPosePanel();
	~CursolPosePanel();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const PosePanelInitData* initData = nullptr) override;
	
private:
	std::shared_ptr<UIImage> m_image;
	int m_currentCursolIndex = 0;
	const int m_maxCursolIndex = 2;
};




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

private:
	std::shared_ptr<UIImage> m_image;
};




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

	//int GetCurrentListIndex() {
	//	return m_currentListIndex;
	//}
private:
	UICanvas m_canvas;

	std::shared_ptr<UIImage> m_image = nullptr;

	std::vector<RefPosePanelBasePtr> m_posePanelList;
	
	int m_currentListIndex = 0;
	const int m_maxListIndex = 2;
};