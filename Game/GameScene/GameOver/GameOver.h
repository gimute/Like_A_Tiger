#pragma once

class GameOver : public IGameObject
{
public:
	//設定する画像データ
	struct GameOverInitData
	{
		const char* gameOverFilePath;
		float frameSize_x;
		float frameSize_y;
	};

private:
	//GameOverUI用のデータ
	const GameOverInitData defaultGameOver = {
		"",
		1600.0f,
		900.0f
	};

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_gameOverUI;

public:
	GameOver();
	~GameOver();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	void Init(const GameOverInitData* initData = nullptr);
};

