package com.josh.hw6;

import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.Matrix;

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
            "uniform mat4 uMVMatrix;" +
            "uniform mat4 uNormalMatrix;" +
            "uniform mat4 uPMatrix;" +
            "uniform vec4 uLightPos;" +

            "attribute vec4 vPosition;" +
            "attribute vec3 vNormal;" +

            "varying vec4 VertPos;" +
            "varying vec4 Light;" +
            "varying vec3 Normal;" +

            "void main() {" +
            "  VertPos = uMVMatrix * vPosition;" +
            "  gl_Position = uPMatrix * VertPos;" +

            "  Light = (uMVMatrix * uLightPos) - VertPos;" +

            "  Normal = vec3(uNormalMatrix * vec4(vNormal, 0));" +
            "}";

    private final String fragmentShaderCode =
            "precision mediump float;" +
            "uniform vec4 vColor;" +
            "uniform vec3 Ambient;" +
            "uniform vec3 Diffuse;" +
            "uniform vec3 LightColor;" +

            "varying vec4 VertPos;" +
            "varying vec4 Light;" +
            "varying vec3 Normal;" +

            "void main() {" +
            "  vec3 color = Ambient;" +

            "  vec3 N = normalize(Normal);" +
            "  vec3 L = normalize(vec3(Light));" +
            "  float Id = dot(L,N);" +
            "  if (Id > 0.0) {" +
            "    color += Id*Diffuse;" +
            "  }" +

            "  gl_FragColor = vColor * vec4(color * LightColor, 1);" +
            "}";

    private int shaderProgram;

    private FloatBuffer vertexBuffer;
    private FloatBuffer normalBuffer;
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

    private float color[] = {1, 1, 1, 1};
    private float ambient[] = {0.3f, 0.3f, 0.3f};
    private float diffuse[] = {0.7f, 0.7f, 0.7f};

    private short drawOrder[] = {0, 1, 2, 0, 2, 3,          // front
                                 4, 5, 6, 4, 6, 7,          // top
                                 8, 9, 10, 8, 10, 11,       // left
                                 12, 13, 14, 12, 14, 15,    // right
                                 16, 17, 18, 16, 18, 19,    // bottom
                                 20, 21, 22, 20, 22, 23,    // back
    };


    private float lightPosition[] = {2, 2, 2, 1};
    private float lightColor[] = {0.2f, 0.2f, 1};

    public Shape()
    {
        // initialize buffer for vertex coordinates
        ByteBuffer vpb = ByteBuffer.allocateDirect(
                // (# of coordinate values * 4 bytes per float)
                faceCoords.length * 4);
        vpb.order(ByteOrder.nativeOrder());
        vertexBuffer = vpb.asFloatBuffer();
        vertexBuffer.put(faceCoords);
        vertexBuffer.position(0);

        // initialize buffer for vertex normals
        ByteBuffer vnb = ByteBuffer.allocateDirect(
                // (# of coordinate values * 4 bytes per float)
                normals.length * 4);
        vnb.order(ByteOrder.nativeOrder());
        normalBuffer = vnb.asFloatBuffer();
        normalBuffer.put(normals);
        normalBuffer.position(0);


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



    private final int vertexCount = faceCoords.length / COORDS_PER_VERTEX;
    private final int vertexStride = COORDS_PER_VERTEX * 4; // 4 bytes per vertex

    public void draw(float[] mvMatrix, float[] pMatrix)
    {
        float[] normalMatrix = new float[16];
        float[] tmpMatrix = new float[16];
        Matrix.invertM(tmpMatrix, 0, mvMatrix, 0);
        Matrix.transposeM(normalMatrix, 0, tmpMatrix, 0);

        // Add program to OpenGL ES environment
        GLES20.glUseProgram(shaderProgram);

        // get handle to vertex shader's vPosition member
        int mPositionHandle = GLES20.glGetAttribLocation(shaderProgram, "vPosition");
        // Enable a handle to the vertex positions
        GLES20.glEnableVertexAttribArray(mPositionHandle);
        // Prepare the coordinate data
        GLES20.glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX,
                GLES20.GL_FLOAT, false,
                vertexStride, vertexBuffer);

        // get handle to vertex shader's vNormal member
        int mNormalHandle = GLES20.glGetAttribLocation(shaderProgram, "vNormal");
        // Enable a handle to the vertex normals
        GLES20.glEnableVertexAttribArray(mNormalHandle);
        GLES20.glVertexAttribPointer(mNormalHandle, COORDS_PER_VERTEX,
                GLES20.GL_FLOAT, false,
                vertexStride, normalBuffer);

        // get handle to fragment shader's vColor member
        int mColorHandle = GLES20.glGetUniformLocation(shaderProgram, "vColor");
        // Set color for drawing the triangle
        GLES20.glUniform4fv(mColorHandle, 1, color, 0);
        int mAmbientHandle = GLES20.glGetUniformLocation(shaderProgram, "Ambient");
        GLES20.glUniform3fv(mAmbientHandle, 1, ambient, 0);
        int mDiffuseHandle = GLES20.glGetUniformLocation(shaderProgram, "Diffuse");
        GLES20.glUniform3fv(mDiffuseHandle, 1, diffuse, 0);

        // get handle to shape's transformation matrix
        int mMVMatrixHandle = GLES20.glGetUniformLocation(shaderProgram, "uMVMatrix");
        int mPMatrixHandle = GLES20.glGetUniformLocation(shaderProgram, "uPMatrix");
        int mNormalMatrixHandle = GLES20.glGetUniformLocation(shaderProgram, "uNormalMatrix");
        // Pass the projection and view transformation to the shader
        GLES20.glUniformMatrix4fv(mMVMatrixHandle, 1, false, mvMatrix, 0);
        GLES20.glUniformMatrix4fv(mPMatrixHandle, 1, false, pMatrix, 0);
        GLES20.glUniformMatrix4fv(mNormalMatrixHandle, 1, false, normalMatrix, 0);

        int mLightPosHandle = GLES20.glGetUniformLocation(shaderProgram, "uLightPos");
        int mLightColorHandle = GLES20.glGetUniformLocation(shaderProgram, "LightColor");
        GLES20.glUniform4fv(mLightPosHandle, 1, lightPosition, 0);
        GLES20.glUniform3fv(mLightColorHandle, 1, lightColor, 0);

        // Draw the triangle
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length, GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

        // Disable arrays
        GLES20.glDisableVertexAttribArray(mPositionHandle);
        GLES20.glDisableVertexAttribArray(mNormalHandle);

    }
}
