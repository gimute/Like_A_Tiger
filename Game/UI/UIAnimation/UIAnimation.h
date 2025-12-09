#pragma once

/*
* UIの座標、大きさ、色などを滑らかに変更させる機能
*/

/// <summary>
/// UIアニメーションの基底クラス
/// UpdateValue関数をオーバーライドしアニメーションの処理を記述
/// UpdateCore関数が時間経過の処理を行い、アニメーションの経過割合を計算してUpdateValue関数を呼び出す
/// </summary>
class UIAnimationBase
{
protected:
	std::shared_ptr<UIBase> m_ui;					//アニメーションさせるUI
	float m_elapsedTime = 0.0f;				//経過時間
	std::vector<float> m_animationTimeList;	//アニメーションの時間リスト
	int m_targetIndex = 0;					//目標値(座標、大きさ、色等)のインデックス
	bool m_isLoop = false;					//ループするか
	bool m_isCompleted = false;				//処理が完了したか
	bool m_isPlay = false;					//再生するかどうか


protected:
	/// <summary>
	/// 継承先のUpdateで呼び出す更新処理、
	/// 時間経過の処理、現在の再生割合を計算しUpdateValueを呼び出す
	/// </summary>
	void UpdateCore();
	/// <summary>
	/// 経過時間の割合を受け取ってUIに変更を与える処理、
	/// UpdateCore関数から呼ばれる
	/// </summary>
	/// <param name="elapsedPercent"></param>
	virtual void UpdateValue(float elapsedPercent) = 0;

	/// <summary>
	/// 更新処理をしてもいいかどうか
	/// </summary>
	/// <returns></returns>
	bool CanUpdate();

public:

	UIAnimationBase(std::shared_ptr<UIBase> ui, const bool isLoop, std::vector<float> animationTimeList)
		:m_ui(ui)
		, m_isLoop(isLoop)
		, m_animationTimeList(animationTimeList)
	{
	}

	virtual ~UIAnimationBase() {};

	//更新処理
	virtual void Update() = 0;

	

	/// <summary>
	/// アニメーションをリストの最初から再生
	/// </summary>
	void Play()
	{
		m_isPlay = true;
		m_isCompleted = false;
		m_elapsedTime = 0.0f;
		m_targetIndex = 0;
	}

	//再生を止める
	void Stop()
	{
		m_isPlay = false;
	}
};


/// <summary>
/// 座標アニメーション
/// </summary>
class PositionUIAnimation : public UIAnimationBase
{
private:
	std::vector<Vector3> m_targetPositionList;	//UIを動かす座標リスト
	Curve<Vector3> m_curve;	//イーズインアウトなどの計算クラス
public:
	PositionUIAnimation(std::shared_ptr<UIBase> ui, const bool isLoop, std::vector<float> animationTimeList, std::vector<Vector3> targetPositionList)
		:UIAnimationBase(ui, isLoop, animationTimeList)
		, m_targetPositionList(targetPositionList)
	{
		K2_ASSERT(m_targetPositionList.size() > 1, "ターゲット座標を2つ以上いれてください\n");
		K2_ASSERT(m_targetPositionList.size() - 1 == m_animationTimeList.size(), "ターゲット座標の数とアニメーション時間の数を見直してください");

		m_curve.Play(targetPositionList[0], targetPositionList[1], animationTimeList[0], EasingType::EaseInOut, LoopMode::Once);
	}

	void UpdateValue(float elapsedPercent) override;

	void Update() override;
};
/// <summary>
/// 相対位置で行う座標アニメーション
/// </summary>
class OffsetPositionUIAnimation : public UIAnimationBase
{
private:
	std::vector<Vector3> m_targetOffsetPositionList;
	Vector3 m_referencePosition = Vector3::Zero;	//基準座標

public:
	OffsetPositionUIAnimation(std::shared_ptr<UIBase> ui, const bool isLoop, std::vector<float> animationTimeList, std::vector<Vector3> targetPositionList)
		:UIAnimationBase(ui, isLoop, animationTimeList)
		,m_targetOffsetPositionList(targetPositionList)
	{
		K2_ASSERT(m_targetOffsetPositionList.size() > 1, "ターゲット座標を2つ以上いれてください\n");
		K2_ASSERT(m_targetOffsetPositionList.size() - 1 == m_animationTimeList.size(), "ターゲット座標の数とアニメーション時間の数を見直してください");
	}

	void UpdateValue(float elapsedPercent) override;

	void Update() override;

	//基準となる座標を設定
	void SetReferencePosition(Vector3 pos)
	{
		m_referencePosition = pos;
	}

	//現在設定されている基準座標を取得
	Vector3 GetReferencePosition()
	{
		return m_referencePosition;
	}
};

/// <summary>
/// 回転アニメーション
/// </summary>
class RotarionUIAnimation : public UIAnimationBase
{
private:
	std::vector<Quaternion> m_targetRotationList;	//UI拡縮させるスケールリスト
public:
	RotarionUIAnimation(std::shared_ptr<UIBase> ui, const bool isLoop, std::vector<float> animationTimeList, std::vector<Quaternion> targetRotationList)
		:UIAnimationBase(ui, isLoop, animationTimeList)
		, m_targetRotationList(targetRotationList)
	{
		K2_ASSERT(m_targetRotationList.size() > 1, "ターゲットローテーションを2つ以上いれてください\n");
		K2_ASSERT(m_targetRotationList.size() - 1 == m_animationTimeList.size(), "ターゲットローテーションの数とアニメーション時間の数を見直してください");
	}

	void UpdateValue(float elapsedPercent) override;

	void Update() override;
};


/// <summary>
/// スケールアニメーション
/// </summary>
class ScaleUIAnimation : public UIAnimationBase
{
private:
	std::vector<Vector2> m_targetScaleList;	//UI拡縮させるスケールリスト
public:
	ScaleUIAnimation(std::shared_ptr<UIBase> ui, const bool isLoop, std::vector<float> animationTimeList, std::vector<Vector2> targetScaleList)
		:UIAnimationBase(ui, isLoop, animationTimeList)
		, m_targetScaleList(targetScaleList)
	{
		K2_ASSERT(m_targetScaleList.size() > 1, "ターゲットスケールを2つ以上いれてください\n");
		K2_ASSERT(m_targetScaleList.size() - 1 == m_animationTimeList.size(), "ターゲットスケールの数とアニメーション時間の数を見直してください");
	}

	void UpdateValue(float elapsedPercent) override;

	void Update() override;
};


/// <summary>
/// 色アニメーション
/// </summary>
class ColorUIAnimation : public UIAnimationBase
{
private:
	std::vector<Vector4> m_targetColorList;	//UI拡縮させるスケールリスト
public:
	ColorUIAnimation(std::shared_ptr<UIBase> ui, const bool isLoop, std::vector<float> animationTimeList, std::vector<Vector4> targetColorList)
		:UIAnimationBase(ui, isLoop, animationTimeList)
		, m_targetColorList(targetColorList)
	{
		K2_ASSERT(m_targetColorList.size() > 1, "ターゲットカラーを2つ以上いれてください\n");
		K2_ASSERT(m_targetColorList.size() - 1 == m_animationTimeList.size(), "ターゲットカラーの数とアニメーション時間の数を見直してください");
	}

	void UpdateValue(float elapsedPercent) override;

	void Update() override;
};