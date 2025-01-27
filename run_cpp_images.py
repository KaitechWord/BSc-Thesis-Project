import subprocess
import sys
import os

def run_cpp_executable(exe_path, args):
    """
    Run a C++ executable with the provided arguments.
    
    :param exe_path: Path to the C++ executable (.exe)
    :param args: List of arguments to pass to the executable
    """
    try:
        # Prepare the command to execute
        command = [exe_path] + args

        # Run the C++ executable using subprocess
        result = subprocess.run(command, capture_output=True, text=True)
        
        # Check if the C++ executable ran successfully
        if result.returncode == 0:
            print(f"C++ Executable Output:\n{result.stdout}")
        else:
            print(f"Error Running C++ Executable:\n{result.stderr}")
    
    except Exception as e:
        print(f"Failed to run the C++ executable: {str(e)}")

if __name__ == "__main__":
    # Example paths and arguments for running the C++ executable
    cpp_exe_path = "./bin/Image" # Change to your executable path
    
    for threads in ["1", "10", "100"]:
        for mask_size in [ "5", "15", "25", "35", "45", "55" ]:
            cpp_args = [threads, "0", "1", "./data/LakeMountain.jpg", mask_size]  # Arguments for the C++ executable
            run_cpp_executable(cpp_exe_path, cpp_args)