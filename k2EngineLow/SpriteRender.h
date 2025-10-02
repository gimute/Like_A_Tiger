#pragma once

namespace nsK2EngineLow
{

	class SpriteRender : public IRenderer
	{
	public:
		//������
		void Init(const char* filePath,								//�t�@�C���p�X
			const float w,											//�摜�̉���
			const float h,											//�摜�̏c��
			AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);	//�f�t�H���g�͔���������

		//���[�U�[���p�ӂ����f�[�^�ŏ�����
		void Init(SpriteInitData initData)
		{
			m_sprite.Init(initData);
		}
		
		//���W�ݒ�
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		//���W�擾
		const Vector3& GetPosition()const
		{
			return m_position;
		}

		//�傫����ݒ�
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		void SetScale(const float& scale)
		{
			m_scale.x = scale;
			m_scale.y = scale;
			m_scale.z = scale;
		}
		//�傫������Z
		void AddScale(const float& scale)
		{
			m_scale *= scale;
		}
		void AddScale(const Vector3& scale)
		{
			m_scale.x *= scale.x;
			m_scale.y *= scale.y;
			m_scale.z *= scale.z;
		}

		//�傫�����擾
		const Vector3& GetScale() const
		{
			return m_scale;
		}
		//��]���擾
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}

		//��]��ݒ�
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		//�s�{�b�g�̐ݒ�
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		//�s�{�b�g���擾
		const Vector2& GetPivot()const
		{
			return m_pivot;
		}

		//��Z�J���[��ݒ�
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}
		//��Z�J���[���擾
		const Vector4& GetMulColor()const
		{
			return m_sprite.GetMulColor();
		}

		//�ǉ�
		//void SetTexture(const std::string& filePath);

		//�X�V����
		void Update()
		{
			m_sprite.Update(
				m_position,
				m_rotation,
				m_scale,
				m_pivot);
		}

		//�`�揈��
		void Draw(RenderContext& rc);

		void OnRender2D(RenderContext& rc) override
		{
			m_sprite.Draw(rc);
		}

	private:
	public:
	private:
		Sprite m_sprite;
		//Texture m_texture;
		Vector3 m_position = Vector3::Zero;
		Vector3 m_scale = Vector3::One;
		Quaternion m_rotation = Quaternion::Identity;
		Vector2	m_pivot = Sprite::DEFAULT_PIVOT;
	};

}