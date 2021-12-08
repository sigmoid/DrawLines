#pragma once

#include <memory>

#include "../Opal/Game.h"
#include "../Opal/Gamestate/Gamestate.h"

#include "../Opal/Graphics/RenderPass.h"
#include "../Opal/Graphics/LineRenderer.h"

class DrawState : public Opal::Gamestate 
{
    public:

    virtual void Tick() override;
    virtual void Render() override;
    virtual void Begin() override;
    virtual void End() override;
    virtual void Resume() override;

    ~DrawState();

    private:

    static std::shared_ptr<Opal::RenderPass> mRenderPass;
    static std::shared_ptr<Opal::LineRenderer> mLineRenderer;
};
