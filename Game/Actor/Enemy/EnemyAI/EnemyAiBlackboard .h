#pragma once
#include "stdafx.h"

struct Blackboard
{
public:
	//対象座標
	Vector3 m_position = Vector3::Zero;
	//対象正面ベクトル
	Vector3 m_forward = Vector3::Zero;
};

struct TargetCharacterView
{
public:
	//対象座標
	Vector3 m_targetPosition = Vector3::Zero;
	//対象正面ベクトル
	Vector3 m_targetForward = Vector3::Zero;
};
