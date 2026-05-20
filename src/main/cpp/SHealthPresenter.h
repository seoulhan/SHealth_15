#pragma once

class SHealth;

namespace SHealthPresenter {

void printAgeBandReport(const SHealth& shealth);
void printNormalBmiUsers(const SHealth& shealth);
void printGlobalBmiRatios(const SHealth& shealth);

}  // namespace SHealthPresenter
