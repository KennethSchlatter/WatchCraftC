#pragma once

typedef struct Recipe {
    char title[1][14];
    char name[1][20];
    char recipe_url[1][128];
    uint8_t type;
} Recipe;