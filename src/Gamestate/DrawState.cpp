#include "DrawState.h"

std::shared_ptr<Opal::RenderPass> DrawState::mRenderPass = nullptr; 
std::shared_ptr<Opal::LineRenderer> DrawState::mLineRenderer = nullptr;

void DrawState::Tick()
{
    auto mouse = Opal::InputHandler::GetMousePos();
    mLineRenderer->DrawLine(glm::vec2(0,0), mouse, glm::vec4(1,1,1,1), 1);
}

void DrawState::Render()
{
    mRenderPass->Record();
    mRenderPass->EndRecord();
    mGame->Renderer->SubmitRenderPass(mRenderPass);

    mLineRenderer->Render();
}

void DrawState::Begin()
{

    if(mRenderPass == nullptr)
    {
        mRenderPass = mGame->Renderer->CreateRenderPass(true);
        mRenderPass->SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        mLineRenderer = std::make_shared<Opal::LineRenderer>();
        mLineRenderer->Init(mGame->Renderer);
    }

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
