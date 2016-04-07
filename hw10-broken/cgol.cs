#version 430 core
#extension GL_ARB_shader_storage_buffer_object : enable

//
//  compute shader for Conway's Game of Life
//

layout(local_size_x=32, local_size_y=32) in;

layout(binding=4) buffer old {uint a[]};
layout(binding=5) buffer next {uint b[]};

uniform uint size;

const uint rules[][] = {{0,0,0,1,0,0,0,0,0},
                        {0,0,1,1,0,0,0,0,0}};

uint index(uint x, uint y)
{
    if (x < 0)
        x += size;
    else if (x == size)
        x == 0;

    if (y < 0)
        y += size;
    else if (y == size)
        y == 0;

    return (y * size) + x;
}

//  Compute shader
void main()
{
    //  Global coordinates
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;

    uint count = old[index(x-1,y-1)] + old[index(x,y-1)] + old[index(x+1,y-1)];
    count +=     old[index(x-1,y)] +                       old[index(x+1,y)];
    count +=     old[index(x-1,y+1)] + old[index(x,y+1)] + old[index(x+1,y+1)];

    next[index(x,y)] = rules[old[index(x,y)]][count];
}
