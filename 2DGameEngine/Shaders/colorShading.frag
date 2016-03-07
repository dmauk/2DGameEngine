#version 130
//Fragment shader operates on each pixel in a given polyon
out vec4 color;


//4 component float vector that outputs to screen for each pixel.
in vec4 fragmentColor;
in vec2 fragmentPosition;

uniform float time;

void main(){
	
	/*color = vec4(fragmentColor.r * (cos(fragmentPosition.x*4.0 + time) + 1.0) * 0.5,
				 fragmentColor.g * (cos(fragmentPosition.y*8.0 + time) + 1.0) * 0.5,
				 fragmentColor.b * (cos(fragmentPosition.x*2.0 + time) + 1.0) * 0.5, fragmentColor.a);*/
	

	color = fragmentColor + vec4((cos(time)+1.0)*0.5,
				  				 (cos(time)+1.0)*0.5,
				  				 (sin(time)+1.0)*0.5, 0.0);
}