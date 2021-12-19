#include "DrawState.h"

#include "../Opal/vendor/imgui/imgui.h"
#include "../Opal/vendor/imgui/imgui_impl_glfw.h"
#include "../Opal/vendor/imgui/imgui_impl_vulkan.h"
#include "../Opal/vendor/imgui/imgui_stdlib.h"

std::shared_ptr<Opal::RenderPass> DrawState::mRenderPass = nullptr; 
std::shared_ptr<Opal::LineRenderer> DrawState::mLineRenderer = nullptr;
std::shared_ptr<Opal::CircleRenderer> DrawState::mCircleRenderer = nullptr;
std::shared_ptr<Opal::BatchRenderer2D> DrawState::mBatchRenderer = nullptr;
std::shared_ptr<Opal::Camera> DrawState::mCamera = nullptr;

void DrawState::Tick()
{
    glm::vec2 mouse =   mCamera->ScreenToWorldPoint(Opal::InputHandler::GetMousePos());
    mLeftShiftDown =    Opal::InputHandler::GetKey(GLFW_KEY_LEFT_SHIFT);
    mLeftCtrlDown =     Opal::InputHandler::GetKey(GLFW_KEY_Q);
    mLMBDown =          Opal::InputHandler::GetLeftMouseButtonDown();
    mSpaceDown =        Opal::InputHandler::GetKey(GLFW_KEY_SPACE);

    if(mSpaceDown && !mLastSpaceDown)
    {
        switch(mCurrentState)
        {
            case ToolState::DRAW:
            mCurrentState = ToolState::SHADE;
            break;

            default:
            mCurrentState = ToolState::DRAW;
            break;
        }
    }

    switch(mCurrentState)
    {

        case ToolState::DRAW:
            
            // Point placement
        if(!mLeftCtrlDown && mLMBDown && !mLastLMBDown)
        {
            if(mLeftShiftDown && mSelectedPoint != nullptr)
                CreateLinePoint(mSelectedPoint->GetComponent<Opal::LinePointComponent>()->Position);
            else
                CreateLinePoint(mouse);
        }
        else if(mLinePoints.size() % 2 == 1)
        {
            auto lastpoint = mLinePoints[mLinePoints.size()-1]->GetComponent<Opal::LinePointComponent>();

            mLineRenderer->DrawLine(lastpoint->Position, mouse, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), 3);
        }

        // Movement
        if(mLeftCtrlDown && mLMBDown && mSelectedPoint != nullptr)
        {
            auto linePoint = mSelectedPoint->GetComponent<Opal::LinePointComponent>();
            auto transform = mSelectedPoint->GetComponent<Opal::TransformComponent>();

            glm::vec2 movement = mouse - mLastMousePos;
            transform->Position += glm::vec3(movement.x, movement.y, 0);
            linePoint->Position += movement;
        }

        // Selection
        if(!mLeftCtrlDown || !mLMBDown)
        {
            if(Opal::InputHandler::GetKey(GLFW_KEY_TAB) && !mLastTabDown)
            {
                mSelectionAlias++;
            }
            
            mSelectedPoint = nullptr;

            std::vector<std::shared_ptr<Opal::Entity> > candidates;
            for(auto point : mLinePoints)
            {
                auto collider = point->GetComponent<Opal::BoxColliderComponent2D>();

                if(Opal::AABBCollision::Intersects(mouse, collider->GetAABB()))
                {
                    candidates.push_back(point);
                }
            }

            if(candidates.size() > 0)
            {
                mSelectedPoint = candidates[mSelectionAlias % candidates.size()];
            }
        }
        break;

        case ToolState::SHADE:
        break;
    }

    mLastTabDown = Opal::InputHandler::GetKey(GLFW_KEY_TAB);
    mLastLMBDown = Opal::InputHandler::GetLeftMouseButtonDown();
    mLastMousePos = mouse;
    mLastSpaceDown = Opal::InputHandler::GetKey(GLFW_KEY_SPACE);
}

void DrawState::Render()
{
    mRenderPass->Record();
    mRenderPass->EndRecord();
    mGame->Renderer->SubmitRenderPass(mRenderPass);

    DrawLines();
    mLineRenderer->Render();

    if(mLeftShiftDown || mLeftCtrlDown)
    {
        DrawPoints();
        mCircleRenderer->Render();
    }

    if(mCurrentState == ToolState::SHADE)
    {
        DrawCrossHatchingUI();
    }
    else
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        ImGui::Render();

        ImGui::EndFrame();
    }
}

void DrawState::Begin()
{

    if(mRenderPass == nullptr)
    {
        // TODO: Magic numbers
        mCamera = mGame->Renderer->CreateOrthoCamera(1920, 1080, -100, 100, true);
        mRenderPass = mGame->Renderer->CreateRenderPass(true);
        mRenderPass->SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        mLineRenderer = std::make_shared<Opal::LineRenderer>();
        mLineRenderer->Init(mGame->Renderer, true);
        mCircleRenderer = std::make_shared<Opal::CircleRenderer>();
        mCircleRenderer->Init(mGame->Renderer, true);

        mBatchRenderer = mGame->Renderer->CreateBatch(mRenderPass,1000, std::vector< std::shared_ptr<Opal::Texture> >(), true);
    }

    mScene = std::make_shared<Opal::Scene>(mBatchRenderer);

}

void DrawState::End()
{

}

void DrawState::Resume()
{

}

DrawState::~DrawState()
{

}

std::shared_ptr<Opal::Entity> DrawState::CreateLinePoint(glm::vec2 position)
{
    auto res = std::make_shared<Opal::Entity>();
    auto trans = std::make_shared<Opal::TransformComponent>();
    trans->Position = glm::vec3(position.x, position.y, 0);
    auto point = std::make_shared<Opal::LinePointComponent>(position, mLinePoints.size());
    auto collider = std::make_shared<Opal::BoxColliderComponent2D>(glm::vec2(mCircleRadius * 2, mCircleRadius *2), glm::vec2(-mCircleRadius, -mCircleRadius), true);

    res->AddComponent(trans);
    res->AddComponent(point);
    res->AddComponent(collider);

    mLinePoints.push_back(res);
    return res;
}

void DrawState::DrawLines()
{
    for(int i = 1; i < mLinePoints.size(); i+= 2)
    {
        auto trans1 = mLinePoints[i-1]->GetComponent<Opal::TransformComponent>();
        auto trans2 = mLinePoints[i]->GetComponent<Opal::TransformComponent>();

        auto point1 = mLinePoints[i-1]->GetComponent<Opal::LinePointComponent>();
        auto point2 = mLinePoints[i]->GetComponent<Opal::LinePointComponent>();

        mLineRenderer->DrawLine(point1->Position, point2->Position, (mLinePoints[i-1] == mSelectedPoint) ? mLineSelectedColor : mLineBaseColor, (mLinePoints[i] == mSelectedPoint) ? mLineSelectedColor : mLineBaseColor, 5);
    }
}

void DrawState::DrawPoints()
{
    for(int i = 0; i < mLinePoints.size(); i++)
    {
        auto point = mLinePoints[i]->GetComponent<Opal::LinePointComponent>();

        mCircleRenderer->DrawCircle(point->Position, mCircleRadius, (mLinePoints[i] == mSelectedPoint) ? mCircleSelectedColor : mCircleBaseColor, mCircleInnerRadius);
    }
}

void DrawState::GenerateCrossHatching()
{
    float y = 0;
    while(true)
    {
        glm::vec2 rayStart = glm::vec2(0, y);
        glm::vec2 rayEnd = glm::vec2(1920, y - sin(mCrossHatchingAngle) * 1080);

        if(rayStart.y > 1080 && rayEnd.y > 1080)
        {
            break;
        }

        std::vector<glm::vec2> castPoints;

        for(int i = 1; i < mLinePoints.size(); i+=2)
        {
            glm::vec2 raycastRes = Opal::AABBCollision::RaycastSegment(rayStart, rayEnd, mLinePoints[i-1]->GetComponent<Opal::LinePointComponent>()->Position,
             mLinePoints[i]->GetComponent<Opal::LinePointComponent>()->Position);
            if(raycastRes != rayEnd)
            {
                castPoints.push_back(raycastRes);
            }
        }


        std::sort(castPoints.begin(), castPoints.end(),
            [rayStart](const glm::vec2& lhs, const glm::vec2& rhs) {return glm::distance(rayStart, lhs) < glm::distance(rayStart, rhs);} );

        // TODO make start value a variable
        bool alternator = false;
        for(int i = 1; i < castPoints.size(); i+=2)
        {
            CreateLinePoint(castPoints[i-1]);
            CreateLinePoint(castPoints[i]);
        }

        y += mCrossHatchingDistance;
    }

}

void DrawState::DrawCrossHatchingUI()
{
    // TODO: Move these two lines into the game or renderer with a special
    // function for rendering UI
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("Cross Hatching");
    ImGui::DragFloat("Distance", &mCrossHatchingDistance);
    ImGui::DragFloat("Angle", &mCrossHatchingAngle);
    ImGui::Checkbox("Draw Overlapping", &mCrossHatchingOverlapping);
    if(mCrossHatchingOverlapping)
    {
        ImGui::DragFloat("Overlap Distance", &mCrossHatchingOverlapDistance);
        ImGui::DragFloat("Overlap Angle", &mCrossHatchingOverlapAngle);
    }
    if(ImGui::Button("Generate"))
    {
        GenerateCrossHatching();
    }
    ImGui::End();

    ImGui::Render();
    ImGui::EndFrame();
    
}

