import argparse
import os

def is_directory_exists(directory: str) -> bool:
    """
    Check if a directory exists.
    Args:
        directory (str): Path to the directory.
    Returns:
        bool: True if the directory exists, False otherwise.
    """
    try:
        return os.path.exists(directory)
    except FileNotFoundError:
        raise FileNotFoundError(f"Directory '{directory}' not found.")

def parse_args():
    """
    Parses command-line arguments for the Memory Asset Comparison Pipeline.
    Returns:
        argparse.Namespace: Parsed arguments.
    """
    parser = argparse.ArgumentParser(
        description="Runtime Memory Reporter: Analyze and compare runtime memory dumps."
    )
    # Define arguments
    runtime_group = parser.add_argument_group("Runtime process options")
    
    runtime_group.add_argument(
        "--config",
        type=str,
        required=True,
        help="Config of the reports."
    )

    # Parse arguments
    args = parser.parse_args()
    
    # Validate paths
    try:
        is_directory_exists(args.config)
    except FileNotFoundError as e:
        raise argparse.ArgumentTypeError(e)
    
    return args