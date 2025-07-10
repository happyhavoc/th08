#pragma once

#define TH_LANG TH_JP

// JP
#define TH_JP_WINDOW_TITLE "東方永夜抄　〜 Imperishable Night. ver 1.00d"
#define TH_JP_ERR_LOGGER_START "東方動作記録 --------------------------------------------- \r\n"
#define TH_JP_ERR_OPTION_CHANGED_RESTART "再起動を要するオプションが変更されたので再起動します\r\n"
#define TH_JP_ERR_D3D_ERR_COULD_NOT_CREATE_OBJ "Direct3D オブジェクトは何故か作成出来なかった\r\n"
#define TH_JP_DBG_SCREEN_INIT_32BITS "初回起動、画面を 32Bits で初期化しました\r\n"
#define TH_JP_DBG_SET_REFRESH_RATE_60HZ "リフレッシュレートを60Hzに変更を試みます\r\n"
#define TH_JP_DBG_TRY_ASYNC_VSYNC "VSync非同期可能かどうかを試みます\r\n"
#define TH_JP_DBG_USING_TL_HAL_MODE "T&L HAL で動作しま〜す\r\n"
#define TH_JP_DBG_TL_HAL_UNAVAILABLE "T&L HAL は使用できないようです\r\n"
#define TH_JP_DBG_HAL_UNAVAILABLE "HAL も使用できないようです\r\n"
#define TH_JP_DBG_USING_HAL_MODE "HAL で動作します\r\n"
#define TH_JP_ERR_NO_SUPPORT_FOR_D3DTEXOPCAPS_ADD "D3DTEXOPCAPS_ADD をサポートしていません、色加算エミュレートモードで動作します\r\n"
#define TH_JP_ERR_NO_LARGE_TEXTURE_SUPPORT "512 以上のテクスチャをサポートしていません。殆どの絵がボケて表示されます。\r\n"
#define TH_JP_ERR_D3DFMT_A8R8G8B8_UNSUPPORTED "D3DFMT_A8R8G8B8 をサポートしていません、減色モードで動作します\r\n"
#define TH_JP_DBG_USING_REF_MODE "REF で動作しますが、重すぎて恐らくゲームになりません...\r\n"
#define TH_JP_DBG_CANT_SET_REFRESH_RATE "リフレッシュレートが変更できません\r\n"
#define TH_JP_ERR_D3D_INIT_FAILED "Direct3D の初期化に失敗、これではゲームは出来ません\r\n"
#define TH_JP_ERR_ASYNC_VSYNC_UNSUPPORTED "非同期更新も行えません。一番汚いモードに変更します\r\n"
#define TH_JP_ERR_CHANGE_REFRESH_RATE "*** リフレッシュレートを60Hzに変更することを推奨します ***\r\n"
#define TH_JP_DBG_D3D_CAPS_START "現在のビデオカード、及びドライバの能力詳細\r\n"
#define TH_JP_DBG_CAPS_READ_SCANLINE "　走査線取得能力 : "
#define TH_JP_DBG_CAPS_WINDOW_MODE_RENDERING "　ウィンドウモードのレンダリング : "
#define TH_JP_DBG_CAPS_IMMEDIATE_PRESENTATION_SWAP "　プレゼンテーション間隔（直接）: "
#define TH_JP_DBG_CAPS_PRESENTATION_VSYNC "　プレゼンテーション間隔（垂直同期）: "
#define TH_JP_DBG_CAPS_DEVICE_START "　-- デバイス能力 ------------------------------\r\n"
#define TH_JP_DBG_CAPS_NONLOCAL_VRAM_BLIT "　System -> 非ローカルVRAMブリット :"
#define TH_JP_DBG_CAPS_HARDWARE_TL "　ハードウェア T&L : "
#define TH_JP_DBG_CAPS_TEXTURES_FROM_NONLOCAL_VRAM "　非ローカルVRAMからテクスチャ取得 : "
#define TH_JP_DBG_CAPS_TEXTURES_FROM_MAIN_MEMORY "　システムメモリからテクスチャ取得 : "
#define TH_JP_DBG_CAPS_TEXTURES_FROM_VRAM "　VRAM からテクスチャ取得 : "
#define TH_JP_DBG_CAPS_VERTEX_BUFFER_IN_RAM "　頂点バッファにシステムメモリを使用 : "
#define TH_JP_DBG_CAPS_VERTEX_BUFFER_IN_VRAM "　頂点バッファにビデオメモリを使用 : "
#define TH_JP_DBG_CAPS_PRIMITIVES_START "　-- プリミティブ能力 ---------------------------\r\n"
#define TH_JP_DBG_CAPS_ALPHA_BLENDING "　半透明処理 : "
#define TH_JP_DBG_CAPS_POINT_CLIPPING "　ポイントのクリッピング処理 : "
#define TH_JP_DBG_CAPS_VERTEX_CLIPPING "　プリミティブのクリッピング処理 : "
#define TH_JP_DBG_CAPS_CULL_CCW "　法線クリップ（反時計周り） : "
#define TH_JP_DBG_CAPS_CULL_CW "　法線クリップ（時計周り） : "
#define TH_JP_DBG_CAPS_CULL_NONE "　法線クリップ無し : "
#define TH_JP_DBG_CAPS_DEPTH_TEST_TOGGLE "　デプステストON/OFF切り替え : "
#define TH_JP_DBG_CAPS_RASTERIZER_START "　-- ラスタ能力 --------------------------------\r\n"
#define TH_JP_DBG_CAPS_ANISOTROPIC_FILTERING "　異方性フィルタリング : "
#define TH_JP_DBG_CAPS_ANTIALIASING "　アンチエイリアシング : "
#define TH_JP_DBG_CAPS_DITHERING "　ディザ処理 : "
#define TH_JP_DBG_CAPS_RANGE_BASED_FOG "　範囲ベースのフォグ : "
#define TH_JP_DBG_CAPS_Z_BASED_FOG "　Zベースのフォグ : "
#define TH_JP_DBG_CAPS_TABLE_BASED_FOG "　テーブルフォグ : "
#define TH_JP_DBG_CAPS_VERTEX_BASED_FOG "　頂点フォグ : "
#define TH_JP_DBG_CAPS_DEPTH_TEST "　デプステスト : "
#define TH_JP_DBG_CAPS_SHADING_START "　-- シェーディング能力 -----------------------\r\n"
#define TH_JP_DBG_CAPS_GOURAUD_SHADING "　グーローシェーディング : "
#define TH_JP_DBG_CAPS_ALPHA_GOURAUD_SHADING "　α成分のグーローシェーディング : "
#define TH_JP_DBG_CAPS_GOURAUD_SHADED_FOG "　グーローシェーディングでフォグ : "
#define TH_JP_DBG_CAPS_TEXTURE_START "　-- テクスチャ能力 ---------------------------\r\n"
#define TH_JP_DBG_CAPS_LARGEST_TEXTURE "　最大テクスチャサイズ : (%d, %d)\r\n"
#define TH_JP_DBG_CAPS_ALPHA_IN_TEXTURE "　α付きテクスチャ : "
#define TH_JP_DBG_CAPS_TEXTURE_TRANSFORM "　テクスチャトランスフォーム : "
#define TH_JP_DBG_CAPS_BILINEAR_FILTER_MAG "　バイリニア補間（拡大） : "
#define TH_JP_DBG_CAPS_BILINEAR_FILTER_MIN "　バイリニア補間（縮小） : "
#define TH_JP_DBG_CAPS_END "--------------------------------------------\r\n"
#define TH_JP_DBG_CAPABILITY_NOT_PRESENT "不可\r\n"
#define TH_JP_DBG_CAPABILITY_PRESENT "可\r\n"
#define TH_JP_ERR_ALREADY_RUNNING "二つは起動できません\r\n"
#define TH_JP_ERR_NO_PAD_FOUND "使えるパッドが存在しないようです、残念\r\n"
#define TH_JP_ERR_MIDI_FAILED_TO_READ_FILE "error : MIDI File が読み込めない %s \rv\r\n"
#define TH_JP_ERR_MIDI_NOT_LOADED "error : まだMIDIが読み込まれていないのに再生しようとしている\n"
#define TH_JP_ERR_ARCFILE_CORRUPTED "ファイル %s のオープン中にエラーが発生しました"
#define TH_JP_ERR_NOT_A_WAV_FILE "Wav ファイルじゃない? %s\r\n"
#define TH_JP_ERR_NO_WAVE_FILE "wave データが無いので、midi にします\r\n"
#define TH_JP_ERR_BGM_VERSION_MISMATCH "BGM データのバージョンが違います\r\n"
#define TH_JP_ERR_NO_VERTEX_BUFFER "頂点バッファの使用を抑制します\r\n"
#define TH_JP_ERR_NO_FOG "フォグの使用を抑制します\r\n"
#define TH_JP_ERR_USE_16BIT_TEXTURES "16Bit のテクスチャの使用を強制します\r\n"
#define TH_JP_ERR_FORCE_BACKBUFFER_CLEAR "バックバッファの消去を強制します\r\n"
#define TH_JP_ERR_DONT_RENDER_ITEMS "ゲーム周りのアイテムの描画を抑制します\r\n"
#define TH_JP_ERR_NO_GOURAUD_SHADING "グーローシェーディングを抑制します\r\n"
#define TH_JP_ERR_NO_DEPTH_TESTING "デプステストを抑制します\r\n"
#define TH_JP_ERR_NO_TEXTURE_COLOR_COMPOSITING "テクスチャの色合成を抑制します\r\n"
#define TH_JP_ERR_LAUNCH_WINDOWED "ウィンドウモードで起動します\r\n"
#define TH_JP_ERR_FORCE_REFERENCE_RASTERIZER "リファレンスラスタライザを強制します\r\n"
#define TH_JP_ERR_DO_NOT_USE_DIRECTINPUT "パッド、キーボードの入力に DirectInput を使用しません\r\n"
#define TH_JP_ERR_REDRAW_HUD_EVERY_FRAME "画面周りを毎回描画します\r\n"
#define TH_JP_ERR_PRELOAD_BGM "ＢＧＭをメモリに読み込みます\r\n"
#define TH_JP_ERR_NO_VSYNC "垂直同期を取りません\r\n"
#define TH_JP_ERR_DONT_DETECT_TEXT_BG "文字描画の環境を自動検出しません\r\n"
#define TH_JP_ERR_CONFIG_NOT_FOUND "コンフィグデータが見つからないので初期化しました\r\n"
#define TH_JP_ERR_CONFIG_ABNORMAL "コンフィグデータが異常でしたので再初期化しました\r\n"
#define TH_JP_ERR_FILE_CANNOT_BE_EXPORTED "ファイルが書き出せません %s\r\n"
#define TH_JP_ERR_FOLDER_HAS_WRITE_PROTECT_OR_DISK_FULL                                                                \
    "フォルダが書込み禁止属性になっているか、ディスクがいっぱいいっぱいになってませんか？\r\n"
#define TH_JP_ERR_SOUNDPLAYER_FAILED_TO_CREATE_BGM_SOUND_BUFFER "error : ストリーミング用サウンドバッファを作成出来ませんでした\r\n"
#define TH_JP_ERR_SOUNDPLAYER_FAILED_TO_INITIALIZE_OBJECT "DirectSound オブジェクトの初期化が失敗したよ\r\n"
#define TH_JP_ERR_SOUNDPLAYER_FAILED_TO_LOAD_SOUND_FILE "error : Sound ファイルが読み込めない データを確認 %s\r\n"
#define TH_JP_DBG_SOUNDPLAYER_INIT_SUCCESS "DirectSound は正常に初期化されました\r\n"
#define TH_JP_DBG_SOUNDPLAYER_BGM_THREAD_TERMINATED "atention : ストリーミング用スレッドは終了しました。\r\n"

// EN
#define TH_EN_WINDOW_TITLE "Touhou Eiyashou ~ Imperishable Night. ver 1.00d"
#define TH_EN_ERR_LOGGER_START "Touhou log ---------------------------------------------\r\n"
#define TH_EN_ERR_OPTION_CHANGED_RESTART "An option that requires a restart has been changed.\r\n"
#define TH_EN_ERR_D3D_ERR_COULD_NOT_CREATE_OBJ "Direct3D object could not be created for some reason\r\n"
#define TH_EN_DBG_SCREEN_INIT_32BITS "First run, using 32-bit textures\r\n"
#define TH_EN_DBG_SET_REFRESH_RATE_60HZ "Attempting to change refresh rate to 60Hz...\r\n"
#define TH_EN_DBG_TRY_ASYNC_VSYNC "Testing whether async vsync is possible...\r\n"
#define TH_EN_DBG_USING_TL_HAL_MODE "Using T&L HAL!\r\n"
#define TH_EN_DBG_TL_HAL_UNAVAILABLE "T&L HAL does not seem to be usable\r\n"
#define TH_EN_DBG_HAL_UNAVAILABLE "HAL isn't usable either\r\n"
#define TH_EN_DBG_USING_HAL_MODE "Using HAL\r\n"
#define TH_EN_ERR_NO_SUPPORT_FOR_D3DTEXOPCAPS_ADD "D3DTEXOPCAPS_ADD isn't supported, using emulated blend mode...\r\n"
#define TH_EN_ERR_NO_LARGE_TEXTURE_SUPPORT "Textures greater than 512 texels are unsupported. Expect textures to be blurred.\r\n"
#define TH_EN_ERR_D3DFMT_A8R8G8B8_UNSUPPORTED "D3DFMT_A8R8G8B8 isn't supported, using low-color mode...\r\n"
#define TH_EN_DBG_USING_REF_MODE "Using REF renderer, but it might become too heavy for the game...\r\n"
#define TH_EN_DBG_CANT_SET_REFRESH_RATE "Can't set refresh rate\r\n"
#define TH_EN_ERR_D3D_INIT_FAILED "Direct3D initialization failed, the game cannot be played.\r\n"
#define TH_EN_ERR_ASYNC_VSYNC_UNSUPPORTED "Can't do async vsync. Switching to the worst mode...\r\n"
#define TH_EN_ERR_CHANGE_REFRESH_RATE "*** You should change your refresh rate to 60Hz ***\r\n"
#define TH_EN_DBG_D3D_CAPS_START "Current video card and driver capabilities\r\n"
#define TH_EN_DBG_CAPS_READ_SCANLINE "  Get current scanline: "
#define TH_EN_DBG_CAPS_WINDOW_MODE_RENDERING "  Render in windowed mode: "
#define TH_EN_DBG_CAPS_IMMEDIATE_PRESENTATION_SWAP "  Immediately swap presentation: "
#define TH_EN_DBG_CAPS_PRESENTATION_VSYNC "  Use vsync for swaps: "
#define TH_EN_DBG_CAPS_DEVICE_START "  -- Driver Capabilities ------------------------------\r\n"
#define TH_EN_DBG_CAPS_NONLOCAL_VRAM_BLIT "  System -> Non-local VRAM blitting: "
#define TH_EN_DBG_CAPS_HARDWARE_TL "  Hardware T&L: "
#define TH_EN_DBG_CAPS_TEXTURES_FROM_NONLOCAL_VRAM "  Use textures from non-local VRAM: "
#define TH_EN_DBG_CAPS_TEXTURES_FROM_MAIN_MEMORY "  Use textures from system memory: "
#define TH_EN_DBG_CAPS_TEXTURES_FROM_VRAM "  Use textures from VRAM: "
#define TH_EN_DBG_CAPS_VERTEX_BUFFER_IN_RAM "  Use vertex buffer from system memory: "
#define TH_EN_DBG_CAPS_VERTEX_BUFFER_IN_VRAM "  Use vertex buffer from VRAM"
#define TH_EN_DBG_CAPS_PRIMITIVES_START "  -- Primitive Capabilities ---------------------------\r\n"
#define TH_EN_DBG_CAPS_ALPHA_BLENDING "  Alpha blending operations: "
#define TH_EN_DBG_CAPS_POINT_CLIPPING "  Do transformed point clipping: "
#define TH_EN_DBG_CAPS_VERTEX_CLIPPING "  Do primitive clipping: "
#define TH_EN_DBG_CAPS_CULL_CCW "  Cull counterclockwise winded primitives: "
#define TH_EN_DBG_CAPS_CULL_CW "  Cull clockwise winded primitives: "
#define TH_EN_DBG_CAPS_CULL_NONE "  Don't do winding-based culling of primitives: "
#define TH_EN_DBG_CAPS_DEPTH_TEST_TOGGLE "  Toggle depth test on and off: "
#define TH_EN_DBG_CAPS_RASTERIZER_START "  -- Rasterizer Capabilities --------------------------------\r\n"
#define TH_EN_DBG_CAPS_ANISOTROPIC_FILTERING "  Anisotropic filtering: "
#define TH_EN_DBG_CAPS_ANTIALIASING "  Antialiasing: "
#define TH_EN_DBG_CAPS_DITHERING "  Dithering: "
#define TH_EN_DBG_CAPS_RANGE_BASED_FOG "  Range based-fog: "
#define TH_EN_DBG_CAPS_Z_BASED_FOG "  Z-based fog: "
#define TH_EN_DBG_CAPS_TABLE_BASED_FOG "  Table-based fog: "
#define TH_EN_DBG_CAPS_VERTEX_BASED_FOG "  Vertex-based fog: "
#define TH_EN_DBG_CAPS_DEPTH_TEST "  Depth test: "
#define TH_EN_DBG_CAPS_SHADING_START "  -- Shading Capabilities ----------------------\r\n"
#define TH_EN_DBG_CAPS_GOURAUD_SHADING "  Gouraud shading: "
#define TH_EN_DBG_CAPS_ALPHA_GOURAUD_SHADING "  Gouraud shading with alpha components: "
#define TH_EN_DBG_CAPS_GOURAUD_SHADED_FOG "  Gouraud-shaded fog: "
#define TH_EN_DBG_CAPS_TEXTURE_START "  -- Texture Capabilities ---------------------------\r\n"
#define TH_EN_DBG_CAPS_LARGEST_TEXTURE "  Maximum texture size: (%d, %d)\r\n"
#define TH_EN_DBG_CAPS_ALPHA_IN_TEXTURE "  Textures with alpha components: "
#define TH_EN_DBG_CAPS_TEXTURE_TRANSFORM "  Texture transform: "
#define TH_EN_DBG_CAPS_BILINEAR_FILTER_MAG "  Bilinear filtering (Magnification): "
#define TH_EN_DBG_CAPS_BILINEAR_FILTER_MIN "  Bilinear filtering (Minification) : "
#define TH_EN_DBG_CAPS_END "--------------------------------------------\r\n"
#define TH_EN_DBG_CAPABILITY_NOT_PRESENT "Unable\r\n"
#define TH_EN_DBG_CAPABILITY_PRESENT "Able\r\n"
#define TH_EN_ERR_ALREADY_RUNNING "Can't start a second game instance!\r\n"
#define TH_EN_ERR_NO_PAD_FOUND "Unfortunately, there doesn't seem to be a pad that can be used.\r\n"
#define TH_EN_ERR_MIDI_FAILED_TO_READ_FILE "error : MIDI File %s could not be read.\n"
#define TH_EN_ERR_MIDI_NOT_LOADED "error : MIDI not loaded before being playback started.\n"
#define TH_EN_ERR_ARCFILE_CORRUPTED "Error opening file %s"
#define TH_EN_ERR_NOT_A_WAV_FILE "%s isn't a wav file?"
#define TH_EN_ERR_NO_WAVE_FILE "There is no wave data, so I'll make it midi!\r\n"
#define TH_EN_ERR_BGM_VERSION_MISMATCH "The BGM data version is wrong.\r\n"
#define TH_EN_ERR_NO_VERTEX_BUFFER "Suppressing the use of the vertex buffer.\r\n"
#define TH_EN_ERR_NO_FOG "Suppressing the use of fog.\r\n"
#define TH_EN_ERR_USE_16BIT_TEXTURES "Enforces the use of 16Bit textures.\r\n"
#define TH_EN_ERR_FORCE_BACKBUFFER_CLEAR "Force clearing of the back buffer.\r\n"
#define TH_EN_ERR_DONT_RENDER_ITEMS "Suppress the rendering of items around the game.\r\n"
#define TH_EN_ERR_NO_GOURAUD_SHADING "Suppress gouraud shading.\r\n"
#define TH_EN_ERR_NO_DEPTH_TESTING "Suppress depth testing.\r\n"
#define TH_EN_ERR_NO_TEXTURE_COLOR_COMPOSITING "Suppress texture color compositing.\r\n"
#define TH_EN_ERR_LAUNCH_WINDOWED "Launch in windowed mode.\r\n"
#define TH_EN_ERR_FORCE_REFERENCE_RASTERIZER "Force reference rasterizer.\r\n"
#define TH_EN_ERR_DO_NOT_USE_DIRECTINPUT "Do not use DirectInput for pad and keyboard input.\r\n"
#define TH_EN_ERR_REDRAW_HUD_EVERY_FRAME "Draw screen surroundings every time.\r\n"
#define TH_EN_ERR_PRELOAD_BGM "Preload BGM in memory.\r\n"
#define TH_EN_ERR_NO_VSYNC "No Vertical Sync.\r\n"
#define TH_EN_ERR_DONT_DETECT_TEXT_BG "Do not detect text drawing brackground.\r\n"
#define TH_EN_ERR_CONFIG_NOT_FOUND "Config not found, initializing with default values.\r\n"
#define TH_EN_ERR_CONFIG_ABNORMAL "Config abnormal, reinitializing with default values.\r\n"
#define TH_EN_ERR_FILE_CANNOT_BE_EXPORTED "File cannot be exported %s.\r\n"
#define TH_EN_ERR_FOLDER_HAS_WRITE_PROTECT_OR_DISK_FULL "Folder has write protect attribute or disk full?\r\n"
#define TH_EN_ERR_SOUNDPLAYER_FAILED_TO_CREATE_BGM_SOUND_BUFFER "error: Could not create sound buffer for music\r\n"
#define TH_EN_ERR_SOUNDPLAYER_FAILED_TO_INITIALIZE_OBJECT "DirectSound: Failed to initialize object\r\n"
#define TH_EN_ERR_SOUNDPLAYER_FAILED_TO_LOAD_SOUND_FILE "error: Could not load sound file %s\r\n"
#define TH_EN_DBG_SOUNDPLAYER_INIT_SUCCESS "DirectSound initialized successfully\r\n"
#define TH_EN_DBG_SOUNDPLAYER_BGM_THREAD_TERMINATED "attention : The streaming BGM task thread has terminated.\r\n"

#define TH_CONCAT_HELPER(x, y) x##y

#define TH_MAKE_LANG_STR(lang, id) TH_CONCAT_HELPER(lang, id)

#define TH_WINDOW_TITLE TH_MAKE_LANG_STR(TH_LANG, _WINDOW_TITLE)
#define TH_ERR_LOGGER_START TH_MAKE_LANG_STR(TH_LANG, _ERR_LOGGER_START)
#define TH_ERR_OPTION_CHANGED_RESTART TH_MAKE_LANG_STR(TH_LANG, _ERR_OPTION_CHANGED_RESTART)
#define TH_ERR_D3D_ERR_COULD_NOT_CREATE_OBJ TH_MAKE_LANG_STR(TH_LANG, _ERR_D3D_ERR_COULD_NOT_CREATE_OBJ)
#define TH_DBG_SCREEN_INIT_32BITS TH_MAKE_LANG_STR(TH_LANG, _DBG_SCREEN_INIT_32BITS)
#define TH_DBG_SET_REFRESH_RATE_60HZ TH_MAKE_LANG_STR(TH_LANG, _DBG_SET_REFRESH_RATE_60HZ)
#define TH_DBG_TRY_ASYNC_VSYNC TH_MAKE_LANG_STR(TH_LANG, _DBG_TRY_ASYNC_VSYNC)
#define TH_DBG_USING_TL_HAL_MODE TH_MAKE_LANG_STR(TH_LANG, _DBG_USING_TL_HAL_MODE)
#define TH_DBG_TL_HAL_UNAVAILABLE TH_MAKE_LANG_STR(TH_LANG, _DBG_TL_HAL_UNAVAILABLE)
#define TH_DBG_HAL_UNAVAILABLE TH_MAKE_LANG_STR(TH_LANG, _DBG_HAL_UNAVAILABLE)
#define TH_DBG_USING_HAL_MODE TH_MAKE_LANG_STR(TH_LANG, _DBG_USING_HAL_MODE)
#define TH_ERR_NO_SUPPORT_FOR_D3DTEXOPCAPS_ADD TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_SUPPORT_FOR_D3DTEXOPCAPS_ADD)
#define TH_ERR_NO_LARGE_TEXTURE_SUPPORT TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_LARGE_TEXTURE_SUPPORT)
#define TH_ERR_D3DFMT_A8R8G8B8_UNSUPPORTED TH_MAKE_LANG_STR(TH_LANG, _ERR_D3DFMT_A8R8G8B8_UNSUPPORTED)
#define TH_DBG_USING_REF_MODE TH_MAKE_LANG_STR(TH_LANG, _DBG_USING_REF_MODE)
#define TH_DBG_CANT_SET_REFRESH_RATE TH_MAKE_LANG_STR(TH_LANG, _DBG_CANT_SET_REFRESH_RATE)
#define TH_ERR_D3D_INIT_FAILED TH_MAKE_LANG_STR(TH_LANG, _ERR_D3D_INIT_FAILED)
#define TH_ERR_ASYNC_VSYNC_UNSUPPORTED TH_MAKE_LANG_STR(TH_LANG, _ERR_ASYNC_VSYNC_UNSUPPORTED)
#define TH_ERR_CHANGE_REFRESH_RATE TH_MAKE_LANG_STR(TH_LANG, _ERR_CHANGE_REFRESH_RATE)
#define TH_DBG_D3D_CAPS_START TH_MAKE_LANG_STR(TH_LANG, _DBG_D3D_CAPS_START)
#define TH_DBG_CAPS_READ_SCANLINE TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_READ_SCANLINE)
#define TH_DBG_CAPS_WINDOW_MODE_RENDERING TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_WINDOW_MODE_RENDERING)
#define TH_DBG_CAPS_IMMEDIATE_PRESENTATION_SWAP TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_IMMEDIATE_PRESENTATION_SWAP)
#define TH_DBG_CAPS_PRESENTATION_VSYNC TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_PRESENTATION_VSYNC)
#define TH_DBG_CAPS_DEVICE_START TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_DEVICE_START)
#define TH_DBG_CAPS_NONLOCAL_VRAM_BLIT TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_NONLOCAL_VRAM_BLIT)
#define TH_DBG_CAPS_HARDWARE_TL TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_HARDWARE_TL)
#define TH_DBG_CAPS_TEXTURES_FROM_NONLOCAL_VRAM TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_TEXTURES_FROM_NONLOCAL_VRAM)
#define TH_DBG_CAPS_TEXTURES_FROM_MAIN_MEMORY TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_TEXTURES_FROM_MAIN_MEMORY)
#define TH_DBG_CAPS_TEXTURES_FROM_VRAM TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_TEXTURES_FROM_VRAM)
#define TH_DBG_CAPS_VERTEX_BUFFER_IN_RAM TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_VERTEX_BUFFER_IN_RAM)
#define TH_DBG_CAPS_VERTEX_BUFFER_IN_VRAM TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_VERTEX_BUFFER_IN_VRAM)
#define TH_DBG_CAPS_PRIMITIVES_START TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_PRIMITIVES_START)
#define TH_DBG_CAPS_ALPHA_BLENDING TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_ALPHA_BLENDING)
#define TH_DBG_CAPS_POINT_CLIPPING TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_POINT_CLIPPING)
#define TH_DBG_CAPS_VERTEX_CLIPPING TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_VERTEX_CLIPPING)
#define TH_DBG_CAPS_CULL_CCW TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_CULL_CCW)
#define TH_DBG_CAPS_CULL_CW TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_CULL_CW)
#define TH_DBG_CAPS_CULL_NONE TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_CULL_NONE)
#define TH_DBG_CAPS_DEPTH_TEST_TOGGLE TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_DEPTH_TEST_TOGGLE)
#define TH_DBG_CAPS_RASTERIZER_START TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_RASTERIZER_START)
#define TH_DBG_CAPS_ANISOTROPIC_FILTERING TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_ANISOTROPIC_FILTERING)
#define TH_DBG_CAPS_ANTIALIASING TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_ANTIALIASING)
#define TH_DBG_CAPS_DITHERING TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_DITHERING)
#define TH_DBG_CAPS_RANGE_BASED_FOG TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_RANGE_BASED_FOG)
#define TH_DBG_CAPS_Z_BASED_FOG TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_Z_BASED_FOG)
#define TH_DBG_CAPS_TABLE_BASED_FOG TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_TABLE_BASED_FOG)
#define TH_DBG_CAPS_VERTEX_BASED_FOG TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_VERTEX_BASED_FOG)
#define TH_DBG_CAPS_DEPTH_TEST TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_DEPTH_TEST)
#define TH_DBG_CAPS_SHADING_START TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_SHADING_START)
#define TH_DBG_CAPS_GOURAUD_SHADING TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_GOURAUD_SHADING)
#define TH_DBG_CAPS_ALPHA_GOURAUD_SHADING TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_ALPHA_GOURAUD_SHADING)
#define TH_DBG_CAPS_GOURAUD_SHADED_FOG TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_GOURAUD_SHADED_FOG)
#define TH_DBG_CAPS_TEXTURE_START TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_TEXTURE_START)
#define TH_DBG_CAPS_LARGEST_TEXTURE TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_LARGEST_TEXTURE)
#define TH_DBG_CAPS_ALPHA_IN_TEXTURE TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_ALPHA_IN_TEXTURE)
#define TH_DBG_CAPS_TEXTURE_TRANSFORM TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_TEXTURE_TRANSFORM)
#define TH_DBG_CAPS_BILINEAR_FILTER_MAG TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_BILINEAR_FILTER_MAG)
#define TH_DBG_CAPS_BILINEAR_FILTER_MIN TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_BILINEAR_FILTER_MIN)
#define TH_DBG_CAPS_END TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPS_END)
#define TH_DBG_CAPABILITY_NOT_PRESENT TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPABILITY_NOT_PRESENT)
#define TH_DBG_CAPABILITY_PRESENT TH_MAKE_LANG_STR(TH_LANG, _DBG_CAPABILITY_PRESENT)
#define TH_ERR_ALREADY_RUNNING TH_MAKE_LANG_STR(TH_LANG, _ERR_ALREADY_RUNNING)
#define TH_ERR_NO_PAD_FOUND TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_PAD_FOUND)
#define TH_ERR_MIDI_FAILED_TO_READ_FILE TH_MAKE_LANG_STR(TH_LANG, _ERR_MIDI_FAILED_TO_READ_FILE)
#define TH_ERR_MIDI_NOT_LOADED TH_MAKE_LANG_STR(TH_LANG, _ERR_MIDI_NOT_LOADED)
#define TH_ERR_ARCFILE_CORRUPTED TH_MAKE_LANG_STR(TH_LANG, _ERR_ARCFILE_CORRUPTED)
#define TH_ERR_NOT_A_WAV_FILE TH_MAKE_LANG_STR(TH_LANG, _ERR_NOT_A_WAV_FILE)
#define TH_ERR_NO_WAVE_FILE TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_WAVE_FILE)
#define TH_ERR_BGM_VERSION_MISMATCH TH_MAKE_LANG_STR(TH_LANG, _ERR_BGM_VERSION_MISMATCH)
#define TH_ERR_NO_VERTEX_BUFFER TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_VERTEX_BUFFER)
#define TH_ERR_NO_FOG TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_FOG)
#define TH_ERR_USE_16BIT_TEXTURES TH_MAKE_LANG_STR(TH_LANG, _ERR_USE_16BIT_TEXTURES)
#define TH_ERR_FORCE_BACKBUFFER_CLEAR TH_MAKE_LANG_STR(TH_LANG, _ERR_FORCE_BACKBUFFER_CLEAR)
#define TH_ERR_DONT_RENDER_ITEMS TH_MAKE_LANG_STR(TH_LANG, _ERR_DONT_RENDER_ITEMS)
#define TH_ERR_NO_GOURAUD_SHADING TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_GOURAUD_SHADING)
#define TH_ERR_NO_DEPTH_TESTING TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_DEPTH_TESTING)
#define TH_ERR_NO_TEXTURE_COLOR_COMPOSITING TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_TEXTURE_COLOR_COMPOSITING)
#define TH_ERR_LAUNCH_WINDOWED TH_MAKE_LANG_STR(TH_LANG, _ERR_LAUNCH_WINDOWED)
#define TH_ERR_FORCE_REFERENCE_RASTERIZER TH_MAKE_LANG_STR(TH_LANG, _ERR_FORCE_REFERENCE_RASTERIZER)
#define TH_ERR_DO_NOT_USE_DIRECTINPUT TH_MAKE_LANG_STR(TH_LANG, _ERR_DO_NOT_USE_DIRECTINPUT)
#define TH_ERR_REDRAW_HUD_EVERY_FRAME TH_MAKE_LANG_STR(TH_LANG, _ERR_REDRAW_HUD_EVERY_FRAME)
#define TH_ERR_PRELOAD_BGM TH_MAKE_LANG_STR(TH_LANG, _ERR_PRELOAD_BGM)
#define TH_ERR_NO_VSYNC TH_MAKE_LANG_STR(TH_LANG, _ERR_NO_VSYNC)
#define TH_ERR_DONT_DETECT_TEXT_BG TH_MAKE_LANG_STR(TH_LANG, _ERR_DONT_DETECT_TEXT_BG)
#define TH_ERR_CONFIG_NOT_FOUND TH_MAKE_LANG_STR(TH_LANG, _ERR_CONFIG_NOT_FOUND)
#define TH_ERR_CONFIG_ABNORMAL TH_MAKE_LANG_STR(TH_LANG, _ERR_CONFIG_ABNORMAL)
#define TH_ERR_FILE_CANNOT_BE_EXPORTED TH_MAKE_LANG_STR(TH_LANG, _ERR_FILE_CANNOT_BE_EXPORTED)
#define TH_ERR_FOLDER_HAS_WRITE_PROTECT_OR_DISK_FULL                                                                   \
    TH_MAKE_LANG_STR(TH_LANG, _ERR_FOLDER_HAS_WRITE_PROTECT_OR_DISK_FULL)
#define TH_ERR_SOUNDPLAYER_FAILED_TO_CREATE_BGM_SOUND_BUFFER TH_MAKE_LANG_STR(TH_LANG, _ERR_SOUNDPLAYER_FAILED_TO_CREATE_BGM_SOUND_BUFFER)
#define TH_ERR_SOUNDPLAYER_FAILED_TO_INITIALIZE_OBJECT TH_MAKE_LANG_STR(TH_LANG, _ERR_SOUNDPLAYER_FAILED_TO_INITIALIZE_OBJECT)
#define TH_ERR_SOUNDPLAYER_FAILED_TO_LOAD_SOUND_FILE TH_MAKE_LANG_STR(TH_LANG, _ERR_SOUNDPLAYER_FAILED_TO_LOAD_SOUND_FILE)
#define TH_DBG_SOUNDPLAYER_INIT_SUCCESS TH_MAKE_LANG_STR(TH_LANG, _DBG_SOUNDPLAYER_INIT_SUCCESS)
#define TH_DBG_SOUNDPLAYER_BGM_THREAD_TERMINATED TH_MAKE_LANG_STR(TH_LANG, _DBG_SOUNDPLAYER_BGM_THREAD_TERMINATED)

#define TH_FONT_NAME "ＭＳ ゴシック"
