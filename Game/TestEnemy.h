#pragma once
class TestEnemy : public IGameObject
{
	public:
	TestEnemy();
	~TestEnemy();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& renderContext) override;


	std::shared_ptr<TestEnemy> self;
	int testValue = 0;
	bool testFlag = true;
};

