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

class BoxInvisibleWall : public InvisibleWallBase
{
public:
    BoxInvisibleWall();
    ~BoxInvisibleWall();

    bool Start() override;
    void Update() override;

    void Create(Vector3 pos, Vector3 size);
    void Delete();
};

class CircleInvisibleWall : public InvisibleWallBase
{
public:
    CircleInvisibleWall();
    ~CircleInvisibleWall();

    bool Start() override;
    void Update() override;

    void Create(Vector3 pos, float radius);
    void Delete();
};