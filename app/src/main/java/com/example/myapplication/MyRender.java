package com.example.myapplication;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyRender implements GLSurfaceView.Renderer {

    protected Context context;

    //1 绘制坐标范围
    private final float[] vertexData = {
//            -1f,0f,
//            0f, -1f,
//            0f, 1f,
//            0f, 1f,
//            0f, -1f,
//            1f, 0f

            -1f, 0f,
            0f, -1f,
            0f, 1f,
            1f, 0f
    };
    //2 为坐标分配本地内存地址
    private FloatBuffer vertexBuffer;
    private int program;
    private int avPosition;
    private int afColor;

    public MyRender(Context context) {
        this.context = context;
        //2 为坐标分配本地内存地址
        vertexBuffer = ByteBuffer.allocateDirect(vertexData.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(vertexData);
        vertexBuffer.position(0);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        String vertexSource = MyShaderUtil.readRawTxt(context, R.raw.vertex_shader);
        String fragmentSource = MyShaderUtil.readRawTxt(context, R.raw.fragment_shader);
        program = MyShaderUtil.createProgram(vertexSource, fragmentSource);
        if (program > 0) {
            avPosition = GLES20.glGetAttribLocation(program, "av_Position"); //8 得到着色器中的属性
            afColor = GLES20.glGetUniformLocation(program, "af_Color");
        }

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height); //设置view宽高
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT); //把颜色缓冲区清屏
        GLES20.glClearColor(0.5f, 0f, 0.8f, 1.0f); // 用颜色来清屏

        GLES20.glUseProgram(program); //9使用源程序

        GLES20.glUniform4f(afColor, 1f, 0f, 0, 1f); //修改着色器颜色

        GLES20.glEnableVertexAttribArray(avPosition); //10使顶点属性数组有效
        GLES20.glVertexAttribPointer(avPosition, 2, GLES20.GL_FLOAT, false, 8, vertexBuffer); //11为顶点属性赋值
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4); //12 绘制图形
    }
}
