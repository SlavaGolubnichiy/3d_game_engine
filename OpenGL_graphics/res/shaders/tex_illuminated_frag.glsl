// fragment shader
#version 330 core

in vec3 object_color;
out vec4 FragColor;

// texture
in vec2 texCoord;
uniform sampler2D tex0;
vec4 texColor;

// ambient light
float ambient_light_strength = 1.0;
uniform vec3 ambient_light_color;

// diffused light
in vec3 normal;
in vec3 frag_pos;
uniform vec3 light_color;
uniform vec3 light_pos;

// specular (reflected) light
uniform vec3 view_pos;
uniform float specular_strength = 1.0;
uniform float material_property1;

// specular map
uniform sampler2D tex_specular_map;
vec4 texSpecMapValue;

vec4 DirectionalLight()
{
	vec3 light_frag_vector = light_pos - frag_pos;
	vec3 normalized_normal = normalize(normal);

	// diffused light
	vec3 diffused_light_vector = normalize(light_frag_vector);
	float diffused_light_strength = dot(diffused_light_vector, normalized_normal);
	vec3 diffused_light_value = diffused_light_strength * light_color;
	if (diffused_light_strength >= 0)
	{
		diffused_light_value = diffused_light_strength * light_color;
	}
	else
	{
		diffused_light_value = vec3(0);
	}

	// texture specular map calculation
	texSpecMapValue = texture(tex_specular_map, texCoord);
	// specular light
	vec3 view_vector = normalize(view_pos - frag_pos);
	vec3 reflection_vector = reflect(-normalize(light_frag_vector), normalized_normal);
	float spec_ = pow(max(dot(view_vector, reflection_vector), 0.0f), material_property1);
	vec3 specular_light_value = specular_strength * spec_ * light_color;

	return vec4
	(
		(
			ambient_light_strength * ambient_light_color + 
			diffused_light_value + 
			texSpecMapValue.r * specular_light_value		// only .r value is enough because specular map is in gray-scale
		),
		1.0f
	);
}

vec4 PointLight()
{
	vec3 light_frag_vector = light_pos - frag_pos;
	vec3 normalized_normal = normalize(normal);

	// light intensity gradient equasion (with respect to the distance)
	float dist = length(light_frag_vector);
	float a = 0.6f;
	float b = 0.4f;
	float light_intensity = 1.0f / (a * dist*dist + b * dist + 1.0f);

	// diffused light
	vec3 diffused_light_vector = normalize(light_frag_vector);
	float diffused_light_strength = dot(diffused_light_vector, normalized_normal);
	vec3 diffused_light_value = diffused_light_strength * light_color;
	if (diffused_light_strength >= 0)
	{
		diffused_light_value = diffused_light_strength * light_color;
	}
	else
	{
		diffused_light_value = vec3(0);
	}

	// texture specular map calculation
	texSpecMapValue = texture(tex_specular_map, texCoord);
	// specular light
	vec3 view_vector = normalize(view_pos - frag_pos);
	vec3 reflection_vector = reflect(-normalize(light_frag_vector), normalized_normal);
	float spec_ = pow(max(dot(view_vector, reflection_vector), 0.0f), material_property1);
	vec3 specular_light_value = specular_strength * spec_ * light_color;

	return vec4
	(
		(
			ambient_light_strength * ambient_light_color + 
			diffused_light_value * light_intensity + 
			texSpecMapValue.r * specular_light_value * light_intensity		// only .r value is enough because specular map is in gray-scale
		),
		1.0f
	);
}

vec4 SpotLight()
{
	float inner_cone_degree_cos = 0.95f;	// inner cone degree cos has to be greater than outer (else - inverted light)
	float outer_cone_degree_cos = 0.90f;

	vec3 light_frag_vector = light_pos - frag_pos;
	vec3 normalized_normal = normalize(normal);

	// diffused light
	vec3 diffused_light_vector = normalize(light_frag_vector);
	float diffused_light_strength = dot(diffused_light_vector, normalized_normal);
	vec3 diffused_light_value = diffused_light_strength * light_color;
	if (diffused_light_strength >= 0)
	{
		diffused_light_value = diffused_light_strength * light_color;
	}
	else
	{
		diffused_light_value = vec3(0);
	}

	// texture specular map calculation
	texSpecMapValue = texture(tex_specular_map, texCoord);
	// specular light
	vec3 view_vector = normalize(view_pos - frag_pos);
	vec3 reflection_vector = reflect(-normalize(light_frag_vector), normalized_normal);
	float spec_ = pow(max(dot(view_vector, reflection_vector), 0.0f), material_property1);
	vec3 specular_light_value = specular_strength * spec_ * light_color;

	// illuminated area calculation
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -normalize(light_frag_vector));
	float light_intensity = clamp((angle - outer_cone_degree_cos) / (inner_cone_degree_cos - outer_cone_degree_cos), 0.0f, 1.0f);

	return vec4
	(
		(
			ambient_light_strength * ambient_light_color + 
			diffused_light_value * light_intensity + 
			texSpecMapValue.r * specular_light_value * light_intensity		// only .r value is enough because specular map is in gray-scale
		),
		1.0f
	);
}

void main()
{
	// texture calculation
	vec4 texColor = texture(tex0, texCoord);

	FragColor = 
		texColor *
		DirectionalLight();
}
