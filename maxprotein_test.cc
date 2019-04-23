///////////////////////////////////////////////////////////////////////////////
// maxprotein_test.cc
//
// Unit tests for maxprotein.hh
//
///////////////////////////////////////////////////////////////////////////////


#include <cassert>
#include <sstream>

#include "maxprotein.hh"
#include "rubrictest.hh"

int main() {
  Rubric rubric;

  FoodVector trivial_foods;
  trivial_foods.push_back(std::shared_ptr<Food>(new Food("banana", "1 each", 20, 100, 1)));
  trivial_foods.push_back(std::shared_ptr<Food>(new Food("hotdog", "1 piece", 25, 150, 5)));
  
  auto all_foods = load_usda_abbrev("ABBREV.txt");
  assert( all_foods );

  auto filtered_foods = filter_food_vector(*all_foods, 1, 2500, all_foods->size());
  
  rubric.criterion("load_usda_abbrev still works", 2,
		   [&]() {
		     TEST_TRUE("non-null", all_foods);
		     TEST_EQUAL("size", 8490, all_foods->size());
		   });
  
  rubric.criterion("filter_food_vector", 2,
		   [&]() {
		     auto three = filter_food_vector(*all_foods, 1, 2000, 3),
		       ten = filter_food_vector(*all_foods, 1, 2000, 10);
		     TEST_TRUE("non-null", three);
		     TEST_TRUE("non-null", ten);
		     TEST_EQUAL("total_size", 3, three->size());
		     TEST_EQUAL("total_size", 10, ten->size());
		     TEST_EQUAL("contents", "BUTTER,WITH SALT", (*ten)[0]->description());
		     TEST_EQUAL("contents", "CHEESE,CHESHIRE", (*ten)[9]->description());
		     for (int i = 0; i < 3; i++) {
		       TEST_EQUAL("contents", (*three)[i]->description(), (*ten)[i]->description());
		     }
		   });

  rubric.criterion("greedy_max_protein trivial cases", 2,
		   [&]() {
		     auto soln = greedy_max_protein(trivial_foods, 99);
		     TEST_TRUE("non-null", soln);
		     TEST_TRUE("empty solution", soln->empty());

		     soln = greedy_max_protein(trivial_foods, 100);
		     TEST_TRUE("non-null", soln);
		     TEST_EQUAL("banana only", 1, soln->size());
		     TEST_EQUAL("banana only", "banana", (*soln)[0]->description());

		     soln = greedy_max_protein(trivial_foods, 150);
		     TEST_TRUE("non-null", soln);
		     TEST_EQUAL("hotdog only", 1, soln->size());
		     TEST_EQUAL("hotdog only", "hotdog", (*soln)[0]->description());

		     soln = greedy_max_protein(trivial_foods, 250);
		     TEST_TRUE("non-null", soln);
		     TEST_EQUAL("hotdog and banana", 2, soln->size());
		   });
  
  rubric.criterion("greedy_max_protein correctness", 4,
		   [&]() {
		     auto soln2000 = greedy_max_protein(*filtered_foods, 2000),
		       soln2500 = greedy_max_protein(*filtered_foods, 2500);

		     TEST_TRUE("non-null", soln2000);
		     TEST_TRUE("non-null", soln2500);

		     TEST_FALSE("non-empty", soln2000->empty());
		     TEST_FALSE("non-empty", soln2500->empty());

		     int kcal2000, protein2000, kcal2500, protein2500;
		     sum_food_vector(kcal2000, protein2000, *soln2000);
		     sum_food_vector(kcal2500, protein2500, *soln2500);
		     TEST_EQUAL("2000 kcal solution", 476, protein2000);
		     TEST_EQUAL("2500 kcal solution", 595, protein2500);
		   });

  rubric.criterion("exhaustive_max_protein trivial cases", 2,
		   [&]() {
		     auto soln = exhaustive_max_protein(trivial_foods, 99);
		     TEST_TRUE("non-null", soln);
		     TEST_TRUE("empty solution", soln->empty());

		     soln = exhaustive_max_protein(trivial_foods, 100);
		     TEST_TRUE("non-null", soln);
		     TEST_EQUAL("banana only", 1, soln->size());
		     TEST_EQUAL("banana only", "banana", (*soln)[0]->description());

		     soln = exhaustive_max_protein(trivial_foods, 150);
		     TEST_TRUE("non-null", soln);
		     TEST_EQUAL("hotdog only", 1, soln->size());
		     TEST_EQUAL("hotdog only", "hotdog", (*soln)[0]->description());

		     soln = exhaustive_max_protein(trivial_foods, 250);
		     TEST_TRUE("non-null", soln);
		     TEST_EQUAL("hotdog and banana", 2, soln->size());
		   });
  
  rubric.criterion("exhaustive_max_protein correctness", 4,
		   [&]() {

		     std::vector<int> optimal_protein_totals = {
		       1, 1, 22, 45, 66, 85, 110, 113, 115, 118, 127, 135, 136,
		       141, 149, 149, 151,
		     };
		     
		     for (int n = 2; n <= 18; n++) {
		       int expected_protein = optimal_protein_totals[n-2];
		       auto small_foods = filter_food_vector(*filtered_foods, 1, 2000, n);
		       TEST_TRUE("non-null", small_foods);
		       auto solution = exhaustive_max_protein(*small_foods, 2000);
		       TEST_TRUE("non-null", solution);
		       int actual_kcal, actual_protein;
		       sum_food_vector(actual_kcal, actual_protein, *solution);
		       std::stringstream ss;
		       ss << "exhaustive search n=" << n
			  << ", expected protein=" << expected_protein
			  << " but algorithm found=" << actual_protein;
		       TEST_EQUAL(ss.str(), expected_protein, actual_protein);
		     }
		   });

  return rubric.run();
}
