package com.josh.hw6;

import android.content.res.Resources;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by Josh on 2/22/2016.
 */
public class MyGLRenderer implements GLSurfaceView.Renderer
{
    private Cube cube;

    private Resources resources;

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

        // Set the camera position (View matrix)
        Matrix.setLookAtM(mViewMatrix, 0, 0, 0, -7, 0f, 0f, 0f, 0f, 1.0f, 0.0f);

        // depth testing
//        GLES20.glEnable(GLES20.GL_DEPTH_TEST);
//        GLES20.glClearDepthf(10.0f);
//        GLES20.glDepthFunc(GLES20.GL_LEQUAL);
//        GLES20.glDepthMask(true);

        // cull backface
        GLES20.glEnable( GLES20.GL_CULL_FACE );
        GLES20.glCullFace(GLES20.GL_BACK);
    }

    public void setResources(Resources r)
    {
        resources = r;
    }

    public void onDrawFrame(GL10 unused)
    {
        if (cube == null)
        {
            cube = new Cube(BitmapFactory.decodeResource(resources, R.drawable.companion_cube));
        }

        // Redraw background color
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

        // Calculate the projection and view transformation
        //Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);

        cube.draw(mViewMatrix, mProjectionMatrix);
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