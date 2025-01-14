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

def run_python_script(script_path, args):
    """
    Run another Python script with the provided arguments.
    
    :param script_path: Path to the Python script (.py)
    :param args: List of arguments to pass to the Python script
    """
    try:
        # Prepare the command to execute
        command = [sys.executable, script_path] + args
        
        # Run the Python script using subprocess
        result = subprocess.run(command, capture_output=True, text=True)
        
        # Check if the Python script ran successfully
        if result.returncode == 0:
            print(f"Python Script Output:\n{result.stdout}")
        else:
            print(f"Error Running Python Script:\n{result.stderr}")
    
    except Exception as e:
        print(f"Failed to run the Python script: {str(e)}")


if __name__ == "__main__":
    # Example paths and arguments for running the C++ executable
    cpp_exe_path = "./bin/Signal" # Change to your executable path
    
    for mask_size in [ "5", "15", "25", "35", "45", "55" ]:
        cpp_args = ["1", "0", "0", "./data/config.json", mask_size]  # Arguments for the C++ executable
        run_cpp_executable(cpp_exe_path, cpp_args)

    # Example path and arguments for running the Python script
    python_script_path = "path_to_your_python_script.py"  # Change to your script path
    python_args = ["param1", "param2"]  # Arguments for the Python script
    
    # Run the Python script
    # run_python_script(python_script_path, python_args)
