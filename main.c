#include <raylib.h>
#define TDAT_IMPL
#include <tdat.h>

void create_test_file(const char* filename) {
    tdat d = CreateTDAT(64, 64);
    
    for (uint32_t y = 0; y < d.height; y++) {
        for (uint32_t x = 0; x < d.width; x++) {
            int index = y * d.width + x;
            if ((x / 8 + y / 8) % 2 == 0) d.data[index] = 0xFFFF0000;
            else d.data[index] = 0xFFFFFFFF;
        }
    }
    
    SaveTDAT(&d, filename);
    FreeTDAT(&d);
}

int main(int argc, char** argv) {
	const char* filename = "test.tdat";
	if (argc > 1) filename = argv[1];
	if (!FileExists(filename)) create_test_file(filename);
	
	tdat d = LoadTDAT(filename);
    if (d.data == NULL) return 1;
	
	SetTraceLogLevel(LOG_NONE);
	InitWindow(800, 600, "TDAT Viewer");
	SetTargetFPS(60);

	Image img = { 0 };
    img.data = d.data;
    img.width = (int)d.width;
    img.height = (int)d.height;
    img.mipmaps = 1;
    img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    Texture2D texture = LoadTextureFromImage(img);

	while (!WindowShouldClose()) {
		BeginDrawing();
        ClearBackground(DARKGRAY);

        if (texture.id > 0) {
            float safeAreaW = (float)GetScreenWidth() * 0.90f;
            float safeAreaH = (float)GetScreenHeight() * 0.90f;

            float scaleX = safeAreaW / d.width;
            float scaleY = safeAreaH / d.height;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;
            if (scale < 1.0f) scale = 1.0f; 

            float finalWidth = d.width * scale;
            float finalHeight = d.height * scale;
            
            float posX = (GetScreenWidth() - finalWidth) / 2;
            float posY = (GetScreenHeight() - finalHeight) / 2;

            DrawTextureEx(texture, (Vector2){ posX, posY }, 0.0f, scale, WHITE);
        }

        DrawText(TextFormat("File: %s", filename), 8, 8, 32, BLACK);
        DrawText(TextFormat("File: %s", filename), 5, 5, 32, WHITE);
        EndDrawing();
	}

	UnloadTexture(texture);
	CloseWindow();

	FreeTDAT(&d);
	return 0;
}
