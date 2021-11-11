#include "FilterUtils.h"
#include <algorithm>
#include <numeric>
#include "math.h"
#include <cstring>

namespace FilterUtils {

inline unsigned char REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

void Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel) {
    // TODO: Task 9 Create buffer to store new image data
    std::vector<RGBA> result;
    int imgSize = width * height;
    result.reserve(imgSize);


    // TODO: Task 10 Obtain kernel dimension
    int kerLen = std::sqrt(kernel.size());
    int kerLenHalf = kerLen / 2;
    size_t centerIndex = 0;
    int kerSum = std::accumulate(kernel.cbegin(), kernel.cend(), 0);
    float kerSumInv = 1.f / kerSum;

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++, centerIndex++) {

            // TODO: Task 11 Initialize color to accumulate convolution data
            float red, green, blue;
            red = green = blue = 0.f;

            // TODO: Task 12
            // Iterate over the kernel using the value from task 10
            // Obtain the value at current index of kernel
            // Find correct index in original image data
            // Accumulate the kernel applied to pixel value in color_acc
            size_t kerId = 0;
            for (int kr = -kerLenHalf; kr <= kerLenHalf; kr++) {
                for (int kc = -kerLenHalf; kc <= kerLenHalf; kc++, kerId++) {
                    int currR = r + kr;
                    int currC = c + kc;
                    if ((currR < 0) || (currR >= height) ||
                            (currC < 0 || (currC) >= width)) {
                        // TODO: Task 13 Ignore outside bounds
                        continue;
                    }
                    int currId = width * currR + currC;
                    red += data[currId].r * kernel[kerId];
                    green += data[currId].g * kernel[kerId];
                    blue += data[currId].b * kernel[kerId];
                }
            }
            red *= kerSumInv;
            green *= kerSumInv;
            blue *= kerSumInv;
            // TODO: Task 14 Update buffer with accumulated color
            result.emplace_back(static_cast<unsigned char>(red + 0.5f),
                                static_cast<unsigned char>(green + 0.5f),
                                static_cast<unsigned char>(blue + 0.5f));
        }
    }

    // TODO: Task 15 Copy over buffer to canvas data
    std::memcpy(data, result.data(), sizeof (RGBA) * imgSize);

}

}
