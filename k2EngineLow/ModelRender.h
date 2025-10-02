#pragma once

#include "geometry/AABB.h"
#include "SceneLight.h"
#include "IRenderer.h"
#include "Shadow.h"
namespace nsK2EngineLow
{
	//class RenderingEngine;
	class ModelRender : public IRenderer
	{
	public:
		//�f�B���N�V�������C�g�̍\����
		struct DirectionLight
		{
			Vector3 direction;
			float pad0;
			//int castShadow = true;
			Vector3 color;
			float pad1;
		};

		//�|�C���g���C�g�̍\����
		struct PointLight
		{
			Vector3 ptPosition;			// �ʒu
			float pd2;
			Vector3 ptColor;			// �J���[
			float ptRange;				// �e���͈�

		};

		//���C�g�̍\����
		struct Light
		{
			DirectionLight directionalLight[4];	//�f�B���N�V�������C�g�̔z��
			PointLight pointLight[4];	//�|�C���g���C�g
			Vector3 eyePos;		//���_�̈ʒu
			float pad1;
			Vector3 ambientLight;	//����
			float pad2;
			Matrix m_mt;
		};

		ModelRender();
		~ModelRender();

		
		void Init(
			const char* tkmFilePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool m_isShadowCaster = true,
			bool m_isShadowReceiver = true
			);

		void InitShadowModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);

		//void InitInstancingShadowDrawModel(
		//	const char* tkmFilePath,
		//	EnModelUpAxis enModelUpAxis);
		void Update();
		/// <summary>
		/// �C���X�^���V���O�f�[�^�̍X�V
		/// </summary>
		/// <param name="instanceNo">�C���X�^���X�ԍ�</param>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		//void UpdateInstancingData(int instanceNo, const Vector3& pos, const Quaternion& rot, const Vector3& scale);
		//�`�揈��
		void Draw(RenderContext& rc);
		
		//�V���h�E�}�b�v�ւ̕`��p�X����Ă΂�鏈��
		void OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix)override;
		/// <summary>
		/// ���f���̕`��
		/// </summary>
		/// <param name="rc">RenderContext</param>
		void OnRenderModel(RenderContext& rc)override;
		//void DirLigMove();
		//void PtLigMove();
		//���W���X�V
		void SetPosition(Vector3 pos)
		{
			m_position = pos;
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}
		/// <summary>
		/// ��]�ݒ�
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}
		

		/// <summary>
		/// �T�C�Y�ݒ�
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		void SetScale(const float x, const float y, const float z)
		{
			SetScale({ x,y,z });
		}
		void SetScale(const float xyz)
		{
			SetScale({ xyz,xyz,xyz });
		}

		/// <summary>
		/// ���W�E�g��E��]��ݒ�
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="rot"></param>
		/// <param name="scale"></param>
		void SetTRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rot);
			SetScale(scale);
		}

		const Matrix& GetLigCameraViewProjection()
		{
			return m_shadow.GetLigCameraViewProjection();
		}

		//���f�����擾
		Model& GetModel()
		{
			return m_model;
		}

		//���f���̍��W���擾
		Vector3& GetPosition()
		{
			return m_position;
		}

		//���f����X���W���擾
		float& GetPositionX()
		{
			return m_position.x;
		}

		//���f����Y���W���擾
		float& GetPositionY()
		{
			return m_position.y;
		}

		//���f����Z���W���擾
		float& GetPositionZ()
		{
			return m_position.z;
		}

		/// <summary>
		/// �X�P���g���̏�����
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// �A�j���[�V�����̏�����
		/// </summary>
		/// <param name="animationClips"></param>
		/// <param name="numAnimationClips"></param>
		/// <param name="enModelUpAxis"></param>
		void InitAnimation(
			AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		);
		/*/// <summary>
		/// �A�j���[�V�����ςݒ��_�o�b�t�@�̌v�Z������������
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitComputeAnimationVertexBuffer(
			const char* tkmFilePath,
			EnModelUpAxis enModelUpAxis
		);*/

		/// <summary>
		/// �A�j���[�V�����Đ�
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ�</param>
		/// <param name="interpolateTime">��Ԏ��ԁi�P��:�b�j</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		//�A�j���[�V�����Đ����H
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		//�A�j���[�V�����Đ��̑��x��ݒ�
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}
		//�A�j���[�V�����C�x���g
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// �X�J�C�L���[�u�̏�����
		/// </summary>
		/// <param name="initData"></param>
		void InitSkyCubeModel(ModelInitData& initData)
		{
			m_model.Init(initData);
		}

		/// <summary>
		/// �V���h�E�L���X�^�[�̃t���O��ݒ肷��
		/// </summary>
		/// <param name="flag"></param>
		void SetShadowCasterFlag(bool flag)
		{
			m_isShadowCaster = flag;
		}

		/// <summary>
		/// �V���h�E�L���X�^�[�H
		/// </summary>
		/// <returns></returns>
		bool IsShadowCaster() const
		{
			return m_isShadowCaster;
		}

		/// <summary>
		/// �J�����O�̑Ώۂ���O��
		/// </summary>
		void SetDontCulling()
		{
			m_doCulling = false;
		}
	private:
		Model m_model;					//���f��
		//Model m_bgModel;
		//Model m_renderToGBufferModel;	//RenderToGBuffer�ŕ`�悳��郂�f��
		Model m_shadowModel;			//�V���h�E�}�b�v
		Light m_light;					//�V�F�[�_�[�ɑ��郉�C�g�̏��
		Vector3 m_position = Vector3::Zero;		//���W
		Quaternion m_rotation;			//��]
		Vector3 m_scale = g_vec3One;	//�g�嗦
		Shadow m_shadow;
		Skeleton m_skeleton;
		AnimationClip* m_animationClips = nullptr;
		int m_numAnimationClips = 0;
		Animation m_animation;
		//ComputeAnimationVertexBuffer m_computeAnimationVertexBuffer;	// �A�j���[�V�����ςݒ��_�o�b�t�@�̌v�Z�����B
		float m_animationSpeed = 1.0f; //�A�j���[�V�����̍Đ��X�s�[�h
		//RenderTarget m_shadowMap;
		int m_moveState = 0;
		bool m_isEnableInstancingDraw = false; //�C���X�^���V���O�`�悪�L�����H
		bool m_isRaytrecingWorld = true;	   //���C�g�����[���h�ɓo�^���邩�H
		StructuredBuffer m_worldMatrixArraySB; //���[���h�s��̔z��̃X�g���N�`���[�h�o�b�t�@
		bool m_isShadowCaster = false;		   //�V���h�E�L���X�^�[�t���O
		bool m_doCulling = true;			   //�J�����O���邩�ǂ���

		int							m_maxInstance = 1;					// �ő�C���X�^���X���B
		Model						m_translucentModel;					// ���������f���B
		Model						m_zprepassModel;					// ZPrepass�ŕ`�悳��郂�f��
		int							m_numInstance = 0;					// �C���X�^���X�̐��B



		std::unique_ptr<Matrix[]>	m_worldMatrixArray;					// ���[���h�s��̔z��B
		std::unique_ptr<int[]>		m_instanceNoToWorldMatrixArrayIndexTable;	// �C���X�^���X�ԍ����烏�[���h�s��̔z��̃C���f�b�N�X�ɕϊ�����e�[�u���B
	};
}

