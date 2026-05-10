import multiprocessing

# Function to perform parallel reduction
def parallel_reduction(data, func):
    # Define the number of processes
    num_processes = multiprocessing.cpu_count()

    # Split the data into chunks for parallel processing
    chunk_size = len(data) // num_processes
    chunks = [data[i:i+chunk_size] for i in range(0, len(data), chunk_size)]

    # Create a pool of processes
    pool = multiprocessing.Pool(processes=num_processes)

    # Apply reduction function in parallel
    results = pool.map(func, chunks)

    # Close the pool to release resources
    pool.close()
    pool.join()

    # Perform final reduction on the results
    return func(results)


# Example functions for reduction operations
def find_min(data):
    return min(data)

def find_max(data):
    return max(data)

def find_sum(data):
    return sum(data)

def find_average(data):
    return sum(data) / len(data)


if __name__ == "__main__":
    # Example dataset
    dataset = [4, 8, 2, 5, 10, 3, 7, 1, 9, 6]

    # Perform parallel reduction operations
    min_value = parallel_reduction(dataset, find_min)
    max_value = parallel_reduction(dataset, find_max)
    sum_value = parallel_reduction(dataset, find_sum)
    average_value = parallel_reduction(dataset, find_average)

    print("Dataset:", dataset)
    print("Minimum:", min_value)
    print("Maximum:", max_value)
    print("Sum:", sum_value)
    print("Average:", average_value)

