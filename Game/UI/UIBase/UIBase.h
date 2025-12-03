#pragma once

//UIのベース、インターフェース
class UIBase : public Noncopyable
{
public:
	Transform m_transform;

protected:
	bool m_isStart = false;
	bool m_isUpdate = true;	//更新するか
	bool m_isDraw = false;	//表示非表示

public:
	UIBase() {}
	virtual ~UIBase() {}

	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

	virtual void SetColor(Vector4 mulColor) {};
	virtual void SetColor(float r, float g, float b, float a) {};
	virtual Vector4 GetColor() = 0;

	/// <summary>
	/// 描画の制御
	/// </summary>
	/// <param name="flag"></param>
	virtual void SetDrawFlag(bool flag)
	{
		m_isDraw = flag;
	}

	/// <summary>
	/// 更新処理の制御
	/// </summary>
	/// <param name="flag"></param>
	virtual void SetUpdateFlag(bool flag)
	{
		m_isUpdate = flag;
	}
};

/// <summary>
/// UIの生成、管理、親子付けをするためのクラス
/// </summary>
class UICanvas : public UIBase
{
	/**
	* 長い型名に、短くて分かりやすいあだ名を付けた
	*/
	using RefUIBasePtr = std::shared_ptr<UIBase>;

public:
	//Transform m_transform;

private:
	std::vector<RefUIBasePtr> m_uiList;

public:
	UICanvas();
	~UICanvas();


	bool Start() override;
	/// <summary>
	/// 登録されているUIの更新処理を呼び出す
	/// </summary>
	void Update() override;
	/// <summary>
	/// 登録されているUIの描画処理を呼び出す
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc) override;

	void SetColor(Vector4 mulColor) override;
	void SetColor(float r, float g, float b, float a)override;
	Vector4 GetColor()override;

	/// <summary>
	/// この関数で生成したUIはデストラクタでdeleteするのでほっといても大丈夫なはず
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template <typename T>
	std::shared_ptr<T> CreateUI()
	{
		//UIBaseを継承しているか確認
		if constexpr (!(std::is_base_of_v<UIBase, T>))
		{
			return nullptr;
		}

		//UI生成
		auto ui = std::make_shared<T>();

		////Canvasのtransformと親子付け
		ui->m_transform.SetParent(&m_transform);

		m_uiList.push_back(ui);

		ui->Start();

		return ui;
	}

	/// <summary>
	/// UIの表示、非表示
	/// </summary>
	void SetDrawFlag(bool visible);

	void SetUpdateFlag(bool enable);
};



/// <summary>
/// 画像を使うUI
/// </summary>
class UIImage : public UIBase
{

protected:
	SpriteRender m_spriteRender;	//表示する画像
	Vector4 m_mulColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);	//乗算カラー
	

public:
	UIImage();
	~UIImage();

	virtual void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
	virtual void SetPivot(float x, float y);
	virtual void SetPivot(const Vector2& pivot);
	/// <summary>
	/// 乗算カラーを設定
	/// </summary>
	/// <param name="mulColor"></param>
	virtual void SetColor(Vector4 mulColor) override;
	virtual void SetColor(float r, float g, float b, float a) override;
	/// <summary>
	/// 乗算カラー取得
	/// </summary>
	/// <returns></returns>
	virtual Vector4 GetColor()override;
};


/// <summary>
/// 伸び縮みするUI(例：HPバー)
/// 何らかの値と連動させて使う、
/// 最大値と現在値をもとに割合を計算して画像のxスケールを調整する
/// </summary>
class UIGauge : public UIImage
{

private:
	float m_maxValue = 1.0f;
	float m_Value = 1.0f;
	Vector4 m_mulColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

public:
	UIGauge();
	~UIGauge();

	void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	void SetPivot(float x, float y) override;
	void SetPivot(const Vector2& pivot) override;
	/// <summary>
	/// 乗算カラーを設定
	/// </summary>
	/// <param name="mulColor"></param>
	virtual void SetColor(Vector4 mulColor) override;
	virtual void SetColor(float r, float g, float b, float a) override;
	virtual Vector4 GetColor()override;

	const float GetMaxValue() const;
	const float GetValue() const;

	void SetMaxValue(float max)
	{
		m_maxValue = max;
	}

	void SetValue(float nowValue)
	{
		m_Value = nowValue;
		if (m_Value < 0.0f)
		{
			m_Value = 0.0f;
		}
	}
};


/// <summary>
/// テキストUI
/// Transformのスケールはxの値を参照する
/// </summary>
class UIText : public UIBase
{

protected:
	FontRender m_fontRender;	//表示する文字

	Vector4 m_color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);	//文字の色

public:
	UIText() {};
	~UIText() {};

	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
	virtual void SetText(const wchar_t* text);
	virtual void SetColor(Vector4 color) override;
	virtual void SetColor(float r, float g, float b, float a) override;
	virtual Vector4 GetColor()override;
	virtual void SetPivot(float x, float y);
	virtual void SetPivot(const Vector2& pivot);
};

