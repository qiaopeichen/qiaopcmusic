//顶点着色器 attribute只能在vertex中使用
attribute vec4 av_Position;
void main() {
    gl_Position = av_Position;
}