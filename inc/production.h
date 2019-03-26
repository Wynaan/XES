#ifndef PRODUCTION_H_
#define PRODUCTION_H_
#include <stdint.h>
#include <cstring>
#include "xav_data.h"

const uint8_t tier1Production[10] = {4, 12, 24, 41, 62, 87, 116, 150, 187, 229};
const uint8_t tier2Production[10] = {3, 10, 21, 35, 52, 73, 98, 129, 160, 199};
const uint8_t tier3Production[10] = {3, 9, 19, 32, 48, 67, 91, 120, 149, 186};
const uint8_t tier4Production[10] = {2, 7, 17, 28, 42, 60, 82, 107, 133, 166};

const resources_t sawmillBaseCost = {.food = 100, .wood = 100, .stone = 250, .iron = 300, .gold = 0};
const resources_t farmBaseCost = {.food = 50, .wood = 300, .stone = 200, .iron = 150, .gold = 0};
const resources_t quarryBaseCost = {.food = 180, .wood = 500, .stone = 150, .iron = 400, .gold = 0};
const resources_t ironMineBaseCost = {.food = 210, .wood = 600, .stone = 500, .iron = 200, .gold = 0};
const resources_t goldMineBaseCost = {.food = 250, .wood = 750, .stone = 625, .iron = 240, .gold = 0};


#endif /* PRODUCTION_H_ */
