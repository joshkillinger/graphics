package com.josh.hw6;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.widget.Toolbar;
import android.view.MotionEvent;
import android.view.View;

public class MainActivity extends Activity
{
    private GLSurfaceView mGLView;

    @Override
    public void onCreate (Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        // Create a GLSurfaceView instance and set it
        // as the ContentView for this Activity.
        mGLView = new MyGLSurfaceView(this);
        setContentView(mGLView);
    }

    class MyGLSurfaceView extends GLSurfaceView
    {

        private final float TOUCH_SCALE_FACTOR = 180.0f / 320;
        private float mPreviousX;
        private float mPreviousY;

        private final MyGLRenderer mRenderer;

        public MyGLSurfaceView(Context context)
        {
            super(context);

            // Create an OpenGL ES 2.0 context
            setEGLContextClientVersion(2);

            mRenderer = new MyGLRenderer();

            // Set the Renderer for drawing on the GLSurfaceView
            setRenderer(mRenderer);

            mRenderer.setResources(getResources());
            setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        }


        @Override
        public boolean onTouchEvent(MotionEvent e) {
            // MotionEvent reports input details from the touch screen
            // and other input controls. In this case, you are only
            // interested in events where the touch position changed.

            float x = e.getX();
            float y = e.getY();

            switch (e.getAction()) {
                case MotionEvent.ACTION_MOVE:

                    float dx = x - mPreviousX;
                    float dy = y - mPreviousY;

                    mRenderer.rotateView(-dy * TOUCH_SCALE_FACTOR, -dx * TOUCH_SCALE_FACTOR);
                    requestRender();
            }

            mPreviousX = x;
            mPreviousY = y;
            return true;
        }
    }
}
