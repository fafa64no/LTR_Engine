#pragma once

// ############################################################################
//                            Cube opti
// ############################################################################
const float cube_vertices[]={
    //Positions            //Colors            //Texture     //Normal vector   
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   -1.0, -1.0, -1.0,  //back  left    bottom
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   -1.0, -1.0,  1.0,  //back  left    top
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   -1.0,  1.0, -1.0,  //back  right   bottom
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f,   -1.0,  1.0,  1.0,  //back  right   top
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,    1.0, -1.0, -1.0,  //front left    bottom
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,    1.0, -1.0,  1.0,  //front left    top
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f,    1.0,  1.0, -1.0,  //front right   bottom
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,    1.0,  1.0,  1.0   //front right   top
};
const unsigned int cube_indices[]={
    0,1,2,1,2,3,    //back
    4,5,6,5,6,7,    //front

    1,3,5,3,5,7,    //top
    0,2,4,2,4,6,    //bottop

    0,1,4,1,4,5,    //left
    2,3,6,3,6,7     //right
};

// ############################################################################
//                            Light cube
// ############################################################################
const float light_vertices[]={
    //Positions            //Colors 
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,  //back  left    bottom
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,  //back  left    top
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,  //back  right   bottom
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,  //back  right   top
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,  //front left    bottom
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,  //front left    top
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,  //front right   bottom
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f   //front right   top
};
const unsigned int light_indices[]={
    0,1,2,1,2,3,    //back
    4,5,6,5,6,7,    //front

    1,3,5,3,5,7,    //top
    0,2,4,2,4,6,    //bottop

    0,1,4,1,4,5,    //left
    2,3,6,3,6,7     //right
};
// ############################################################################
//                            Cube normal
// ############################################################################
const float cube_with_normals_vertices[]={
    //Positions            //Colors            //Texture     //Normal vector   
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   -1.0,  0.0,  0.0,  //back  left    bottom
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   -1.0,  0.0,  0.0,  //back  left    top
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   -1.0,  0.0,  0.0,  //back  right   bottom
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   -1.0,  0.0,  0.0,  //back  right   top

     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,    1.0,  0.0,  0.0,  //front left    bottom
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    1.0,  0.0,  0.0,  //front left    top
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    1.0,  0.0,  0.0,  //front right   bottom
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    1.0,  0.0,  0.0,  //front right   top

    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    0.0, -1.0,  0.0,  //back  left    bottom
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    0.0, -1.0,  0.0,  //back  left    top
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,    0.0, -1.0,  0.0,  //front left    bottom
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    0.0, -1.0,  0.0,  //front left    top

    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,    0.0,  1.0,  0.0,  //back  right   bottom
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    0.0,  1.0,  0.0,  //back  right   top
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    0.0,  1.0,  0.0,  //front right   bottom
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    0.0,  1.0,  0.0,  //front right   top

    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    0.0,  0.0, -1.0,  //back  left    bottom
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    0.0,  0.0, -1.0,  //back  right   bottom
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,    0.0,  0.0, -1.0,  //front left    bottom
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    0.0,  0.0, -1.0,  //front right   bottom

    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    0.0,  0.0,  1.0,  //back  left    top
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    0.0,  0.0,  1.0,  //back  right   top
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,    0.0,  0.0,  1.0,  //front left    top
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    0.0,  0.0,  1.0   //front right   top
};
const unsigned int cube_with_normals_indices[]={
    0,1,2,1,2,3,        //back
    4,5,6,5,6,7,        //front

    8,9,10,9,10,11,     //left
    12,13,14,13,14,15,  //right

    16,17,18,17,18,19,    //top
    20,21,22,21,22,23     //bottop
};

// ############################################################################
//                            Rectangle
// ############################################################################
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