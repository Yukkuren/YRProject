#include "AnimShader.hlsli"

[maxvertexcount(4)]
void main(point GSInput In[1],
	inout TriangleStream<PSInput> ParticleStream)
{
	// ���W�ϊ� (���[���h���W�n �� �r���[���W�n)
	float4 pos = mul(float4(In[0].Position, 1.0), View);

	float w = In[0].Size.x;
	float h = In[0].Size.y;
	float4 left_top_offset = mul(float4(-w, h, 0.0f, 0.0f), SR);
	float4 left_bottom_offset = mul(float4(-w, -h, 0.0f, 0.0f), SR);
	float4 right_top_offset = mul(float4(w, h, 0.0f, 0.0f), SR);
	float4 right_bottom_offset = mul(float4(w, -h, 0.0f, 0.0f), SR);
	// �_��ʂɂ���(�S���_�����)

	float4 pos_left_top = pos + left_top_offset;
	float4 pos_left_bottom = pos + left_bottom_offset;
	float4 pos_right_top = pos + right_top_offset;
	float4 pos_right_bottom = pos + right_bottom_offset;

	// ����̓_�̈ʒu(�ˉe���W�n)�EUV�E�F���v�Z���ďo��
	PSInput Out = (PSInput)0;
	uint type = In[0].Color.r; //Color r:type g:b:���� a:�s�����x�Ƃ��ė��p
	float u = In[0].UV_pos.x;
	float v = In[0].UV_pos.y;
	Out.Color = float4(1, 1, 1, In[0].Color.a);
	Out.Position = mul(pos_left_top, Projection);
	Out.Position /= Out.Position.w;
	Out.Tex = float2(u, v);
	ParticleStream.Append(Out);
	// �E��̓_�̈ʒu(�ˉe���W�n) �ƃe�N�X�`�����W�̌v�Z�����ďo��
	Out.Position = mul(pos_right_top, Projection);
	Out.Position /= Out.Position.w;
	Out.Tex = float2(u + In[0].UV_size.x, v);
	Out.Color = float4(1, 1, 1, In[0].Color.a);
	ParticleStream.Append(Out);

	// �����̓_�̈ʒu(�ˉe���W�n) �ƃe�N�X�`�����W�̌v�Z�����ďo��
	Out.Position = mul(pos_left_bottom, Projection);
	Out.Position /= Out.Position.w;
	Out.Tex = float2(u, v + In[0].UV_size.y);
	Out.Color = float4(1, 1, 1, In[0].Color.a);
	ParticleStream.Append(Out);
	// �E���̓_�̈ʒu(�ˉe���W�n) �ƃe�N�X�`�����W�̌v�Z�����ďo��
	Out.Position = mul(pos_right_bottom, Projection);
	Out.Position /= Out.Position.w;
	Out.Tex = float2(u + In[0].UV_size.x, v + In[0].UV_size.y);
	Out.Color = float4(1, 1, 1, In[0].Color.a);
	ParticleStream.Append(Out);

	Out.Position /= Out.Position.w;

	ParticleStream.RestartStrip();
}