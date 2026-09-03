#pragma once

class InputClass
{
public:
    static constexpr int KEY_COUNT = 256;

    InputClass() = default;
    InputClass(const InputClass&) = default;
    ~InputClass() = default;

    void initialize();

    void key_down(unsigned int);
    void key_up(unsigned int);
    bool is_key_down(unsigned int);

private:
    bool m_keys[KEY_COUNT];
};