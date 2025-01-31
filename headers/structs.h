#ifndef STRUCTS_H
#define STRUCTS_H

// Define a câmera do jogo.
typedef struct Camera {
    SDL_Point position;
    int speed;
} Camera;

// Estrutura para armazenar as propriedades de cada tipo de bloco.
typedef struct BlockProperties {
    SDL_Rect src;
    _Bool isSolid;
    _Bool isBackground;
} BlockProperties;

// Estrututa que representa um bloco.
typedef struct Block {
    BlockProperties properties;
    SDL_Rect dst;
    BLOCK_TYPE type;
} Block;

// Estrututa que representa uma masmorra.
typedef struct Dungeon {
    Block block[DUNGEON_WIDTH][DUNGEON_HEIGHT];
    SDL_Texture *texture;
    int level;
} Dungeon;

// Estrutura que representa os pontos de vida do jogador.
typedef struct Heart {
    SDL_Texture *texture;
    SDL_Rect src, dst;
} Heart;

// Estrutura que representa o jogador.
typedef struct Player {
    SDL_Rect src, dst;
    SDL_Texture *texture;
    Heart hearts;
} Player;

// Estrutura que representa um foco de luz.
typedef struct Light {
    SDL_Texture *texture;
    SDL_Rect dst;
} Light;

// Estrututa que representa o inventário do jogador.
typedef struct Inventory {
    SDL_Rect src[INVENTORY_SIZE], dst[INVENTORY_SIZE];
    SDL_Texture *texture;
} Inventory;

// Estrutura para texto.
typedef struct Text {
    SDL_Texture *texture;
    SDL_Rect dst;
} Text;

// Estrutura que representa um botão.
typedef struct Button {
    SDL_Rect src, dst;
    Text text;
} Button;

// Estrutura para o menu principal do jogo.
typedef struct MainMenu {
    Button button[MAX_MAIN_MENU_BUTTONS];
    SDL_Texture *texture, *backgroundTexture;
    SDL_Rect dst;
    Text version;
} MainMenu;

// Estrutura que armazena os estados do mouse.
typedef struct Mouse {
    SDL_Point position;
    _Bool leftButton, rightButton;
} Mouse;

// Estrutura que armazena os estados das teclas do teclado.
typedef struct Keyboard {
    const Uint8 *states;
} Keyboard;

// Estrutura que armazena todos os estados das entradas do usuário.
typedef struct UserInputs {
    Mouse mouse;
    Keyboard keyboard;
} UserInputs;

// Estrutura principal que representa o jogo.
typedef struct Game {
    GAME_STATE state;

    lua_State *L;

    SDL_Window *window;

    SDL_Renderer *renderer;
    SDL_Color backgroundColor;
    SDL_Rect viewport;

    SDL_Event event;

    _Bool running;
    Uint32 frameStart, frameTime;

    TTF_Font *font[MAX_FONTS];

    UserInputs userInputs;
    Camera camera;
    Dungeon dungeon;
    MainMenu mainMenu;
    Player player;
    Inventory inventory;
    Light light;
} Game;

#endif // STRUCTS_H