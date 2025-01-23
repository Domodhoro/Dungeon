#ifndef INIT_H
#define INIT_H

// Função responsável por criar a janela do jogo.
static _Bool createWindow(Game *game) {
    // Cria a janela do jogo com o título e dimensões definidas e caso ocorra erro, exibe uma mensagem.
    game->window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!game->window) {
        fprintf(stderr, "Falha ao criar janela de visualização: %s.\n", SDL_GetError());
        return false;
    }

    // Carrega a imagem do ícone da janela e caso ocorra erro ao carregar o ícone, exibe uma mensagem de erro.
    SDL_Surface *iconSurface = IMG_Load("./icon.png");
    if (!iconSurface) {
        fprintf(stderr, "Falha ao carregar o ícone: %s\n", IMG_GetError());
        return false;
    }
    SDL_SetWindowIcon(game->window, iconSurface);
    // Libera a superfície que foi usada para o ícone.
    SDL_FreeSurface(iconSurface);

    // Posiciona o mouse no centro da janela.
    SDL_WarpMouseInWindow(game->window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    return true;
}

// Função responsável por criar o renderizador.
static _Bool createRenderer(Game *game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game->renderer) {
        // Caso ocorra erro ao criar o renderizador, exibe a mensagem de erro.
        fprintf(stderr, "Falha ao criar o renderizador: %s.\n", SDL_GetError());
        return false;
    }

    // Define a cor de fundo.
    game->backgroundColor = (SDL_Color) {
        100, 100, 100
    };

    // Define as dimensões da janela de visualização do renderizador.
    game->viewport = (SDL_Rect) {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };
    return true;
}

// Criar a superfície com o texto.
static Text createText(Game *game, const char *input, int x, int y, SDL_Color color) {
    // Cria uma estrutura de texto.
    Text text = {
        NULL,
        (SDL_Rect) {0, 0, 0, 0}
    };

    SDL_Surface *textSurface = TTF_RenderText_Blended(game->font, input, color);
    if (!textSurface) {
        printf("Erro ao criar superfície de texto: %s\n", TTF_GetError());
        return text;
    }

    // Definir o retângulo para renderizar o texto.
    text.dst = (SDL_Rect) {
        x, y, textSurface->w, textSurface->h
    };

    // Criar a textura a partir da superfície.
    text.texture = SDL_CreateTextureFromSurface(game->renderer, textSurface);
    // Liberar a superfície após criar a textura.
    SDL_FreeSurface(textSurface);
    return text;
}

// Função responsável por criar o menu principal do jogo.
static _Bool createMainMenu(Game *game) {
    // Aloca memória para a estrutura do menu principal.
    game->mainMenu = malloc(sizeof(MainMenu));
    if (!game->mainMenu) {
        fprintf(stderr, "Falha ao alocar mémoria para o menu principal.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura da jogador.
    memset(game->mainMenu, 0, sizeof(MainMenu));

    // Carrega a textura do menu principal e caso ocorra erro, exibe uma mensagem.
    game->mainMenu->texture = IMG_LoadTexture(game->renderer, "./img/mainMenu.png");
    if (!game->mainMenu->texture) {
        fprintf(stderr, "Falha ao carregar textura do menu principal: %s\n", IMG_GetError());
        return false;
    }

    // Define a textura dos botões e o posicionamento na tela.
    const int buttonWidth = 5 * 32;
    const int buttonHeight = 2 * 32;
    // Botão 'start'.
    game->mainMenu->src[0] = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    game->mainMenu->dst[0] = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, (SCREEN_HEIGHT - buttonHeight) / 2 - 48, buttonWidth, buttonHeight
    };
    // Botão 'settings'.
    game->mainMenu->src[1] = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    game->mainMenu->dst[1] = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, (SCREEN_HEIGHT - buttonHeight) / 2, buttonWidth, buttonHeight
    };
    // Botão 'exit'.
    game->mainMenu->src[2] = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    game->mainMenu->dst[2] = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, (SCREEN_HEIGHT - buttonHeight) / 2 + 48, buttonWidth, buttonHeight
    };

    // Textos para o menu principal.
    game->mainMenu->text[0] = createText(game, "Start", 0, 0, BLACK);
    game->mainMenu->text[1] = createText(game, "Settings", 0, 0, BLACK);
    game->mainMenu->text[2] = createText(game, "Exit", 0, 0, BLACK);
    if (!game->mainMenu->text[0].texture || !game->mainMenu->text[1].texture || !game->mainMenu->text[2].texture) {
        return false;
    }
    // Posiciona os textos no centro das texturas dos botões.
    game->mainMenu->text[0].dst.x = (SCREEN_WIDTH - game->mainMenu->text[0].dst.w) / 2;
    game->mainMenu->text[0].dst.y = (SCREEN_HEIGHT - game->mainMenu->text[0].dst.h) / 2 - 48;
    game->mainMenu->text[1].dst.x = (SCREEN_WIDTH - game->mainMenu->text[1].dst.w) / 2;
    game->mainMenu->text[1].dst.y = (SCREEN_HEIGHT - game->mainMenu->text[1].dst.h) / 2;
    game->mainMenu->text[2].dst.x = (SCREEN_WIDTH - game->mainMenu->text[2].dst.w) / 2;
    game->mainMenu->text[2].dst.y = (SCREEN_HEIGHT - game->mainMenu->text[2].dst.h) / 2 + 48;
    return true;
}

// Função responsável por criar o jogador.
static _Bool createPlayer(Game *game) {
    // Aloca memória para a estrutura de dados do jogador.
    game->player = malloc(sizeof(Entity));
    if (!game->player) {
        fprintf(stderr, "Falha ao alocar mémoria para o jogador.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura da jogador.
    memset(game->player, 0, sizeof(Entity));

    // Carrega a textura do jogador e caso ocorra erro, exibe uma mensagem.
    game->player->texture = IMG_LoadTexture(game->renderer, "./img/player.png");
    if (!game->player->texture) {
        fprintf(stderr, "Falha ao carregar textura do jogador: %s\n", IMG_GetError());
        return false;
    }

    // Define o retângulo de origem da textura do jogador.
    game->player->src = (SDL_Rect) {
        0, 0, 32, 32
    };
    // Define a posição e o tamanho do jogador na tela.
    game->player->dst = (SDL_Rect) {
        (SCREEN_WIDTH - PLAYER_WIDTH) / 2, (SCREEN_HEIGHT - PLAYER_HEIGHT) / 2, PLAYER_WIDTH, PLAYER_HEIGHT
    };
    return true;
}

// Define o tipo de bloco.
static void setBlockType(Game *game, const int i, const int j) {
    // Lê o tipo de bloco da tabela lua.
    game->dungeon->block[i][j].type = (int)lua_tonumber(game->L, -1);
    // Define as propriedades de cada bloco.
    switch (game->dungeon->block[i][j].type) {
    case AIR:
        game->dungeon->block[i][j].isSolid = false;
        break;
    case WOOD:
        game->dungeon->block[i][j].src = (SDL_Rect) { 
            0, 0, 32, 32 
        };
        game->dungeon->block[i][j].isSolid = false;
        break;
    case STONE:
        game->dungeon->block[i][j].src = (SDL_Rect) { 
            32, 0, 32, 32 
        };
        game->dungeon->block[i][j].isSolid = true;
        break;
    }
}

// Função responsável por criar uma masmorra.
_Bool createDungeon(Game *game) {
    // Aloca memória para a estrutura de dados da masmorra.
    game->dungeon = malloc(sizeof(Dungeon));
    if (!game->dungeon) {
        fprintf(stderr, "Falha ao alocar memória para a masmorra.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura da masmorra.
    memset(game->dungeon, 0, sizeof(Dungeon));

    // Carrega a textura da masmorra.
    game->dungeon->texture = IMG_LoadTexture(game->renderer, "./img/dungeon.png");
    if (!game->dungeon->texture) {
        fprintf(stderr, "Falha ao carregar texturas da masmorra: %s\n", IMG_GetError());
        return false;
    }

    // Acessa a tabela principal.
    lua_getglobal(game->L, "dungeon");
    if (!lua_istable(game->L, -1)) {
        fprintf(stderr, "Erro ao acessar a tabela 'dungeon': %s\n", lua_tostring(game->L, -1));
        lua_pop(game->L, 1);
        return false;
    }

    // Acessa a sala específica usando o nome fornecido.
    const char *roomName = "room_3";
    lua_getfield(game->L, -1, roomName);
    if (!lua_istable(game->L, -1)) {
        fprintf(stderr, "Erro ao acessar a sala '%s': %s\n", roomName, lua_tostring(game->L, -1));
         // Remove a tabela principal e o erro da pilha.
        lua_pop(game->L, 2);
        return false;
    }

    // Preenche a masmorra com blocos a partir dos dados da tabela lua.
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            // Calcula o índice na tabela.
            lua_pushnumber(game->L, i * DUNGEON_HEIGHT + j + 1);
            lua_gettable(game->L, -2);

            // Verifica se o valor é um número.
            if (lua_isnumber(game->L, -1)) {
                // Define as propriedades de cada bloco.
                setBlockType(game, i, j);
            } else {
                // Em caso de erro na leitura da tabela define o bloco como ar.
                game->dungeon->block[i][j].type = AIR;
                game->dungeon->block[i][j].isSolid = false;
            }
            // Remove o valor da pilha.
            lua_pop(game->L, 1);

            // COnfigura a posição dos blocos.
            game->dungeon->block[i][j].dst = (SDL_Rect) { 
                i * BLOCK_WIDTH, j * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT 
            };
        }
    }

    // Lê o nível da masmorra.
    lua_getfield(game->L, -1, "level");
    if (lua_isnumber(game->L, -1)) {
        game->dungeon->level = (int)lua_tonumber(game->L, -1);
    } else {
        game->dungeon->level = 1;
    }
    // Remove a referência à sala.
    lua_pop(game->L, 1);
    return true;
}

// Função responsável por criar o inventário do jogador.
static _Bool createInventory(Game *game) {
    // Aloca memória para a estrutura de dados do inventário.
    game->inventory = malloc(sizeof(Dungeon));
    if (!game->inventory) {
        fprintf(stderr, "Falha ao alocar memória para o inventário.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura do inventário.
    memset(game->inventory, 0, sizeof(Inventory));

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        game->inventory->src[i] = (SDL_Rect) {
            0, 0, 32, 32
        };
        game->inventory->dst[i] = (SDL_Rect) {
            i * 32 + (SCREEN_WIDTH - 320) / 2, SCREEN_HEIGHT - 32, 32, 32
        };
    }

    // Carrega a textura do inventário e caso ocorra erro, exibe uma mensagem.
    game->inventory->texture = IMG_LoadTexture(game->renderer, "./img/inventory.png");
    if (!game->inventory->texture) {
        fprintf(stderr, "Falha ao carregar textura do inventário: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

// Função responsável por inicializar todos os recursos necessários para o jogo.
_Bool init(Game *game) {
    // Inicializa um estado lua.
    game->L = luaL_newstate();
    if (!game->L) {
        fprintf(stderr, "Falha ao criar estado lua.\n");
        return false;
    }
    // Abre as bibliotecas do lua.
    luaL_openlibs(game->L);
    // Abre/lê arquivo de script lua e caso ocorra erro, exibe uma mensagem.
    if (luaL_dofile(game->L, "./scripts/dungeon.lua") != LUA_OK) {
        fprintf(stderr, "Falha ao abrir/ler arquivo de script lua: %s\n", lua_tostring(game->L, -1));
        return false;
    }

    // Inicializa a biblioteca para manipulação de vídeo.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Falha ao iniciar SDL: %s.\n", SDL_GetError());
        return false;
    }
    // Inicializa a biblioteca para carregar imagens.
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        fprintf(stderr, "Falha ao iniciar SDL_image: %s.\n", IMG_GetError());
        SDL_Quit();
        return false;
    }
    // Inicializa a biblioteca para carregar fontes.
    if (TTF_Init() < 0) {
        fprintf(stderr, "Falha ao iniciar SDL_ttf: %s.\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    // Inicializa a biblioteca para manipulação de áudio.
    if (Mix_Init(MIX_INIT_MP3) < 0) {
        fprintf(stderr, "Falha ao inicializar SDL_mixer: %s.\n", Mix_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Criação da janela do jogo e do renderizador.
    if (!createWindow(game) || !createRenderer(game)) {
        finish(game);
        return false;
    }

    // Abre e carregar a fonte ttf.
    game->font = TTF_OpenFont("./font/04B_03__.TTF", FONT_SIZE);
    if (!game->font) {
        fprintf(stderr, "Falha ao carregar a fonte: %s\n", TTF_GetError());
        finish(game);
        return false;
    }

    // Criação dos objetos do jogo.
    if (!createMainMenu(game) || !createPlayer(game) || !createDungeon(game) || !createInventory(game)) {
        finish(game);
        return false;
    }

    // Define a posição inicial da câmera.
    game->camera.position = (SDL_Point) {
        0, 0
    };
    // Define a velocidade inicial.
    game->camera.speed = 5;

    // Configura o estado do jogo para o menu principal.
    game->state = ACTIVE;
    return true;
}

#endif // INIT_H