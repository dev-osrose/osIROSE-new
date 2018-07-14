// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file random
 * \brief A simple random singleton class to be able to generate random numbers anywhere
 *
 * \author L3nn0x
 * \date march 2016
 */

#pragma once

#include <random>
#include <tuple>

#include "singleton.h"

namespace Core {

class Random : public Singleton<Random> {
    public:
        int get_uniform(int min, int max) {
            std::uniform_int_distribution<> uniform{min, max};
            return uniform(engine_);
        }

        double get_uniform(double min, double max) {
            std::uniform_real_distribution<> uniform{min, max};
            return uniform(engine_);
        }

        double get_normal(double mean, double deviation) {
            std::normal_distribution<> normal{mean, deviation};
            return normal(engine_);
        }

        std::tuple<double, double> random_in_unit_circle(double x, double y) {
            return {get_uniform(0.f, 1.f), get_uniform(0.f, 1.f)};
        }

        std::tuple<double, double> random_in_circle(double x, double y, double radius) {
            return random_in_unit_circle(x, y) * radius;
        }

        std::tuple<int, int> random_in_circle(int x, int y, int radius) {
            auto unit = random_in_unit_circle(x, y);
            return {std::floor(std::get<0>(unit) * radius),
                    std::floor(std::get<1>(unit) * radius)};
        }

    protected:
        virtual ~Random() = default;
        Random() : engine_(std::random_device()()) {}

    private:
        std::default_random_engine engine_;
};

}
