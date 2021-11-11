#include "FilterShift.h"
#include "FilterUtils.h"

FilterShift::FilterShift(ShiftDirection direction):
    m_shiftDir(direction),
    m_kernel(std::vector<float>(9, 0.f))
{
    // TODO: Task 19 Initialize kernel for shift
    switch (m_shiftDir) {
    case ShiftDirection::SHIFT_LEFT:
        *(m_kernel.begin() + m_kernel.size() / 2 + 1) = 1.f;
        break;
    case ShiftDirection::SHIFT_RIGHT:
        *(m_kernel.begin() + m_kernel.size() / 2 - 1) = 1.f;
        break;
    }
}

FilterShift::~FilterShift()
{
}

void FilterShift::apply(Canvas2D *canvas) {
    // TODO: Task 18 Call convolve function from superclass
    FilterUtils::Convolve2D(canvas->data(), canvas->width(), canvas->height(), m_kernel);

}
