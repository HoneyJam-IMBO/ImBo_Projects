

// �ȼ� ���̴��� ����� �ȼ��� �� �������Դϴ�.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

// (������) �� �����Ϳ� ���� ��� �Լ��Դϴ�.
float4 main(VS_OUTPUT input) : SV_TARGET
{
	return input.color;
	//return float4(0,0,1,1);
}