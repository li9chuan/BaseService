#pragma once
#include <game_share/sigslot.h>
#include "iostream"

struct dog
{
    sigslot::signal1<int> bark;
    void beaten()
    {
        bark(100);
    }
};

struct human : public sigslot::has_slots<>
{
    void dog_bark(int vol)
    {
        std::cout<<"human:"<<vol<<std::endl;
    }
};

int test_sigsolt();

