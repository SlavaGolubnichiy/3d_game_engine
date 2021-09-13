#version 330 core

in vec3 object_color;
out vec4 FragColor;

float ambient_light_strength = 1.0f;
uniform vec3 ambient_light_color;

// diffused light
in vec3 normal;
in vec3 frag_pos;
uniform vec3 light_color;
uniform vec3 light_pos;

// specular (reflected) light
uniform vec3 view_pos;
uniform float specular_strength = 0.5;
uniform float material_property1;

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
			specular_light_value
		),
		1.0f
	);
}

vec4 PointLight()
{
	vec3 light_frag_vector = light_pos - frag_pos;
	vec3 normalized_normal = normalize(normal);

	// light intensity gradient equasion
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
			specular_light_value * light_intensity
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
			specular_light_value * light_intensity
		),
		1.0f
	);
}

void main()
{

	// light calculation
	vec3 light_vector = normalize(light_pos - frag_pos);
	vec3 normalized_normal = normalize(normal);

	// diffused light
	float diffused_light_strength = dot(light_vector, normalized_normal);
	vec3 diffused_light_value = diffused_light_strength * light_color;
	if (diffused_light_strength >= 0)
	{
		diffused_light_value = diffused_light_strength * light_color;
	}
	else
	{
		diffused_light_value = vec3(0);
	}

	// specular light
	vec3 view_vector = normalize(view_pos - frag_pos);
	vec3 reflection_vector = reflect(-light_vector, normalized_normal);
	float spec_ = pow(max(dot(view_vector, reflection_vector), 0.0f), material_property1);
	vec3 specular_light_value = specular_strength * spec_ * light_color;

	//FragColor = vec4((ambient_light_color + diffused_light_value + specular_light_value) * object_color, 1.0f);
	FragColor = DirectionalLight() * vec4(object_color, 1.0f);

}
