#pragma once

const float cube_vertices[]={
    //Positions            //Colors            //Texture position    
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //back  left    bottom
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,  //back  left    top
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  //back  right   bottom
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f,  //back  right   top
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  //front left    bottom
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,  //front left    top
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f,  //front right   bottom
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f   //front right   top
};
const unsigned int cube_indices[]={
    0,1,2,1,2,3,    //back
    4,5,6,5,6,7,    //front

    1,3,5,3,5,7,    //top
    0,2,4,2,4,6,    //bottop

    0,1,4,1,4,5,    //left
    2,3,6,3,6,7     //right
};
const float rectangle_vertices[]={
    //Positions            //Colors            //Texture position 
     0.8f,  0.8f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //right top
     0.8f, -0.8f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  //right bottom
    -0.8f, -0.8f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  //left  bottom
    -0.8f,  0.8f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   //left  top
};
const unsigned int rectangle_indices[]={  
    0,1,3,
    1,2,3
};