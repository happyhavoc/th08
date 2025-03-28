#pragma once

#define TH_LANG TH_JP

// JP
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
