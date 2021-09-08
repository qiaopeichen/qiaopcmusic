#version 120
//片元着色器
precision mediump float;
uniform vec4 af_Color; //uniform用于在application中向vertex和fragment中传递值
void main() {
     gl_FragColor = af_Color;
}
