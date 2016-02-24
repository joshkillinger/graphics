package com.josh.hw6;

import android.graphics.Color;
import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

/**
 * Created by Josh on 2/22/2016.
 */
public class Shape
{
    private final String vertexShaderCode =
            // This matrix member variable provides a hook to manipulate
            // the coordinates of the objects that use this vertex shader
            "uniform mat4 uMVPMatrix;" +
            "attribute vec4 vPosition;" +
            "void main() {" +
            // the matrix must be included as a modifier of gl_Position
            // Note that the uMVPMatrix factor *must be first* in order
            // for the matrix multiplication product to be correct.
            "  gl_Position = uMVPMatrix * vPosition;" +
            "}";

    private final String fragmentShaderCode =
            "precision mediump float;" +
            "uniform vec4 vColor;" +
            "void main() {" +
            "  gl_FragColor = vColor;" +
            "}";

    private int shaderProgram;

    private FloatBuffer vertexBuffer;
    private ShortBuffer drawListBuffer;

    // number of coordinates per vertex in this array
    static final int COORDS_PER_VERTEX = 3;
    static float faceCoords[] = {
            -1,  1, -1,    // front top left
            -1, -1, -1,    // front bottom left
             1, -1, -1,    // front bottom right
             1,  1, -1,    // front top right
            -1,  1,  1,    // top back left
            -1,  1, -1,    // top front left
             1,  1, -1,    // top front right
             1,  1,  1,    // top back right
            -1,  1,  1,    // left top back
            -1, -1,  1,    // left bottom back
            -1, -1, -1,    // left bottom front
            -1,  1, -1,    // left top front
             1,  1, -1,    // right top front
             1, -1, -1,    // right bottom front
             1, -1,  1,    // right bottom back
             1,  1,  1,    // right top back
            -1, -1, -1,    // bottom front left
            -1, -1,  1,    // bottom back left
             1, -1,  1,    // bottom back right
             1, -1, -1,    // bottom front right
             1,  1,  1,    // back top right
             1, -1,  1,    // back bottom right
            -1, -1,  1,    // back bottom left
            -1,  1,  1,    // back top left
    };

    static float normals[] = {
             0,  0, -1,    0,  0, -1,    0,  0, -1,    0,  0, -1,   //front
             0,  1,  0,    0,  1,  0,    0,  1,  0,    0,  1,  0,   //top
            -1,  0,  0,   -1,  0,  0,   -1,  0,  0,   -1,  0,  0,   //left
             1,  0,  0,    1,  0,  0,    1,  0,  0,    1,  0,  0,   //right
             0, -1,  0,    0, -1,  0,    0, -1,  0,    0, -1,  0,   //bottom
             0,  0,  1,    0,  0,  1,    0,  0,  1,    0,  0,  1,   //back
    };

    private float color[] = {0.5f, 0.5f, 0.0f, 1.0f};

    private short drawOrder[] = {0, 1, 2, 0, 2, 3,          // front
                                 4, 5, 6, 4, 6, 7,          // top
                                 8, 9, 10, 8, 10, 11,       // left
                                 12, 13, 14, 12, 14, 15,    // right
                                 16, 17, 18, 16, 18, 19,    // bottom
                                 20, 21, 22, 20, 22, 23,    // back
    };

    public Shape()
    {
        // initialize vertex byte buffer for shape coordinates
        ByteBuffer vpb = ByteBuffer.allocateDirect(
                // (# of coordinate values * 4 bytes per float)
                faceCoords.length * 4);
        vpb.order(ByteOrder.nativeOrder());
        vertexBuffer = vpb.asFloatBuffer();
        vertexBuffer.put(faceCoords);
        vertexBuffer.position(0);


        // initialize byte buffer for the draw list
        ByteBuffer dlb = ByteBuffer.allocateDirect(
                // (# of coordinate values * 2 bytes per short)
                drawOrder.length * 2);
        dlb.order(ByteOrder.nativeOrder());
        drawListBuffer = dlb.asShortBuffer();
        drawListBuffer.put(drawOrder);
        drawListBuffer.position(0);


        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

        shaderProgram = GLES20.glCreateProgram();
        GLES20.glAttachShader(shaderProgram, vertexShader);
        GLES20.glAttachShader(shaderProgram, fragmentShader);
        GLES20.glLinkProgram(shaderProgram);
    }

    public int loadShader(int type, String shaderCode)
    {
        // create a vertex shader type (GLES20.GL_VERTEX_SHADER)
        // or a fragment shader type (GLES20.GL_FRAGMENT_SHADER)
        int shader = GLES20.glCreateShader(type);

        // add the source code to the shader and compile it
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        return shader;
    }


    private int mPositionHandle;
    private int mColorHandle;

    private final int vertexCount = faceCoords.length / COORDS_PER_VERTEX;
    private final int vertexStride = COORDS_PER_VERTEX * 4; // 4 bytes per vertex

    public void draw(float[] mvpMatrix)
    {
        // Add program to OpenGL ES environment
        GLES20.glUseProgram(shaderProgram);

        // get handle to vertex shader's vPosition member
        mPositionHandle = GLES20.glGetAttribLocation(shaderProgram, "vPosition");

        // Enable a handle to the triangle vertices
        GLES20.glEnableVertexAttribArray(mPositionHandle);

        // Prepare the triangle coordinate data
        GLES20.glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX,
                GLES20.GL_FLOAT, false,
                vertexStride, vertexBuffer);

        // get handle to fragment shader's vColor member
        mColorHandle = GLES20.glGetUniformLocation(shaderProgram, "vColor");

        // Set color for drawing the triangle
        GLES20.glUniform4fv(mColorHandle, 1, color, 0);

        // get handle to shape's transformation matrix
        int mMVPMatrixHandle = GLES20.glGetUniformLocation(shaderProgram, "uMVPMatrix");

        // Pass the projection and view transformation to the shader
        GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mvpMatrix, 0);

        // Draw the triangle
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length, GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

        // Disable vertex array
        GLES20.glDisableVertexAttribArray(mPositionHandle);

    }
}
