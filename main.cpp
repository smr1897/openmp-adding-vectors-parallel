#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

const int VECTOR_SIZE = 10000000; // Size of the vectors 
    
// Function to initialize vectors with random values
void initialize_vectors(std::vector<int>& vector1, std::vector<int>& vector2) {
    std::srand(static_cast<unsigned int>(std::time(0))); // Seed the random number generator
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector1[i] = std::rand() % 100 + 1; // Random values between 1 and 100
        vector2[i] = std::rand() % 100 + 1;
    }
}

// Function to add two vectors sequentially
void add_vectors_sequential(const std::vector<int>& vec1, const std::vector<int>& vec2, std::vector<int>& result) {
    for (int i = 0; i < vec1.size(); i++) {
        result[i] = vec1[i] + vec2[i];
    }
}

// Function to add two vectors in parallel 
void add_vectors_parallel(const std::vector<int>& vec1, const std::vector<int>& vec2, std::vector<int>& result, int totlathreads) {
    int vsize = vec1.size();
    int local_vsize = vsize / totlathreads;

    #pragma omp parallel for num_threads(totlathreads)
    
        for(int i=0; i<vsize; i++)
        {
        	result[i] = vec1[i] + vec2[i];
		}
    
}

// Section 1: Measure the execution time of 'add_vectors_sequential'
double measure_sequential_execution_time(const std::vector<int>& vector1, const std::vector<int>& vector2, std::vector<int>& result) {
    
	double start_time_sequential = omp_get_wtime();
    add_vectors_sequential(vector1, vector2, result);
    double end_time_sequential = omp_get_wtime();
    
    double elapsed_time_sequential = end_time_sequential - start_time_sequential;
    
	return elapsed_time_sequential;
}

// Section 2: Measure the execution time of 'add_vectors_parallel'
double measure_parallel_execution_time(const std::vector<int>& vector1, const std::vector<int>& vector2, std::vector<int>& result, int num_threads) {
    
	double start_time_parallel = omp_get_wtime();
	add_vectors_parallel(vector1,vector2,result,num_threads);
	double end_time_parallel = omp_get_wtime();
	
	double elapsed_time_parallel = end_time_parallel - start_time_parallel;
	return elapsed_time_parallel;
}

// Section 3: Measure the execution time for different thread counts (1, 2, 4, 8, etc.)
void measure_thread_count_vs_time(const std::vector<int>& vector1, const std::vector<int>& vector2, std::vector<int>& result, int amax_threads) {
    
	std::cout << "\n\nThread Count vs. Time:\nMaximum available threads in the system is " << omp_get_max_threads() << "\n" << std::endl;
    
	for (int num_threads = 1; num_threads <= amax_threads; num_threads *= 2) {
        omp_set_num_threads(num_threads);
        double elapsed_time_parallel_threaded = measure_parallel_execution_time(vector1, vector2, result, num_threads);
        std::cout << "Threads: " << num_threads << ", Execution Time: " << elapsed_time_parallel_threaded << " seconds" << std::endl;
    }
}

int main() {
    int max_threads = omp_get_max_threads();

    // Create two vectors 'vector1' and 'vector2' of size VECTOR_SIZE
    std::vector<int> vector1(VECTOR_SIZE);
    std::vector<int> vector2(VECTOR_SIZE);
    // Create a vector 'result' to store the sum of vectors
    std::vector<int> result(VECTOR_SIZE);

    // Initialize vectors with random values
    initialize_vectors(vector1, vector2);

    // Section 1: Measure the execution time of 'add_vectors_sequential'
    double elapsed_time_sequential = measure_sequential_execution_time(vector1, vector2, result);

    // Section 2: Measure the execution time of 'add_vectors_parallel'
    double elapsed_time_parallel = measure_parallel_execution_time(vector1, vector2, result, max_threads);

    std::cout << "\nVector Size: " << VECTOR_SIZE << std::endl;
    std::cout << "\nSequential Execution Time: " << elapsed_time_sequential << " seconds" << std::endl;
    std::cout << "Parallel Execution Time: " << elapsed_time_parallel << " seconds" << ", Threads: " << max_threads << std::endl;

    // Section 3: Measure the execution time for different thread counts (1, 2, 4, 8, etc.)
    int assumed_max_threads = 128; // Assumed maximum number of threads
	measure_thread_count_vs_time(vector1, vector2, result, assumed_max_threads);

    return 0;
}