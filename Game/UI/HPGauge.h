#pragma once

//TODO?
//HPの値の管理はどこがするかによって調整しないとなぁ

class HPGauge : public IGameObject
{
public:
	//HPUIに設定する画像のデータ
	//バーは実際に伸び縮みする部分、フレームはバーの周りの枠部分
	//バーとフレームの位置関係は、フレームの中央部分にバーが来るように計算して配置する
	struct HPGaugeUIInitData
	{
		const char* frameFilePath;			//フレーム画像のファイルパス
		int frameSize_x;					//フレーム画像の横のサイズ
		int frameSize_y;					//フレーム画像の縦のサイズ
		const char* hpBarFilePath;			//HPバー部分の画像ファイルパス
		const char* delayedHPBarFilePath;	//HPが減った時に見える、遅れてHPバーに追従する残像HPバー？の画像ファイルパス
		int hpBarSize_x;					//HPバーの横サイズ
		int hpBarSize_y;					//HPバーの縦サイズ
	};

	

	//定数
private:
	//仮のHPバー用のデータ
	const HPGaugeUIInitData defaultHPGauge =
	{
		"Assets/spriteData/HPGauge/Test/HPGauge_Frame.DDS",
		1024,
		32,
		"Assets/spriteData/HPGauge/Test/HPGauge_BarA_Texture.DDS",
		"Assets/spriteData/HPGauge/Test/HPGauge_BarB_Texture.DDS",
		1004,
		22
	};

private:
	UICanvas m_canvas;
	std::shared_ptr<UIImage> m_frame;	//フレーム
	std::shared_ptr<UIGauge> m_HPBar;	//HPバー
	std::shared_ptr<UIGauge> m_delayedHPBar;	//HP減少時にHPバーに遅れて追従するHPバー、ディレイHPバーと呼ぶことにする

	float m_delayTimer = 0.2f;	//ディレイHPバーがHPバーに追従開始するまでのタイマー
	float m_lerpVal = 0.0f;		//ディレイHPバーがHPバーにラープで追従する時用

public:
	HPGauge();
	~HPGauge();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	void Init(const HPGaugeUIInitData* initData = nullptr);

	void SetMaxHP(float maxHP);
	void SetHP(float nowHP);
	void DecreaseHP(float val);
	void IncreaseHP(float val);

	void SetPosition(Vector3 pos);

private:
	/// <summary>
	/// ディレイHPバーの準備
	/// </summary>
	void InitializeDelaydHPBar();

	/// <summary>
	/// ディレイHPバーの更新
	/// </summary>
	void DelaydHPBarUpdate();
};

