#pragma once

#include <memory>

#include "../Opal/Game.h"
#include "../Opal/Gamestate/Gamestate.h"

#include "../Opal/EntityComponent/Scene.h"
#include "../Opal/EntityComponent/Entity.h"
#include "../Opal/EntityComponent/TransformComponent.h"
#include "../Opal/EntityComponent/BoxColliderComponent2D.h"
#include "../Opal/EntityComponent/LinePointComponent.h"

#include "../Opal/Graphics/RenderPass.h"
#include "../Opal/Graphics/LineRenderer.h"
#include "../Opal/Graphics/CircleRenderer.h"
#include "../Opal/Graphics/BatchRenderer2D.h"

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
    static std::shared_ptr<Opal::CircleRenderer> mCircleRenderer;
    static std::shared_ptr<Opal::BatchRenderer2D> mBatchRenderer;

    std::shared_ptr<Opal::Scene> mScene;

    std::vector<std::shared_ptr<Opal::Entity> > mLinePoints;
    std::shared_ptr<Opal::Entity> mSelectedPoint;
    int mSelectionAlias = 0;
    std::shared_ptr<Opal::Entity> CreateLinePoint(glm::vec2 position);

    glm::vec4 mLineBaseColor = glm::vec4(1,1,1,1);
    glm::vec4 mLineSelectedColor = glm::vec4(0,1,0,1);
    void DrawLines();

    float mCircleRadius = 20;
    float mCircleInnerRadius = 2;
    glm::vec4 mCircleBaseColor = glm::vec4(1,1,1,1);
    glm::vec4 mCircleSelectedColor = glm::vec4(0,1,0,1);
    void DrawPoints();

    bool mLMBDown = false;
    bool mLastLMBDown = false;
    bool mLeftShiftDown = false;
    bool mLastTabDown = false;
    bool mLeftCtrlDown = false;

    glm::vec2 mLastMousePos = glm::vec2(0,0);
};
