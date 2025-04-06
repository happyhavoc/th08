#pragma once
#include <d3d8.h>
#include <d3dx8math.h>
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "utils.hpp"

#define GAME_WINDOW_WIDTH 640
#define GAME_WINDOW_HEIGHT 480

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

// Touhou 8 uses DirectX 8.1, but evidently Zun used some mismatched DirectX 8 headers as well
// D3DXIMAGE_INFO changed from 20 to 28 bytes between DX8 and DX8.1, but somehow IN uses the the DX8 version
// This struct is a redefinition of the DX8 D3DXIMAGE_INFO for that
// The only reason this ABI mismatch doesn't cause issues is because no surface indices are ever loaded other than 0
struct ZunImageInfo
{
    u32 width;
    u32 height;
    u32 depth;
    u32 mipLevels;
    D3DFORMAT format;
};
C_ASSERT(sizeof(ZunImageInfo) == 0x14);

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
    ~SprtCtrl() { }

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

    void ReleaseSurfaces()
    {
        i32 i;

        for (i = 0; i < ARRAY_SIZE_SIGNED(m_Surfaces); i++)
        {
            if (m_Surfaces[i] != NULL)
            {
                m_Surfaces[i]->Release();
                m_Surfaces[i] = NULL;
            }
        }
    }

    void TakeScreencaptures()
    {
        if (m_CaptureAnmIdx > -1)
        {
            CaptureToTexture(m_CaptureAnmIdx, m_TextureCaptureSrcX, m_TextureCaptureSrcY, m_TextureCaptureSrcW, m_TextureCaptureSrcH,
                            m_TextureCaptureDstX, m_TextureCaptureDstY, m_TextureCaptureDstW, m_TextureCaptureDstH);
            m_CaptureAnmIdx = -1;
        }

        if (m_CaptureSurfaceIdx > -1)
        {
            CaptureToSurface(m_CaptureSurfaceIdx, m_SurfaceCaptureSrcX, m_SurfaceCaptureSrcY, m_SurfaceCaptureSrcW, m_SurfaceCaptureSrcH,
                            m_SurfaceCaptureDstX, m_SurfaceCaptureDstY, m_SurfaceCaptureDstW, m_SurfaceCaptureDstH);
            m_CaptureSurfaceIdx = -1;
        }
    }

    void CaptureToTexture (i32 captureAnmIdx, i32 srcX, i32 srcY, i32 srcW, i32 srcH, i32 dstX, i32 dstY, i32 dstW, i32 dstH);
    void CaptureToSurface (i32 captureSurfaceIdx, i32 srcX, i32 srcY, i32 srcW, i32 srcH, i32 dstX, i32 dstY, i32 dstW, i32 dstH);

    void ClearVertexBuffer();
    void FlushVertexBuffer();

    unknown_fields(0x0, 0x8);
    i32 m_CaptureSurfaceIdx;
    unknown_fields(0xc, 0x1c18);
    D3DXVECTOR3 unk0x1c24;
    unknown_fields(0x1c30, 0x34);
    Sprt unk0x1c64;
    unknown_fields(0x1f08, 0x130);

    IDirect3DSurface8 *m_Surfaces[32];
    IDirect3DSurface8 *m_SurfacesBis[32];
    u8 *m_SurfaceData[32];
    u32 m_SurfaceDataSizes[32];
    ZunImageInfo m_SurfaceInfo[32];

    unknown_fields(0x24b8, 0x4);

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
    VertexTex1DiffuseXyzrhw m_VertexBuffer[0x18000];
    VertexTex1DiffuseXyzrhw *m_VertexBufferEndPtr;
    VertexTex1DiffuseXyzrhw *m_VertexBufferStartPtr;
    i32 m_CaptureAnmIdx;
    i32 m_TextureCaptureSrcX;
    i32 m_TextureCaptureSrcY;
    i32 m_TextureCaptureSrcW;
    i32 m_TextureCaptureSrcH;
    i32 m_TextureCaptureDstX;
    i32 m_TextureCaptureDstY;
    i32 m_TextureCaptureDstW;
    i32 m_TextureCaptureDstH;
    i32 m_SurfaceCaptureSrcX;
    i32 m_SurfaceCaptureSrcY;
    i32 m_SurfaceCaptureSrcW;
    i32 m_SurfaceCaptureSrcH;
    i32 m_SurfaceCaptureDstX;
    i32 m_SurfaceCaptureDstY;
    i32 m_SurfaceCaptureDstW;
    i32 m_SurfaceCaptureDstH;
};
C_ASSERT(sizeof(SprtCtrl) == 0x2a2570);

DIFFABLE_EXTERN(SprtCtrl *, g_SprtCtrl);

}; // namespace th08
