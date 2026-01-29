#include "GoldmannSizes.h"

MeteoroidSizeID getSizeByNumber(int number) {
    if (number == 0) {
        return MeteoroidSizeID::None;
    } else if (number == 1) {
        return MeteoroidSizeID::I;
    } else if (number == 2) {
        return MeteoroidSizeID::II;
    } else if (number == 3) {
        return MeteoroidSizeID::III;
    } else if (number == 4) {
        return MeteoroidSizeID::IV;
    } else if (number == 5) {
        return MeteoroidSizeID::V;
    } else {
        return MeteoroidSizeID::None;
    }
}
