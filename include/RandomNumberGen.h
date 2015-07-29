
#ifndef RANDOMNUMBERGEN
#define RANDOMNUMBERGEN

#include <random>
#include <time.h>


namespace PSTD {
	namespace PMath {

		class RandomNumberGen {
			public:
				RandomNumberGen(unsigned int seed = 0) {
					if (seed == 0) {
						_Engine.seed((unsigned int)time(0));
					}
					else {
						_Engine.seed(seed);
					}
				};

				~RandomNumberGen(void) {};

				void Seed_RNG(unsigned int newSeed) {
					_Engine.seed(newSeed);
				};

				// get an unsigned int from [0, max)
				unsigned int Get(unsigned int max) {
					return _Engine() % max;
				};

				// get an unsigned int from [0, max)
				unsigned int Get(size_t max) {
					if (max == 0) return 0;
					return _Engine() % max;
				};

				// get an int from [0, max)
				int Get(int max) {
					if (max <= 0) return 0;
					return _Engine() % max;
				};

				// get an int from [0, max]
				int Get_Inclusive(int max) {
					if (max <= 0) return 0;
					return _Engine() % (max + 1);
				};



				int Get(int min, int max) {
					std::uniform_int_distribution<> dist(min, max);
					return dist(_Engine);
				};

				float Get(float min, float max) {
					std::uniform_real_distribution<> dist(min, max);
					return (float)dist(_Engine);
				}



			//private:
				std::mt19937 _Engine;

		};
	}
}


#endif