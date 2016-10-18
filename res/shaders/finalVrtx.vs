void main(void)
{   
  gl_FrontColor = gl_Color;
  gl_BackColor = gl_Color;
      gl_FrontSecondaryColor = gl_SecondaryColor;
        gl_BackSecondaryColor = gl_SecondaryColor;
  gl_Position = ftransform();
  gl_TexCoord [0] = gl_MultiTexCoord0;
}
