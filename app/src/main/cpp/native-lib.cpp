#include "_Project/Engine/engine.h"
#include "_Project/Game/game.h"
#include <GLES2/gl2.h>
#include "../Core/Config.h"

void android_main(android_app* app) 
{
    bool isGameInitialized = false;
    int readyFramesCount = 0;

    Engine& engine = Engine::Get();
    engine.SetApp(app);
    Game* game = new Game();
    app->userData = game;

    engine.PrintInfo("App launched");

    while (app->destroyRequested == false) 
    {
        engine.Update();

        if (engine.IsContextReady())
        {
            if (readyFramesCount < 3) 
            {
                glClearColor(0, 0, 0, 1);
                glClear(GL_COLOR_BUFFER_BIT);
                engine.SwapBuffers();
                readyFramesCount++;
                continue;
            }

            if (!isGameInitialized)
            {
                game->Start();
                isGameInitialized = true;
            }

            glClearColor(
                Config::Color::COLOR_BACKGROUND[0],
                Config::Color::COLOR_BACKGROUND[1],
                Config::Color::COLOR_BACKGROUND[2],
                1.0f
            );

            glClear(GL_COLOR_BUFFER_BIT);

            game->Update();

            engine.SwapBuffers();
        }
        else
        {
            readyFramesCount = 0;
        }
    }

    engine.PrintInfo("App terminated");
}
