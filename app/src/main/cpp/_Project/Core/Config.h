#pragma once
#include <string>
#include <string_view>

namespace Config
{
    // === Шейдерная тема ===
    inline const std::string SHADER_PATH = "Shaders/";
    
    inline std::string GetVertexShaderPath(const std::string& name) 
    {
        return SHADER_PATH + name + "/vertex.glsl";
    }

    inline std::string GetFragmentShaderPath(const std::string& name) 
    {
        return SHADER_PATH + name + "/fragment.glsl";
    }
    
    // === Гемплей ===
    namespace Gameplay
    {
        inline constexpr float CELL_SIZE = 0.17f;

        inline constexpr int BOARD_WIDTH = 10;
        inline constexpr int BOARD_HEIGHT = 20;
        inline constexpr float BOARD_OFFSET_X = ((BOARD_WIDTH / 2.0) * -1) * CELL_SIZE;
        inline constexpr float BOARD_OFFSET_Y = ((BOARD_HEIGHT / 2.0) * -1) * CELL_SIZE;

        inline constexpr float LINE_THICKNESS = 0.008f;
        inline constexpr float GRID_LINE_WIDTH = 3.0f;

        inline constexpr float BACKGROUND_SPEED = 0.01f;

        inline constexpr float DROP_INTERVAL = 0.5f;
        inline constexpr float LOCK_DELAY = 1.0f;
        inline constexpr int MAX_LOCK_MOVES = 15;
        inline constexpr float RESET_INPUT_DELAY = 0.5f;
        inline constexpr float DIFFICULTY_MULTIPLIER = 0.9f;

        inline constexpr float SCORE_TIME_MEANING = 100.0f;

        inline constexpr float OUTLINE_THICKNESS = 1.15f;

        inline constexpr float BUTTON_PERFORM_DELAY = 0.1f;

        inline constexpr float SHAKE_INTENSY = 0.0f;
        inline constexpr float SHAKE_DURATION = 0.45f;
        inline constexpr float STREGTH_MOVE_BOARD_SIDE = 0.2f;
        inline constexpr float ACCEL_MOVE_BOARD_TO_ZERO_X = 0.035f;
        inline constexpr float LEAN_INTENSY = 1.5f;
        inline constexpr float LEAN_ACCEL = 15.0f;

        inline constexpr float SPRING_STIFFNESS = 300.0f;
        inline constexpr float SPRING_DAMPING = 15.0f;
        inline constexpr float TETROMINO_LEAN_FORCE = 0.05f;
        inline constexpr float TETROMINO_LEAN_SPEED = 15.0f;
        inline constexpr float SQUASH_Y_FORCE = 0.75f;
        inline constexpr float SQUASH_VELOCITY = -15.0f;
    }
    
    // === Цвета ===
    namespace Color
    {
        inline constexpr float COLOR_BACKGROUND[] = {0.1f, 0.1f, 0.1f, 1.0f};
        inline constexpr float COLOR_GRID[] = {1.0f, 1.0f, 1.0f, 0.1f};
        inline constexpr float COLOR_TETROMINO_I[] = {1.0f, 1.0f, 0.0f, 1.0f};

        inline constexpr float TETROMINO_COLORS[] = {
            0.0f, 1.0f, 1.0f, // 0: Голубой (I)
            1.0f, 1.0f, 0.0f, // 1: Желтый  (O) 
            0.5f, 0.0f, 0.5f, // 2: Пурпурный (T)
            0.0f, 1.0f, 0.0f, // 3: Зеленый (S)
            1.0f, 0.0f, 0.0f, // 4: Красный (Z)
            0.0f, 0.0f, 1.0f, // 5: Синий   (J)
            1.0f, 0.5f, 0.0f  // 6: Оранжевый (L)
        };

        inline constexpr float COLOR_TETROMINO_O[] = {1.0f, 1.0f, 0.0f, 1.0f};
    }
    
    // === Настройки рендера ===
    namespace Render
    {
        inline constexpr bool VSYNC_ENABLED = true;
        inline constexpr int TARGET_FPS = 60;
    }
    
    // === Пути к текстурам ===
    inline const std::string TEXTURE_PATH = "Textures/";
    
    inline std::string GetTexturePath(const std::string_view& name) 
    {
        return TEXTURE_PATH + static_cast<std::string>(name) + ".png";
    }

    // === Пути к звукам ===
    inline const std::string AUDIO_PATH = "Audio/";
    
    inline std::string GetSoundPathWav(const std::string_view& name) 
    {
        std::string stringName = (std::string)name;
        return AUDIO_PATH + stringName + ".wav";
    }

    // === Пути к шрифтам ===
    inline const std::string FONT_PATH = "Fonts/";
    
    inline std::string GetFontPathOTF(const std::string& name) 
    {
        return FONT_PATH + name + ".ttf";
    }

    inline std::string GetFontPathTTF(const std::string& name) 
    {
        return FONT_PATH + name + ".ttf";
    }

    namespace Control
    {
        // Базовая чувствительность (чем МЕНЬШЕ число, тем БЫСТРЕЕ двигается фигура)
        inline constexpr float DRAG_SENSITIVITY_X = 50.0f; // Переводим в экранные пиксели для предсказуемости
        inline constexpr float DRAG_SENSITIVITY_Y = 70.0f; 

        // Мультипликатор для ПЕРВОГО шага (0.5f означает, что первый блок сдвинется в 2 раза легче)
        inline constexpr float FIRST_STEP_SENSITIVITY_SCALE = 0.5f;

        // Порог в пикселях, после которого игра понимает, что начался осознанный свайп
        inline constexpr float THRESHOLD_DRAGGING = 120.0f; // Уменьшил с 225, чтобы сенса отзывалась быстрее

        // Микро-порог в пикселях. Если палец сместился БОЛЬШЕ этого значения, поворот ТАПОМ запрещен!
        inline constexpr float ROTATE_CANCEL_DISTANCE = 15.0f; 

        // Максимальное время в мс для регистрации тапа (поворота). Уменьшил до 160 мс для резкости
        inline constexpr int TOUCH_UP_INTERVAL = 160; 

        inline constexpr float HARD_DROP_DISTANCE = 150.0f;
        inline constexpr int HARD_DROP_INTERVAL = 150.0f;
    }
    
    // Настройки эффектиков
    namespace VFX
    {
        inline constexpr float PARTICLES_DURATION = 0.7f;
        inline constexpr float GHOST_EFFECT_DURATION = 0.85f;
    }

    // Настройки UI
    namespace UI
    {
        inline constexpr float SCORE_TEXT_SCALE = 0.45f;
        inline constexpr float SCORE_TEXT_COLOR[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        inline constexpr float SCORE_TEXT_ANIMATION_INTERVAL = 0.15f;
        inline constexpr int SCORE_TEXT_ANIMATION_COUNT = 5;
        inline constexpr float NEXT_TETROMINO_SCALE = 0.6f;
        inline constexpr float FONT_MULTIPLIER = 0.35f;
        inline constexpr float GAME_OVER_TEXT_SIZE = 1.1f;
        inline constexpr float SCORE_GAME_OVER_TEXT_SIZE = 0.6f;
    }

    // Настройки звука
    namespace Sound
    {
        inline constexpr std::string_view GAMEPLAY_BG_MUSIC = "GameplayMusicBG";
        inline constexpr std::string_view TETROMINO_FREEZE_SOUND = "Freeze";
        inline constexpr std::string_view CLEAR_LINE_SOUND = "ClearLine";
        inline constexpr std::string_view GAME_OVER_SOUND = "GameOver";
        inline constexpr std::string_view MOVE_TETROMINO = "MoveTetromino";
        inline constexpr std::string_view ROTATE_TETROMINO = "RotateTetromino";
        inline constexpr std::string_view BUTTON_PRESS = "ButtonPress";
    }

    namespace TextureNames
    {
        inline constexpr std::string_view GAMEPLAY_BACKGROUND = "GameplayBackground";
        inline constexpr std::string_view MENU_BUTTON_BG = "MenuButton";
        inline constexpr std::string_view SWITCH_BG = "SwitchBackground";
        inline constexpr std::string_view SWITCH_TUMBLER = "SwitchTumbler";
        inline constexpr std::string_view PAUSE = "pause";
        inline constexpr std::string_view PAUSE_BG = "pauseBG";
        inline constexpr std::string_view GAME_OVER_BG = "gameoverBG";
    }

    namespace ShaderNames
    {
        inline constexpr std::string_view DEFAULT_TEXTURE = "DefaultTex";
        inline constexpr std::string_view DEFAULT_NO_TEXTURE = "DefaultNoTex";
        inline constexpr std::string_view LOCK_DELAY = "LockDelay";
        inline constexpr std::string_view GRID = "Grid";
    }
}
