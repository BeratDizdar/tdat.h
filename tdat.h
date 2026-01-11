/*
  tdat.h - Single Header - RGBA Texture Data Format Library
  
  Copyright (c) 2026 BeratDizdar

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef TDAT_H
#define TDAT_H
#include <stdint.h>
 
typedef struct {
    uint32_t width, height;
    uint32_t* data;
} tdat;
tdat CreateTDAT(uint32_t w, uint32_t h);
tdat LoadTDAT(const char* name);
void SaveTDAT(tdat* self, const char* name);
void FreeTDAT(tdat* self);

#endif
#ifdef TDAT_IMPL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tdat CreateTDAT(uint32_t w, uint32_t h) {
    return (tdat){w, h, malloc(w*h*sizeof(uint32_t))};
}

tdat LoadTDAT(const char* name) {
    tdat g = {0, 0, NULL};
    FILE *f = fopen(name, "rb"); if (!f) return g;
    char magic[16]; uint32_t w, h;
    if (fscanf(f, "%s %u %u", magic, &w, &h) != 3) goto _load_end;
    if (strcmp(magic, "TDAT") == 0) {
        fgetc(f);
        g = CreateTDAT(w, h);
        if (g.data) fread(g.data, sizeof(uint32_t), w*h, f);
    }

 _load_end:
    fclose(f);
    return g;
}

void SaveTDAT(tdat* self, const char* name) {
    FILE* f = fopen(name, "wb"); if (!f) return;
    fprintf(f, "TDAT %u %u\n", self->width, self->height);
    fwrite(self->data, 4, self->width * self->height, f);
    fclose(f);
}

void FreeTDAT(tdat* self) {
    if (!self) return;
    if (self->data) free(self->data);
	self->data = NULL;
}

#endif
