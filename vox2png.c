/*
 * Vox2Png
 * Converts a MagicaVoxel Vox file into layered sprites and writes them to a Png file
 *
 * Written by Stijn Brouwer
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#include "stdlib.h"
#include "stdio.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/* A single voxel in MagicaVoxel's XYZI format */
typedef struct {
    unsigned char x, y, z;
    unsigned char colorIndex;
} voxel;

/* An RGBA color */
typedef struct {
    unsigned char r, g, b, a;
} color;

/* MagicaVoxel's default palette encoded as 256 RGBA colors */
const unsigned char defaultPal[256 * 4] = {
    "\377\377\377\377\377\377\314\377\377\377\231\377\377\377f\377\377\377\063"
    "\377\377\377\000\377\377\314\377\377\377\314\314\377\377\314\231\377\377\314"
    "f\377\377\314\063\377\377\314\000\377\377\231\377\377\377\231\314\377\377\231"
    "\231\377\377\231f\377\377\231\063\377\377\231\000\377\377f\377\377\377f\314"
    "\377\377f\231\377\377ff\377\377f\063\377\377f\000\377\377\063\377\377\377\063"
    "\314\377\377\063\231\377\377\063f\377\377\063\063\377\377\063\000\377\377\000\377"
    "\377\377\000\314\377\377\000\231\377\377\000f\377\377\000\063\377\377\000\000\377\314"
    "\377\377\377\314\377\314\377\314\377\231\377\314\377f\377\314\377\063\377"
    "\314\377\000\377\314\314\377\377\314\314\314\377\314\314\231\377\314\314f\377"
    "\314\314\063\377\314\314\000\377\314\231\377\377\314\231\314\377\314\231\231"
    "\377\314\231f\377\314\231\063\377\314\231\000\377\314f\377\377\314f\314\377"
    "\314f\231\377\314ff\377\314f\063\377\314f\000\377\314\063\377\377\314\063\314"
    "\377\314\063\231\377\314\063f\377\314\063\063\377\314\063\000\377\314\000\377\377"
    "\314\000\314\377\314\000\231\377\314\000f\377\314\000\063\377\314\000\000\377\231\377"
    "\377\377\231\377\314\377\231\377\231\377\231\377f\377\231\377\063\377\231"
    "\377\000\377\231\314\377\377\231\314\314\377\231\314\231\377\231\314f\377\231"
    "\314\063\377\231\314\000\377\231\231\377\377\231\231\314\377\231\231\231\377"
    "\231\231f\377\231\231\063\377\231\231\000\377\231f\377\377\231f\314\377\231"
    "f\231\377\231ff\377\231f\063\377\231f\000\377\231\063\377\377\231\063\314\377"
    "\231\063\231\377\231\063f\377\231\063\063\377\231\063\000\377\231\000\377\377\231"
    "\000\314\377\231\000\231\377\231\000f\377\231\000\063\377\231\000\000\377f\377\377\377"
    "f\377\314\377f\377\231\377f\377f\377f\377\063\377f\377\000\377f\314\377\377"
    "f\314\314\377f\314\231\377f\314f\377f\314\063\377f\314\000\377f\231\377\377"
    "f\231\314\377f\231\231\377f\231f\377f\231\063\377f\231\000\377ff\377\377ff\314"
    "\377ff\231\377fff\377ff\063\377ff\000\377f\063\377\377f\063\314\377f\063\231\377"
    "f\063f\377f\063\063\377f\063\000\377f\000\377\377f\000\314\377f\000\231\377f\000f\377f"
    "\000\063\377f\000\000\377\063\377\377\377\063\377\314\377\063\377\231\377\063\377f\377"
    "\063\377\063\377\063\377\000\377\063\314\377\377\063\314\314\377\063\314\231\377"
    "\063\314f\377\063\314\063\377\063\314\000\377\063\231\377\377\063\231\314\377\063"
    "\231\231\377\063\231f\377\063\231\063\377\063\231\000\377\063f\377\377\063f\314\377"
    "\063f\231\377\063ff\377\063f\063\377\063f\000\377\063\063\377\377\063\063\314\377\063"
    "\063\231\377\063\063f\377\063\063\063\377\063\063\000\377\063\000\377\377\063\000\314\377"
    "\063\000\231\377\063\000f\377\063\000\063\377\063\000\000\377\000\377\377\377\000\377\314\377"
    "\000\377\231\377\000\377f\377\000\377\063\377\000\377\000\377\000\314\377\377\000\314\314"
    "\377\000\314\231\377\000\314f\377\000\314\063\377\000\314\000\377\000\231\377\377\000\231"
    "\314\377\000\231\231\377\000\231f\377\000\231\063\377\000\231\000\377\000f\377\377\000f"
    "\314\377\000f\231\377\000ff\377\000f\063\377\000f\000\377\000\063\377\377\000\063\314\377"
    "\000\063\231\377\000\063f\377\000\063\063\377\000\063\000\377\000\000\377\377\000\000\314\377\000"
    "\000\231\377\000\000f\377\000\000\063\377\356\000\000\377\335\000\000\377\273\000\000\377\252\000"
    "\000\377\210\000\000\377w\000\000\377U\000\000\377D\000\000\377\"\000\000\377\021\000\000\377\000\356"
    "\000\377\000\335\000\377\000\273\000\377\000\252\000\377\000\210\000\377\000w\000\377\000U\000\377"
    "\000D\000\377\000\"\000\377\000\021\000\377\000\000\356\377\000\000\335\377\000\000\273\377\000\000\252"
    "\377\000\000\210\377\000\000w\377\000\000U\377\000\000D\377\000\000\"\377\000\000\021\377\356\356"
    "\356\377\335\335\335\377\273\273\273\377\252\252\252\377\210\210\210\377"
    "www\377UUU\377DDD\377\"\"\"\377\021\021\021\377\000\000\000\377",
};

/* I'm just putting allocated resources here so we can just free them easily */

/* File handle to the .vox file */
FILE *voxHandle = NULL;
/* A buffer containing the content of the .vox file */
char *voxBuffer = NULL;
/* A buffer with the color data we're going to write to the .png file */
color *pngData = NULL;

/* Called on error */
void die(const char *msg) {
    fprintf(stderr, "[fatal] %s\n", msg);
    if (voxHandle != NULL) fclose(voxHandle);
    if (voxBuffer != NULL) free(voxBuffer);
    if (pngData != NULL) free(pngData);
    exit(-1);
}

int main(int argc, char **argv) {
    if (argc != 3) die("Wrong number of arguments");
    char *voxPath = argv[1];
    char *pngPath = argv[2];

    /* Read the .vox file into memory */

    FILE *voxHandle = fopen(voxPath, "rb");
    if (!voxHandle) die("Couldn't open .vox file, are you sure it exists?");

    fseek(voxHandle, 0, SEEK_END);
    size_t voxLength = ftell(voxHandle);
    fseek(voxHandle, 0, SEEK_SET);
    voxBuffer = malloc(voxLength + 1);
    if (!voxBuffer) die("Couldn't allocate the .vox file buffer");

    fread(voxBuffer, voxLength, 1, voxHandle);
    fclose(voxHandle);
    voxHandle = NULL;

    if (voxLength < 8 ||
        voxBuffer[0] != 'V' || voxBuffer[1] != 'O' ||
        voxBuffer[2] != 'X' || voxBuffer[3] != ' ') {
        die("The specified file is either corrupted, or not a .vox file");
    }

    printf("Loaded the .vox file into memory\n");

    /* Read the SIZE chunk to get the dimensions of the voxel object */

    int voxXDim, voxYDim, voxZDim;
    for (int i = 0; i < voxLength - 4; ++i) {
        if (voxBuffer[i + 0] == 'S' && voxBuffer[i + 1] == 'I' &&
            voxBuffer[i + 2] == 'Z' && voxBuffer[i + 3] == 'E') {
            int *xDimPtr = (int *) &voxBuffer[i + 12 + 4 * 0];
            int *yDimPtr = (int *) &voxBuffer[i + 12 + 4 * 1];
            int *zDimPtr = (int *) &voxBuffer[i + 12 + 4 * 2];
            voxXDim = *xDimPtr;
            voxYDim = *yDimPtr;
            voxZDim = *zDimPtr;
            goto sizeFound;
        }
    }
    die("The .vox file is missing a SIZE chunk");

sizeFound:
    printf("Found size chunk: [%i, %i, %i]\n", voxXDim, voxYDim, voxZDim);

    /* Read the XYZI chunk to get the voxel data */

    int voxCount;
    const voxel *voxVoxels = NULL;
    for (int i = 0; i < voxLength - 4; ++i) {
        if (voxBuffer[i + 0] == 'X' && voxBuffer[i + 1] == 'Y' &&
            voxBuffer[i + 2] == 'Z' && voxBuffer[i + 3] == 'I') {
            int *countPtr = (int *) &voxBuffer[i + 12];
            void *dataPtr = &voxBuffer[i + 12 + 4];
            voxVoxels = (voxel *) dataPtr;
            voxCount = *countPtr;
            goto voxelsFound;
        }
    }
    die("The .vox file is missing a XYZI chunk");

voxelsFound:
    printf("Found voxel chunk: %i voxels\n", voxCount);

    /* If there is a RGBA palette chunk, load that too */
    /* Otherwise just use the default one */

    const color *voxPal = NULL;
    for (int i = 0; i < voxLength - 4; ++i) {
        if (voxBuffer[i + 0] == 'R' && voxBuffer[i + 1] == 'G' &&
            voxBuffer[i + 2] == 'B' && voxBuffer[i + 3] == 'A') {
            voxPal = (color *) &voxBuffer[i + 12];
            printf("Found palette chunk\n");
            break;
        }
    }
    if (!voxPal) {
        printf("No palette chunk found, using the default palette\n");
        voxPal = (color *) defaultPal;
    }

    /* Create the Png image data and write our voxels to it */

    int pngWidth = voxXDim * voxZDim;
    int pngHeight = voxYDim;
    pngData = calloc(pngWidth * pngHeight, sizeof(color));
    if (!pngData) die("Couldn't allocate a buffer for the color data");
    for (int i = 0; i < voxCount; ++i) {
        voxel currentVoxel = voxVoxels[i];
        color currentColor = voxPal[currentVoxel.colorIndex];
        int dataIndex = currentVoxel.x + voxXDim * currentVoxel.z +
                        currentVoxel.y * (voxXDim * voxZDim);
        pngData[dataIndex] = currentColor;
    }
    printf("Voxels have been loaded into memory\n");

    /* Write the image data to the Png */

    if (!stbi_write_png(pngPath, pngWidth, pngHeight, 4, pngData, pngWidth * 4)) {
        die("Failed to write Png image");
    }
    printf("Wrote Png\n");
    printf("The cells are [%i, %i], and there are %i of them\n", voxXDim, voxYDim, voxZDim);

    /* Clean up */

    free(pngData);
    free(voxBuffer);
    voxBuffer = NULL;
}

