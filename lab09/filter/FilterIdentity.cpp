#include "FilterIdentity.h"
#include "FilterUtils.h"

FilterIdentity::FilterIdentity() :
    m_kernel(std::vector<float>(9, 0.f))
{
    // TODO: Task 17 Initialize kernel for identity
    int kerSize = m_kernel.size();
    m_kernel[kerSize / 2] = 1.f;
}

FilterIdentity::~FilterIdentity()
{
}

void FilterIdentity::apply(Canvas2D *canvas) {
    // TODO: Task 16 Call convolve function from superclass
    FilterUtils::Convolve2D(canvas->data(), canvas->width(), canvas->height(), m_kernel);

}
