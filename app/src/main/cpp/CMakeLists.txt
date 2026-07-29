cmake_minimum_required(VERSION 3.10)
project("Dino Drop")

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/freetype)

set(OBOE_DIR /home/kesha/oboe)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${OBOE_DIR} ./oboe)

add_library(DinoDrop SHARED
        native-lib.cpp
        _Project/Engine/engine.cpp
        _Project/Game/game.cpp
        _Project/Render/render.cpp
        _Project/Grid/grid.cpp
        _Project/Shaders/Shader.cpp
        _Project/Board/board.cpp
        _Project/Tetrominos/draw_tetromino.cpp
        _Project/Tetrominos/activeTetromino.cpp
        _Project/Camera/camera.cpp
        _Project/ParticleSystem/LineNeonFlash/line_neon_flash.cpp
        _Project/ParticleSystem/LineNeonFlash/particle_explosion.cpp
        _Project/ParticleSystem/GhostTrail/ghost_trail.cpp
        _Project/SpriteRenderer/sprite_renderer.cpp
        _Project/Resources/resource_manager.cpp
        _Project/Background/moving_background.cpp
        _Project/Shaders/ShaderManager.cpp
        _Project/FontRenderer/FontRenderer.cpp
        _Project/GameState/GameStateManager.cpp
        _Project/GameState/Gameplay/GameplayState.cpp
        _Project/GameState/MainMenu/MainMenuState.cpp
        _Project/GameState/GameOver/GameOverState.cpp
        _Project/Audio/AudioManager.cpp
        _Project/LockDelayStatusBar/LockBar.cpp
)

target_include_directories(DinoDrop PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue
        ${CMAKE_CURRENT_SOURCE_DIR}/_Project
        ${CMAKE_CURRENT_SOURCE_DIR}/_Project/SmartPointers
        ${CMAKE_CURRENT_SOURCE_DIR}/_Project/Plugins
        ${CMAKE_CURRENT_SOURCE_DIR}/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/freetype/include
        /home/kesha/oboe/include
)

target_link_options(DinoDrop PRIVATE
        -Wl,--undefined=ANativeActivity_onCreate
)

target_link_libraries(DinoDrop
        freetype
        native_app_glue
        android
        log
        EGL          
        GLESv3      
        oboe
)
