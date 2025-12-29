#pragma once

namespace
{
    const float CIRCLE_MODEL_RADIUS = 100.0f;   //円形透明壁用モデルの内側の円の半径
    const float BOX_SIZE = 100.0f;  //箱型透明壁用の立方体モデルの辺の長さ
}

/// <summary>
/// 見えない壁の形状モデルを保持するクラス
/// 同じモデルを見えない壁ごとに保持するのは勿体ないので分けてみた
/// </summary>
class InvisibleWallShape
{
public:
    enum ShapeName
    {
        Box,
        Circle,
        ShapeNum
    };

    Model m_shapeModels[ShapeNum];

    Model* GetShapeModel(ShapeName shape)
    {
        return &m_shapeModels[shape];
    }

private:
    InvisibleWallShape();
    ~InvisibleWallShape() {};


    //シングルトン用
private:
    static InvisibleWallShape* m_instance;

public:
    static InvisibleWallShape& GetInstance()
    {
        if (m_instance == nullptr)
        {
            m_instance = new InvisibleWallShape();
        }

        return *m_instance;
    }
};


class InvisibleWallBase : public IGameObject
{
protected:
    PhysicsStaticObject m_collision;    //壁コリジョン
    Effect m_wallEffect;    //壁エフェクト
    Effect m_lineEffect;    //壁の表面を流れる横線エフェクト

    bool m_active = false;

};

class InvisibleWall : public InvisibleWallBase
{
public:
    InvisibleWall();
    ~InvisibleWall();

    bool Start() override;
    void Update() override;

    /// <summary>
    /// 透明壁の生成
    /// 厚みは一定
    /// </summary>
    /// <param name="pos">座標</param>
    /// <param name="Width">横幅</param>
    /// <param name="Hight">高さ</param>
    /// <param name="angle">Y軸回転角度</param>
    void Create(Vector3 pos, float width, float hight, float angle);
    void Delete();
};

class CircleInvisibleWall : public InvisibleWallBase
{
public:
    CircleInvisibleWall();
    ~CircleInvisibleWall();

    bool Start() override;
    void Update() override;

    /// <summary>
    /// 透明壁の生成
    /// </summary>
    /// <param name="pos">座標</param>
    /// <param name="radius">半径</param>
    void Create(Vector3 pos, float radius);
    void Delete();
};