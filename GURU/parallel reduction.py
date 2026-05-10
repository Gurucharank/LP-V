import multiprocessing

# Worker functions for the first stage (Chunk Reduction)
def chunk_min(data): return min(data)
def chunk_max(data): return max(data)
def chunk_sum(data): return sum(data)
def chunk_stats(data): return (sum(data), len(data)) # For Average

def parallel_reduction():
    dataset = [4, 8, 2, 5, 10, 3, 7, 1, 9, 6, 12, 15]
    num_processes = multiprocessing.cpu_count()
    
    # Split data into chunks
    chunk_size = (len(dataset) + num_processes - 1) // num_processes
    chunks = [dataset[i:i + chunk_size] for i in range(0, len(dataset), chunk_size)]

    with multiprocessing.Pool(processes=num_processes) as pool:
        # 1. Min
        mins = pool.map(chunk_min, chunks)
        final_min = min(mins)

        # 2. Max
        maxs = pool.map(chunk_max, chunks)
        final_max = max(maxs)

        # 3. Sum
        sums = pool.map(chunk_sum, chunks)
        final_sum = sum(sums)

        # 4. Average (Sum / Count)
        stats = pool.map(chunk_stats, chunks)
        total_sum = sum(s for s, c in stats)
        total_count = sum(c for s, c in stats)
        final_avg = total_sum / total_count

    print(f"Min: {final_min}, Max: {final_max}, Sum: {final_sum}, Avg: {final_avg}")

if __name__ == "__main__":
    parallel_reduction()