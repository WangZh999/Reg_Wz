#pragma once
#ifndef TRAIN_H
#define TRAIN_H

#include<opencv.hpp>

#define CLASS_NUM 10


int train(std::string dataPath);

float test(std::string dataPath);

#endif // !TRAIN_H
