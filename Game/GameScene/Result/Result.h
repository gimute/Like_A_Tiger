#pragma once

class Result : public IGameObject
{
public:
	//Resultに設定する画像データ
	struct ResultInitData {
		const char* resultFilePath;
		float frameSize_x;
		float frameSize_y;
	};
	
private:
	//ResultUI用のデータ
	const ResultInitData defaultResult = {
		"Assets/spriteData/Result/TestResult.DDS",
		1600.0f,
		900.0f
	};

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_resultUI;

public:
	Result();
	~Result();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	void Init(const ResultInitData* initData = nullptr);
};

