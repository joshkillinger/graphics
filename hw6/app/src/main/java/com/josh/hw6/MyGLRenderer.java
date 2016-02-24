package com.josh.hw6;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.SystemClock;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by Josh on 2/22/2016.
 */
public class MyGLRenderer implements GLSurfaceView.Renderer
{
    private Shape quad;

    // mMVPMatrix is an abbreviation for "Model View Projection Matrix"
    private float[] mMVPMatrix = new float[16];
    private float[] mProjectionMatrix = new float[16];
    private float[] mViewMatrix = new float[16];

    public void rotateView(float x, float y)
    {
        Matrix.rotateM(mViewMatrix, 0, x, 1.0f, 0, 0);
        Matrix.rotateM(mViewMatrix, 0, y, 0, 1.0f, 0);
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        // Set the background frame color
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        quad = new Shape();

        // Set the camera position (View matrix)
        Matrix.setLookAtM(mViewMatrix, 0, 0, 0, -6, 0f, 0f, 0f, 0f, 1.0f, 0.0f);

        // depth testing
//        GLES20.glEnable(GLES20.GL_DEPTH_TEST);
//        GLES20.glClearDepthf(10.0f);
//        GLES20.glDepthFunc(GLES20.GL_LEQUAL);
//        GLES20.glDepthMask(true);

        // cull backface
        GLES20.glEnable( GLES20.GL_CULL_FACE );
        GLES20.glCullFace(GLES20.GL_BACK);
    }


    public void onDrawFrame(GL10 unused)
    {
        // Redraw background color
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

        // Calculate the projection and view transformation
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);

        quad.draw(mMVPMatrix);
    }

    public void onSurfaceChanged(GL10 unused, int width, int height)
    {
        GLES20.glViewport(0, 0, width, height);

        float ratio = (float) width / height;

        // this projection matrix is applied to object coordinates
        // in the onDrawFrame() method
        Matrix.frustumM(mProjectionMatrix, 0, ratio, -ratio, -1, 1, 3, 10);
    }
}