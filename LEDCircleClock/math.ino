#define POWER_CACHE_SIZE 200
#define POSSIBLE_POWERS RINGS

class PowerEntry {
  public:
    int exponent;
    double results[POWER_CACHE_SIZE];
};

PowerEntry powers[POSSIBLE_POWERS];
int powerIndex = 0;

double fastPow(double base, int exponent) {
  if (base == 1.0) return 1.0;

  if (0 <= base && base < 1.0) {
    int intBase = base * POWER_CACHE_SIZE;
    for (int i = 0; i < powerIndex; i++) {
      if (powers[i].exponent == exponent) {
        return powers[i].results[intBase];
      }
    }

    // fill the cache for this power
    powers[powerIndex].exponent = exponent;
    for (int intBase = 0; intBase < POWER_CACHE_SIZE; intBase++) {
      double doubleBase = (double)intBase / POWER_CACHE_SIZE;
      double result = pow(doubleBase, (double)exponent);
      powers[powerIndex].results[intBase] = result;
    }
    powerIndex++;
  }
  return pow(base, (double)exponent);
}
