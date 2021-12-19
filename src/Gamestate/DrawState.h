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
#include "../Opal/Graphics/Camera.h"

class DrawState : public Opal::Gamestate 
{
    public:

    enum ToolState
    {
        DRAW,
        SHADE
    };

    virtual void Tick() override;
    virtual void Render() override;
    virtual void Begin() override;
    virtual void End() override;
    virtual void Resume() override;

    ~DrawState();

    private:

    ToolState mCurrentState = ToolState::DRAW;

    static std::shared_ptr<Opal::RenderPass> mRenderPass;
    static std::shared_ptr<Opal::LineRenderer> mLineRenderer;
    static std::shared_ptr<Opal::CircleRenderer> mCircleRenderer;
    static std::shared_ptr<Opal::BatchRenderer2D> mBatchRenderer;
    static std::shared_ptr<Opal::Camera> mCamera;

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
    bool mSpaceDown = false;
    bool mLastSpaceDown = false;

    glm::vec2 mLastMousePos = glm::vec2(0,0);

    float mCrossHatchingDistance = 10;
    float mCrossHatchingAngle = 0;
    bool mCrossHatchingOverlapping = false;
    float mCrossHatchingOverlapDistance = 10;
    float mCrossHatchingOverlapAngle = 0;
    std::vector<std::shared_ptr<Opal::Entity> > mCrossHatchingBounds;
    std::vector<std::shared_ptr<Opal::Entity> > mCrossHatchingPreview;
    void GenerateCrossHatching();
    void DrawCrossHatchingUI();
};
