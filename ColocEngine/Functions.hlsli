
float3 Pallet0(float val)
{
    float3 phase = float3(1, 0.92, 0.5);
    float3 amp   = float3(3.12, 0.51, 0.8);
    float3 freq  = float3(0.7, 0.5, 0.2);
    float3 offset= float3(0, 0, 0);
    
    return offset + amp  * cos(val * freq + phase);
};

float3 Pallet1(float val)
{
    float3 phase = float3(1, 0.92, 0.5);
    float3 amp = float3(3.12, 0.51, 0.8);
    float3 freq = float3(0.7, 0.5, 0.2);
    float3 offset = float3(0, 0, 0);
    
    return phase + amp * cos(6.28318 * (freq * val + offset));
};