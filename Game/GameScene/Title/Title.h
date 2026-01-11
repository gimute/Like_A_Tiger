#pragma once

class Title : public IGameObject {
public:
	/**
	* TitleUIに設定する画像のデータ
	*/
	struct TitleInitData {
		const char* titleFilePath;		//画像ファイルパス
		int frameSize_x;				//画像の横のサイズ
		int frameSiza_y;				//画像の縦のサイズ
	};


	//定数
private:
	//TitleUI用のデータ
	const TitleInitData defaultTitle = {
		"Assets/spriteData/Title/Title_LikeTiger.DDS",
		1920,
		1080,
	};

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_titleUI;		//タイトルUI

public:
	Title();
	~Title();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	void Init(const TitleInitData* initData = nullptr);


};


