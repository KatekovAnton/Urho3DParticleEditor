//
//  BitmapComposer.hpp
//  maxtouchTool
//
//  Created by Katekov Anton on 8/11/16.
//  Copyright © 2016 katekovanton. All rights reserved.
//

#ifndef BitmapComposer_hpp
#define BitmapComposer_hpp

#include <stdio.h>
#include <Shared/Utils/Math/Geometry.h>
#include <Shared/Utils/Math/Colors.h>



class BitmapTexture;



class BitmapComposer {
    
    BitmapTexture *_resultBitmap;
    
public:
    
    BitmapComposer(GSize2D size);
    ~BitmapComposer();
    
    void setColor(Color color, int x, int y);
    
    GSize2D getSize();
    Color *getColorBuffer();
    
};

#endif /* PBitmapComposer_hpp */
