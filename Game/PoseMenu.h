#pragma once

/**
* 
*/

class PoseMenu : public IGameObject
{
public:
	struct PoseMenuInitData {
		const char* frameFilePath;		//フレーム画像のファイルパス
		int frameSize_x;				//フレーム画像の横のサイズ
		int frameSize_y;				//フレーム画像の縦のサイズ
	};


private:
	const PoseMenuInitData defaultPoseMenu = {
		"Assets/spriteData/PoseMenu/test.DDS",
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

private:
	

	UICanvas m_canvas;

	std::shared_ptr<UIImage> m_image = nullptr;
};

