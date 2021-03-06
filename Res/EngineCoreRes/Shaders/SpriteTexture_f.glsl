#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif


uniform sampler2D TU_tex1;
uniform vec4 TU_color;
varying vec2 v_texcoord;

//! [0]
void main()
{
    // Set fragment color from texture
    vec4 col = texture2D(TU_tex1,v_texcoord)*TU_color;
    gl_FragColor = col;
}
//! [0]

