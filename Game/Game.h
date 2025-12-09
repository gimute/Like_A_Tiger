#pragma once
class Game : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	UICanvas m_canvas;
	std::shared_ptr<UIImage> m_image;
	std::unique_ptr<PositionUIAnimation> m_posanim;
};

