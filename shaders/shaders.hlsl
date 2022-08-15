
cbuffer ObjectConstant : register(b0) { row_major float4x4 modelMatrix; };

cbuffer CameraConstant : register(b1) {
  row_major float4x4 viewMatrix;
  row_major float4x4 projMatrix;
  float3 camPosition;
};

cbuffer LighConstant : register(b2) {
  float3 position;
  float3 direction;

  float cutOff;
  float outerCutOff;

  float constant;
  float linearV;
  float quadratic;
}

struct VS_Input {
  float3 pos : POS;
  float2 uv : TEX;
  float3 normal : NORMAL;
};

struct VS_Output {
  float4 pos : SV_POSITION;
  float2 uv : TEXCOORD;
  float3 normal : NORMAL;
  float3 FragPos : FRAGPOS;
};

SamplerState Texsampler : register(s0);

Texture2D Textexture : register(t0);

VS_Output vs_main(VS_Input input) {
  VS_Output output;
  output.FragPos = mul(modelMatrix, input.pos);
  float4 pos = float4(input.pos, 1.0f);
  float4 positionWS = mul(pos, mul(modelMatrix, mul(viewMatrix, projMatrix)));
  output.pos = mul(pos, mul(modelMatrix, mul(viewMatrix, projMatrix)));

  output.uv = input.uv;
  output.normal = input.normal;
  return output;
}

float4 ps_main(VS_Output input) : SV_Target {
  float3 lightPos = float3(-66.095, 45.3086, 5.13903);
  float3 lightDir = normalize(float3(-66.095, 45.3086, 5.13903));

  float distance = length(lightPos - input.FragPos);

  float specularStrength = 0.5f;
  float ambientStrength = 0.1f;

  // Find colour value by sampling texture
  float4 colour = Textexture.Sample(Texsampler, input.uv);

  if (colour.w < 0.1) {
    discard;
  }

  // Define colour for the light
  float3 lightColour = float3(1, 1, 1);

  // Calculate Diffuse and Ambient
  float3 diffuse = min(dot(input.normal, lightDir), 0.5f) * lightColour;
  float3 ambient = ambientStrength * lightColour;
  // Calculate Specular
  float3 viewDirection = normalize(camPosition - input.FragPos);
  float3 reflectDir = reflect(-lightDir, input.normal);
  float3 spec = pow(max(dot(viewDirection, reflectDir), 0.0f), 1);
  float3 specular = specularStrength * spec * lightColour;

  return colour; //* float4(diffuse + ambient + specular, 1);
}

float4 spotLight(float3 lightPos, float3 camPos, float3 normal) {
  float outerCone = 0.90f;
  float innerCone = 0.95f;

  float ambient = 0.20f;
}