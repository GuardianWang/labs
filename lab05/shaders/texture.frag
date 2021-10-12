#version 400
in vec2 uv;
out vec4 color;

// TODO: Declare a sampler2D uniform (Task 16)
uniform sampler2D textureUniform;

void main(){
    // TODO: Set the output color to the corresponding texel (Task 16)
    color = texture(textureUniform, uv);
}
