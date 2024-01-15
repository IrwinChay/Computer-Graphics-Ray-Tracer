import sys
import glob
import numpy as np
import matplotlib.pyplot as plt

def read_ppm(filename):
    with open(filename, "rb") as f:
        header = f.readline().decode().strip()
        if header != "P6":
            raise ValueError(f"Unsupported PPM format: {header}")

        while True:
            line = f.readline().decode().strip()
            if line.startswith("#"):
                continue  # Skip comments
            width, height = [int(x) for x in line.split()]
            break

        maxval = int(f.readline().decode().strip())
        if maxval > 255:
            raise ValueError("Only 8-bit PPM files are supported")

        data = np.fromfile(f, dtype=np.uint8).reshape(height, width, 3)
    return data

def mse(image1, image2):
    if image1.shape != image2.shape:
        raise ValueError("Images must have the same dimensions")

    num_pixels = image1.shape[0] * image1.shape[1]
    err = np.sum((image1.astype(np.float64) - image2.astype(np.float64)) ** 2) / num_pixels
    return err

def calculate_mse_for_files(file_pattern, reference_image):
    mse_values = []
    input_files = sorted(glob.glob(file_pattern))

    for input_file in input_files:
        frame_number = int(input_file.split("_")[-1].split(".")[0])
        input_image = read_ppm(input_file)
        mse_value = mse(input_image, reference_image)
        mse_values.append((frame_number, mse_value))

    return mse_values

def moving_average(data, window_size):
    if window_size < 1:
        raise ValueError("Window size must be at least 1")

    return [np.mean(data[max(0, i - window_size):i + 1]) for i in range(len(data))]

if __name__ == "__main__":
    reference_image_file = "output_thin_lens_compare_gt.ppm"
    # reference_image_file = "area_light_compare_gt.ppm"
    reference_image = read_ppm(reference_image_file)

    jitter_files_pattern = "output_thin_lens_compare_jitter_*.ppm"
    # jitter_files_pattern = "area_light_compare_jitter_*.ppm"
    random_files_pattern = "output_thin_lens_compare_random_*.ppm"
    # random_files_pattern = "area_light_compare_random_*.ppm"

    jitter_mse_values = calculate_mse_for_files(jitter_files_pattern, reference_image)
    random_mse_values = calculate_mse_for_files(random_files_pattern, reference_image)

    print("Jitter MSE Values:")
    print(jitter_mse_values)
    print("Random MSE Values:")
    print(random_mse_values)

    # Sort the data by frame number
    jitter_mse_values.sort(key=lambda x: x[0])
    random_mse_values.sort(key=lambda x: x[0])

    # Unzip the frame numbers and MSE values
    jitter_numbers, jitter_mse_values_list = zip(*jitter_mse_values)
    random_numbers, random_mse_values_list = zip(*random_mse_values)

    plt.plot(jitter_numbers, jitter_mse_values_list, label="Jitter", marker="o")
    plt.plot(random_numbers, random_mse_values_list, label="Random", marker="o")
    plt.xlabel("Sample Number")
    plt.ylabel("MSE (log2 scale)")
    plt.yscale("log", base=2)  # Set the y-axis to log base 2 scale
    plt.title("MSE Comparison for Jitter and Random Sampling in Thin Lens Camera")
    # plt.title("MSE Comparison for Jitter and Random Sampling in Area Light")
    plt.legend()
    plt.grid()
    plt.show()


