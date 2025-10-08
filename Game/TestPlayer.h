#pragma once

class TestPlayer : public IGameObject
{
public:
	TestPlayer();

	~TestPlayer();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& renderContext) override;

	std::shared_ptr<TestPlayer> self;

	int testValue = 0;

	bool testFlag = true;
};

