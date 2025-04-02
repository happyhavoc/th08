#pragma once
#include <d3d8.h>
#include <d3dx8math.h>
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "utils.hpp"

namespace th08
{
struct VertexDiffuseXyzrhw
{
    D3DXVECTOR3 pos;
    f32 w;
    D3DCOLOR diffuse;
};

struct VertexTex1DiffuseXyzrhw
{
    D3DXVECTOR3 pos;
    f32 w;
    D3DCOLOR diffuse;
    D3DXVECTOR2 textureUV;
};

enum SprtBlendMode
{
    SprtBlendMode_Unset = -1
};

enum SprtColorOp
{
    SprtColorOp_Unset = -1
};

enum SprtVertexShader
{
    SprtVertexShader_Unset = -1
};

enum SprtCameraMode
{
    SortCameraMode_Unset = -1
};

// Unofficial name: AnmVm 
struct Sprt
{
    unknown_fields(0x0, 0x2a4);
};
C_ASSERT(sizeof(Sprt) == 0x2a4);

// Unofficial name: AnmManager
struct SprtCtrl
{
    void ClearBlendMode()
    {
        m_CurrentBlendMode = SprtBlendMode_Unset;
    }

    void ClearColorOp()
    {
        m_CurrentColorOp = SprtColorOp_Unset;
    }

    void ClearVertexShader()
    {
        m_CurrentVertexShader = SprtVertexShader_Unset;
    }

    void ClearTexture()
    {
        m_CurrentTexture = NULL;
    }

    void ClearCameraSettings()
    {
        m_CameraMode = SortCameraMode_Unset;
    }

    unknown_fields(0x0, 0x1c24);
    D3DXVECTOR3 unk0x1c24;
    unknown_fields(0x1c30, 0x34);
    Sprt unk0x1c64;
    unknown_fields(0x1f08, 0x5b4);
    IDirect3DBaseTexture8 *m_CurrentTexture;
    u8 m_CurrentBlendMode;
    u8 m_CurrentColorOp;
    u8 m_CurrentVertexShader;
    u8 m_DisableZWrite;
    u8 m_CameraMode;
    unknown_fields(0x24c5, 3); // Padding?
    void *m_CurrentSprite;
    unknown_fields(0x24cc, 4);
    VertexDiffuseXyzrhw m_UntexturedVector[4];
    u32 m_SpritesToDraw;
    VertexTex1DiffuseXyzrhw m_TexturedVector[0x18000];
    VertexTex1DiffuseXyzrhw *m_DrawBufferEndPtr;
    VertexTex1DiffuseXyzrhw *m_DrawBufferStartPtr;
    unknown_fields(0x2a252c, 0x44);
};
C_ASSERT(sizeof(SprtCtrl) == 0x2a2570);

DIFFABLE_EXTERN(SprtCtrl *, g_SprtCtrl);

}; // namespace th08
