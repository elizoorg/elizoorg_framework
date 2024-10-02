#pragma once
#include "Exports.h"
#include "SphereComponent.h"
class ENGINE_API LightComponent :
    public SphereComponent
{
public:
    LightComponent(Engine::Application* _app);
    
    
    void Draw();

    

};

