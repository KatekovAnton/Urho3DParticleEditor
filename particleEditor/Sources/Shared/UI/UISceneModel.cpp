#include "UISceneModel.h"

#include <Urho3D/Core/ProcessUtils.h>

#include <chrono>



UISceneModel::UISceneModel()
{

}

UISceneModel::~UISceneModel()
{

}

void UISceneModel::onCursorPosition(const UIUserClickResult &clickResult)
{
    _currentCursor = clickResult;
    _onCursorChanged(_currentCursor);
}

void UISceneModel::onUserClicked(const UIUserClickResult &clickResult)
{
    _currentCursor = clickResult;
    onCursorPosition(clickResult);
    onUserClickedNothing(clickResult);
}

void UISceneModel::onUserClickedNothing(const UIUserClickResult &clickResult)
{
    _onClickedNothing(clickResult);
}
