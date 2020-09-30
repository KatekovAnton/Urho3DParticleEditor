#ifndef UISceneModel_h
#define UISceneModel_h

#include "UIDefines.h"
#include <functional>



class UISceneModel {
        
    UIUserClickResult _currentCursor;
    
public:
    
    // callbacks
    std::function<void(const UIUserClickResult &clickResult)> _onClickedNothing = nullptr;
    std::function<void(const UIUserClickResult &clickResult)> _onCursorChanged = nullptr;
    
    UISceneModel();
    ~UISceneModel();
    
    void onUserClicked(const UIUserClickResult &clickResult);
    
    void onCursorPosition(const UIUserClickResult &clickResult);
    void onUserClickedNothing(const UIUserClickResult &clickResult);
};

#endif
