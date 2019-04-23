
#include "maxprotein.hh"
#include "timer.hh"
#include <string>

using namespace std;

int main() {
		//Change n for testing
		int n = 30;
		auto all_foods = load_usda_abbrev("ABBREV.txt");
		auto filtered_foods = filter_food_vector(*all_foods, 1, 2000, n);
		Timer timer;
		
		//Comment one function at a time to test

		//greedy_max_protein(*filtered_foods, 2000);
		exhaustive_max_protein(*filtered_foods, 2000);
		double elapsed = timer.elapsed();
		cout << "elapsed time = " << elapsed << " seconds" << endl;
		cout << "The n is " << n << endl;

		return 0;

}
