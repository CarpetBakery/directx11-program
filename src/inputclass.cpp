#include "inputclass.h"

void InputClass::initialize()
{
    for (int i = 0; i < KEY_COUNT; i++)
    {
        m_keys[i] = false;
    }
}

void InputClass::key_down(unsigned int input)
{
    m_keys[input] = true;
}

void InputClass::key_up(unsigned int input)
{
    m_keys[input] = false;
}

bool InputClass::is_key_down(unsigned int input)
{
    return m_keys[input];
}