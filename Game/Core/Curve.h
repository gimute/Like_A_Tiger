#pragma once
#include <algorithm>

//Curve::AIすげーーーーーー

enum class EasingType { Linear, EaseIn, EaseOut, EaseInOut };
enum class LoopMode { Once, Loop, PingPong };

template <typename T>
class Curve 
{
private:
    T _startValue;
    T _endValue;
    float _duration;
    float _currentTime;
    EasingType _easingType = EasingType::EaseInOut;
    LoopMode _loopMode;
    bool _isPlaying;
    int _direction;

    // 時間の計算ロジック
    float ApplyEasingInternal(float t) const {
        switch (_easingType) {
        case EasingType::Linear:
            return t;

        case EasingType::EaseIn:    
            return t * t;

        case EasingType::EaseOut:   
            return t * (2.0f - t);

        case EasingType::EaseInOut:
            if (t < 0.5f)
            {
                return 2.0f * t * t;
            }
            else 
            { 
                return -1.0f + (4.0f - 2.0f * t) * t; 
            }
        default:
            {
                return t;
            }
        }
    }
public:
        Curve() : _currentTime(0), _duration(1.0f), _isPlaying(false), _direction(1) {}

        // 初期化
        void Play(T start, T end, float timeSec, EasingType type = EasingType::EaseInOut, LoopMode loopMode = LoopMode::Once) 
        {
            //もろもろの値の初期化
            _startValue = start;        
            _endValue = end; 
            _duration = (std::max)(0.0001f, timeSec);
            _easingType = type;
            _loopMode = loopMode;
            _currentTime = 0.0f;
            _isPlaying = true;
            _direction = 1;
        }
        // 更新処理
        void Update() {
            if (!_isPlaying) return;

            if (_loopMode == LoopMode::PingPong)
            {
                _currentTime += g_gameTime->GetFrameDeltaTime() * _direction;
                
            }
            else
            {
                _currentTime += g_gameTime->GetFrameDeltaTime();
            }

            if (_currentTime >= _duration) 
            {
                if (_loopMode == LoopMode::Once) 
                {
                    _currentTime = _duration;
                    _isPlaying = false;
                }
                else if (_loopMode == LoopMode::Loop)
                {
                    _currentTime = 0.0f;
                }
                else if (_loopMode == LoopMode::PingPong) 
                {
                    _currentTime = _duration;
                    _direction = -1;
                }
            }
            else if (_currentTime <= 0.0f) {
                if (_loopMode == LoopMode::PingPong) 
                {
                    _currentTime = 0.0f;
                    _direction = 1;
                }
            }
        }

        // 現在の値を取得
        T GetCurrentValue() const {
            float t = (std::max)(_currentTime / _duration, 0.0f);
            t = (std::min)(t, 1.0f);
            float rate = ApplyEasingInternal(t);

            // 線形補間 (Lerp)
            return _startValue + (_endValue - _startValue) * rate;
        }

        bool IsPlaying() const { return _isPlaying; }
};
