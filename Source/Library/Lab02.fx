float4 VS(float4 pos: Position) : SV_POSITION
{
	return pos;
}

float4 PS(float4 Pos: SV_POSITION) : SV_TARGET
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}