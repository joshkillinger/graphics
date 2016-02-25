package com.josh.hw6;

import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.Matrix;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

/**
 * Created by Josh on 2/22/2016.
 */
public class Cube
{
    private final String vertexShaderCode =
            // This matrix member variable provides a hook to manipulate
            // the coordinates of the objects that use this vertex shader
            "uniform mat4 MVMatrix;" +
            "uniform mat4 NormalMatrix;" +
            "uniform mat4 PMatrix;" +
            "uniform vec4 LightPos;" +

            "attribute vec4 vPosition;" +
            "attribute vec3 vNormal;" +

            "varying vec4 VertPos;" +
            "varying vec4 Light;" +
            "varying vec3 Normal;" +
            "varying vec3 View;" +

            "void main() {" +
            "  VertPos = MVMatrix * vPosition;" +
            "  gl_Position = PMatrix * VertPos;" +

            "  Light = (MVMatrix * LightPos) - VertPos;" +
            "  View = -vec3(VertPos);" +

            "  Normal = vec3(NormalMatrix * vec4(vNormal, 0));" +
            "}";

    private final String fragmentShaderCode =
            "precision mediump float;" +
            "uniform vec4 Color;" +
            "uniform vec3 Ambient;" +
            "uniform vec3 Diffuse;" +
            "uniform vec3 Specular;" +
            "uniform vec3 LightColor;" +
            "uniform float Shininess;" +

            "varying vec4 VertPos;" +
            "varying vec4 Light;" +
            "varying vec3 Normal;" +
            "varying vec3 View;" +

            "void main() {" +
            "  vec3 color = Ambient;" +

            "  vec3 N = normalize(Normal);" +
            "  vec3 L = normalize(vec3(Light));" +
            "  float Id = dot(L,N);" +
            "  if (Id > 0.0) {" +
            "    color += Id*Diffuse;" +
            "    vec3 R = reflect(-L,N);" +
            "    vec3 V = normalize(View);" +
            "    float Is = dot(R,V);" +
            "    if (Is > 0.0) color += pow(Is,Shininess) * Specular;" +
            "  }" +

            "  gl_FragColor = Color * vec4(color * LightColor, 1);" +
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
    private float specular[] = {1f, 1f, 1f};
    private float shininess = 32.f;

    private short drawOrder[] = {0, 1, 2, 0, 2, 3,          // front
                                 4, 5, 6, 4, 6, 7,          // top
                                 8, 9, 10, 8, 10, 11,       // left
                                 12, 13, 14, 12, 14, 15,    // right
                                 16, 17, 18, 16, 18, 19,    // bottom
                                 20, 21, 22, 20, 22, 23,    // back
    };


    private float lightPosition[] = {2, 2, 2, 1};
    private float lightColor[] = {0.2f, 0.2f, 1};

    public Cube()
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

        String logmsg = GLES20.glGetProgramInfoLog(shader);
        Log.d("ShaderCompile",logmsg);

        return shader;
    }


    private final int vertexStride = COORDS_PER_VERTEX * 4; // 4 bytes per vertex

    public void draw(float[] mvMatrix, float[] pMatrix)
    {
        float[] normalMatrix = new float[16];
        float[] tmpMatrix = new float[16];
        Matrix.invertM(tmpMatrix, 0, mvMatrix, 0);
        Matrix.transposeM(normalMatrix, 0, tmpMatrix, 0);

        // Add program to OpenGL ES environment
        GLES20.glUseProgram(shaderProgram);

        // vertex position attribute array
        int mPositionHandle = GLES20.glGetAttribLocation(shaderProgram, "vPosition");
        GLES20.glEnableVertexAttribArray(mPositionHandle);
        GLES20.glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX,
                GLES20.GL_FLOAT, false,
                vertexStride, vertexBuffer);

        // vertex normal attribute array
        int mNormalHandle = GLES20.glGetAttribLocation(shaderProgram, "vNormal");
        GLES20.glEnableVertexAttribArray(mNormalHandle);
        GLES20.glVertexAttribPointer(mNormalHandle, COORDS_PER_VERTEX,
                GLES20.GL_FLOAT, false,
                vertexStride, normalBuffer);

        // material data
        int mColorHandle = GLES20.glGetUniformLocation(shaderProgram, "Color");
        int mAmbientHandle = GLES20.glGetUniformLocation(shaderProgram, "Ambient");
        int mDiffuseHandle = GLES20.glGetUniformLocation(shaderProgram, "Diffuse");
        int mSpecularHandle = GLES20.glGetUniformLocation(shaderProgram, "Specular");
        int mShininessHandle = GLES20.glGetUniformLocation(shaderProgram, "Shininess");
        GLES20.glUniform4fv(mColorHandle, 1, color, 0);
        GLES20.glUniform3fv(mAmbientHandle, 1, ambient, 0);
        GLES20.glUniform3fv(mDiffuseHandle, 1, diffuse, 0);
        GLES20.glUniform3fv(mSpecularHandle, 1, specular, 0);
        GLES20.glUniform1f(mShininessHandle, shininess);

        // transformation matrices
        int mMVMatrixHandle = GLES20.glGetUniformLocation(shaderProgram, "MVMatrix");
        int mPMatrixHandle = GLES20.glGetUniformLocation(shaderProgram, "PMatrix");
        int mNormalMatrixHandle = GLES20.glGetUniformLocation(shaderProgram, "NormalMatrix");
        GLES20.glUniformMatrix4fv(mMVMatrixHandle, 1, false, mvMatrix, 0);
        GLES20.glUniformMatrix4fv(mPMatrixHandle, 1, false, pMatrix, 0);
        GLES20.glUniformMatrix4fv(mNormalMatrixHandle, 1, false, normalMatrix, 0);

        // lighting data
        int mLightPosHandle = GLES20.glGetUniformLocation(shaderProgram, "LightPos");
        int mLightColorHandle = GLES20.glGetUniformLocation(shaderProgram, "LightColor");
        GLES20.glUniform4fv(mLightPosHandle, 1, lightPosition, 0);
        GLES20.glUniform3fv(mLightColorHandle, 1, lightColor, 0);

        // Draw the object
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length, GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

        // Disable arrays
        GLES20.glDisableVertexAttribArray(mPositionHandle);
        GLES20.glDisableVertexAttribArray(mNormalHandle);

    }
}
