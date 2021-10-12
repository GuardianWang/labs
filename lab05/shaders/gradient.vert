#version 400

layout(location = 0) in vec3 position;
// TODO: Add a new in variable for the color attribute (Task 12)
layout(location = 2) in vec3 vertColor;

// TODO: Declare a new out vec3 to send the color attribute (Task 13)
out vec3 outColor;


void main() {
    gl_Position = vec4(position, 1.0);

    // TODO: Set the output variable you created to a per-vertex
    // color attribute (the incoming color). (Task 13)
    outColor = vertColor;

}
