#include "SHealth.h"
#include "SHealthPresenter.h"

int main() {
    SHealth shealth;
    shealth.calculateBmi("shealth.dat");

    SHealthPresenter::printAgeBandReport(shealth);
    SHealthPresenter::printNormalBmiUsers(shealth);
    SHealthPresenter::printGlobalBmiRatios(shealth);

    return 0;
}
